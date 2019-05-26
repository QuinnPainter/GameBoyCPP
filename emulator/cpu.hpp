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
        bool getFlag(byte flag);
        instrInfo LD_R_R(byte* dstReg, byte* srcReg);
        instrInfo LD_R_N(byte* dstReg, byte n);
        instrInfo LD_R_HL(byte* dstReg);
        instrInfo LD_HL_R(byte* srcReg);
        instrInfo LD_HL_N(byte n);
        instrInfo LD_A_BC();
        instrInfo LD_A_DE();
        instrInfo LD_A_C();
        instrInfo LD_C_A();
        instrInfo LD_A_N(byte n);
        instrInfo LD_N_A(byte n);
        instrInfo LD_A_NN(ushort nn);
        instrInfo LD_NN_A(ushort nn);
        instrInfo LDI_A_HL();
        instrInfo LDD_A_HL();
        instrInfo LD_BC_A();
        instrInfo LD_DE_A();
        instrInfo LDI_HL_A();
        instrInfo LDD_HL_A();
        instrInfo LD_DD_NN(ushort* regPair, ushort nn);
        instrInfo LD_SP_HL();
        instrInfo PUSH_QQ(ushort* regPair);
        instrInfo POP_QQ(ushort* regPair);
        instrInfo LDHL_SP_N(byte n);
        instrInfo LD_NN_SP(ushort nn);
        instrInfo ADD_A_R(byte* srcReg);
        instrInfo ADD_A_N(byte n);
        instrInfo ADD_A_HL();
        instrInfo ADC_A_R(byte* srcReg);
        instrInfo ADC_A_N(byte n);
        instrInfo ADC_A_HL();
        instrInfo SUB_A_R(byte* srcReg);
        instrInfo SUB_A_N(byte n);
        instrInfo SUB_A_HL();
        //instrInfo NOP();
        //instrInfo LD_N_SP(ushort addr);
        //instrInfo LD_R_N(ushort* regPair, ushort value);
        //instrInfo ADD_HL_R(ushort* regPair);
        //instrInfo LD_R_A();
};