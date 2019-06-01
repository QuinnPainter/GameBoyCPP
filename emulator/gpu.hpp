#pragma once

#include "memory.hpp"

class gpu
{
    public:
        gpu(memory* mem);
        ~gpu();
        void update(int cycles);
        void displayScreen();
    private:
        memory* Memory;
        SDL_Window* window;
        byte screenData[160 * 144 * 3]{};
        SDL_Surface* screenSurface;
        SDL_Surface* screenBuffer;
        int cycleCounter;
        void setStatus();
        void setLCDMode(int mode);
        void drawScanline();
        void drawBackground();
        byte paletteAdjustColour(byte colour, byte palette);
        void drawPixel(byte x, byte y, byte colour);
};