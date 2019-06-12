#pragma once

enum mbc
{
    MBC_None,
    MBC_MBC1
};

class memory
{
    public:
        memory();
        ~memory();
        void init (byte* gameRom, byte* input, int* timer, byte* bootrom = nullptr);
        void set8 (ushort address, byte value, bool force = false);
        byte get8 (ushort address);
        void set16 (ushort address, ushort value);
        ushort get16 (ushort address);
    private:
        bool bootromMapped;
        byte* inputState;
        int* timerCounter;
        byte* memBytes;
        byte* bootrom;
        void updateInput();
        void doDMA(byte value);
        ushort fixMemAddress(ushort address);
        //MBC stuff
        mbc mbcVersion;
        unsigned int romSize;
        byte* rom;
        byte romBank;
        byte upperBits;
        bool mbcMode;
};