#include "includes.hpp"
#include "squarechannel.hpp"

void squareChannel::step()
{
    squareChannel::freqTimer--;
    if (squareChannel::freqTimer == 0)
    {
        squareChannel::freqTimer = ((2048 - squareChannel::frequency) * 4);
        squareChannel::waveformIndex++;
        if (squareChannel::waveformIndex > 7)
        {
            squareChannel::waveformIndex = 0;
        }
    }
}

void squareChannel::updateRegister(byte reg, byte value)
{
    switch (reg)
    {
        case 0:
            break;
        case 1:
            squareChannel::duty = (value >> 6);
            squareChannel::lengthCounter = (64 - (value & 0x3F));
            break;
        case 2:
            squareChannel::dacEnabled = ((value & 0xF8) != 0);
            squareChannel::initialVolume = (value >> 4);
            squareChannel::volume = initialVolume;
            squareChannel::volumeEnvelopeMode = getBit(value, 3);
            squareChannel::volumeEnvelopePeriod = (value & 0x7);
            squareChannel::volumeEnvelopeCounter = squareChannel::volumeEnvelopePeriod;
            break;
        case 3:
            squareChannel::frequency = ((squareChannel::frequency & 0x700) | value);
            break;
        case 4:
            squareChannel::frequency = ((squareChannel::frequency & 0xFF) | ((value & 0x7) << 8));
            squareChannel::lengthEnabled = getBit(value, 6);
            if (value >> 7)
            {
                squareChannel::trigger();
            }
            break;
    }
}

bool squareChannel::getStatus()
{
    if (squareChannel::lengthCounter > 0 || squareChannel::lengthEnabled == false)
    {
        return 1;
    }
    return 0;
}

byte squareChannel::getCurrentOutput()
{
    if (!enabled || !dacEnabled)
    {
        return 0;
    }
    if (squareChannel::dutyTable[squareChannel::duty][squareChannel::waveformIndex])
    {
        return volume;
    }
    return 0;
}

void squareChannel::trigger()
{
    squareChannel::enabled = true;
    if (squareChannel::lengthCounter == 0)
    {
        squareChannel::lengthCounter = 64;
    }
    squareChannel::freqTimer = ((2048 - squareChannel::frequency) * 4);
    squareChannel::volume = squareChannel::initialVolume;
    squareChannel::volumeEnvelopeCounter = squareChannel::volumeEnvelopePeriod;
    squareChannel::volumeEnvelopeRunning = true;
}

void squareChannel::lengthCounterClock()
{
    if (squareChannel::lengthEnabled && squareChannel::lengthCounter > 0)
    {
        squareChannel::lengthCounter--;
        if (squareChannel::lengthCounter == 0)
        {
            squareChannel::enabled = false;
        }
    }
}

void squareChannel::volumeEnvelopeClock()
{
    if (squareChannel::volumeEnvelopeRunning)
    {
        squareChannel::volumeEnvelopeCounter--;
        if (squareChannel::volumeEnvelopeCounter <= 0)
        {
            squareChannel::volumeEnvelopeCounter = squareChannel::volumeEnvelopePeriod;
            if (squareChannel::volumeEnvelopeCounter == 0)
            {
                //?????
                squareChannel::volumeEnvelopeCounter = 8;
            }
            if (squareChannel::volumeEnvelopePeriod != 0)
            {
                if (squareChannel::volumeEnvelopeMode)
                {
                    squareChannel::volume++;
                }
                else
                {
                    squareChannel::volume--;
                }
            }
            
            if (squareChannel::volume == 0 || squareChannel::volume == 15)
            {
                squareChannel::volumeEnvelopeRunning = false;
            }
        }
    }
}