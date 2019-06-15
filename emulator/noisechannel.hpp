#pragma once

class noiseChannel
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
        bool enabled = false;
        bool dacEnabled = false;
        byte clockShift = 0;
        bool widthMode = 0;
        byte divisorCode = 0;
        ushort freqTimer = 0;
        const byte divisorTable[8] = {8, 16, 32, 48, 64, 80, 96, 112};
        ushort lfsr = 0;
        bool lengthEnabled = 0;
        byte lengthCounter = 0;
        byte volume = 0;
        byte initialVolume = 0;
        int volumeEnvelopePeriod = 0;
        int volumeEnvelopeCounter = 0;
        bool volumeEnvelopeMode = 0;
        bool volumeEnvelopeRunning = false;
};