#pragma once

class memory
{
    public:
        memory();
        ~memory();
        void init (byte* rom, byte* input, byte* bootrom = nullptr);
        void set8 (ushort address, byte value, bool force = false);
        byte get8 (ushort address);
        void set16 (ushort address, ushort value);
        ushort get16 (ushort address);
    private:
        bool bootromMapped;
        byte* inputState;
        byte* memBytes;
        byte* bootrom;
        void updateInput();
        void doDMA(byte value);
        ushort fixMemAddress(ushort address);
};