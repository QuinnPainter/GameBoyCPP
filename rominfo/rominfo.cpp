#include <iostream>

/*
void displayBinaryByte(unsigned char *byte)
{
    for (int i = 0; i < 8; i++)
    {
        bool bit = (*byte & (1 << (7-i))) > 0;
        if (bit)
        {
            std::cout << (unsigned char)219; //ascii for filled in block
        }
        else
        {
            std::cout << (unsigned char)32; //ascii for space
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
        std::cout << "\n";
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
            std::cout << (unsigned char)219 << (unsigned char)219; //ascii for filled in block
        }
        else
        {
            std::cout << (unsigned char)32 << (unsigned char)32; //ascii for space
        }
    }
}

void displayLine(unsigned char* memloc, bool firstnib)
{
    for (int i = 0; i < 24; i += 2)
    {
        displayBinaryNibble(memloc + i, firstnib);
    }
    std::cout << "\n";
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
    std::cout << "Game name: " << std::string(reinterpret_cast<char*>(buffer + 0x134), 16) << "\n";
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
    std::cout << "Boot logo: " << (valid ? "Valid" : "Invalid") << "\n";
    displayNintendoLogo(buffer + 0x104);
    std::cout << "Game Boy Color status: ";
    if (buffer[0x143] == 0x80)
    {
        std::cout << "Enhanced by Game Boy Color\n";
    }
    else if (buffer[0x143] == 0xC0)
    {
        std::cout << "Only plays on Game Boy Color\n";
    }
    else
    {
        std::cout << "No Game Boy Color enhancements\n";
    }
    std::cout << "Super Game Boy enhanced: " << ((buffer[0x146] == 0x03) ? "Yes" : "No") << "\n";
    std::cout << "Cart Type: ";
    switch (buffer[0x147])
    {
        case (0x00): std::cout<<"ROM ONLY\n"; break;
        case (0x01): std::cout<<"MBC1\n"; break;
        case (0x02): std::cout<<"MBC1+RAM\n"; break;
        case (0x03): std::cout<<"MBC1+RAM+BATTERY\n"; break;
        case (0x05): std::cout<<"MBC2\n"; break;
        case (0x06): std::cout<<"MBC2+BATTERY\n"; break;
        case (0x08): std::cout<<"ROM+RAM\n"; break;
        case (0x09): std::cout<<"ROM+RAM+BATTERY\n"; break;
        case (0x0B): std::cout<<"MMM01\n"; break;
        case (0x0C): std::cout<<"MMM01+RAM\n"; break;
        case (0x0D): std::cout<<"MMM01+RAM+BATTERY\n"; break;
        case (0x0F): std::cout<<"MBC3+TIMER+BATTERY\n"; break;
        case (0x10): std::cout<<"MBC3+TIMER+RAM+BATTERY\n"; break;
        case (0x11): std::cout<<"MBC3\n"; break;
        case (0x12): std::cout<<"MBC3+RAM\n"; break;
        case (0x13): std::cout<<"MBC3+RAM+BATTERY\n"; break;
        case (0x19): std::cout<<"MBC5\n"; break;
        case (0x1A): std::cout<<"MBC5+RAM\n"; break;
        case (0x1B): std::cout<<"MBC5+RAM+BATTERY\n"; break;
        case (0x1C): std::cout<<"MBC5+RUMBLE\n"; break;
        case (0x1D): std::cout<<"MBC5+RUMBLE+RAM\n"; break;
        case (0x1E): std::cout<<"MBC5+RUMBLE+RAM+BATTERY\n"; break;
        case (0x20): std::cout<<"MBC6\n"; break;
        case (0x22): std::cout<<"MBC7+SENSOR+RUMBLE+RAM+BATTERY\n"; break;
        case (0xFC): std::cout<<"POCKET CAMERA\n"; break;
        case (0xFD): std::cout<<"BANDAI TAMA5\n"; break;
        case (0xFE): std::cout<<"HuC3\n"; break;
        case (0xFF): std::cout<<"HuC1+RAM+BATTERY\n"; break;
        default: std::cout<<"Unknown\n"; break;
    }
    std::cout << "ROM Size: ";
    switch (buffer[0x148])
    {
        case (0x00): std::cout<<"32KByte (no ROM banking)\n"; break;
        case (0x01): std::cout<<"64KByte (4 banks)\n"; break;
        case (0x02): std::cout<<"128KByte (8 banks)\n"; break;
        case (0x03): std::cout<<"256KByte (16 banks)\n"; break;
        case (0x04): std::cout<<"512KByte (32 banks)\n"; break;
        case (0x05): std::cout<<"1MByte (64 banks)\n"; break;
        case (0x06): std::cout<<"2MByte (128 banks)\n"; break;
        case (0x07): std::cout<<"4MByte (256 banks)\n"; break;
        case (0x08): std::cout<<"8MByte (512 banks)\n"; break;
        //case (0x52): std::cout<<"1.1MByte (72 banks)\n"; break;
        //case (0x53): std::cout<<"1.2MByte (80 banks)\n"; break;
        //case (0x54): std::cout<<"1.5MByte (96 banks)\n"; break;
        default: std::cout<<"Unknown\n"; break;
    }
    std::cout << "RAM Size: ";
    switch (buffer[0x149])
    {
        case (0x00): std::cout<<"None\n"; break;
        case (0x01): std::cout<<"2 KBytes\n"; break;
        case (0x02): std::cout<<"8 Kbytes\n"; break;
        case (0x03): std::cout<<"32 KBytes (4 banks of 8KBytes each)\n"; break;
        case (0x04): std::cout<<"128 KBytes (16 banks of 8KBytes each)\n"; break;
        case (0x05): std::cout<<"64 KBytes (8 banks of 8KBytes each)\n"; break;
        default: std::cout<<"Unknown\n"; break;
    }
    std::cout << "Region: " << ((buffer[0x14A] == 0x01) ? "Outside of Japan" : "Japan") << "\n";
    std::cout << "ROM Version: " << (int)buffer[0x14C] << "\n";
    std::cout << "Header checksum: ";
    unsigned char x=0;
    for(int i=0;i!=0x19;i++){
        x=x-buffer[0x134 + i]-1;
    }
    std::cout << ((buffer[0x014D] == x) ? "Valid" : "Invalid") << "\n";
    std::cout << "Global checksum: ";
    unsigned short y=0;
    for(int i = 0; i < fsize; i++)
    {
        if (i != 0x14E && i != 0x14F)
        {
            y += buffer[i];
        }
    }
    unsigned short toCheck = ((unsigned short)buffer[0x14E] << 8) | buffer[0x14F];
    std::cout << ((y == toCheck) ? "Valid" : "Invalid") << "\n";
    return 0;
}