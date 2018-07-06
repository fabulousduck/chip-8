#pragma once

#include <SDL2/SDL.h>

SDL_Renderer * create_emu_window();
void handle_sdl_event(Machine *, SDL_Event *, int *);
void update_screen_pixels(Machine *, SDL_Renderer *);
void clear_screen(SDL_Renderer *);
void store_key_input(Machine *, SDL_Event *, unsigned int);
extern unsigned int key_map[16];