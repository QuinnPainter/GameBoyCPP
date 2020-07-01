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
        //SDL_Surface* screenSurface;
        //SDL_Surface* screenBuffer;
        SDL_Texture* screenTexture;
        SDL_Renderer* screenRenderer;
        int cycleCounter;
        int currentWindowLine;
        void setLCDMode(byte mode);
        void drawScanline();
        void drawBackground();
        void drawSprites();
        byte paletteAdjustColour(byte colour, byte palette);
        void drawPixel(byte x, byte y, byte colour);
};