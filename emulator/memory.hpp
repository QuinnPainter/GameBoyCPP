#pragma once

class memory
{
    public:
        memory();
        ~memory();
        void init (byte* rom, byte* bootrom = nullptr);
        void set (ushort address, byte value);
        byte get (ushort address);
    private:
        byte* memBytes;
        byte* bootrom;
        ushort fixMemAddress(ushort address);
};