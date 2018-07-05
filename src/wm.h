#include <SDL2/SDL.h>

SDL_Renderer * create_emu_window();
void update_screen_pixels(Emu *, SDL_Renderer *);
void clear_screen(SDL_Renderer *);
void store_key_input(Emu *, SDL_Event *, unsigned int);
extern unsigned int key_map[16];