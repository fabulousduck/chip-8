#include <SDL2/SDL.h>

SDL_Window * create_emu_window();
SDL_Window * create_visualizer_window();
void updateScreenPixels(Emu *, SDL_Window *);