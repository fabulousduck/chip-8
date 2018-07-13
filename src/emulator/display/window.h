#pragma once
#include <SDL2/SDL.h>
#include "../../../nuklear/nuklear.h"

SDL_Window * create_window(char *, int, int, int, int);
SDL_Renderer * create_window_with_renderer(char *, int, int, int, int);
void create_nk_window(struct nk_context *);
struct nk_context * create_nk_context(SDL_Window *);