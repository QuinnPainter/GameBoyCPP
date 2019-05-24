#include "includes.hpp"
#include "cpu.hpp"

void cpu::initState(cpuState s, memory* m)
{
    cpu::state = s;
    cpu::Memory = m;
}

instrInfo cpu::emulateOp()
{
    //Instructions can be 1 to 3 bytes
    byte firstByte = (*cpu::Memory).get8(cpu::state.PC);
    byte secondByte = (*cpu::Memory).get8(cpu::state.PC + 1);
    byte thirdByte = (*cpu::Memory).get8(cpu::state.PC + 2);
    //look at the first 2 bits
    switch (firstByte << 6)
    {
        case 0b00:
        {
            switch (firstByte)
            {
                case 0x00: return NOP();
                case 0x08: return LD_N_SP(combineBytes(secondByte, thirdByte));
                case 0x01: return LD_R_N(&cpu::state.BC, combineBytes(secondByte, thirdByte));
                case 0x11: return LD_R_N(&cpu::state.DE, combineBytes(secondByte, thirdByte));
                case 0x21: return LD_R_N(&cpu::state.HL, combineBytes(secondByte, thirdByte));
                case 0x31: return LD_R_N(&cpu::state.SP, combineBytes(secondByte, thirdByte));
            }
        }
        case 0b01:
        {
            logging::logerr("Unimplemented instruction: " + byteToString(firstByte));
            return instrInfo {1,4};
        }
        case 0b10:
        {
            logging::logerr("Unimplemented instruction: " + byteToString(firstByte));
            return instrInfo {1,4};
        }
        case 0b11:
        {
            logging::logerr("Unimplemented instruction: " + byteToString(firstByte));
            return instrInfo {1,4};
        }
    }
}

void cpu::step()
{
    instrInfo info = cpu::emulateOp();
    cpu::state.PC += info.numBytes;
}

void cpu::setFlag(byte flag, bool value)
{
    cpu::state.F = setBit(cpu::state.F, flag, value);
}

//    Instructions

//do nothing
instrInfo cpu::NOP()
{
    return {1,4};
}

//Copies the Stack Pointer into a provided memory address
instrInfo cpu::LD_N_SP(ushort addr)
{
    cpu::Memory->set16(addr, cpu::state.SP);
    return {3,20};
}

//Loads the given 16 bit value into a register pair
instrInfo cpu::LD_R_N(ushort* regPair, ushort value)
{
    *regPair = value;
    return {3,12};
}

//Add the given register pair to HL, save result in HL
//Flags: N = 0, H if carry bit 11,  C if carry bit 15
//instrInfo cpu::ADD_HL_R(ushort* regPair)
//{
//    cpu::setFlag(N_flag, 0);
//    cpu::state.HL += *regPair;
//    return {1,8};
//}