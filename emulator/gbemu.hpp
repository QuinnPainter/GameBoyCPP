#pragma once

#include "memory.hpp"
#include "cpu.hpp"
#include "gpu.hpp"
#include "input.hpp"

void initSDL();
byte handleInterrupts(cpu* CPU, memory* mem);
void handleTimers(int cycles, memory* mem);