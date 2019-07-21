#include "includes.hpp"
#include "gbemu.hpp"

//Options
const unsigned int clockspeed = 4194304; //number of clock cycles in a second / speed of CPU in hz
const unsigned int framerate = 60; //framerate to run emulator at. native gameboy is 60

//Timers
unsigned int DividerCounter = 0;
unsigned int TimerFrequency = 4096;
unsigned int TimerCounter = 0;

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
    
    initSDL();
    memory Memory{};
    cpuState state{};
    input Input(&Memory);
    apu APU(clockspeed);
    byte* bootrom = new byte[256];
    if (argv[2] != NULL)
    {
        //Load the bootrom
	    f = fopen(argv[2], "rb");
	    if (f==NULL)
	    {
	    	logging::logerr("error: Couldn't open " + std::string(argv[2]), true);
    	}
        fread(bootrom, 256, 1, f);
        fclose(f);
        Memory.init(cart, &(Input.inputState), &TimerCounter, &APU, bootrom);
        //no need to initialize cpu state if bootrom is present
    }
    else
    {
        Memory.init(cart, &(Input.inputState), &TimerCounter, &APU);
        state.PC = 0x0100; //skip bootrom
        state.SP = 0xFFFE; //initialise stack pointer as bootrom would
        state.AF = 0x01B0; //these init values are different between DMG, CGB and SGB - these are DMG
        state.BC = 0x0013;
        state.DE = 0x00D8;
        state.HL = 0x014D;
    }
    Memory.set8(0xFF40, 0x91, true); //initialise video control registers
    Memory.set8(0xFF47, 0xFC, true);
    Memory.set8(0xFF48, 0xFF, true);
    Memory.set8(0xFF49, 0xFF, true);

    Memory.set8(0xFF00, 0xCF, true); //initialise input register

    cpu CPU(state, &Memory);
    gpu GPU(&Memory);

    int clocksPerFrame = clockspeed / framerate;
    int cycleCounter = 0;
    bool quit = false;
    SDL_Event event;
    timePoint frameStart;
    timePoint frameEnd;
    while(!quit)
    {
        frameStart = Clock::now();
        while(SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    quit = true;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    Input.keyChanged(event.key.keysym.sym, 0);
                    break;
                }
                case SDL_KEYUP:
                {
                    Input.keyChanged(event.key.keysym.sym, 1);
                    break;
                }
            }
        }
        cycleCounter = 0;
        while (cycleCounter < clocksPerFrame)
        {
            //interrupt handling happens first
            cycleCounter += handleInterrupts(&CPU, &Memory);
            instrInfo info;
            if (!CPU.getState().HALTED)
            {
                info = CPU.step();
            }
            else
            {
                info = {1,4}; //NOP - pretend to continue if console is halted so timers continue
            }
            cycleCounter += info.numCycles;
            handleTimers(info.numCycles, &Memory);
            APU.handleSound(info.numCycles);
            GPU.update(info.numCycles);
        }
        GPU.displayScreen();
        
        frameEnd = Clock::now();
        float duration = (float)(std::chrono::duration_cast<std::chrono::microseconds>( frameEnd - frameStart ).count()) / 1000;
        float delay = ((1000/(float)framerate) - duration);
        //SDL_Delay(delay);
        //std::cout <<delay << "\n";
        APU.dumpBuffer();
        accurateSleep(delay);
        
    }

    //cleanup before exit here
    SDL_Quit();
    delete[] cart;
    delete[] bootrom;
    logging::log("Exited successfully");
    return 0;
}

void accurateSleep(float ms)
{
    if (ms <= 0)
    {
        return;
    }
    //SDL_Delay((Uint32)ms);
    timePoint start = Clock::now();
    timePoint timer = start;
    while ((float)(std::chrono::duration_cast<std::chrono::microseconds>(timer - start).count()) / 1000 < ms)
    {
        timer = Clock::now();
    }
}

//Interrupts:
//0 = V-Blank
//1 = LCD Status
//2 = Timer
//3 = Serial
//4 = Joypad
//Interrupt enable register  : 0xFFFF
//Interrupt request register : 0xFF0F
byte handleInterrupts(cpu* CPU, memory* mem)
{
    byte enabled = mem->get8(0xFFFF);
    byte requested = mem->get8(0xFF0F);
    bool unhalt = false;
    byte cycles = 0;
    if (getBit(requested, 0) && getBit(enabled, 0))
    {
        if (CPU->getState().IME)
        {
            cycles += 20;
            mem->set8(0xFF0F, setBit(mem->get8(0xFF0F), 0, 0));
            CPU->serviceInterrupt(V_Blank_interrupt);
        }
        unhalt = true;
    }
    else if (getBit(requested, 1) && getBit(enabled, 1))
    {
        if (CPU->getState().IME)
        {
            cycles += 20;
            mem->set8(0xFF0F, setBit(mem->get8(0xFF0F), 1, 0));
            CPU->serviceInterrupt(LCD_interrupt);
        }
        unhalt = true;
    }
    else if (getBit(requested, 2) && getBit(enabled, 2))
    {
        if (CPU->getState().IME)
        {
            cycles += 20;
            mem->set8(0xFF0F, setBit(mem->get8(0xFF0F), 2, 0));
            CPU->serviceInterrupt(Timer_interrupt);
        }
        unhalt = true;
    }
    else if (getBit(requested, 3) && getBit(enabled, 3))
    {
        if (CPU->getState().IME)
        {
            cycles += 20;
            mem->set8(0xFF0F, setBit(mem->get8(0xFF0F), 3, 0));
            CPU->serviceInterrupt(Serial_interrupt);
        }
        unhalt = true;
    }
    else if (getBit(requested, 4) && getBit(enabled, 4))
    {
        if (CPU->getState().IME)
        {
            cycles += 20;
            mem->set8(0xFF0F, setBit(mem->get8(0xFF0F), 4, 0));
            CPU->serviceInterrupt(Joypad_interrupt);
        }
        unhalt = true;
    }
    if (unhalt)
    {
        if (CPU->getState().HALTED)
        {
            cycles += 4;
            CPU->unhalt();
        }
    }
    return cycles;
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
    unsigned int newFrequency;
    byte frequencySetting = mem->get8(0xFF07) & 0x3; //Get first 2 bits of timer control register
    switch (frequencySetting)
    {
        case 0b00: newFrequency = 4096; break;
        case 0b01: newFrequency = 262144; break;
        case 0b10: newFrequency = 65536; break;
        case 0b11: newFrequency = 16384; break;
    }
    if (TimerFrequency != newFrequency)
    {
        TimerFrequency = newFrequency;
        TimerCounter = 0;
        return;
    }
    if (enabled)
    {
        TimerCounter += cycles;
        if (TimerCounter >= (clockspeed / TimerFrequency))
        {
            TimerCounter = 0;
            if (mem->get8(0xFF05) == 0xFF) //about to overflow
            {
                mem->set8(0xFF05, mem->get8(0xFF06));
                mem->set8(0xFF0F, setBit(mem->get8(0xFF0F), 2, 1)); // request timer interrupt
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