#include "includes.hpp"
#include "sound.hpp"

/*
       Square 1
NR10 FF10 -PPP NSSS Sweep period, negate, shift
NR11 FF11 DDLL LLLL Duty, Length load (64-L)
NR12 FF12 VVVV APPP Starting volume, Envelope add mode, period
NR13 FF13 FFFF FFFF Frequency LSB
NR14 FF14 TL-- -FFF Trigger, Length enable, Frequency MSB

       Square 2
     FF15 ---- ---- Not used
NR21 FF16 DDLL LLLL Duty, Length load (64-L)
NR22 FF17 VVVV APPP Starting volume, Envelope add mode, period
NR23 FF18 FFFF FFFF Frequency LSB
NR24 FF19 TL-- -FFF Trigger, Length enable, Frequency MSB

       Wave
NR30 FF1A E--- ---- DAC power
NR31 FF1B LLLL LLLL Length load (256-L)
NR32 FF1C -VV- ---- Volume code (00=0%, 01=100%, 10=50%, 11=25%)
NR33 FF1D FFFF FFFF Frequency LSB
NR34 FF1E TL-- -FFF Trigger, Length enable, Frequency MSB

       Noise
     FF1F ---- ---- Not used
NR41 FF20 --LL LLLL Length load (64-L)
NR42 FF21 VVVV APPP Starting volume, Envelope add mode, period
NR43 FF22 SSSS WDDD Clock shift, Width mode of LFSR, Divisor code
NR44 FF23 TL-- ---- Trigger, Length enable

       Control/Status
NR50 FF24 ALLL BRRR Vin L enable, Left vol, Vin R enable, Right vol
NR51 FF25 NW21 NW21 Left enables, Right enables
NR52 FF26 P--- NW21 Power control/status, Channel length statuses

       Wave Table
     FF30 0000 1111 Samples 0 and 1
     ....
     FF3F 0000 1111 Samples 30 and 31
*/

apu::apu(unsigned int clock)
{
    apu::clockspeed = clock;

    SDL_AudioSpec want;
    SDL_AudioSpec have;
    want.freq = sampleFrequency;
    want.format = AUDIO_S16;
    want.channels = 2;
    want.samples = sampleSize;
    want.callback = NULL;

    apu::device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    SDL_PauseAudioDevice(apu::device, 0);
}

apu::~apu()
{
    SDL_CloseAudioDevice(apu::device);
}

byte apu::requestStatus()
{
    byte status = 0;
    status = setBit(status, 7, apu::power);
    status = setBit(status, 0, apu::channel1.getStatus());
    status = setBit(status, 1, apu::channel2.getStatus());
    status = setBit(status, 2, apu::channel3.getStatus());
    status = setBit(status, 3, apu::channel4.getStatus());
    return status;
}

void apu::handleSound(int cycles)
{
    while (cycles-- > 0)
    {
        //Frame sequencer runs at 512Hz
        frameSequenceCounter++;
        if (frameSequenceCounter >= (clockspeed / 512))
        {
            frameSequenceCounter = 0;

            if (frameSequencer == 0 || frameSequencer == 2 || frameSequencer == 4 || frameSequencer == 6)
            {
                apu::channel1.lengthCounterClock();
                apu::channel2.lengthCounterClock();
                apu::channel3.lengthCounterClock();
                apu::channel4.lengthCounterClock();
                if (frameSequencer == 2 || frameSequencer == 6)
                {
                    //sweep clock
                }
            }
            if (frameSequencer == 7)
            {
                apu::channel1.volumeEnvelopeClock();
                apu::channel2.volumeEnvelopeClock();
                apu::channel4.volumeEnvelopeClock();
            }

            frameSequencer++;
            if (frameSequencer > 7)
            {
                frameSequencer = 0;
            }
        }

        apu::channel1.step();
        apu::channel2.step();
        apu::channel3.step();
        apu::channel4.step();

        apu::addToBufferCounter++;
        if (apu::addToBufferCounter >= (clockspeed / sampleFrequency))
        {
            apu::addToBufferCounter = 0;
            short lBuffer = 0;
            short rBuffer = 0;
            int lVolume = (apu::lVolume + 1) * masterVolume;
            int rVolume = (apu::rVolume + 1) * masterVolume;
            //left channel
            if (getBit(apu::enables, 4))
            {
                lBuffer += apu::channel1.getCurrentOutput() * lVolume;
            }
            if (getBit(apu::enables, 5))
            {
                lBuffer += apu::channel2.getCurrentOutput() * lVolume;
            }
            if (getBit(apu::enables, 6))
            {
                lBuffer += apu::channel3.getCurrentOutput() * lVolume;
            }
            if (getBit(apu::enables, 7))
            {
                lBuffer += apu::channel4.getCurrentOutput() * lVolume;
            }
            //right channel
            if (getBit(apu::enables, 0))
            {
                rBuffer += apu::channel1.getCurrentOutput() * rVolume;
            }
            if (getBit(apu::enables, 1))
            {
                rBuffer += apu::channel2.getCurrentOutput() * rVolume;
            }
            if (getBit(apu::enables, 2))
            {
                rBuffer += apu::channel3.getCurrentOutput() * rVolume;
            }
            if (getBit(apu::enables, 3))
            {
                rBuffer += apu::channel4.getCurrentOutput() * rVolume;
            }
            
            //if DAC is off, just load 0 into the audio queue to keep the audio device running and in sync
            if (power)
            {
                apu::buffer[apu::bufferIndex] = lBuffer;
                apu::buffer[apu::bufferIndex + 1] = rBuffer;
            }
            apu::bufferIndex += 2;
            /*
            if (apu::bufferIndex >= sampleSize)
            {
                //Wait until we need more audio
                //float delay = (float)((signed int)(SDL_GetQueuedAudioSize(apu::device) / (sizeof(short) * 2)) - (signed int)sampleSize * 2) / (float)sampleFrequency;
                //if (delay > 0)
                //{
                //    SDL_Delay((Uint32)(delay * 1000));
                //}
                if (power)
                {
                    SDL_QueueAudio(apu::device, buffer, sampleSize * sizeof(short));
                }
                apu::bufferIndex = 0;
            }
            */
            //short tempBuf[2] = {lBuffer, rBuffer};
            //SDL_QueueAudio(apu::device, tempBuf, sizeof(tempBuf));
        }
    }
}
byte count = 0;
void apu::dumpBuffer()
{
    count++;
    if (count == 60)
    {
        count = 0;
        float audioDelay = ((SDL_GetQueuedAudioSize(apu::device) / (sizeof(short) * 2)) / (float)sampleFrequency) * 1000;
        if (audioDelay > 300)
        {
            //emergency measure to keep audio in sync: if queue gets too long, purge it so audio stays in sync
            SDL_ClearQueuedAudio(apu::device);
            logging::log("Audio out of sync by " + std::to_string(audioDelay) + "ms");
        }
    }
    SDL_QueueAudio(apu::device, apu::buffer, bufferIndex * sizeof(short));
    apu::bufferIndex = 0;
}

