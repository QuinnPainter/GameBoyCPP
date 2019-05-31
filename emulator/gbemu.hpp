#pragma once

#include "memory.hpp"
#include "cpu.hpp"
#include "gpu.hpp"

void initSDL();
void handleInterrupts(cpu* CPU, memory* mem);
void handleTimers(int cycles, memory* mem);