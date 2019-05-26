#include "includes.hpp"
#include "gbemu.hpp"

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
    //Read the ROM into RAM
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
        //TODO: handle not having a bootrom, initialize ram and registers as the bootrom would etc
        logging::logerr("need a bootrom for now", true);
    }
    cpu CPU;
    CPU.initState(state, &Memory);
    Memory.set8(0xFF50, 1); //disable bootrom for testing
    for (int i = 0; i < 10; i++)
    {
        CPU.step();
    }
    /*
    for (ushort i = 0; i < fsize; i++)
    {
        printf("%x", Memory.get(i));
        if (i == 255)
        {
            printf("\n\n");
        }
    }
    */

    //cleanup before exit here
    logging::log("Exited successfully");
    return 0;
}