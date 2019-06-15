#include "includes.hpp"
#include "noisechannel.hpp"

void noiseChannel::step()
{
    freqTimer--;
    if (freqTimer == 0)
    {
        freqTimer = divisorTable[divisorCode] << clockShift;
        bool x = (lfsr & 0x1) ^ ((lfsr >> 1) & 0x1);
        lfsr >>= 1;
        lfsr &= 0x3FFF;
        lfsr |= (x << 14);
        if (widthMode)
        {
            lfsr &= 0x7FBF;
            lfsr |= (x << 6);
        }
    }
}

void noiseChannel::updateRegister(byte reg, byte value)
{
    switch (reg)
    {
        case 1:
            lengthCounter = (64 - (value & 0x3F));
            break;
        case 2:
            dacEnabled = ((value & 0xF8) != 0);
            initialVolume = (value >> 4);
            volume = initialVolume;
            volumeEnvelopeMode = getBit(value, 3);
            volumeEnvelopePeriod = (value & 0x7);
            volumeEnvelopeCounter = volumeEnvelopePeriod;
            break;
        case 3:
            clockShift = (value >> 4);
            widthMode = getBit(value, 3);
            divisorCode = (value & 0x7);
            break;
        case 4:
            lengthEnabled = getBit(value, 6);
            if (value >> 7)
            {
                trigger();
            }
            break;
    }
}

byte noiseChannel::getCurrentOutput()
{
    //logging::log(ushortToString(lfsr));
    if (!enabled || !dacEnabled)
    {
        return 0;
    }
    if (!getBit(lfsr, 0))
    {
        return volume;
    }
    return 0;
}

bool noiseChannel::getStatus()
{
    if (lengthCounter > 0 || lengthEnabled == false)
    {
        return 1;
    }
    return 0;
}

void noiseChannel::lengthCounterClock()
{
    if (lengthEnabled && lengthCounter > 0)
    {
        lengthCounter--;
        if (lengthCounter == 0)
        {
            enabled = false;
        }
    }
}

void noiseChannel::volumeEnvelopeClock()
{
    if (volumeEnvelopeRunning)
    {
        volumeEnvelopeCounter--;
        if (volumeEnvelopeCounter <= 0)
        {
            volumeEnvelopeCounter = volumeEnvelopePeriod;
            if (volumeEnvelopeCounter == 0)
            {
                //?????
                volumeEnvelopeCounter = 8;
            }
            if (volumeEnvelopePeriod != 0)
            {
                if (volumeEnvelopeMode)
                {
                    volume++;
                }
                else
                {
                    volume--;
                }
            }
            
            if (volume == 0 || volume == 15)
            {
                volumeEnvelopeRunning = false;
            }
        }
    }
}

void noiseChannel::trigger()
{
    enabled = true;
    if (lengthCounter == 0)
    {
        lengthCounter = 64;
    }
    freqTimer = divisorTable[divisorCode] << clockShift;
    volume = initialVolume;
    volumeEnvelopeCounter = volumeEnvelopePeriod;
    volumeEnvelopeRunning = true;
    lfsr = 0x7FFF;
}