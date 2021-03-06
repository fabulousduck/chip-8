#pragma once

#include <SDL2/SDL.h>

SDL_Renderer * create_emu_window();
void handle_machine_sdl_event(Machine *, SDL_Event *);
void update_screen_pixels(Machine *, SDL_Renderer *);
void clear_screen(SDL_Renderer *);
void store_key_input(Machine *, SDL_Event *, unsigned int);
extern unsigned int key_map[16];