#pragma once

#include "memory.hpp"

class input
{
    public:
        input(memory* mem);
        void keyChanged(SDL_Keycode key, bool value);
        byte inputState;
    private:
        memory* Memory;
};