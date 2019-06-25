#include "includes.hpp"
#include "gpu.hpp"

/*
    Video Registers:
FF40 = LCD Control  
  Bit 7 - LCD Display Enable             (0=Off, 1=On)
  Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
  Bit 5 - Window Display Enable          (0=Off, 1=On)
  Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
  Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
  Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
  Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
  Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
FF41 = LCD Status
  Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
  Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
  Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
  Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
  Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
  Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
            0: During H-Blank
            1: During V-Blank
            2: During Searching OAM-RAM
            3: During Transfering Data to LCD Driver
FF42 = Scroll Y
FF43 = Scroll X
FF44 = LY / Current Scanline
FF45 = LY Compare
FF4A = Window Y
FF4B = Window X (-7)
FF47 = Background Palette
FF48 = Sprite Palette 1
FF49 = Sprite Palette 2
    Memory Areas:
$8000-$97FF 	Character RAM (Tile Data)
$9800-$9BFF 	BG Map Data 1
$9C00-$9FFF 	BG Map Data 2
$FE00-$FE9F 	OAM - Object Attribute Memory (Sprite Data)
*/

/*
//Black and White Colour Scheme
const SDL_Color colour0 = {255,255,255,SDL_ALPHA_OPAQUE};  //00: White
const SDL_Color colour1 = {170,170,170,SDL_ALPHA_OPAQUE};  //01: Light Grey
const SDL_Color colour2 = {85,85,85,SDL_ALPHA_OPAQUE};  //10: Dark Grey
const SDL_Color colour3 = {000,000,000,SDL_ALPHA_OPAQUE};  //11: Black
*/
//Green Colour Scheme
const SDL_Color colour0 = {224,248,208,SDL_ALPHA_OPAQUE};  //00: White
const SDL_Color colour1 = {136,192,112,SDL_ALPHA_OPAQUE};  //01: Light Grey
const SDL_Color colour2 = {52,104,86,SDL_ALPHA_OPAQUE};  //10: Dark Grey
const SDL_Color colour3 = {8,24,32,SDL_ALPHA_OPAQUE};  //11: Black
const int XResolution = 160;
const int YResolution = 144;
const int Scale = 2;
const int SCREEN_WIDTH = XResolution * Scale;
const int SCREEN_HEIGHT = YResolution * Scale;

