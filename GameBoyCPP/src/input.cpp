#include "includes.hpp"
#include "input.hpp"

/*
Input Register : FF00
  Bit 7 - Not used
  Bit 6 - Not used
  Bit 5 - P15 Select Button Keys      (0=Select)
  Bit 4 - P14 Select Direction Keys   (0=Select)
  Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
  Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
  Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
  Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
Button Mapping : (same as BGB)
  Arrow Keys = DPad
  Enter = Start
  Right Shift = Select
  A key = B
  S key = A
*/

input::input(memory* mem)
{
    input::Memory = mem;
    //Initialise all buttons to not pressed
    input::inputState = 0xFF;
}

void input::keyChanged(SDL_Keycode key, bool value)
{
    bool direction = false;
    byte inputReg = input::Memory->get8(0xFF00);
    switch (key)
    {
        case SDLK_DOWN: direction = true; input::inputState = setBit(input::inputState, 7, value); break;
        case SDLK_UP: direction = true; input::inputState = setBit(input::inputState, 6, value); break;
        case SDLK_LEFT: direction = true; input::inputState = setBit(input::inputState, 5, value); break;
        case SDLK_RIGHT: direction = true; input::inputState = setBit(input::inputState, 4, value); break;
        case SDLK_RETURN: input::inputState = setBit(input::inputState, 3, value); break;
        case SDLK_RSHIFT: input::inputState = setBit(input::inputState, 2, value); break;
        case SDLK_a: input::inputState = setBit(input::inputState, 1, value); break;
        case SDLK_s: input::inputState = setBit(input::inputState, 0, value); break;
    }
    if ((direction && !value && !getBit(inputReg, 4)) || (!direction && !value && !getBit(inputReg, 5)))
    {
        input::Memory->set8(0xFF0F, setBit(input::Memory->get8(0xFF0F), 4, 1)); // request input interrupt
    }
}