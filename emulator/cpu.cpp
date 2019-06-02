#include "includes.hpp"
#include "cpu.hpp"

cpu::cpu(cpuState s, memory* m)
{
    cpu::state = s;
    cpu::Memory = m;
}

cpuState cpu::getState()
{
    return cpu::state;
}

void cpu::serviceInterrupt(interrupts i)
{
    cpu::state.IME = 0;
    cpu::state.HALTED = false;
    cpu::pushOntoStack(cpu::state.PC);
    cpu::state.PC = i;
}

void cpu::unhalt()
{
    cpu::state.HALTED = false;
}

instrInfo cpu::emulateOp()
{
    //Instructions can be 1 to 3 bytes
    byte firstByte = cpu::Memory->get8(cpu::state.PC);
    byte secondByte = cpu::Memory->get8(cpu::state.PC + 1);
    byte thirdByte = cpu::Memory->get8(cpu::state.PC + 2);
    //logging::log(ushortToString(cpu::state.PC));
    //look at the first byte for instruction type
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

        case 0xA8: return cpu::XOR_A_R(&cpu::state.B);
        case 0xA9: return cpu::XOR_A_R(&cpu::state.C);
        case 0xAA: return cpu::XOR_A_R(&cpu::state.D);
        case 0xAB: return cpu::XOR_A_R(&cpu::state.E);
        case 0xAC: return cpu::XOR_A_R(&cpu::state.H);
        case 0xAD: return cpu::XOR_A_R(&cpu::state.L);
        case 0xAF: return cpu::XOR_A_R(&cpu::state.A);

        case 0xEE: return cpu::XOR_A_N(secondByte);

        case 0xAE: return cpu::XOR_A_HL();

        case 0xB8: return cpu::CP_A_R(&cpu::state.B);
        case 0xB9: return cpu::CP_A_R(&cpu::state.C);
        case 0xBA: return cpu::CP_A_R(&cpu::state.D);
        case 0xBB: return cpu::CP_A_R(&cpu::state.E);
        case 0xBC: return cpu::CP_A_R(&cpu::state.H);
        case 0xBD: return cpu::CP_A_R(&cpu::state.L);
        case 0xBF: return cpu::CP_A_R(&cpu::state.A);

        case 0xFE: return cpu::CP_A_N(secondByte);

        case 0xBE: return cpu::CP_A_HL();

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

        case 0x09: return cpu::ADD_HL_SS(&cpu::state.BC);
        case 0x19: return cpu::ADD_HL_SS(&cpu::state.DE);
        case 0x29: return cpu::ADD_HL_SS(&cpu::state.HL);
        case 0x39: return cpu::ADD_HL_SS(&cpu::state.SP);

        case 0xE8: return cpu::ADD_SP_E(secondByte);
        
        case 0x03: return cpu::INC_SS(&cpu::state.BC);
        case 0x13: return cpu::INC_SS(&cpu::state.DE);
        case 0x23: return cpu::INC_SS(&cpu::state.HL);
        case 0x33: return cpu::INC_SS(&cpu::state.SP);

        case 0x0B: return cpu::DEC_SS(&cpu::state.BC);
        case 0x1B: return cpu::DEC_SS(&cpu::state.DE);
        case 0x2B: return cpu::DEC_SS(&cpu::state.HL);
        case 0x3B: return cpu::DEC_SS(&cpu::state.SP);

        case 0xC3: return cpu::JP_NN(combineBytes(thirdByte, secondByte));

        case 0xC2: return cpu::JP_CC_NN(cpu::getFlag(Z_flag) == 0, combineBytes(thirdByte, secondByte));
        case 0xCA: return cpu::JP_CC_NN(cpu::getFlag(Z_flag) == 1, combineBytes(thirdByte, secondByte));
        case 0xD2: return cpu::JP_CC_NN(cpu::getFlag(C_flag) == 0, combineBytes(thirdByte, secondByte));
        case 0xDA: return cpu::JP_CC_NN(cpu::getFlag(C_flag) == 1, combineBytes(thirdByte, secondByte));

        case 0x18: return cpu::JR_E(secondByte);

        case 0x20: return cpu::JR_CC_E(cpu::getFlag(Z_flag) == 0, secondByte);
        case 0x28: return cpu::JR_CC_E(cpu::getFlag(Z_flag) == 1, secondByte);
        case 0x30: return cpu::JR_CC_E(cpu::getFlag(C_flag) == 0, secondByte);
        case 0x38: return cpu::JR_CC_E(cpu::getFlag(C_flag) == 1, secondByte);

        case 0xE9: return cpu::JP_HL();

        case 0xCD: return cpu::CALL_NN(combineBytes(thirdByte, secondByte));

        case 0xC4: return cpu::CALL_CC_NN(cpu::getFlag(Z_flag) == 0, combineBytes(thirdByte, secondByte));
        case 0xCC: return cpu::CALL_CC_NN(cpu::getFlag(Z_flag) == 1, combineBytes(thirdByte, secondByte));
        case 0xD4: return cpu::CALL_CC_NN(cpu::getFlag(C_flag) == 0, combineBytes(thirdByte, secondByte));
        case 0xDC: return cpu::CALL_CC_NN(cpu::getFlag(C_flag) == 1, combineBytes(thirdByte, secondByte));

        case 0xC9: return cpu::RET();

        case 0xD9: return cpu::RETI();

        case 0xC0: return cpu::RET_CC(cpu::getFlag(Z_flag) == 0);
        case 0xC8: return cpu::RET_CC(cpu::getFlag(Z_flag) == 1);
        case 0xD0: return cpu::RET_CC(cpu::getFlag(C_flag) == 0);
        case 0xD8: return cpu::RET_CC(cpu::getFlag(C_flag) == 1);

        case 0xC7: return cpu::RST(0x0000);
        case 0xCF: return cpu::RST(0x0008);
        case 0xD7: return cpu::RST(0x0010);
        case 0xDF: return cpu::RST(0x0018);
        case 0xE7: return cpu::RST(0x0020);
        case 0xEF: return cpu::RST(0x0028);
        case 0xF7: return cpu::RST(0x0030);
        case 0xFF: return cpu::RST(0x0038);

        case 0x27: return cpu::DAA();

        case 0x2F: return cpu::CPL();

        case 0x00: return cpu::NOP();

        case 0x3F: return cpu::CCF();

        case 0x37: return cpu::SCF();

        case 0xF3: return cpu::DI();

        case 0xFB: return cpu::EI();

        case 0x76: return cpu::HALT();

        case 0x10: return cpu::STOP();

        case 0x07: return cpu::RLCA();
        case 0x17: return cpu::RLA();
        case 0x0F: return cpu::RRCA();
        case 0x1F: return cpu::RRA();

        case 0xCB:
            switch (secondByte)
            {
                case 0x00: return cpu::RLC_R(&cpu::state.B);
                case 0x01: return cpu::RLC_R(&cpu::state.C);
                case 0x02: return cpu::RLC_R(&cpu::state.D);
                case 0x03: return cpu::RLC_R(&cpu::state.E);
                case 0x04: return cpu::RLC_R(&cpu::state.H);
                case 0x05: return cpu::RLC_R(&cpu::state.L);
                case 0x07: return cpu::RLC_R(&cpu::state.A);

                case 0x06: return cpu::RLC_HL();

                case 0x10: return cpu::RL_R(&cpu::state.B);
                case 0x11: return cpu::RL_R(&cpu::state.C);
                case 0x12: return cpu::RL_R(&cpu::state.D);
                case 0x13: return cpu::RL_R(&cpu::state.E);
                case 0x14: return cpu::RL_R(&cpu::state.H);
                case 0x15: return cpu::RL_R(&cpu::state.L);
                case 0x17: return cpu::RL_R(&cpu::state.A);
                
                case 0x16: return cpu::RL_HL();

                case 0x08: return cpu::RRC_R(&cpu::state.B);
                case 0x09: return cpu::RRC_R(&cpu::state.C);
                case 0x0A: return cpu::RRC_R(&cpu::state.D);
                case 0x0B: return cpu::RRC_R(&cpu::state.E);
                case 0x0C: return cpu::RRC_R(&cpu::state.H);
                case 0x0D: return cpu::RRC_R(&cpu::state.L);
                case 0x0F: return cpu::RRC_R(&cpu::state.A);

                case 0x0E: return cpu::RRC_HL();

                case 0x18: return cpu::RR_R(&cpu::state.B);
                case 0x19: return cpu::RR_R(&cpu::state.C);
                case 0x1A: return cpu::RR_R(&cpu::state.D);
                case 0x1B: return cpu::RR_R(&cpu::state.E);
                case 0x1C: return cpu::RR_R(&cpu::state.H);
                case 0x1D: return cpu::RR_R(&cpu::state.L);
                case 0x1F: return cpu::RR_R(&cpu::state.A);

                case 0x1E: return cpu::RR_HL();
                
                case 0x20: return cpu::SLA_R(&cpu::state.B);
                case 0x21: return cpu::SLA_R(&cpu::state.C);
                case 0x22: return cpu::SLA_R(&cpu::state.D);
                case 0x23: return cpu::SLA_R(&cpu::state.E);
                case 0x24: return cpu::SLA_R(&cpu::state.H);
                case 0x25: return cpu::SLA_R(&cpu::state.L);
                case 0x27: return cpu::SLA_R(&cpu::state.A);

                case 0x26: return cpu::SLA_HL();

                case 0x28: return cpu::SRA_R(&cpu::state.B);
                case 0x29: return cpu::SRA_R(&cpu::state.C);
                case 0x2A: return cpu::SRA_R(&cpu::state.D);
                case 0x2B: return cpu::SRA_R(&cpu::state.E);
                case 0x2C: return cpu::SRA_R(&cpu::state.H);
                case 0x2D: return cpu::SRA_R(&cpu::state.L);
                case 0x2F: return cpu::SRA_R(&cpu::state.A);
                
                case 0x2E: return cpu::SRA_HL();

                case 0x38: return cpu::SRL_R(&cpu::state.B);
                case 0x39: return cpu::SRL_R(&cpu::state.C);
                case 0x3A: return cpu::SRL_R(&cpu::state.D);
                case 0x3B: return cpu::SRL_R(&cpu::state.E);
                case 0x3C: return cpu::SRL_R(&cpu::state.H);
                case 0x3D: return cpu::SRL_R(&cpu::state.L);
                case 0x3F: return cpu::SRL_R(&cpu::state.A);

                case 0x3E: return cpu::SRL_HL();
                
                case 0x30: return cpu::SWAP_R(&cpu::state.B);
                case 0x31: return cpu::SWAP_R(&cpu::state.C);
                case 0x32: return cpu::SWAP_R(&cpu::state.D);
                case 0x33: return cpu::SWAP_R(&cpu::state.E);
                case 0x34: return cpu::SWAP_R(&cpu::state.H);
                case 0x35: return cpu::SWAP_R(&cpu::state.L);
                case 0x37: return cpu::SWAP_R(&cpu::state.A);

                case 0x36: return cpu::SWAP_HL();

                case 0x40: return cpu::BIT_B_R(0, &cpu::state.B);
                case 0x41: return cpu::BIT_B_R(0, &cpu::state.C);
                case 0x42: return cpu::BIT_B_R(0, &cpu::state.D);
                case 0x43: return cpu::BIT_B_R(0, &cpu::state.E);
                case 0x44: return cpu::BIT_B_R(0, &cpu::state.H);
                case 0x45: return cpu::BIT_B_R(0, &cpu::state.L);
                case 0x47: return cpu::BIT_B_R(0, &cpu::state.A);
                case 0x48: return cpu::BIT_B_R(1, &cpu::state.B);
                case 0x49: return cpu::BIT_B_R(1, &cpu::state.C);
                case 0x4A: return cpu::BIT_B_R(1, &cpu::state.D);
                case 0x4B: return cpu::BIT_B_R(1, &cpu::state.E);
                case 0x4C: return cpu::BIT_B_R(1, &cpu::state.H);
                case 0x4D: return cpu::BIT_B_R(1, &cpu::state.L);
                case 0x4F: return cpu::BIT_B_R(1, &cpu::state.A);
                case 0x50: return cpu::BIT_B_R(2, &cpu::state.B);
                case 0x51: return cpu::BIT_B_R(2, &cpu::state.C);
                case 0x52: return cpu::BIT_B_R(2, &cpu::state.D);
                case 0x53: return cpu::BIT_B_R(2, &cpu::state.E);
                case 0x54: return cpu::BIT_B_R(2, &cpu::state.H);
                case 0x55: return cpu::BIT_B_R(2, &cpu::state.L);
                case 0x57: return cpu::BIT_B_R(2, &cpu::state.A);
                case 0x58: return cpu::BIT_B_R(3, &cpu::state.B);
                case 0x59: return cpu::BIT_B_R(3, &cpu::state.C);
                case 0x5A: return cpu::BIT_B_R(3, &cpu::state.D);
                case 0x5B: return cpu::BIT_B_R(3, &cpu::state.E);
                case 0x5C: return cpu::BIT_B_R(3, &cpu::state.H);
                case 0x5D: return cpu::BIT_B_R(3, &cpu::state.L);
                case 0x5F: return cpu::BIT_B_R(3, &cpu::state.A);
                case 0x60: return cpu::BIT_B_R(4, &cpu::state.B);
                case 0x61: return cpu::BIT_B_R(4, &cpu::state.C);
                case 0x62: return cpu::BIT_B_R(4, &cpu::state.D);
                case 0x63: return cpu::BIT_B_R(4, &cpu::state.E);
                case 0x64: return cpu::BIT_B_R(4, &cpu::state.H);
                case 0x65: return cpu::BIT_B_R(4, &cpu::state.L);
                case 0x67: return cpu::BIT_B_R(4, &cpu::state.A);
                case 0x68: return cpu::BIT_B_R(5, &cpu::state.B);
                case 0x69: return cpu::BIT_B_R(5, &cpu::state.C);
                case 0x6A: return cpu::BIT_B_R(5, &cpu::state.D);
                case 0x6B: return cpu::BIT_B_R(5, &cpu::state.E);
                case 0x6C: return cpu::BIT_B_R(5, &cpu::state.H);
                case 0x6D: return cpu::BIT_B_R(5, &cpu::state.L);
                case 0x6F: return cpu::BIT_B_R(5, &cpu::state.A);
                case 0x70: return cpu::BIT_B_R(6, &cpu::state.B);
                case 0x71: return cpu::BIT_B_R(6, &cpu::state.C);
                case 0x72: return cpu::BIT_B_R(6, &cpu::state.D);
                case 0x73: return cpu::BIT_B_R(6, &cpu::state.E);
                case 0x74: return cpu::BIT_B_R(6, &cpu::state.H);
                case 0x75: return cpu::BIT_B_R(6, &cpu::state.L);
                case 0x77: return cpu::BIT_B_R(6, &cpu::state.A);
                case 0x78: return cpu::BIT_B_R(7, &cpu::state.B);
                case 0x79: return cpu::BIT_B_R(7, &cpu::state.C);
                case 0x7A: return cpu::BIT_B_R(7, &cpu::state.D);
                case 0x7B: return cpu::BIT_B_R(7, &cpu::state.E);
                case 0x7C: return cpu::BIT_B_R(7, &cpu::state.H);
                case 0x7D: return cpu::BIT_B_R(7, &cpu::state.L);
                case 0x7F: return cpu::BIT_B_R(7, &cpu::state.A);

                case 0x46: return cpu::BIT_B_HL(0);
                case 0x4E: return cpu::BIT_B_HL(1);
                case 0x56: return cpu::BIT_B_HL(2);
                case 0x5E: return cpu::BIT_B_HL(3);
                case 0x66: return cpu::BIT_B_HL(4);
                case 0x6E: return cpu::BIT_B_HL(5);
                case 0x76: return cpu::BIT_B_HL(6);
                case 0x7E: return cpu::BIT_B_HL(7);

                case 0xC0: return cpu::SET_B_R(0, &cpu::state.B);
                case 0xC1: return cpu::SET_B_R(0, &cpu::state.C);
                case 0xC2: return cpu::SET_B_R(0, &cpu::state.D);
                case 0xC3: return cpu::SET_B_R(0, &cpu::state.E);
                case 0xC4: return cpu::SET_B_R(0, &cpu::state.H);
                case 0xC5: return cpu::SET_B_R(0, &cpu::state.L);
                case 0xC7: return cpu::SET_B_R(0, &cpu::state.A);
                case 0xC8: return cpu::SET_B_R(1, &cpu::state.B);
                case 0xC9: return cpu::SET_B_R(1, &cpu::state.C);
                case 0xCA: return cpu::SET_B_R(1, &cpu::state.D);
                case 0xCB: return cpu::SET_B_R(1, &cpu::state.E);
                case 0xCC: return cpu::SET_B_R(1, &cpu::state.H);
                case 0xCD: return cpu::SET_B_R(1, &cpu::state.L);
                case 0xCF: return cpu::SET_B_R(1, &cpu::state.A);
                case 0xD0: return cpu::SET_B_R(2, &cpu::state.B);
                case 0xD1: return cpu::SET_B_R(2, &cpu::state.C);
                case 0xD2: return cpu::SET_B_R(2, &cpu::state.D);
                case 0xD3: return cpu::SET_B_R(2, &cpu::state.E);
                case 0xD4: return cpu::SET_B_R(2, &cpu::state.H);
                case 0xD5: return cpu::SET_B_R(2, &cpu::state.L);
                case 0xD7: return cpu::SET_B_R(2, &cpu::state.A);
                case 0xD8: return cpu::SET_B_R(3, &cpu::state.B);
                case 0xD9: return cpu::SET_B_R(3, &cpu::state.C);
                case 0xDA: return cpu::SET_B_R(3, &cpu::state.D);
                case 0xDB: return cpu::SET_B_R(3, &cpu::state.E);
                case 0xDC: return cpu::SET_B_R(3, &cpu::state.H);
                case 0xDD: return cpu::SET_B_R(3, &cpu::state.L);
                case 0xDF: return cpu::SET_B_R(3, &cpu::state.A);
                case 0xE0: return cpu::SET_B_R(4, &cpu::state.B);
                case 0xE1: return cpu::SET_B_R(4, &cpu::state.C);
                case 0xE2: return cpu::SET_B_R(4, &cpu::state.D);
                case 0xE3: return cpu::SET_B_R(4, &cpu::state.E);
                case 0xE4: return cpu::SET_B_R(4, &cpu::state.H);
                case 0xE5: return cpu::SET_B_R(4, &cpu::state.L);
                case 0xE7: return cpu::SET_B_R(4, &cpu::state.A);
                case 0xE8: return cpu::SET_B_R(5, &cpu::state.B);
                case 0xE9: return cpu::SET_B_R(5, &cpu::state.C);
                case 0xEA: return cpu::SET_B_R(5, &cpu::state.D);
                case 0xEB: return cpu::SET_B_R(5, &cpu::state.E);
                case 0xEC: return cpu::SET_B_R(5, &cpu::state.H);
                case 0xED: return cpu::SET_B_R(5, &cpu::state.L);
                case 0xEF: return cpu::SET_B_R(5, &cpu::state.A);
                case 0xF0: return cpu::SET_B_R(6, &cpu::state.B);
                case 0xF1: return cpu::SET_B_R(6, &cpu::state.C);
                case 0xF2: return cpu::SET_B_R(6, &cpu::state.D);
                case 0xF3: return cpu::SET_B_R(6, &cpu::state.E);
                case 0xF4: return cpu::SET_B_R(6, &cpu::state.H);
                case 0xF5: return cpu::SET_B_R(6, &cpu::state.L);
                case 0xF7: return cpu::SET_B_R(6, &cpu::state.A);
                case 0xF8: return cpu::SET_B_R(7, &cpu::state.B);
                case 0xF9: return cpu::SET_B_R(7, &cpu::state.C);
                case 0xFA: return cpu::SET_B_R(7, &cpu::state.D);
                case 0xFB: return cpu::SET_B_R(7, &cpu::state.E);
                case 0xFC: return cpu::SET_B_R(7, &cpu::state.H);
                case 0xFD: return cpu::SET_B_R(7, &cpu::state.L);
                case 0xFF: return cpu::SET_B_R(7, &cpu::state.A);

                case 0xC6: return cpu::SET_B_HL(0);
                case 0xCE: return cpu::SET_B_HL(1);
                case 0xD6: return cpu::SET_B_HL(2);
                case 0xDE: return cpu::SET_B_HL(3);
                case 0xE6: return cpu::SET_B_HL(4);
                case 0xEE: return cpu::SET_B_HL(5);
                case 0xF6: return cpu::SET_B_HL(6);
                case 0xFE: return cpu::SET_B_HL(7);

                case 0x80: return cpu::RES_B_R(0, &cpu::state.B);
                case 0x81: return cpu::RES_B_R(0, &cpu::state.C);
                case 0x82: return cpu::RES_B_R(0, &cpu::state.D);
                case 0x83: return cpu::RES_B_R(0, &cpu::state.E);
                case 0x84: return cpu::RES_B_R(0, &cpu::state.H);
                case 0x85: return cpu::RES_B_R(0, &cpu::state.L);
                case 0x87: return cpu::RES_B_R(0, &cpu::state.A);
                case 0x88: return cpu::RES_B_R(1, &cpu::state.B);
                case 0x89: return cpu::RES_B_R(1, &cpu::state.C);
                case 0x8A: return cpu::RES_B_R(1, &cpu::state.D);
                case 0x8B: return cpu::RES_B_R(1, &cpu::state.E);
                case 0x8C: return cpu::RES_B_R(1, &cpu::state.H);
                case 0x8D: return cpu::RES_B_R(1, &cpu::state.L);
                case 0x8F: return cpu::RES_B_R(1, &cpu::state.A);
                case 0x90: return cpu::RES_B_R(2, &cpu::state.B);
                case 0x91: return cpu::RES_B_R(2, &cpu::state.C);
                case 0x92: return cpu::RES_B_R(2, &cpu::state.D);
                case 0x93: return cpu::RES_B_R(2, &cpu::state.E);
                case 0x94: return cpu::RES_B_R(2, &cpu::state.H);
                case 0x95: return cpu::RES_B_R(2, &cpu::state.L);
                case 0x97: return cpu::RES_B_R(2, &cpu::state.A);
                case 0x98: return cpu::RES_B_R(3, &cpu::state.B);
                case 0x99: return cpu::RES_B_R(3, &cpu::state.C);
                case 0x9A: return cpu::RES_B_R(3, &cpu::state.D);
                case 0x9B: return cpu::RES_B_R(3, &cpu::state.E);
                case 0x9C: return cpu::RES_B_R(3, &cpu::state.H);
                case 0x9D: return cpu::RES_B_R(3, &cpu::state.L);
                case 0x9F: return cpu::RES_B_R(3, &cpu::state.A);
                case 0xA0: return cpu::RES_B_R(4, &cpu::state.B);
                case 0xA1: return cpu::RES_B_R(4, &cpu::state.C);
                case 0xA2: return cpu::RES_B_R(4, &cpu::state.D);
                case 0xA3: return cpu::RES_B_R(4, &cpu::state.E);
                case 0xA4: return cpu::RES_B_R(4, &cpu::state.H);
                case 0xA5: return cpu::RES_B_R(4, &cpu::state.L);
                case 0xA7: return cpu::RES_B_R(4, &cpu::state.A);
                case 0xA8: return cpu::RES_B_R(5, &cpu::state.B);
                case 0xA9: return cpu::RES_B_R(5, &cpu::state.C);
                case 0xAA: return cpu::RES_B_R(5, &cpu::state.D);
                case 0xAB: return cpu::RES_B_R(5, &cpu::state.E);
                case 0xAC: return cpu::RES_B_R(5, &cpu::state.H);
                case 0xAD: return cpu::RES_B_R(5, &cpu::state.L);
                case 0xAF: return cpu::RES_B_R(5, &cpu::state.A);
                case 0xB0: return cpu::RES_B_R(6, &cpu::state.B);
                case 0xB1: return cpu::RES_B_R(6, &cpu::state.C);
                case 0xB2: return cpu::RES_B_R(6, &cpu::state.D);
                case 0xB3: return cpu::RES_B_R(6, &cpu::state.E);
                case 0xB4: return cpu::RES_B_R(6, &cpu::state.H);
                case 0xB5: return cpu::RES_B_R(6, &cpu::state.L);
                case 0xB7: return cpu::RES_B_R(6, &cpu::state.A);
                case 0xB8: return cpu::RES_B_R(7, &cpu::state.B);
                case 0xB9: return cpu::RES_B_R(7, &cpu::state.C);
                case 0xBA: return cpu::RES_B_R(7, &cpu::state.D);
                case 0xBB: return cpu::RES_B_R(7, &cpu::state.E);
                case 0xBC: return cpu::RES_B_R(7, &cpu::state.H);
                case 0xBD: return cpu::RES_B_R(7, &cpu::state.L);
                case 0xBF: return cpu::RES_B_R(7, &cpu::state.A);

                case 0x86: return cpu::RES_B_HL(0);
                case 0x8E: return cpu::RES_B_HL(1);
                case 0x96: return cpu::RES_B_HL(2);
                case 0x9E: return cpu::RES_B_HL(3);
                case 0xA6: return cpu::RES_B_HL(4);
                case 0xAE: return cpu::RES_B_HL(5);
                case 0xB6: return cpu::RES_B_HL(6);
                case 0xBE: return cpu::RES_B_HL(7);

                default: logging::logerr("Unimplemented CB instruction: " + byteToString(firstByte) + byteToString(secondByte)); return cpu::NOP();
            }
        case 0xD3: case 0xE3: case 0xE4: case 0xF4: case 0xDB: case 0xDD: case 0xEB: case 0xEC: case 0xED: case 0xFC: case 0xFD: 
            logging::logerr("Illegal instruction: " + byteToString(firstByte));
            return cpu::NOP();
        default: logging::logerr("Unimplemented instruction: " + byteToString(firstByte)); return cpu::NOP();
    }
}

