#include "includes.hpp"
#include "gbemu.hpp"

//Options
const int clockspeed = 4194304; //number of clock cycles in a second / speed of CPU in hz
const int framerate = 60; //framerate to run emulator at. native gameboy is 60

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
        Memory.set8(0xFF50, 1); //unmap bootrom
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
        }
        SDL_Delay(1000/framerate);
    }

    //cleanup before exit here
    SDL_Quit();
    logging::log("Exited successfully");
    return 0;
}

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0)
    {
        logging::logerr("SDL Init Error: " + std::string(SDL_GetError()), true);
    }
}