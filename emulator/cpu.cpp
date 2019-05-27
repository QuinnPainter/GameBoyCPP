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
    byte firstByte = cpu::Memory->get8(cpu::state.PC);
    byte secondByte = cpu::Memory->get8(cpu::state.PC + 1);
    byte thirdByte = cpu::Memory->get8(cpu::state.PC + 2);
    logging::log(ushortToString(cpu::state.PC));
    //look at the first 2 bits
    switch (firstByte)
    {
        case 0x40: return cpu::LD_R_R(&cpu::state.B,&cpu::state.B);
        case 0x41: return cpu::LD_R_R(&cpu::state.B,&cpu::state.C);
        case 0x42: return cpu::LD_R_R(&cpu::state.B,&cpu::state.D);
        case 0x43: return cpu::LD_R_R(&cpu::state.B,&cpu::state.E);
        case 0x44: return cpu::LD_R_R(&cpu::state.B,&cpu::state.H);
        case 0x45: return cpu::LD_R_R(&cpu::state.B,&cpu::state.L);
        case 0x47: return cpu::LD_R_R(&cpu::state.B,&cpu::state.A);
        case 0x48: return cpu::LD_R_R(&cpu::state.C,&cpu::state.B);
        case 0x49: return cpu::LD_R_R(&cpu::state.C,&cpu::state.C);
        case 0x4A: return cpu::LD_R_R(&cpu::state.C,&cpu::state.D);
        case 0x4B: return cpu::LD_R_R(&cpu::state.C,&cpu::state.E);
        case 0x4C: return cpu::LD_R_R(&cpu::state.C,&cpu::state.H);
        case 0x4D: return cpu::LD_R_R(&cpu::state.C,&cpu::state.L);
        case 0x4F: return cpu::LD_R_R(&cpu::state.C,&cpu::state.A);
        case 0x50: return cpu::LD_R_R(&cpu::state.D,&cpu::state.B);
        case 0x51: return cpu::LD_R_R(&cpu::state.D,&cpu::state.C);
        case 0x52: return cpu::LD_R_R(&cpu::state.D,&cpu::state.D);
        case 0x53: return cpu::LD_R_R(&cpu::state.D,&cpu::state.E);
        case 0x54: return cpu::LD_R_R(&cpu::state.D,&cpu::state.H);
        case 0x55: return cpu::LD_R_R(&cpu::state.D,&cpu::state.L);
        case 0x57: return cpu::LD_R_R(&cpu::state.D,&cpu::state.A);
        case 0x58: return cpu::LD_R_R(&cpu::state.E,&cpu::state.B);
        case 0x59: return cpu::LD_R_R(&cpu::state.E,&cpu::state.C);
        case 0x5A: return cpu::LD_R_R(&cpu::state.E,&cpu::state.D);
        case 0x5B: return cpu::LD_R_R(&cpu::state.E,&cpu::state.E);
        case 0x5C: return cpu::LD_R_R(&cpu::state.E,&cpu::state.H);
        case 0x5D: return cpu::LD_R_R(&cpu::state.E,&cpu::state.L);
        case 0x5F: return cpu::LD_R_R(&cpu::state.E,&cpu::state.A);
        case 0x60: return cpu::LD_R_R(&cpu::state.H,&cpu::state.B);
        case 0x61: return cpu::LD_R_R(&cpu::state.H,&cpu::state.C);
        case 0x62: return cpu::LD_R_R(&cpu::state.H,&cpu::state.D);
        case 0x63: return cpu::LD_R_R(&cpu::state.H,&cpu::state.E);
        case 0x64: return cpu::LD_R_R(&cpu::state.H,&cpu::state.H);
        case 0x65: return cpu::LD_R_R(&cpu::state.H,&cpu::state.L);
        case 0x67: return cpu::LD_R_R(&cpu::state.H,&cpu::state.A);
        case 0x68: return cpu::LD_R_R(&cpu::state.L,&cpu::state.B);
        case 0x69: return cpu::LD_R_R(&cpu::state.L,&cpu::state.C);
        case 0x6A: return cpu::LD_R_R(&cpu::state.L,&cpu::state.D);
        case 0x6B: return cpu::LD_R_R(&cpu::state.L,&cpu::state.E);
        case 0x6C: return cpu::LD_R_R(&cpu::state.L,&cpu::state.H);
        case 0x6D: return cpu::LD_R_R(&cpu::state.L,&cpu::state.L);
        case 0x6F: return cpu::LD_R_R(&cpu::state.L,&cpu::state.A);
        case 0x78: return cpu::LD_R_R(&cpu::state.A,&cpu::state.B);
        case 0x79: return cpu::LD_R_R(&cpu::state.A,&cpu::state.C);
        case 0x7A: return cpu::LD_R_R(&cpu::state.A,&cpu::state.D);
        case 0x7B: return cpu::LD_R_R(&cpu::state.A,&cpu::state.E);
        case 0x7C: return cpu::LD_R_R(&cpu::state.A,&cpu::state.H);
        case 0x7D: return cpu::LD_R_R(&cpu::state.A,&cpu::state.L);
        case 0x7F: return cpu::LD_R_R(&cpu::state.A,&cpu::state.A);

        case 0x06: return cpu::LD_R_N(&cpu::state.B,secondByte);
        case 0x0E: return cpu::LD_R_N(&cpu::state.C,secondByte);
        case 0x16: return cpu::LD_R_N(&cpu::state.D,secondByte);
        case 0x1E: return cpu::LD_R_N(&cpu::state.E,secondByte);
        case 0x26: return cpu::LD_R_N(&cpu::state.H,secondByte);
        case 0x2E: return cpu::LD_R_N(&cpu::state.L,secondByte);
        case 0x3E: return cpu::LD_R_N(&cpu::state.A,secondByte);

        case 0x46: return cpu::LD_R_HL(&cpu::state.B);
        case 0x4E: return cpu::LD_R_HL(&cpu::state.C);
        case 0x56: return cpu::LD_R_HL(&cpu::state.D);
        case 0x5E: return cpu::LD_R_HL(&cpu::state.E);
        case 0x66: return cpu::LD_R_HL(&cpu::state.H);
        case 0x6E: return cpu::LD_R_HL(&cpu::state.L);
        case 0x7E: return cpu::LD_R_HL(&cpu::state.A);

        case 0x70: return cpu::LD_HL_R(&cpu::state.B);
        case 0x71: return cpu::LD_HL_R(&cpu::state.C);
        case 0x72: return cpu::LD_HL_R(&cpu::state.D);
        case 0x73: return cpu::LD_HL_R(&cpu::state.E);
        case 0x74: return cpu::LD_HL_R(&cpu::state.H);
        case 0x75: return cpu::LD_HL_R(&cpu::state.L);
        case 0x77: return cpu::LD_HL_R(&cpu::state.A);

        case 0x36: return cpu::LD_HL_N(secondByte);

        case 0x0A: return cpu::LD_A_BC();

        case 0x1A: return cpu::LD_A_DE();

        case 0xF2: return cpu::LD_A_C();

        case 0xE2: return cpu::LD_C_A();

        case 0xF0: return cpu::LD_A_N(secondByte);

        case 0xE0: return cpu::LD_N_A(secondByte);

        case 0xFA: return cpu::LD_A_NN(combineBytes(thirdByte, secondByte));

        case 0xEA: return cpu::LD_NN_A(combineBytes(thirdByte, secondByte));

        case 0x2A: return cpu::LDI_A_HL();

        case 0x3A: return cpu::LDD_A_HL();

        case 0x02: return cpu::LD_BC_A();

        case 0x12: return cpu::LD_DE_A();

        case 0x22: return cpu::LDI_HL_A();

        case 0x32: return cpu::LDD_HL_A();

        case 0x01: return cpu::LD_DD_NN(&cpu::state.BC, combineBytes(thirdByte, secondByte));
        case 0x11: return cpu::LD_DD_NN(&cpu::state.DE, combineBytes(thirdByte, secondByte));
        case 0x21: return cpu::LD_DD_NN(&cpu::state.HL, combineBytes(thirdByte, secondByte));
        case 0x31: return cpu::LD_DD_NN(&cpu::state.SP, combineBytes(thirdByte, secondByte));

        case 0xF9: return cpu::LD_SP_HL();

        case 0xC5: return cpu::PUSH_QQ(&cpu::state.BC);
        case 0xD5: return cpu::PUSH_QQ(&cpu::state.DE);
        case 0xE5: return cpu::PUSH_QQ(&cpu::state.HL);
        case 0xF5: return cpu::PUSH_QQ(&cpu::state.AF);

        case 0xC1: return cpu::POP_QQ(&cpu::state.BC);
        case 0xD1: return cpu::POP_QQ(&cpu::state.DE);
        case 0xE1: return cpu::POP_QQ(&cpu::state.HL);
        case 0xF1: return cpu::POP_QQ(&cpu::state.AF);

        case 0xF8: return cpu::LDHL_SP_N(secondByte);

        case 0x08: return cpu::LD_NN_SP(combineBytes(thirdByte, secondByte));

        case 0x80: return cpu::ADD_A_R(&cpu::state.B);
        case 0x81: return cpu::ADD_A_R(&cpu::state.C);
        case 0x82: return cpu::ADD_A_R(&cpu::state.D);
        case 0x83: return cpu::ADD_A_R(&cpu::state.E);
        case 0x84: return cpu::ADD_A_R(&cpu::state.H);
        case 0x85: return cpu::ADD_A_R(&cpu::state.L);
        case 0x87: return cpu::ADD_A_R(&cpu::state.A);

        case 0xC6: return cpu::ADD_A_N(secondByte);

        case 0x86: return cpu::ADD_A_HL();

        case 0x88: return cpu::ADC_A_R(&cpu::state.B);
        case 0x89: return cpu::ADC_A_R(&cpu::state.C);
        case 0x8A: return cpu::ADC_A_R(&cpu::state.D);
        case 0x8B: return cpu::ADC_A_R(&cpu::state.E);
        case 0x8C: return cpu::ADC_A_R(&cpu::state.H);
        case 0x8D: return cpu::ADC_A_R(&cpu::state.L);
        case 0x8F: return cpu::ADC_A_R(&cpu::state.A);

        case 0xCE: return cpu::ADC_A_N(secondByte);

        case 0x8E: return cpu::ADC_A_HL();

        case 0x90: return cpu::SUB_A_R(&cpu::state.B);
        case 0x91: return cpu::SUB_A_R(&cpu::state.C);
        case 0x92: return cpu::SUB_A_R(&cpu::state.D);
        case 0x93: return cpu::SUB_A_R(&cpu::state.E);
        case 0x94: return cpu::SUB_A_R(&cpu::state.H);
        case 0x95: return cpu::SUB_A_R(&cpu::state.L);
        case 0x97: return cpu::SUB_A_R(&cpu::state.A);

        case 0xD6: return cpu::SUB_A_N(secondByte);

        case 0x96: return cpu::SUB_A_HL();

        case 0x98: return cpu::SBC_A_R(&cpu::state.B);
        case 0x99: return cpu::SBC_A_R(&cpu::state.C);
        case 0x9A: return cpu::SBC_A_R(&cpu::state.D);
        case 0x9B: return cpu::SBC_A_R(&cpu::state.E);
        case 0x9C: return cpu::SBC_A_R(&cpu::state.H);
        case 0x9D: return cpu::SBC_A_R(&cpu::state.L);
        case 0x9F: return cpu::SBC_A_R(&cpu::state.A);

        case 0xDE: return cpu::SBC_A_N(secondByte);

        case 0x9E: return cpu::SBC_A_HL();

        case 0xA0: return cpu::AND_A_R(&cpu::state.B);
        case 0xA1: return cpu::AND_A_R(&cpu::state.C);
        case 0xA2: return cpu::AND_A_R(&cpu::state.D);
        case 0xA3: return cpu::AND_A_R(&cpu::state.E);
        case 0xA4: return cpu::AND_A_R(&cpu::state.H);
        case 0xA5: return cpu::AND_A_R(&cpu::state.L);
        case 0xA7: return cpu::AND_A_R(&cpu::state.A);

        case 0xE6: return cpu::AND_A_N(secondByte);

        case 0xA6: return cpu::AND_A_HL();

        case 0xB0: return cpu::OR_A_R(&cpu::state.B);
        case 0xB1: return cpu::OR_A_R(&cpu::state.C);
        case 0xB2: return cpu::OR_A_R(&cpu::state.D);
        case 0xB3: return cpu::OR_A_R(&cpu::state.E);
        case 0xB4: return cpu::OR_A_R(&cpu::state.H);
        case 0xB5: return cpu::OR_A_R(&cpu::state.L);
        case 0xB7: return cpu::OR_A_R(&cpu::state.A);

        case 0xF6: return cpu::OR_A_N(secondByte);

        case 0xB6: return cpu::OR_A_HL();

        case 0x04: return cpu::INC_R(&cpu::state.B);
        case 0x0C: return cpu::INC_R(&cpu::state.C);
        case 0x14: return cpu::INC_R(&cpu::state.D);
        case 0x1C: return cpu::INC_R(&cpu::state.E);
        case 0x24: return cpu::INC_R(&cpu::state.H);
        case 0x2C: return cpu::INC_R(&cpu::state.L);
        case 0x3C: return cpu::INC_R(&cpu::state.A);

        case 0x34: return cpu::INC_HL();

        case 0x05: return cpu::DEC_R(&cpu::state.B);
        case 0x0D: return cpu::DEC_R(&cpu::state.C);
        case 0x15: return cpu::DEC_R(&cpu::state.D);
        case 0x1D: return cpu::DEC_R(&cpu::state.E);
        case 0x25: return cpu::DEC_R(&cpu::state.H);
        case 0x2D: return cpu::DEC_R(&cpu::state.L);
        case 0x3D: return cpu::DEC_R(&cpu::state.A);

        case 0x35: return cpu::DEC_HL();
        //case 0x00: return cpu::NOP();
        //case 0x08: return cpu::LD_N_SP(combineBytes(secondByte, thirdByte));
        //case 0x09: return cpu::ADD_HL_R(&cpu::state.BC);
        //case 0x19: return cpu::ADD_HL_R(&cpu::state.DE);
        //case 0x29: return cpu::ADD_HL_R(&cpu::state.HL);
        //case 0x39: return cpu::ADD_HL_R(&cpu::state.SP);
        default: logging::logerr("Unimplemented instruction: " + byteToString(firstByte)); return instrInfo {1,4};
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

bool cpu::getFlag(byte flag)
{
    return getBit(cpu::state.F, flag);
}

//              ----Instructions----
// 8 Bit Loads

//Copies srcReg into dstReg
instrInfo cpu::LD_R_R(byte* dstReg, byte* srcReg)
{
    *dstReg = *srcReg;
    return {1,4};
}
//Loads given byte into dstReg
instrInfo cpu::LD_R_N(byte* dstReg, byte n)
{
    *dstReg = n;
    return {2,8};
}
//Loads the value at the memory address in HL to dstReg
instrInfo cpu::LD_R_HL(byte* dstReg)
{
    *dstReg = cpu::Memory->get8(cpu::state.HL);
    return {1,8};
}
//Loads srcReg into the memory address in HL
instrInfo cpu::LD_HL_R(byte* srcReg)
{
    cpu::Memory->set8(cpu::state.HL, *srcReg);
    return {1,8};
}
//Loads n into the mrmory address in HL
instrInfo cpu::LD_HL_N(byte n)
{
    cpu::Memory->set8(cpu::state.HL, n);
    return {2,12};
}
//Loads from the memory address in BC into A
instrInfo cpu::LD_A_BC()
{
    cpu::state.A = cpu::Memory->get8(cpu::state.BC);
    return {1,8};
}
//Loads from the memory address in DE into A
instrInfo cpu::LD_A_DE()
{
    cpu::state.A = cpu::Memory->get8(cpu::state.DE);
    return {1,8};
}
//Loads from the memory address (FF00 + C) into A
instrInfo cpu::LD_A_C()
{
    cpu::state.A = cpu::Memory->get8(0xFF00 + cpu::state.C);
    return {1,8};
}
//Copies A into the memory address (FF00 + C)
instrInfo cpu::LD_C_A()
{
    cpu::Memory->set8(0xFF00 + cpu::state.C, cpu::state.A);
    return {1,8};
}
//Loads from the memory address (FF00 + n) into A
instrInfo cpu::LD_A_N(byte n)
{
    cpu::state.A = cpu::Memory->get8(0xFF00 + n);
    return {2,12};
}
//Copies A into the memory address (FF00 + n)
instrInfo cpu::LD_N_A(byte n)
{
    cpu::Memory->set8(0xFF00 + n, cpu::state.A);
    return {2,12};
}
//Loads from the memory address nn into A
instrInfo cpu::LD_A_NN(ushort nn)
{
    cpu::state.A = cpu::Memory->get8(nn);
    return {3,16};
}
//Copies A into the memory address nn
instrInfo cpu::LD_NN_A(ushort nn)
{
    cpu::Memory->set8(nn, cpu::state.A);
    return {3,16};
}
//Loads the memory address in HL into A, and increments HL
instrInfo cpu::LDI_A_HL()
{
    cpu::state.A = cpu::Memory->get8(cpu::state.HL);
    cpu::state.HL++;
    return {1,8};
}
//Loads the memory address in HL into A, and decrements HL
instrInfo cpu::LDD_A_HL()
{
    cpu::state.A = cpu::Memory->get8(cpu::state.HL);
    cpu::state.HL--;
    return {1,8};
}
//Copies A into the memory address held in BC
instrInfo cpu::LD_BC_A()
{
    cpu::Memory->set8(cpu::state.BC, cpu::state.A);
    return {1,8};
}
//Copies A into the memory address held in DE
instrInfo cpu::LD_DE_A()
{
    cpu::Memory->set8(cpu::state.DE, cpu::state.A);
    return {1,8};
}
//Copies A ino the memory held in HL, and increments HL
instrInfo cpu::LDI_HL_A()
{
    cpu::Memory->set8(cpu::state.HL, cpu::state.A);
    cpu::state.HL++;
    return {1,8};
}
//Copies A ino the memory held in HL, and decrements HL
instrInfo cpu::LDD_HL_A()
{
    cpu::Memory->set8(cpu::state.HL, cpu::state.A);
    cpu::state.HL--;
    return {1,8};
}

// 16 Bit Loads

//Loads given short nn into regPair
instrInfo cpu::LD_DD_NN(ushort* regPair, ushort nn)
{
    *regPair = nn;
    return {3,12};
}
//Loads HL into SP
instrInfo cpu::LD_SP_HL()
{
    cpu::state.SP = cpu::state.HL;
    return {1,8};
}
//Pushes regPair onto the stack
instrInfo cpu::PUSH_QQ(ushort* regPair)
{
    cpu::Memory->set8(cpu::state.SP - 1, highByte(*regPair));
    cpu::Memory->set8(cpu::state.SP - 2, lowByte(*regPair));
    cpu::state.SP -= 2;
    return {1,16};
}
//Pops from the stack into regPair
instrInfo cpu::POP_QQ(ushort* regPair)
{
    *regPair = combineBytes(cpu::Memory->get8(cpu::state.SP + 1), cpu::Memory->get8(cpu::state.SP));
    cpu::state.SP += 2;
    return {1,12};
}
//The result of (SP + n) is saved in HL
//Flags: Z = 0, H if carry bit 11, N = 0, C if carry bit 15
instrInfo cpu::LDHL_SP_N(byte n)
{
    cpu::setFlag(Z_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((n + cpu::state.SP) & 0x10000) == 0x10000);
    cpu::setFlag(H_flag, ((n&0xFFF + cpu::state.SP&0xFFF) & 0x1000) == 0x1000);
    cpu::state.HL = cpu::state.SP + n;
    return {2,12};
}
//Copies SP into memory address nn
instrInfo cpu::LD_NN_SP(ushort nn)
{
    cpu::Memory->set16(nn, cpu::state.SP);
    return {3,20};
}

// 8 Bit Arithmetic

//Adds srcReg to A and stores result in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADD_A_R(byte* srcReg)
{
    cpu::setFlag(Z_flag, (byte)(cpu::state.A + *srcReg) == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((*srcReg + cpu::state.A) & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((*srcReg&0xF + cpu::state.A&0xF) & 0x10) == 0x10);
    cpu::state.A += *srcReg;
    return {1,4};
}
//Adds n to A and stores in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADD_A_N(byte n)
{
    cpu::setFlag(Z_flag, (byte)(cpu::state.A + n) == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((n + cpu::state.A) & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((n&0xF + cpu::state.A&0xF) & 0x10) == 0x10);
    cpu::state.A += n;
    return {2,8};
}
//Adds contents of memory address in HL to A, stores in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADD_A_HL()
{
    byte n = cpu::Memory->get8(cpu::state.HL);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A + n) == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((n + cpu::state.A) & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((n&0xF + cpu::state.A&0xF) & 0x10) == 0x10);
    cpu::state.A += n;
    return {1,8};
}
//Adds srcReg to A and the carry flag and stores result in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADC_A_R(byte* srcReg)
{
    byte result = cpu::state.A + *srcReg + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((cpu::state.A + *srcReg + cpu::getFlag(C_flag)) & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((cpu::state.A&0xF + *srcReg&0xF + cpu::getFlag(C_flag)) & 0x10) == 0x10);
    cpu::state.A = result;
    return {1,4};
}
//Adds n to A and the carry flag and stores in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADC_A_N(byte n)
{
    byte result = cpu::state.A + n + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((cpu::state.A + n + cpu::getFlag(C_flag)) & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((cpu::state.A&0xF + n&0xF + cpu::getFlag(C_flag)) & 0x10) == 0x10);
    cpu::state.A = result;
    return {2,8};
}
//Adds contents of memory address in HL to A + carry flag, stores in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADC_A_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = cpu::state.A + hl + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((cpu::state.A + hl + cpu::getFlag(C_flag)) & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((cpu::state.A&0xF + hl&0xF + cpu::getFlag(C_flag)) & 0x10) == 0x10);
    cpu::state.A = result;
    return {1,8};
}
//Subtracts srcReg from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SUB_A_R(byte* srcReg)
{
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - *srcReg) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - *srcReg) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - *srcReg&0xF) < 0);
    cpu::state.A -= *srcReg;
    return {1,4};
}
//Subtracts n from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SUB_A_N(byte n)
{
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - n) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - n) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - n&0xF) < 0);
    cpu::state.A -= n;
    return {2,8};
}
//Subtracts the value held in memory address HL from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SUB_A_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - hl) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - hl) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - hl&0xF) < 0);
    cpu::state.A -= hl;
    return {1,8};
}
//Subtracts (srcReg + carry) from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SBC_A_R(byte* srcReg)
{
    byte toSub = *srcReg + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - toSub) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - toSub) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - toSub&0xF) < 0);
    cpu::state.A -= toSub;
    return {1,4};
}
//Subtracts (n + carry) from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SBC_A_N(byte n)
{
    byte toSub = n + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - toSub) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - toSub) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - toSub&0xF) < 0);
    cpu::state.A -= toSub;
    return {2,8};
}
//Subtracts ((value in mem addr HL) + carry) from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SBC_A_HL()
{
    byte toSub = cpu::Memory->get8(cpu::state.HL) + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - toSub) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - toSub) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - toSub&0xF) < 0);
    cpu::state.A -= toSub;
    return {1,8};
}
//Performs bitwise AND on srcReg and A, stores in A
//Flags: C = 0, H = 1, N = 0, Z if result is 0
instrInfo cpu::AND_A_R(byte* srcReg)
{
    byte result = cpu::state.A & *srcReg;
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 1);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {1,4};
}
//Performs bitwise AND on n and A, stores in A
//Flags: C = 0, H = 1, N = 0, Z if result is 0
instrInfo cpu::AND_A_N(byte n)
{
    byte result = cpu::state.A & n;
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 1);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {2,8};
}
//Performs bitwise AND on (value of memaddress in HL) and A, stores in A
//Flags: C = 0, H = 1, N = 0, Z if result is 0
instrInfo cpu::AND_A_HL()
{
    byte result = cpu::state.A & cpu::Memory->get8(cpu::state.HL);
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 1);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {1,8};
}
//Performs bitwise OR on srcReg and A, stores in A
//Flags: C = 0, H = 0, N = 0, Z if result is 0
instrInfo cpu::OR_A_R(byte* srcReg)
{
    byte result = cpu::state.A | *srcReg;
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {1,4};
}
//Performs bitwise OR on n and A, stores in A
//Flags: C = 0, H = 0, N = 0, Z if result is 0
instrInfo cpu::OR_A_N(byte n)
{
    byte result = cpu::state.A | n;
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {2,8};
}
//Performs bitwise OR on (value of memaddress in HL) and A, stores in A
//Flags: C = 0, H = 0, N = 0, Z if result is 0
instrInfo cpu::OR_A_HL()
{
    byte result = cpu::state.A | cpu::Memory->get8(cpu::state.HL);
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {1,8};
}
//Performs bitwise XOR on srcReg and A, stores in A
//Flags: C = 0, H = 0, N = 0, Z if result is 0
instrInfo cpu::XOR_A_R(byte* srcReg)
{
    byte result = cpu::state.A ^ *srcReg;
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {1,4};
}
//Performs bitwise XOR on n and A, stores in A
//Flags: C = 0, H = 0, N = 0, Z if result is 0
instrInfo cpu::XOR_A_N(byte n)
{
    byte result = cpu::state.A ^ n;
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {2,8};
}
//Performs bitwise XOR on (value of memaddress in HL) and A, stores in A
//Flags: C = 0, H = 0, N = 0, Z if result is 0
instrInfo cpu::XOR_A_HL()
{
    byte result = cpu::state.A ^ cpu::Memory->get8(cpu::state.HL);
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::state.A = result;
    return {1,8};
}
//"Compares" srcReg and A
//Basically does SUB, but only sets the flags, it doesn't save a result anywhere.
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::CP_A_R(byte* srcReg)
{
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - *srcReg) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - *srcReg) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - *srcReg&0xF) < 0);
    return {1,4};
}
//"Compares" n and A
//Basically does SUB, but only sets the flags, it doesn't save a result anywhere.
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::CP_A_N(byte n)
{
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - n) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - n) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - n&0xF) < 0);
    return {2,8};
}
//"Compares" (value of memaddress in HL) and A
//Basically does SUB, but only sets the flags, it doesn't save a result anywhere.
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::CP_A_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - hl) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - hl) < 0);
    cpu::setFlag(H_flag, (cpu::state.A&0xF - hl&0xF) < 0);
    return {1,8};
}

