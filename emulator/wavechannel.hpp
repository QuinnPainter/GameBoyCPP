#pragma once

class waveChannel
{
    public:
        void step();
        void updateRegister(byte reg, byte value);
        byte getCurrentOutput();
        bool getStatus();
        void lengthCounterClock();
        void updateWavetable(byte index, byte value);
    private:
        void trigger();
        byte wavetable[16] = {};
        bool enabled = false;
        bool dacEnabled = false;
        ushort frequency = 0;
        ushort freqTimer = 0;
        byte volume = 0;
        byte waveformIndex = 0;
        ushort lengthCounter = 0;
        bool lengthEnabled = false;
};