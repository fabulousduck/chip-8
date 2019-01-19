#pragma once
#include <SDL2/SDL.h>

SDL_Window * create_window(Emulator *, char *);
void apply_sdl_linux_renderer_fix(SDL_Renderer *);