instrInfo cpu::step()
{
    if(false)//if (cpu::state.PC == 0x0286) //breakpoint
    {
        debug = true;
    }
    if (debug)
    {
        //std::cin.ignore();
        SDL_Delay(100);
        logging::log("AF: " + ushortToString(cpu::state.AF), false);
        logging::log(" BC: " + ushortToString(cpu::state.BC), false);
        logging::log(" DE: " + ushortToString(cpu::state.DE), false);
        logging::log(" HL: " + ushortToString(cpu::state.HL), false);
        logging::log(" SP: " + ushortToString(cpu::state.SP), false);
        logging::log(" PC: " + ushortToString(cpu::state.PC), false);
    }
    ushort pc = cpu::state.PC;

    instrInfo info = cpu::emulateOp();
    if (debug)
    {
        logging::log(" "  + byteToString(cpu::Memory->get8(pc)), false);
        if (info.numBytes > 1)
        {
            logging::log(" "  + byteToString(cpu::Memory->get8(pc + 1)), false);
            if (info.numBytes > 2)
            {
                logging::log(" "  + byteToString(cpu::Memory->get8(pc + 2)), false);
            }
        }
        logging::log(""); //newline
    }
    if (info.incPC)
    {
        cpu::state.PC += info.numBytes;
    }
    return info;
}

