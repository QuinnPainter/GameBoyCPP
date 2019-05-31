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
FF4B = Window X
FF47 = Palette
*/

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
        logging::log("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    gpu::screenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

gpu::~gpu()
{
    SDL_DestroyRenderer(gpu::screenRenderer);
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