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
    if (memory::bootrom != nullptr)
    {
        delete[] memory::bootrom;
    }
}

void memory::init(byte* rom, byte* bootrom)
{
    memcpy(memory::memBytes, rom, 0x8000);
    if (bootrom != nullptr)
    {
        memory::bootrom = new byte[256];
        memcpy(memory::bootrom, bootrom, 256);
    }
}

byte memory::get8(ushort address)
{
    if (address <= 0x00FF && memory::memBytes[0xFF50] != 1)
    {
        //if 0xFF50 is not 1, reading 0 to FF (255) gives the bootrom
        return memory::bootrom[address];
    }
    if (address >= 0xFF4C && address < 0xFF80)
    {
        //unused IO area
        return 0xFF;
    }
    if (address >= 0xFEA0 && address < 0xFEFF)
    {
        //unused area
        return 0x00;
    }
    if (address >= 0xA000 && address < 0xC000)
    {
        //cartridge RAM       this will need to be accessible when I implement banking
        return 0xFF;
    }
    return memory::memBytes[fixMemAddress(address)];
}

void memory::set8(ushort address, byte value, bool force)
{
    //force allows you to change normally unchangeable memory
    //it should only be used for internal stuff
    if (!force)
    {
        if (address <= 0x00FF && memory::memBytes[0xFF50] != 1)
        {
            //if 0xFF50 is not 1, reading 0 to FF (255) gives the bootrom
            return;
        }
        else if (address >= 0xFF4C && address < 0xFF80)
        {
            //unused IO area
            return;
        }
        else if (address >= 0xFEA0 && address < 0xFEFF)
        {
            //unused area
            return;
        }
        else if (address >= 0xA000 && address < 0xC000)
        {
            //cartridge RAM       this will need to be accessible when I implement banking
            return;
        }
        else if (address == 0xFF04)
        {
            //reset divider register
            memory::memBytes[0xFF04] = 0;
            return;
        }
    }
    if (address == 0xFF02 && value == 0x81) //serial port / link cable
    {
        logging::log(std::string(1, memory::get8(0xFF01)), false);
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

ushort memory::fixMemAddress(ushort address)
{
    if (address >= 0xFEA0 && address <= 0xFEFF)
    {
        logging::logerr(ushortToString(address) + " is a forbidden memory address!");
        return address;
    }
    //echo RAM
    if (address >= 0xE000 && address <= 0xFDFF)
    {
        return address - 0x200;
    }
    return address;
}