void cpu::setFlag(byte flag, bool value)
{
    cpu::state.F = setBit(cpu::state.F, flag, value);
}

bool cpu::getFlag(byte flag)
{
    return getBit(cpu::state.F, flag);
}

void cpu::pushOntoStack(ushort value)
{
    //cpu::Memory->set8(cpu::state.SP - 1, highByte(value));
    //cpu::Memory->set8(cpu::state.SP - 2, lowByte(value));
    cpu::state.SP -= 2;
    cpu::Memory->set16(cpu::state.SP, value);
}
ushort cpu::popOffStack()
{
    ushort ret = cpu::Memory->get16(cpu::state.SP);
    cpu::state.SP += 2;
    return ret;
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
    cpu::pushOntoStack(*regPair);
    return {1,16};
}
//Pops from the stack into regPair
instrInfo cpu::POP_QQ(ushort* regPair)
{
    *regPair = combineBytes(cpu::Memory->get8(cpu::state.SP + 1), cpu::Memory->get8(cpu::state.SP));
    cpu::state.SP += 2;
    cpu::state.F &= 0xF0; //just in case F was changed, make sure last 4 bits are 0
    return {1,12};
}
//The result of (SP + n) is saved in HL
//Flags: Z = 0, H if carry bit 11, N = 0, C if carry bit 15
instrInfo cpu::LDHL_SP_N(byte n)
{
    sbyte s = static_cast<sbyte>(n);
    ushort result = cpu::state.SP + s;
    cpu::setFlag(Z_flag, 0);
    cpu::setFlag(N_flag, 0);
    //cpu::setFlag(C_flag, ((s + cpu::state.SP) & 0x10000) == 0x10000);
    //cpu::setFlag(H_flag, ((s + (cpu::state.SP&0xFFF)) & 0x1000) == 0x1000);
    ushort check = cpu::state.SP ^ s ^ ((cpu::state.SP + s) & 0xFFFF);
    cpu::setFlag(C_flag, (check & 0x100) == 0x100);
    cpu::setFlag(H_flag, ((check & 0x10) == 0x10));
    cpu::state.HL = result;
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
    cpu::setFlag(H_flag, (((*srcReg&0xF) + (cpu::state.A&0xF)) & 0x10) == 0x10);
    //cpu::setFlag(C_flag, (*srcReg + cpu::state.A) > 0xFF);
    //cpu::setFlag(H_flag, ((*srcReg&0xF) + (cpu::state.A&0xF)) > 0xF);                alternate approach
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
    cpu::setFlag(H_flag, (((n&0xF) + (cpu::state.A&0xF)) & 0x10) == 0x10);
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
    cpu::setFlag(H_flag, (((n&0xF) + (cpu::state.A&0xF)) & 0x10) == 0x10);
    cpu::state.A += n;
    return {1,8};
}
//Adds srcReg to A and the carry flag and stores result in A
//Flags: Z if result is 0, H if carry bit 3, N = 0, C if carry bit 7
instrInfo cpu::ADC_A_R(byte* srcReg)
{
    //int fullResult = cpu::state.A + *srcReg + cpu::getFlag(C_flag);
    //byte result = static_cast<byte>(fullResult);
    byte result = cpu::state.A + *srcReg + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(H_flag, (((cpu::state.A&0xF) + (*srcReg&0xF) + cpu::getFlag(C_flag)) & 0x10) == 0x10);
    cpu::setFlag(C_flag, ((cpu::state.A + *srcReg + cpu::getFlag(C_flag)) & 0x100) == 0x100);
    //cpu::setFlag(C_flag, fullResult > 0xFF);
    //cpu::setFlag(H_flag, ((cpu::state.A&0xF) + (*srcReg&0xF) + cpu::getFlag(C_flag)) > 0xF);
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
    cpu::setFlag(H_flag, (((cpu::state.A&0xF) + (n&0xF) + cpu::getFlag(C_flag)) & 0x10) == 0x10);
    cpu::setFlag(C_flag, ((cpu::state.A + n + cpu::getFlag(C_flag)) & 0x100) == 0x100);
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
    cpu::setFlag(H_flag, (((cpu::state.A&0xF) + (hl&0xF) + cpu::getFlag(C_flag)) & 0x10) == 0x10);
    cpu::setFlag(C_flag, ((cpu::state.A + hl + cpu::getFlag(C_flag)) & 0x100) == 0x100);
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
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (*srcReg&0xF)) < 0);
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
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (n&0xF)) < 0);
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
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (hl&0xF)) < 0);
    cpu::state.A -= hl;
    return {1,8};
}
//Subtracts (srcReg + carry) from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SBC_A_R(byte* srcReg)
{
    /*
    byte toSub = *srcReg + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - toSub) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - toSub) < 0);
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (toSub&0xF)) < 0);
    cpu::state.A -= toSub;
    */
    byte result = cpu::state.A - *srcReg - cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(H_flag, (static_cast<byte>((cpu::state.A&0xF) - (*srcReg&0xF) - cpu::getFlag(C_flag)) > 0xF));
    cpu::setFlag(C_flag, (static_cast<uint16_t>(cpu::state.A - *srcReg - cpu::getFlag(C_flag)) > 0xff));
    cpu::state.A = result;
    return {1,4};
}
//Subtracts (n + carry) from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SBC_A_N(byte n)
{
    byte result = cpu::state.A - n - cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(H_flag, (static_cast<byte>((cpu::state.A&0xF) - (n&0xF) - cpu::getFlag(C_flag)) > 0xF));
    cpu::setFlag(C_flag, (static_cast<uint16_t>(cpu::state.A - n - cpu::getFlag(C_flag)) > 0xff));
    cpu::state.A = result;
    return {2,8};
}
//Subtracts ((value in mem addr HL) + carry) from A, stores in A
//Flags: Z if result is 0, H if carry bit 4, N = 1, C if carry
instrInfo cpu::SBC_A_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = cpu::state.A - hl - cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(H_flag, (static_cast<byte>((cpu::state.A&0xF) - (hl&0xF) - cpu::getFlag(C_flag)) > 0xF));
    cpu::setFlag(C_flag, (static_cast<uint16_t>(cpu::state.A - hl - cpu::getFlag(C_flag)) > 0xff));
    cpu::state.A = result;
    return {1,8};
    /*
    byte toSub = cpu::Memory->get8(cpu::state.HL) + cpu::getFlag(C_flag);
    cpu::setFlag(Z_flag, (byte)(cpu::state.A - toSub) == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(C_flag, (cpu::state.A - toSub) < 0);
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (toSub&0xF)) < 0);
    cpu::state.A -= toSub;
    */
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
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (*srcReg&0xF)) < 0);
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
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (n&0xF)) < 0);
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
    cpu::setFlag(H_flag, ((cpu::state.A&0xF) - (hl&0xF)) < 0);
    return {1,8};
}
//Increments srcReg
//Flags: Z if result it 0, H if carry bit 3, N = 0
instrInfo cpu::INC_R(byte* srcReg)
{
    byte result = *srcReg + 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(H_flag, (((*srcReg&0xF) + 1) & 0x10) == 0x10);
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
    cpu::setFlag(H_flag, (((cpu::Memory->get8(cpu::state.HL)&0xF) + 1) & 0x10) == 0x10);
    cpu::Memory->set8(cpu::state.HL, result);
    return {1,12};
}
//Decrements srcReg
//Flags: Z if result it 0, H if carry bit 4, N = 1
instrInfo cpu::DEC_R(byte* srcReg)
{
    byte result = *srcReg - 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(H_flag, ((*srcReg&0xF) - 1) < 0);
    *srcReg = result;
    return {1,4};
}
//Decrements (value of memaddress in HL)
//Flags: Z if result it 0, H if carry bit 4, N = 1
instrInfo cpu::DEC_HL()
{
    byte result = cpu::Memory->get8(cpu::state.HL) - 1;
    cpu::setFlag(Z_flag, result == 0);
    cpu::setFlag(N_flag, 1);
    cpu::setFlag(H_flag, ((cpu::Memory->get8(cpu::state.HL)&0xF) - 1) < 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {1,12};
}

// 16 Bit Arithmetic

//Add the given register pair to HL, save result in HL
//Flags: N = 0, H if carry bit 11,  C if carry bit 15
instrInfo cpu::ADD_HL_SS(ushort* regPair)
{
    //ushort result = *regPair + cpu::state.HL;
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, ((*regPair + cpu::state.HL) & 0x10000) == 0x10000);
    cpu::setFlag(H_flag, (((*regPair&0xFFF) + (cpu::state.HL&0xFFF)) & 0x1000) == 0x1000);
    cpu::state.HL += *regPair;
    return {1,8};
}
//Adds e to SP, stores in SP
//Flags: N = 0, Z = 0, H if carry bit 11, C if carry bit 15
instrInfo cpu::ADD_SP_E(byte e)
{
    sbyte n = static_cast<sbyte>(e);
    ushort result = cpu::state.SP + n;
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, 0);
    //cpu::setFlag(C_flag, ((cpu::state.SP + n) & 0x10000) == 0x10000);
    //cpu::setFlag(H_flag, (((cpu::state.SP&0xFFF) + n) & 0x1000) == 0x1000);
    cpu::setFlag(C_flag, (result&0xFF) < (cpu::state.SP&0xFF));
    cpu::setFlag(H_flag, (result&0xF) < (cpu::state.SP&0xF));
    cpu::state.SP = result;
    return {2,16};
}
//Increment regPair
instrInfo cpu::INC_SS(ushort* regPair)
{
    *regPair += 1;
    return {1,8};
}
//Decrement regPair
instrInfo cpu::DEC_SS(ushort* regPair)
{
    *regPair -= 1;
    return {1,8};
}