//Increments srcReg
//Flags: Z if result it 0, H if carry bit 3, N = 0
instrInfo cpu::INC_R(byte* srcReg)
{
    byte result = *srcReg + 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(H_flag, ((*srcReg&0xF + 1) & 0x10) == 0x10);
    *srcReg = result;
    return {1,4};
}
//Increments (value of memaddress in HL)
//Flags: Z if result it 0, H if carry bit 3, N = 0
instrInfo cpu::INC_HL()
{
    byte result = cpu::Memory->get8(cpu::state.HL) + 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(H_flag, ((cpu::Memory->get8(cpu::state.HL)&0xF + 1) & 0x10) == 0x10);
    cpu::Memory->set8(cpu::state.HL, result);
    return {1,12};
}
//Decrements srcReg
//Flags: Z if result it 0, H if carry bit 4, N = 1
instrInfo cpu::DEC_R(byte* srcReg)
{
    byte result = *srcReg - 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(H_flag, (*srcReg&0xF - 1) < 0);
    *srcReg = result;
    return {1,4};
}
//Decrements (value of memaddress in HL)
//Flags: Z if result it 0, H if carry bit 4, N = 1
instrInfo cpu::DEC_HL()
{
    byte result = cpu::Memory->get8(cpu::state.HL) - 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(H_flag, (cpu::Memory->get8(cpu::state.HL)&0xF - 1) < 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {1,12};
}
/*
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

//Add the given register pair to HL, save result in HL
//Flags: N = 0, H if carry bit 11,  C if carry bit 15
instrInfo cpu::ADD_HL_R(ushort* regPair)
{
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((*regPair + cpu::state.HL) & 0x10000) == 0x10000);
    cpu::setFlag(H_flag, ((*regPair&0xFFF + cpu::state.HL&0xFFF) & 0x1000) == 0x1000);
    cpu::state.HL += *regPair;
    return {1,8};
}
*/