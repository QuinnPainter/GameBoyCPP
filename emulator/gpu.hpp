#pragma once

#include "memory.hpp"

class gpu
{
    public:
        gpu(memory* mem);
        ~gpu();
        void update(int cycles);
    private:
        memory* Memory;
        SDL_Window* window;
        SDL_Renderer* screenRenderer;
        int cycleCounter;
        void setStatus();
        void setLCDMode(int mode);
};