// Jump Instructions

//Sets the program counter to dest
instrInfo cpu::JP_NN(ushort dest)
{
    cpu::state.PC = dest;
    return {3,16,false};
}

//Sets program counter to dest if condition is true
instrInfo cpu::JP_CC_NN(bool condition, ushort dest)
{
    if (condition)
    {
        cpu::state.PC = dest;
        return {3,16,false};
    }
    return {3,12,true};
}
//Jumps a signed amount relative to the memory address after the instruction
instrInfo cpu::JR_E(byte dest)
{
    sbyte relativeDest = static_cast<sbyte>(dest);
    cpu::state.PC = cpu::state.PC + 2 + relativeDest;
    return {2,12,false};
}
//Jumps a signed amount relative to the memory address after the instruction if condition is true
instrInfo cpu::JR_CC_E(bool condition, byte dest)
{
    if (condition)
    {
        sbyte relativeDest = static_cast<sbyte>(dest);
        cpu::state.PC = cpu::state.PC + 2 + relativeDest;
        return {2,12,false};
    }
    return {2,8,true};
}
//Jumps to the memory address in HL
instrInfo cpu::JP_HL()
{
    cpu::state.PC = cpu::state.HL;
    return {1,4,false};
}

// Call and Return Instructions

//Jumps to subroutine at dest, puts address after the CALL on the stack
instrInfo cpu::CALL_NN(ushort dest)
{
    cpu::pushOntoStack(cpu::state.PC + 3);
    cpu::state.PC = dest;
    return {3, 24, false};
}
//Jumps to subroutine at dest, puts address after the CALL on the stack (if condition is true)
instrInfo cpu::CALL_CC_NN(bool condition, ushort dest)
{
    if (condition)
    {
        return cpu::CALL_NN(dest);
    }
    return {3,12, true};
}

