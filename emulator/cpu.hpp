#pragma once

#include "memory.hpp"

struct instrInfo
{
    byte numBytes;  //the length of the instruction
    byte numCycles; //the number of cycles the instruction takes to execute
    bool incPC = true; //do we increase the PC after the instruction? for jumps, this is false
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
    ushort PC; //program counter
    ushort SP; //stack pointer
    bool IME;  //interrupt enable flag
    bool HALTED; //is CPU halted?  (halts are ended by an interrupt)
    bool STOPPED;//is CPU stopped? (stops are ended by a button press)
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
        instrInfo SBC_A_R(byte* srcReg);
        instrInfo SBC_A_N(byte n);
        instrInfo SBC_A_HL();
        instrInfo AND_A_R(byte* srcReg);
        instrInfo AND_A_N(byte n);
        instrInfo AND_A_HL();
        instrInfo OR_A_R(byte* srcReg);
        instrInfo OR_A_N(byte n);
        instrInfo OR_A_HL();
        instrInfo XOR_A_R(byte* srcReg);
        instrInfo XOR_A_N(byte n);
        instrInfo XOR_A_HL();
        instrInfo CP_A_R(byte* srcReg);
        instrInfo CP_A_N(byte n);
        instrInfo CP_A_HL();
        instrInfo INC_R(byte* srcReg);
        instrInfo INC_HL();
        instrInfo DEC_R(byte* srcReg);
        instrInfo DEC_HL();
        instrInfo ADD_HL_SS(ushort* regPair);
        instrInfo ADD_SP_E(byte e);
        instrInfo INC_SS(ushort* regPair);
        instrInfo DEC_SS(ushort* regPair);
        //rotate shift and bit go here
        instrInfo JP_NN(ushort dest);
        instrInfo JP_CC_NN(bool condition, ushort dest);
        instrInfo JR_E(byte dest);
        instrInfo JR_CC_E(bool condition, byte dest);
        instrInfo JP_HL();
        instrInfo CALL_NN(ushort dest);
        instrInfo CALL_CC_NN(bool condition, ushort dest);
        instrInfo RET();
        instrInfo RETI();
        instrInfo RET_CC(bool condition);
        instrInfo RST(ushort dest);
        instrInfo DAA();
        instrInfo CPL();
        instrInfo NOP();
        instrInfo CCF();
        instrInfo SCF();
        instrInfo DI();
        instrInfo EI();
        instrInfo HALT();
        instrInfo STOP();
};