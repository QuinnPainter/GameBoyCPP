#pragma once

#include "squarechannel.hpp"
#include "wavechannel.hpp"
#include "noisechannel.hpp"

const int sampleFrequency = 44100;
const ushort sampleSize = 4096;
const unsigned int masterVolume = 50;

class apu
{
    public:
        apu(unsigned int clock);
        ~apu();
        void handleSound(int cycles);
        void dumpBuffer();
        byte requestStatus();
        void updateRegister(ushort address, byte value);
    private:
        unsigned int clockspeed;
        SDL_AudioDeviceID device;
        short buffer[sampleSize * 10] = {};
        unsigned int bufferIndex = 0;
        unsigned int addToBufferCounter = 0;

        squareChannel channel1;
        squareChannel channel2;
        waveChannel channel3;
        noiseChannel channel4;

        unsigned int frameSequenceCounter = 0;
        byte frameSequencer = 0;
        byte lVolume = 0;
        byte rVolume = 0;
        byte enables = 0;
        bool power = true;
};