//Pops the return address from the stack and sets the PC to that.
instrInfo cpu::RET()
{
    //cpu::state.PC = cpu::Memory->get16(cpu::state.SP);
    //cpu::state.SP += 2;
    cpu::state.PC = cpu::popOffStack();
    return {1,16, false};
}
//Enables interrupts and returns. Same as doing EI and RET but only takes as many cycles as doing RET
instrInfo cpu::RETI()
{
    cpu::EI();
    return cpu::RET();
}
//Returns if condition is true.
instrInfo cpu::RET_CC(bool condition)
{
    if (condition)
    {
        //this one takes 4 additional cycles
        instrInfo i = cpu::RET();
        i.numCycles += 4;
        return i;
    }
    return {1,8};
}
//Similar to CALL, but goes to predefined spots in page 0 memory
instrInfo cpu::RST(ushort dest)
{
    //cpu::state.SP -= 2;
    //cpu::Memory->set16(cpu::state.SP, cpu::state.PC + 1);
    cpu::pushOntoStack(cpu::state.PC + 1);
    cpu::state.PC = dest;
    return {1, 16, false};
}

// General Purpose Arithmetic & CPU Control

//Something about BCD values? This one is a mystery. Copied the code for it from https://forums.nesdev.com/viewtopic.php?f=20&t=15944
//Flags: ?????
instrInfo cpu::DAA()
{
    byte a = cpu::state.A;
    if (!cpu::getFlag(N_flag))
    {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
        if (cpu::getFlag(C_flag) || a > 0x99) { a += 0x60; cpu::setFlag(C_flag, 1); }
        if (cpu::getFlag(H_flag) || (a & 0x0f) > 0x09) { a += 0x6; }
    } 
    else // after a subtraction, only adjust if (half-)carry occurred
    {
        if (cpu::getFlag(C_flag)) { a -= 0x60; }
        if (cpu::getFlag(H_flag)) { a -= 0x6; }
    }
    cpu::setFlag(Z_flag, a == 0);
    cpu::setFlag(H_flag, 0);
    cpu::state.A = a;
    return {1,4};
}
//Bit-flips A, stores in A
//Flags: H = 1, N = 1
instrInfo cpu::CPL()
{
    cpu::state.A = ~cpu::state.A;
    cpu::setFlag(H_flag, 1);
    cpu::setFlag(N_flag, 1);
    return {1,4};
}
//does nothing
instrInfo cpu::NOP()
{
    return {1,4};
}
//Flips the carry flag
//Flags: C = ~C, H = 0, N = 0
instrInfo cpu::CCF()
{
    cpu::setFlag(C_flag, !cpu::getFlag(C_flag));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    return {1,4};
}
//Sets the carry flag to 1
//Flags: C = 1, H = 0, N = 0
instrInfo cpu::SCF()
{
    cpu::setFlag(C_flag, 1);
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    return {1,4};
}
//Disables master interrupt flag
instrInfo cpu::DI()
{
    cpu::state.IME = 0;
    return {1,4};
}
//Enables master interrupt flag
instrInfo cpu::EI()
{
    cpu::state.IME = 1;
    return {1,4};
}
//Halts the CPU. Interrupts pull it back into normal operation.
//TODO: HALT bug. This bug is triggered when a HALT instruction is executed while interrupts are disabled,
//and there are pending interrupts waiting to be serviced. That is, IF & IE != 0. In this case, the CPU will not halt,
//and instead fail to increment the PC, causing the next instruction to be executed twice. It does not execute HALT twice, 
//because the PC has already incremented during instruction decoding.
instrInfo cpu::HALT()
{
    cpu::state.HALTED = true;
    return {1,4};
}
//Stops the CPU. A button press pulls it back into normal operation.
instrInfo cpu::STOP()
{
    cpu::state.STOPPED = true;
    return {1,4};
}

