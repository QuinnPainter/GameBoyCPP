#include "includes.hpp"
#include "memory.hpp"

/*
    Interrupt Enable Register
    --------------------------- FFFF
    Internal RAM
    --------------------------- FF80
    Empty but unusable for I/O
    --------------------------- FF4C
    I/O ports
    --------------------------- FF00
    Empty but unusable for I/O
    --------------------------- FEA0
    Sprite Attrib Memory (OAM)
    --------------------------- FE00
    Echo of 8kB Internal RAM
    --------------------------- E000
    8kB Internal RAM
    --------------------------- C000
    8kB switchable RAM bank
    --------------------------- A000
    8kB Video RAM
    --------------------------- 8000 --
    16kB switchable ROM bank          |
    --------------------------- 4000  |= 32kB Cartridge
    16kB ROM bank #0                  |
    --------------------------- 0000 --
*/

memory::memory()
{
    memory::memBytes = new byte[0xFFFF];
    memory::bootrom = nullptr;
}

memory::~memory()
{
    delete[] memory::memBytes;
}

void memory::init(byte* gameRom, byte* input, int* timer, byte* bootrom)
{
    memory::inputState = input;
    memory::timerCounter = timer;
    memory::rom = gameRom;
    //memcpy(memory::memBytes, gameRom, 0x8000);
    if (bootrom != nullptr)
    {
        memory::bootromMapped = true;
        memory::bootrom = bootrom;
    }
    else
    {
        memory::bootromMapped = false;
    }
    switch (memory::rom[0x0147])
    {
        case 0x00:
            memory::mbcVersion = MBC_None;
            break;
        case 0x01: case 0x02: case 0x03:
            memory::mbcVersion = MBC_MBC1;
            memory::romBank = 1;
            memory::mbcMode = 0;
            memory::upperBits = 0;
            break;
        default:
            logging::log("Unsupported MBC chip! Continuing anyway...");
            memory::mbcVersion = MBC_None;
            break;
    }
    memory::romSize = (32768 << memory::rom[0x0148]);
    /*
    switch (memory::rom[0x0148])
    {
        case 0x00: memory::romSize = 32768; break;
        case 0x01: memory::romSize = 65536; break;
        case 0x02: memory::romSize = 131072; break;
        case 0x03: memory::romSize = 262144; break;
        case 0x04: memory::romSize = 
    }*/
}

byte memory::get8(ushort address)
{
    if (address <= 0x00FF && memory::bootromMapped)
    {
        //if 0xFF50 is not 1, reading 0 to FF (255) gives the bootrom
        return memory::bootrom[address];
    }
    if (address >= 0xFF4C && address < 0xFF80)
    {
        //unused IO area
        return 0xFF;
    }
    if (address >= 0xFEA0 && address < 0xFF00)
    {
        //unused area
        return 0x00;
    }
    if (address >= 0xA000 && address < 0xC000)
    {
        //cartridge RAM       this will need to be accessible when I implement banking
        return 0xFF;
    }
    if (address < 0x8000)
    {
        //game ROM
        //first 16K is always bank 0
        if (address < 0x4000)
        {
            return memory::rom[address];
        }
        switch (memory::mbcVersion)
        {
            case (MBC_None):
            {
                return memory::rom[address];
            }
            case (MBC_MBC1):
            {
                byte bank = memory::romBank;
                if (!memory::mbcMode)
                {
                    bank |= (memory::upperBits << 5);
                }
                unsigned int adjustedAddress = (((bank - 1) * 0x4000) + address);
                if (adjustedAddress >= memory::romSize)
                {
                    //selected bank is outside of range of available rom
                    //todo : find out what this is meant to do
                    return 0xFF;
                }
                return memory::rom[adjustedAddress];
            }
        }
    }
    if (address == 0xFF00)
    {
        memory::updateInput();
    }
    return memory::memBytes[fixMemAddress(address)];
}

void memory::updateInput()
{
    byte inputReg = (memory::memBytes[0xFF00] & 0xF0); //Reset last 4 bits
    bool directionSelected = !getBit(inputReg, 4);
    bool buttonSelected = !getBit(inputReg, 5);
    if (directionSelected)
    {
        memory::set8(0xFF00, inputReg | (*(memory::inputState) >> 4), true);
    }
    else if (buttonSelected)
    {
        memory::set8(0xFF00, inputReg | (*(memory::inputState) & 0xF), true);
    }
}