void apu::updateRegister(ushort address, byte value)
{
    switch (address)
    {
        case 0xFF10: apu::channel1.updateRegister(0, value); break;
        case 0xFF11: apu::channel1.updateRegister(1, value); break;
        case 0xFF12: apu::channel1.updateRegister(2, value); break;
        case 0xFF13: apu::channel1.updateRegister(3, value); break;
        case 0xFF14: apu::channel1.updateRegister(4, value); break;

        case 0xFF16: apu::channel2.updateRegister(1, value); break;
        case 0xFF17: apu::channel2.updateRegister(2, value); break;
        case 0xFF18: apu::channel2.updateRegister(3, value); break;
        case 0xFF19: apu::channel2.updateRegister(4, value); break;
        
        case 0xFF1A: apu::channel3.updateRegister(0, value); break;
        case 0xFF1B: apu::channel3.updateRegister(1, value); break;
        case 0xFF1C: apu::channel3.updateRegister(2, value); break;
        case 0xFF1D: apu::channel3.updateRegister(3, value); break;
        case 0xFF1E: apu::channel3.updateRegister(4, value); break;

        case 0xFF20: apu::channel4.updateRegister(1, value); break;
        case 0xFF21: apu::channel4.updateRegister(2, value); break;
        case 0xFF22: apu::channel4.updateRegister(3, value); break;
        case 0xFF23: apu::channel4.updateRegister(4, value); break;

        case 0xFF24:
            apu::lVolume = ((value & 0x70) >> 4);
            apu::rVolume = (value & 0x7);
            break;
        case 0xFF25: apu::enables = value; break;
        case 0xFF26: apu::power = getBit(value, 7); break;

        case 0xFF30: apu::channel3.updateWavetable(0, value); break;
        case 0xFF31: apu::channel3.updateWavetable(1, value); break;
        case 0xFF32: apu::channel3.updateWavetable(2, value); break;
        case 0xFF33: apu::channel3.updateWavetable(3, value); break;
        case 0xFF34: apu::channel3.updateWavetable(4, value); break;
        case 0xFF35: apu::channel3.updateWavetable(5, value); break;
        case 0xFF36: apu::channel3.updateWavetable(6, value); break;
        case 0xFF37: apu::channel3.updateWavetable(7, value); break;
        case 0xFF38: apu::channel3.updateWavetable(8, value); break;
        case 0xFF39: apu::channel3.updateWavetable(9, value); break;
        case 0xFF3A: apu::channel3.updateWavetable(10, value); break;
        case 0xFF3B: apu::channel3.updateWavetable(11, value); break;
        case 0xFF3C: apu::channel3.updateWavetable(12, value); break;
        case 0xFF3D: apu::channel3.updateWavetable(13, value); break;
        case 0xFF3E: apu::channel3.updateWavetable(14, value); break;
        case 0xFF3F: apu::channel3.updateWavetable(15, value); break;
    }
}