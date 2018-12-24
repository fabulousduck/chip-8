#pragma once

#include <SDL2/SDL.h>
#include "../emulator.h"

#define MEM_BLOCK_HEIGHT 10
#define MEM_BLOCK_WIDTH 10


void render_debugger(Machine *, SDL_Renderer *);