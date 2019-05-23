#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main (int argc, char** argv)
{
    if (argv[1] == NULL)
    {
        printf("No file was provided!\n");
        exit(1);
    }
	FILE *f= fopen(argv[1], "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", argv[1]);
		exit(1);
	}
    //Get the file size
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
    unsigned char* buffer = (unsigned char *)malloc(fsize);
    fread(buffer, fsize, 1, f);
    fclose(f);
    return 0;
}