#pragma once
#include <SDL2/SDL.h>
#include "../../../nuklear/nuklear.h"

SDL_Window * create_window(char *, int, int, int, int);
SDL_Renderer * create_window_with_renderer(char *, int, int, int, int);
void create_nk_window(struct nk_context *);
void apply_sdl_linux_renderer_fix(SDL_Renderer *);
void set_SDL_nuklear_attributes();
void set_nk_font_atlas();
struct nk_context * create_nk_context(SDL_Window *);