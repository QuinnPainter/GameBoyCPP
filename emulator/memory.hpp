#pragma once

class memory
{
    public:
        memory();
        ~memory();
        void init (byte* rom, byte* bootrom = nullptr);
        void set8 (ushort address, byte value, bool force = false);
        byte get8 (ushort address);
        void set16 (ushort address, ushort value);
        ushort get16 (ushort address);
    private:
        byte* memBytes;
        byte* bootrom;
        ushort fixMemAddress(ushort address);
};