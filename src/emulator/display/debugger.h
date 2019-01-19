#pragma once

#include <SDL2/SDL.h>
#include "../emulator.h"

#define MEM_BLOCK_HEIGHT 100
#define MEM_BLOCK_WIDTH 10


void render_memory_map(Machine *, SDL_Renderer *, TTF_Font *);
void init_debugger_areas(SDL_Renderer *);
