#include "includes.hpp"
#include "gbemu.hpp"

//Options
const int clockspeed = 4194304; //number of clock cycles in a second / speed of CPU in hz
const int framerate = 60; //framerate to run emulator at. native gameboy is 60

//Timers
int DividerCounter;
int TimerFrequency;
int TimerCounter;

//arg 1 is game rom, arg 2 is bootrom
int main (int argc, char** argv)
{
    if (argv[1] == NULL)
    {
        logging::logerr("A ROM is needed!", true);
    }
    //Load the ROM
	FILE *f = fopen(argv[1], "rb");
	if (f==NULL)
	{
        logging::logerr("error: Couldn't open " + std::string(argv[1]), true);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
    byte* cart = new byte[fsize];
    fread(cart, fsize, 1, f);
    fclose(f);

    memory Memory{};
    cpuState state{};
    if (argv[2] != NULL)
    {
        //Load the bootrom
        byte* bootrom = new byte[256];
	    f = fopen(argv[2], "rb");
	    if (f==NULL)
	    {
	    	logging::logerr("error: Couldn't open " + std::string(argv[2]), true);
    	}
        fread(bootrom, 256, 1, f);
        fclose(f);
        Memory.init(cart, bootrom);
        delete[] bootrom;
        //no need to initialize cpu state if bootrom is present
    }
    else
    {
        Memory.init(cart);
        state.PC = 0x0100; //skip bootrom
        state.SP = 0xFFFE; //initialise stack pointer as bootrom would
        Memory.set8(0xFF50, 1, true); //unmap bootrom
    }
    state.AF = 0x01B0; //these init values are different between DMG, CGB and SGB - these are DMG
    state.BC = 0x0013;
    state.DE = 0x00D8;
    state.HL = 0x014D;
    cpu CPU;

    CPU.initState(state, &Memory);

    initSDL();

    int clocksPerFrame = clockspeed / framerate;
    int cycleCounter = 0;
    while(true)
    {
        cycleCounter = 0;
        while (cycleCounter < clocksPerFrame)
        {
            instrInfo info = CPU.step();
            cycleCounter += info.numCycles;
            handleTimers(info.numCycles, &Memory);
        }
        SDL_Delay(1000/framerate);
    }

    //cleanup before exit here
    SDL_Quit();
    logging::log("Exited successfully");
    return 0;
}

void handleTimers(int cycles, memory* mem)
{
    //Divider Register Timer - Increments at 16384 Hz
    DividerCounter += cycles;
    if (DividerCounter >= (clockspeed / 16384))
    {
        DividerCounter = 0;
        mem->set8(0xFF04, mem->get8(0xFF04) + 1, true);
    }
    //Timer
    bool enabled = getBit(mem->get8(0xFF07), 2);
    int newFrequency;
    byte frequencySetting = mem->get8(0xFF07) & 0x3; //Get first 2 bits of timer control register
    switch (frequencySetting)
    {
        case 0b00: newFrequency = 4096; break;
        case 0b01: newFrequency = 262144; break;
        case 0b10: newFrequency = 65536; break;
        case 0b11: newFrequency = 16384; break;
    }
    if (frequencySetting != newFrequency)
    {
        frequencySetting = newFrequency;
        TimerCounter = newFrequency;
        return;
    }
    if (enabled)
    {
        TimerCounter -= (clockspeed / cycles);
        if (TimerCounter <= 0)
        {
            TimerCounter = frequencySetting;
            if (mem->get8(0xFF05) == 0xFF) //about to overflow
            {
                mem->set8(0xFF05, mem->get8(0xFF06));
                // todo: trigger overflow interrupt
            }
            else
            {
                mem->set8(0xFF05, mem->get8(0xFF05) + 1);
            }
        }
    }
}

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0)
    {
        logging::logerr("SDL Init Error: " + std::string(SDL_GetError()), true);
    }
}