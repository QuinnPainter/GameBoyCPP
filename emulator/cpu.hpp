#pragma once

#include "memory.hpp"

struct instrInfo
{
    byte numBytes;  //the length of the instruction
    byte numCycles; //the number of cycles the instruction takes to execute
};

enum flags
{
    Z_flag = 7,
    N_flag = 6,
    H_flag = 5,
    C_flag = 4
};

struct cpuState
{
    //using unions allows me to access both registers seperately, or together as a short if I want to
    //when you change one, it updates the other. how have I never heard of this?
    union
    {
        struct
        {
            byte C;
            byte B;
        };
        ushort BC;
    };
    union
    {
        struct
        {
            byte E;
            byte D;
        };
        ushort DE;
    };
    union
    {
        struct
        {
            byte L;
            byte H;
        };
        ushort HL;
    };
    union
    {
        struct
        {
            byte F;
            byte A;
        };
        ushort AF;
    };
    ushort PC;
    ushort SP;
};

class cpu
{
    public:
        void step();
        void initState(cpuState s, memory* m);
    private:
        instrInfo emulateOp();
        cpuState state;
        memory* Memory;
        void setFlag(byte flag, bool value);
        instrInfo NOP();
        instrInfo LD_N_SP(ushort addr);
        instrInfo LD_R_N(ushort* regPair, ushort value);
        instrInfo ADD_HL_R(ushort* regPair);
};