#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
void displayBinaryByte(unsigned char *byte)
{
    for (int i = 0; i < 8; i++)
    {
        bool bit = (*byte & (1 << (7-i))) > 0;
        if (bit)
        {
            cout << (unsigned char)219; //ascii for filled in block
        }
        else
        {
            cout << (unsigned char)32; //ascii for space
        }
    }
}

void displayBinaryBytes(unsigned char* start, int groupSize, int numGroups)
{
    for (int group = 0; group < numGroups; group++)
    {
        for (int i = 0; i < groupSize; i++)
        {
            displayBinaryByte(start + i + group);
        }
        cout << "\n";
    }
}
*/

void displayBinaryNibble(unsigned char* byte, bool firstnib) //if true, first nibble, if false, second nibble
{
    unsigned char nibble = *byte;
    if (firstnib)
    {
        nibble >>= 4;
    }
    for (int i = 0; i < 4; i++)
    {
        bool bit = (nibble & (1 << (3-i))) > 0;
        if (bit)
        {
            //print character twice for square pixels
            cout << (unsigned char)219 << (unsigned char)219; //ascii for filled in block
        }
        else
        {
            cout << (unsigned char)32 << (unsigned char)32; //ascii for space
        }
    }
}

void displayLine(unsigned char* memloc, bool firstnib)
{
    for (int i = 0; i < 24; i += 2)
    {
        displayBinaryNibble(memloc + i, firstnib);
    }
    cout << "\n";
}

void displayNintendoLogo(unsigned char* memloc)
{
    displayLine(memloc, true);
    displayLine(memloc, false);
    displayLine(memloc + 1, true);
    displayLine(memloc + 1, false);
    displayLine(memloc + 24, true);
    displayLine(memloc + 24, false);
    displayLine(memloc + 25, true);
    displayLine(memloc + 25, false);
}

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
    cout << "Game name: " << string(reinterpret_cast<char*>(buffer + 0x134), 16) << "\n";
    unsigned char validLogo[48] = {
        0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
        0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
        0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
    };
    bool valid = true;
    for (int i = 0; i < 48; i++)
    {
        if (buffer[0x104 + i] != validLogo[i])
        {
            valid = false;
        }
    }
    cout << "Boot logo: " << (valid ? "Valid" : "Invalid") << "\n";
    displayNintendoLogo(buffer + 0x104);
    cout << "Game Boy Color status: ";
    if (buffer[0x143] == 0x80)
    {
        cout << "Enhanced by Game Boy Color\n";
    }
    else if (buffer[0x143] == 0xC0)
    {
        cout << "Only plays on Game Boy Color\n";
    }
    else
    {
        cout << "No Game Boy Color enhancements\n";
    }
    cout << "Super Game Boy enhanced: " << ((buffer[0x146] == 0x03) ? "Yes" : "No") << "\n";
    cout << "Cart Type: ";
    switch (buffer[0x147])
    {
        case (0x00): cout<<"ROM ONLY\n"; break;
        case (0x01): cout<<"MBC1\n"; break;
        case (0x02): cout<<"MBC1+RAM\n"; break;
        case (0x03): cout<<"MBC1+RAM+BATTERY\n"; break;
        case (0x05): cout<<"MBC2\n"; break;
        case (0x06): cout<<"MBC2+BATTERY\n"; break;
        case (0x08): cout<<"ROM+RAM\n"; break;
        case (0x09): cout<<"ROM+RAM+BATTERY\n"; break;
        case (0x0B): cout<<"MMM01\n"; break;
        case (0x0C): cout<<"MMM01+RAM\n"; break;
        case (0x0D): cout<<"MMM01+RAM+BATTERY\n"; break;
        case (0x0F): cout<<"MBC3+TIMER+BATTERY\n"; break;
        case (0x10): cout<<"MBC3+TIMER+RAM+BATTERY\n"; break;
        case (0x11): cout<<"MBC3\n"; break;
        case (0x12): cout<<"MBC3+RAM\n"; break;
        case (0x13): cout<<"MBC3+RAM+BATTERY\n"; break;
        case (0x19): cout<<"MBC5\n"; break;
        case (0x1A): cout<<"MBC5+RAM\n"; break;
        case (0x1B): cout<<"MBC5+RAM+BATTERY\n"; break;
        case (0x1C): cout<<"MBC5+RUMBLE\n"; break;
        case (0x1D): cout<<"MBC5+RUMBLE+RAM\n"; break;
        case (0x1E): cout<<"MBC5+RUMBLE+RAM+BATTERY\n"; break;
        case (0x20): cout<<"MBC6\n"; break;
        case (0x22): cout<<"MBC7+SENSOR+RUMBLE+RAM+BATTERY\n"; break;
        case (0xFC): cout<<"POCKET CAMERA\n"; break;
        case (0xFD): cout<<"BANDAI TAMA5\n"; break;
        case (0xFE): cout<<"HuC3\n"; break;
        case (0xFF): cout<<"HuC1+RAM+BATTERY\n"; break;
        default: cout<<"Unknown\n"; break;
    }
    cout << "ROM Size: ";
    switch (buffer[0x148])
    {
        case (0x00): cout<<"32KByte (no ROM banking)\n"; break;
        case (0x01): cout<<"64KByte (4 banks)\n"; break;
        case (0x02): cout<<"128KByte (8 banks)\n"; break;
        case (0x03): cout<<"256KByte (16 banks)\n"; break;
        case (0x04): cout<<"512KByte (32 banks)\n"; break;
        case (0x05): cout<<"1MByte (64 banks)\n"; break;
        case (0x06): cout<<"2MByte (128 banks)\n"; break;
        case (0x07): cout<<"4MByte (256 banks)\n"; break;
        case (0x08): cout<<"8MByte (512 banks)\n"; break;
        case (0x52): cout<<"1.1MByte (72 banks)\n"; break;
        case (0x53): cout<<"1.2MByte (80 banks)\n"; break;
        case (0x54): cout<<"1.5MByte (96 banks)\n"; break;
        default: cout<<"Unknown\n"; break;
    }
    cout << "RAM Size: ";
    switch (buffer[0x149])
    {
        case (0x00): cout<<"None\n"; break;
        case (0x01): cout<<"2 KBytes\n"; break;
        case (0x02): cout<<"8 Kbytes\n"; break;
        case (0x03): cout<<"32 KBytes (4 banks of 8KBytes each)\n"; break;
        case (0x04): cout<<"128 KBytes (16 banks of 8KBytes each)\n"; break;
        case (0x05): cout<<"64 KBytes (8 banks of 8KBytes each)\n"; break;
        default: cout<<"Unknown\n"; break;
    }
    cout << "Region: " << ((buffer[0x14A] == 0x01) ? "Outside of Japan" : "Japan") << "\n";
    cout << "ROM Version: " << (int)buffer[0x14C] << "\n";
    cout << "Header checksum: ";
    unsigned char x=0;
    for(int i=0;i!=0x19;i++){
        x=x-buffer[0x134 + i]-1;
    }
    cout << ((buffer[0x014D] == x) ? "Valid" : "Invalid") << "\n";
    cout << "Global checksum: ";
    unsigned short y=0;
    for(int i = 0; i < fsize; i++)
    {
        if (i != 0x14E && i != 0x14F)
        {
            y += buffer[i];
        }
    }
    unsigned short toCheck = ((unsigned short)buffer[0x14E] << 8) | buffer[0x14F];
    cout << ((y == toCheck) ? "Valid" : "Invalid") << "\n";
    return 0;
}