#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h> //needed by memcpy, for some reason
using namespace std;

//arg 1 is bootrom, arg 2 is game rom
int main (int argc, char** argv)
{
    if (argv[1] == NULL || argv[2] == NULL)
    {
        printf("Both a bootrom and a game ROM must be provided!\n");
        exit(1);
    }
    //Load the ROM first
	FILE *f = fopen(argv[2], "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", argv[2]);
		exit(1);
	}
    //Read the ROM into RAM
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
    unsigned char* memory = (unsigned char *)malloc(fsize);
    fread(memory, fsize, 1, f);
    fclose(f);
    //Store the first 256 bytes of the ROM into a buffer so it can be restored
    //after the boot process is finished
    unsigned char* romStartBuffer = (unsigned char *)malloc(256);
    memcpy(romStartBuffer, memory, 256);
    //Load the bootrom
	f = fopen(argv[1], "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", argv[1]);
		exit(1);
	}
    fread(memory, 256, 1, f);
    fclose(f);
    /*
    for (int i = 0; i < fsize; i++)
    {
        printf("%x", memory[i]);
        if (i == 255)
        {
            printf("\n\n");
        }
    }
    */
    return 0;
}