void memory::set8(ushort address, byte value, bool force)
{
    //force allows you to change normally unchangeable memory
    //it should only be used for internal stuff
    if (!force)
    {
        if (address <= 0x00FF && memory::bootromMapped)
        {
            //can't set the bootrom area
            return;
        }
        if (address >= 0xFF4C && address < 0xFF80)
        {
            if (address == 0xFF50 && value == 1)
            {
                memory::bootromMapped = false;
            }
            //unused IO area
            return;
        }
        if (address >= 0xFEA0 && address < 0xFF00)
        {
            //unused area
            //logging::logerr(ushortToString(address) + " shouldn't be accessed! " + ushortToString(*PC));
            return;
        }
        if (address >= 0xA000 && address < 0xC000)
        {
            //cartridge RAM       this will need to be accessible when I implement banking
            return;
        }
        if (address == 0xFF04)
        {
            //reset divider register - also resets timer counter
            memory::memBytes[0xFF04] = 0;
            *(memory::timerCounter) = 0;
            return;
        }
        if (address == 0xFF44)
        {
            //reset current scanline register
            memory::memBytes[0xFF44] = 0;
            return;
        }
        if (address == 0xFF46)
        {
            //DMA block of memory
            memory::doDMA(value);
            return;
        }
        if (address == 0xFF00)
        {
            //input register - only set bits 4 and 5
            memory::memBytes[0xFF00] = setBit(memory::memBytes[0xFF00], 4, getBit(value, 4));
            memory::memBytes[0xFF00] = setBit(memory::memBytes[0xFF00], 5, getBit(value, 5));
            return;
        }
        if (address == 0xFF02)
        {
            //serial control register
            memory::memBytes[0xFF02] = value | 0x7E;
            if (value == 0x81)
            {
                logging::log(std::string(1, memory::get8(0xFF01)), false);
            }
            return;
        }
        if (address == 0xFF07)
        {
            //timer control register
            memory::memBytes[0xFF07] = value | 0xF8;
            return;
        }
        if (address == 0xFF0F)
        {
            //interrupt request register
            memory::memBytes[0xFF0F] = value | 0xE0;
            return;
        }
        if (address == 0xFF41)
        {
            //LCD status register
            memory::memBytes[0xFF41] = value | 0x80;
            return;
        }

        //ROM area
        if (address < 0x8000)
        {
            switch (memory::mbcVersion)
            {
                case (MBC_None):
                {
                    return;
                }
                case (MBC_MBC1):
                {
                    if (address >= 0x2000 && address < 0x4000)
                    {
                        byte adjustedValue = (value & 0x1F);
                        if (adjustedValue == 0x00 || adjustedValue == 0x20 || adjustedValue == 0x40 || adjustedValue == 0x60)
                        {
                            adjustedValue++;
                        }
                        memory::romBank = adjustedValue;
                    }
                    else if (address >= 0x4000 && address < 0x6000)
                    {
                        memory::upperBits = (value & 0x03);
                    }
                    else if (address >= 0x6000 && address < 0x8000)
                    {
                        memory::mbcMode = (value & 0x01);
                    }
                    return;
                }
            }
        }
    }
    memory::memBytes[fixMemAddress(address)] = value;
}

ushort memory::get16(ushort address)
{
    return combineBytes(memory::get8(address + 1), memory::get8(address));
}

void memory::set16(ushort address, ushort value)
{
    memory::set8(address + 1, highByte(value));
    memory::set8(address, lowByte(value));
}

//Copies given block of data into the OAM
void memory::doDMA(byte value)
{
    ushort address = memory::fixMemAddress(value << 8);
    for (int i = 0; i < 0xA0; i++)
    {
        memory::set8(0xFE00 + i, memory::get8(address + i));
    }
}

ushort memory::fixMemAddress(ushort address)
{
    //echo RAM
    if (address >= 0xE000 && address <= 0xFDFF)
    {
        return address - 0x200;
    }
    return address;
}