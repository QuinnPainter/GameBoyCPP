#pragma once

class squareChannel
{
    public:
        void step();
        void updateRegister(byte reg, byte value);
        byte getCurrentOutput();
        bool getStatus();
        void lengthCounterClock();
        void volumeEnvelopeClock();
    private:
        void trigger();
        const bool dutyTable[4][8] = {{0,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,1,1,1}, {0,1,1,1,1,1,1,0}};
        ushort freqTimer = 0;
        byte waveformIndex = 0;
        ushort frequency = 0;
        byte duty = 0;
        bool enabled = false;
        bool dacEnabled = true;
        bool lengthEnabled = false;
        byte volume = 0;
        byte initialVolume = 0;
        byte lengthCounter = 0;
        int volumeEnvelopePeriod = 0;
        int volumeEnvelopeCounter = 0;
        bool volumeEnvelopeMode = 0;
        bool volumeEnvelopeRunning = false;
};