#include "includes.hpp"
#include "wavechannel.hpp"

void waveChannel::step()
{
    freqTimer--;
    if (freqTimer == 0)
    {
        freqTimer = ((2048 - frequency) * 2);
        waveformIndex++;
        if (waveformIndex > 31)
        {
            waveformIndex = 0;
        }
    }
}

void waveChannel::updateRegister(byte reg, byte value)
{
    switch (reg)
    {
        case 0:
            dacEnabled = (value >> 7);
            break;
        case 1:
            lengthCounter = (256 - value);
            break;
        case 2:
            volume = ((value & 0x60) >> 5);
            break;
        case 3:
            frequency = ((frequency & 0x700) | value);
            break;
        case 4:
            lengthEnabled = getBit(value, 6);
            frequency = ((frequency & 0xFF) | ((value & 0x7) << 8));
            if (value >> 7)
            {
                trigger();
            }
            break;
    }
}

byte waveChannel::getCurrentOutput()
{
    if (!enabled || !dacEnabled)
    {
        return 0;
    }
    byte shift = 0;
    switch (volume)
    {
        case 0: shift = 4; break;
        case 1: shift = 0; break;
        case 2: shift = 1; break;
        case 3: shift = 2; break;
    }
    bool isUpperNib = ((waveformIndex % 2) == 0);
    byte sample = isUpperNib ? (wavetable[waveformIndex / 2] >> 4) : (wavetable[(waveformIndex - 1) / 2] & 0xF);
    sample >>= shift;
    return sample;
}

bool waveChannel::getStatus()
{
    if (lengthCounter > 0 || lengthEnabled == false)
    {
        return 1;
    }
    return 0;
}

void waveChannel::lengthCounterClock()
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

void waveChannel::updateWavetable(byte index, byte value)
{
    wavetable[index] = value;
}

void waveChannel::trigger()
{
    if (lengthCounter == 0)
    {
        lengthCounter = 256;
    }
    enabled = true;
    freqTimer = ((2048 - frequency) * 4);
    waveformIndex = 0;
}