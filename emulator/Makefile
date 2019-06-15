OBJS = gbemu.cpp logging.cpp memory.cpp cpu.cpp gpu.cpp input.cpp sound.cpp squarechannel.cpp wavechannel.cpp noisechannel.cpp helpers.cpp
CC = g++
INCLUDE_PATHS = -I"C:\src\SDL2-2.0.9\i686-w64-mingw32\include\SDL2"
LIBRARY_PATHS = -L"C:\src\SDL2-2.0.9\i686-w64-mingw32\lib"
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
OBJ_NAME = gbemu

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -Wall -o $(OBJ_NAME)