// Rotate Shift Instructions

//Rotates A to the left.
//Flags: H = 0, N = 0, Z = 0, C = bit 7 of A before operation
instrInfo cpu::RLCA()
{
    byte result = (cpu::state.A << 1) | (cpu::state.A >> 7);
    cpu::setFlag(C_flag, getBit(cpu::state.A, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, 0);
    cpu::state.A = result;
    return {1,4};
}
//Rotates A to the left, replacing bit 0 with carry flag.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of A before operation
instrInfo cpu::RLA()
{
    byte result = (cpu::state.A << 1) | cpu::getFlag(C_flag);
    cpu::setFlag(C_flag, getBit(cpu::state.A, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, 0);
    cpu::state.A = result;
    return {1,4};
}
//Rotates A to the right.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of A before operation.
instrInfo cpu::RRCA()
{
    byte result = (cpu::state.A >> 1) | (cpu::state.A << 7);
    cpu::setFlag(C_flag, getBit(cpu::state.A, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, 0);
    cpu::state.A = result;
    return {1,4};
}
//Rotates A to the right, replacing bit 7 with the carry flag.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of A before operation.
instrInfo cpu::RRA()
{
    byte result = (cpu::state.A >> 1) | (cpu::getFlag(C_flag) << 7);
    cpu::setFlag(C_flag, getBit(cpu::state.A, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, 0);
    cpu::state.A = result;
    return {1,4};
}
//Rotates srcReg to the left.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of srcReg before operation
instrInfo cpu::RLC_R(byte* srcReg)
{
    byte result = (*srcReg << 1) | (*srcReg >> 7);
    cpu::setFlag(C_flag, getBit(*srcReg, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Rotates (value of memaddress in HL) to the left.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of (value of memaddress in HL) before operation
instrInfo cpu::RLC_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl << 1) | (hl >> 7);
    cpu::setFlag(C_flag, getBit(hl, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Rotates srcReg to the left, replacing bit 0 with carry flag.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of srcReg before operation
instrInfo cpu::RL_R(byte* srcReg)
{
    byte result = (*srcReg << 1) | cpu::getFlag(C_flag);
    cpu::setFlag(C_flag, getBit(*srcReg, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Rotates (value of memaddress in HL) to the left, replacing bit 0 with carry flag.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of (value of memaddress in HL) before operation
instrInfo cpu::RL_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl << 1) | cpu::getFlag(C_flag);
    cpu::setFlag(C_flag, getBit(hl, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Rotates srcReg to the right.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of srcReg before operation.
instrInfo cpu::RRC_R(byte* srcReg)
{
    byte result = (*srcReg >> 1) | (*srcReg << 7);
    cpu::setFlag(C_flag, getBit(*srcReg, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Rotates (value of memaddress in HL) to the right.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of (value of memaddress in HL) before operation.
instrInfo cpu::RRC_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl >> 1) | (hl << 7);
    cpu::setFlag(C_flag, getBit(hl, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Rotates srcReg to the right, replacing bit 7 with the carry flag.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of srcReg before operation.
instrInfo cpu::RR_R(byte* srcReg)
{
    byte result = (*srcReg >> 1) | (cpu::getFlag(C_flag) << 7);
    cpu::setFlag(C_flag, getBit(*srcReg, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Rotates (value of memaddress in HL) to the right, replacing bit 7 with the carry flag.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of (value of memaddress in HL) before operation.
instrInfo cpu::RR_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl >> 1) | (cpu::getFlag(C_flag) << 7);
    cpu::setFlag(C_flag, getBit(hl, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Shifts srcReg to the left.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of srcReg before operation.
instrInfo cpu::SLA_R(byte* srcReg)
{
    byte result = (*srcReg << 1);
    cpu::setFlag(C_flag, getBit(*srcReg, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Shifts (value of memaddress in HL) to the left.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 7 of (value of memaddress in HL) before operation.
instrInfo cpu::SLA_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl << 1);
    cpu::setFlag(C_flag, getBit(hl, 7));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Shifts srcReg to the right, but leave bit 7 unchanged.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of srcReg before operation.
instrInfo cpu::SRA_R(byte* srcReg)
{
    byte result = (*srcReg >> 1) | (*srcReg & 0x80);
    cpu::setFlag(C_flag, getBit(*srcReg, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Shifts (value of memaddress in HL) to the right, but leave bit 7 unchanged.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of (value of memaddress in HL) before operation.
instrInfo cpu::SRA_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl >> 1) | (hl & 0x80);
    cpu::setFlag(C_flag, getBit(hl, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Shifts srcReg to the right.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of srcReg before operation.
instrInfo cpu::SRL_R(byte* srcReg)
{
    byte result = (*srcReg >> 1);
    cpu::setFlag(C_flag, getBit(*srcReg, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Shifts (value of memaddress in HL) to the right.
//Flags: H = 0, N = 0, Z if result is 0, C = bit 0 of (value of memaddress in HL) before operation.
instrInfo cpu::SRL_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte result = (hl >> 1);
    cpu::setFlag(C_flag, getBit(hl, 0));
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Swaps the first and second nibbles of srcReg.
//Flags: H = 0, N = 0, C = 0, Z if result is 0
instrInfo cpu::SWAP_R(byte* srcReg)
{
    byte firstNib = *srcReg << 4;
    byte secondNib = *srcReg >> 4;
    byte result = firstNib | secondNib;
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    *srcReg = result;
    return {2,8};
}
//Swaps the first and second nibbles of (value of memaddress in HL).
//Flags: H = 0, N = 0, C = 0, Z if result is 0
instrInfo cpu::SWAP_HL()
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    byte firstNib = hl << 4;
    byte secondNib = hl >> 4;
    byte result = firstNib | secondNib;
    cpu::setFlag(H_flag, 0);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(C_flag, 0);
    cpu::setFlag(Z_flag, result == 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}

// Bit Operations

//Sets Z flag to complement of bit bitIndex of srcReg
//Flags: H = 1, N = 0, Z = complement
instrInfo cpu::BIT_B_R(byte bitIndex, byte* srcReg)
{
    bool result = !getBit(*srcReg, bitIndex);
    cpu::setFlag(H_flag, 1);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result);
    return {2,8};
}
//Sets Z flag to complement of bit bitIndex of (value of memaddress in HL)
//Flags: H = 1, N = 0, Z = complement
instrInfo cpu::BIT_B_HL(byte bitIndex)
{
    byte hl = cpu::Memory->get8(cpu::state.HL);
    bool result = !getBit(hl, bitIndex);
    cpu::setFlag(H_flag, 1);
    cpu::setFlag(N_flag, 0);
    cpu::setFlag(Z_flag, result);
    return {2,12};
}
//Sets bit bitIndex in srcReg to 1
instrInfo cpu::SET_B_R(byte bitIndex, byte* srcReg)
{
    byte result = setBit(*srcReg, bitIndex, 1);
    *srcReg = result;
    return {2,8};
}
//Sets bit bitIndex in (value of memaddress in HL) to 1
instrInfo cpu::SET_B_HL(byte bitIndex)
{
    byte result = setBit(cpu::Memory->get8(cpu::state.HL), bitIndex, 1);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}
//Sets bit bitIndex in srcReg to 0
instrInfo cpu::RES_B_R(byte bitIndex, byte* srcReg)
{
    byte result = setBit(*srcReg, bitIndex, 0);
    *srcReg = result;
    return {2,8};
}
//Sets bit bitIndex in (value of memaddress in HL) to 0
instrInfo cpu::RES_B_HL(byte bitIndex)
{
    byte result = setBit(cpu::Memory->get8(cpu::state.HL), bitIndex, 0);
    cpu::Memory->set8(cpu::state.HL, result);
    return {2,16};
}