gpu::gpu(memory* mem)
{
    gpu::Memory = mem;
    gpu::cycleCounter = 456; //Takes 456 cycles to draw one scanline
    gpu::window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gpu::window == NULL)
    {
        logging::logerr("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    /*
    gpu::screenSurface = SDL_GetWindowSurface(gpu::window);
    if (gpu::screenSurface == NULL)
    {
        logging::logerr("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    */
    gpu::screenRenderer = SDL_CreateRenderer(gpu::window, -1, SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);
    if (gpu::screenRenderer == NULL)
    {
        logging::logerr("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    gpu::screenTexture = SDL_CreateTexture(gpu::screenRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, XResolution, YResolution);
}

gpu::~gpu()
{
    SDL_DestroyTexture(gpu::screenTexture);
    SDL_DestroyRenderer(gpu::screenRenderer);
    //SDL_FreeSurface(gpu::screenBuffer);
    SDL_DestroyWindow(gpu::window);
}

void gpu::update(int cycles)
{
    gpu::setStatus();
    if (getBit(Memory->get8(0xFF40), 7) == 0) //LCD is disabled
    {
        return;
    }
    gpu::cycleCounter -= cycles;
    if (gpu::cycleCounter <= 0)
    {
        gpu::Memory->set8(0xFF44, gpu::Memory->get8(0xFF44) + 1, true);
        byte currentScanline = gpu::Memory->get8(0xFF44);
        gpu::cycleCounter = 456;

        if (currentScanline == 144)
        {
            gpu::Memory->set8(0xFF0F, setBit(gpu::Memory->get8(0xFF0F), 0, 1)); // request VBlank interrupt
        }
        else if (currentScanline > 153)
        {
            gpu::Memory->set8(0xFF44, 0, true);
        }
        //hacky solution for bringing currentScanline from a range of 1-143 to 0-143
        if (currentScanline < 145)
        {
            gpu::Memory->set8(0xFF44, gpu::Memory->get8(0xFF44) - 1, true);
            gpu::drawScanline();
            gpu::Memory->set8(0xFF44, gpu::Memory->get8(0xFF44) + 1, true);
        }
    }
}

void gpu::setStatus()
{
    byte status = gpu::Memory->get8(0xFF41);
    if (getBit(gpu::Memory->get8(0xFF40), 7) == 0) //LCD is disabled
    {
        gpu::cycleCounter = 456;
        gpu::Memory->set8(0xFF44, 0);
        gpu::setLCDMode(1);
        return;
    }
    byte currentScanline = gpu::Memory->get8(0xFF44);
    bool interruptsEnabled = false;
    byte oldMode = status & 0x3;
    byte newMode;
    if (currentScanline >= 144)
    {
        gpu::setLCDMode(1);
        interruptsEnabled = getBit(status, 4);
        newMode = 1;
    }
    else
    {
        //Layout of each scanline (456 cycles):
        //Mode 2 for first 80 cycles
        //Mode 3 for next 172 cycles
        //Mode 0 for last 204 cycles
        int mode2period = 456 - 80;
        int mode3period = mode2period - 172;
        if (gpu::cycleCounter >= mode2period)
        {
            gpu::setLCDMode(2);
            interruptsEnabled = getBit(status, 5);
            newMode = 2;
        }
        else if (gpu::cycleCounter >= mode3period)
        {
            gpu::setLCDMode(3);
            newMode = 3;
        }
        else
        {
            gpu::setLCDMode(0);
            interruptsEnabled = getBit(status, 3);
            newMode = 0;
        }
    }
    if (interruptsEnabled && (oldMode != newMode))
    {
        gpu::Memory->set8(0xFF0F, setBit(gpu::Memory->get8(0xFF0F), 1, 1)); // request LCD interrupt
    }
    bool coincidence = (gpu::Memory->get8(0xFF45) == currentScanline);
    if (coincidence)
    {
        //coincidence interrupt
        if (getBit(status, 6))
        {
            gpu::Memory->set8(0xFF0F, setBit(gpu::Memory->get8(0xFF0F), 1, 1)); // request LCD interrupt
        }
    }
    gpu::Memory->set8(0xFF41, setBit(gpu::Memory->get8(0xFF41), 2, coincidence));
}

void gpu::setLCDMode(int mode)
{
    byte status = gpu::Memory->get8(0xFF41);
    status &= 0xFC; //reset last 2 bits
    switch (mode)
    {
        case 1:
            status = setBit(status, 0, 1);
            break;
        case 2:
            status = setBit(status, 1, 1);
            break;
        case 3:
            status = setBit(status, 0, 1);
            status = setBit(status, 1, 1);
            break;
    }
    gpu::Memory->set8(0xFF41, status);
}

void gpu::drawScanline()
{
    byte options = Memory->get8(0xFF40);
    if (getBit(options, 0))
    {
        gpu::drawBackground();
    }
    if (getBit(options, 1))
    {
        gpu::drawSprites();
    }
}

//Draws the background and window layers
void gpu::drawBackground()
{
    byte scrollY = gpu::Memory->get8(0xFF42);
    byte scrollX = gpu::Memory->get8(0xFF43);
    byte windowY = gpu::Memory->get8(0xFF4A);
    byte windowX = gpu::Memory->get8(0xFF4B) - 7;
    byte control = gpu::Memory->get8(0xFF40);
    byte currentScanline = gpu::Memory->get8(0xFF44);

    ushort tileData;
    ushort tileMap;
    bool isSigned;

    if (getBit(control, 4))
    {
        tileData = 0x8000;
        isSigned = false;
    }
    else
    {
        tileData = 0x8800;
        isSigned = true;
    }
    
    for (int x = 0; x < XResolution; x++)
    {
        bool drawingWindow = (getBit(control, 5) && (windowY <= currentScanline) && (x >= windowX));
        bool dataBank = drawingWindow ? getBit(control, 6) : getBit(control, 3);
        tileMap = dataBank ? 0x9C00 : 0x9800;

        byte adjustedY;
        byte adjustedX;
        if (drawingWindow)
        {
            adjustedY = currentScanline - windowY;
            adjustedX = x - windowX;
        }
        else
        {
            adjustedY = currentScanline + scrollY;
            adjustedX = x + scrollX;
        }
        
        byte tileIdentifier = gpu::Memory->get8(tileMap + (adjustedX/8) + ((byte)(adjustedY/8) * 32));
        ushort tileAddress;
        if (isSigned)
        {
            tileAddress = (tileData + ((static_cast<sbyte>(tileIdentifier) + 128) * 16));
        }
        else
        {
            tileAddress = (tileData + (tileIdentifier * 16));
        }
        byte lineIndex = ((adjustedY % 8) * 2); //which line of the tile to use
        byte line1 = gpu::Memory->get8(tileAddress + lineIndex);
        byte line2 = gpu::Memory->get8(tileAddress + lineIndex + 1);

        int colourIndex = adjustedX % 8;
        //convert from 0 - 7 to 7 - 0
        colourIndex = ((colourIndex - 7) * -1);

        byte colourNumber = ((getBit(line2, colourIndex) << 1) | getBit(line1, colourIndex));
        colourNumber = gpu::paletteAdjustColour(colourNumber, gpu::Memory->get8(0xFF47));
        gpu::drawPixel(x, currentScanline, colourNumber);
    }
}

void gpu::drawSprites()
{
    byte currentScanline = gpu::Memory->get8(0xFF44);
    byte spriteSize = getBit(gpu::Memory->get8(0xFF40), 2) ? 16 : 8;
    //do sprites backwards for semi-correct depth priorities
    for (int spriteNum = 39; spriteNum >= 0; spriteNum--)
    {
        byte spriteIndex = spriteNum * 4;
        int yPos = gpu::Memory->get8(0xFE00 + spriteIndex) - 16;
        int xPos = gpu::Memory->get8(0xFE00 + spriteIndex + 1) - 8;
        byte tileIdentifier = gpu::Memory->get8(0xFE00 + spriteIndex + 2);
        byte attributes = gpu::Memory->get8(0xFE00 + spriteIndex + 3);

        bool priority = getBit(attributes, 7); //0 = Above BG, 1 = Behind BG
        bool yFlip = getBit(attributes, 6);
        bool xFlip = getBit(attributes, 5);
        ushort spritePaletteAddress = getBit(attributes, 4) ? 0xFF49 : 0xFF48;

        if (spriteSize == 16)
        {
            //last bit of tileIdentifier is ignored in 8x16 mode
            tileIdentifier = setBit(tileIdentifier, 0, 0);
        }

        if ((currentScanline >= yPos) && (currentScanline < (yPos + spriteSize)))
        {
            byte tileLine = currentScanline - yPos;
            if (yFlip)
            {
                //convert from 0 - 7 to 7 - 0
                //or 0 - 15 to 15 - 0
                tileLine = ((tileLine - (spriteSize - 1)) * -1);
            }
            ushort lineAddress = (0x8000 + (tileIdentifier * 16) + (tileLine * 2));
            byte line1 = gpu::Memory->get8(lineAddress);
            byte line2 = gpu::Memory->get8(lineAddress + 1);
            if (xFlip)
            {
                line1 = reverseByte(line1);
                line2 = reverseByte(line2);
            }
            for (int pixel = 0; pixel < 8; pixel++)
            {
                int colourIndex = ((pixel - 7) * -1);
                byte colourNumber = ((getBit(line2, colourIndex) << 1) | getBit(line1, colourIndex));
                //check for transparency before palette adjusting
                if (colourNumber == 0)
                {
                    continue;
                }
                colourNumber = gpu::paletteAdjustColour(colourNumber, gpu::Memory->get8(spritePaletteAddress));
                int pixelX = xPos + pixel;
                //check if pixel is off the screen
                if (pixelX > XResolution || pixelX < 0)
                {
                    continue;
                }
                int pixelIndex = ((pixelX * 3) + (currentScanline * XResolution * 3));
                if (priority)
                {
                    byte newColour = gpu::paletteAdjustColour(0, gpu::Memory->get8(0xFF47));
                    SDL_Color pixColour;
                    switch (newColour)
                    {
                        case 0:
                            pixColour = colour0;
                            break;
                        case 1:
                            pixColour = colour1;
                            break;
                        case 2:
                            pixColour = colour2;
                            break;
                        case 3:
                            pixColour = colour3;
                            break;
                    }
                    //sprite is behind background
                    if (!(screenData[pixelIndex] == pixColour.r && screenData[pixelIndex + 1] == pixColour.g && screenData[pixelIndex + 2] == pixColour.b))
                    {
                        continue;
                    }
                }
                gpu::drawPixel(pixelX, currentScanline, colourNumber);
            }
        }
    }
}

byte gpu::paletteAdjustColour(byte colour, byte palette)
{
    switch (colour)
    {
        case 0: return (palette & 0x03);
        case 1: return ((palette & 0x0C) >> 2);
        case 2: return ((palette & 0x30) >> 4);
        case 3: return ((palette & 0xC0) >> 6);
    }
    return 0;
}

void gpu::drawPixel(byte x, byte y, byte colour)
{
    SDL_Colour pixColour = {0, 0, 0, 0};
    switch (colour)
    {
        case 0:
            pixColour = colour0;
            break;
        case 1:
            pixColour = colour1;
            break;
        case 2:
            pixColour = colour2;
            break;
        case 3:
            pixColour = colour3;
            break;
    }
    int memLocation = ((x * 3) + (y * XResolution * 3));
    gpu::screenData[memLocation] = pixColour.r;
    gpu::screenData[memLocation + 1] = pixColour.g;
    gpu::screenData[memLocation + 2] = pixColour.b;
}

void gpu::displayScreen()
{
    /*
    gpu::screenBuffer = SDL_CreateRGBSurfaceWithFormatFrom(gpu::screenData, XResolution, YResolution, 24, XResolution * 3, SDL_PIXELFORMAT_RGB24);
    SDL_BlitScaled(gpu::screenBuffer, NULL, gpu::screenSurface, NULL);
    SDL_UpdateWindowSurface(gpu::window);
    */
    SDL_UpdateTexture(gpu::screenTexture, NULL, gpu::screenData, XResolution * 3);
    SDL_RenderCopy(gpu::screenRenderer, gpu::screenTexture, NULL, NULL);
    SDL_RenderPresent(gpu::screenRenderer);
}