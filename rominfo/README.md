# Rominfo
This is a command line utility to view basic information about a ROM file.  
Also prints out the boot logo stored in the file, shown when the Gameboy starts.
## Building
For Windows only.
- Install Visual Studio Community 2019
- Build rominfo.sln
## Usage
Accepts the ROM file path as its only argument.  
e.g. `rominfo.exe tetris.gb`
## Example Output
```Game name: TETRIS
Boot logo: Valid
████      ████  ████                                                          ████
██████    ████  ████                ████                                      ████
██████    ████                    ████████                                    ████
████  ██  ████  ████  ████  ████    ████    ████████    ████  ████      ██████████    ████████
████  ██  ████  ████  ██████  ████  ████  ████    ████  ██████  ████  ████    ████  ████    ████
████    ██████  ████  ████    ████  ████  ████████████  ████    ████  ████    ████  ████    ████
████    ██████  ████  ████    ████  ████  ████          ████    ████  ████    ████  ████    ████
████      ████  ████  ████    ████  ████    ██████████  ████    ████    ██████████    ████████
Game Boy Color status: No Game Boy Color enhancements
Super Game Boy enhanced: No
Cart Type: ROM ONLY
ROM Size: 32KByte (no ROM banking)
RAM Size: None
Region: Japan
ROM Version: 1
Header checksum: Valid
Global checksum: Valid
```