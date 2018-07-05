#include <SDL2/SDL.h>

#include "emu.h"
#include "wm.h"

unsigned int key_map[16] = {
    SDLK_0,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_5,
    SDLK_6,
    SDLK_7,
    SDLK_8,
    SDLK_9,
    SDLK_a,
    SDLK_b,
    SDLK_c,
    SDLK_d,
    SDLK_e,
    SDLK_f
};

SDL_Renderer * create_emu_window()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow("chip-8", 0, 0, EMU_WIDTH, EMU_HEIGHT,0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    //fix for linux systems
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return renderer;
}

void handle_sdl_event(Emu * emu, SDL_Event * event, int * emulator_on_off_state) { 
    switch(event->type) {
        case SDL_QUIT:
            *emulator_on_off_state = EMU_OFF;
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            for(int i = 0; i < 16; ++i) {
                if(event->key.keysym.sym == key_map[i]) {
                    const unsigned char state = (event->type == SDL_KEYDOWN) ? 1 : 0;
                    emu->keys[i] = state;
                }
            }
            break;
    }
    return;
}

void update_screen_pixels(Emu * emu, SDL_Renderer * emu_renderer) {
    unsigned short pp = 0;
    for(int y = 0; y < EMU_HEIGHT; y += PIXEL_HEIGHT) {
        for(int x = 0; x < EMU_WIDTH; x += PIXEL_WIDTH) {
            if(emu->gfx[pp] == 1) {
                SDL_Rect pixel = {x,y,PIXEL_WIDTH,PIXEL_HEIGHT};
                SDL_SetRenderDrawColor(emu_renderer, 255,255,255, 255);
                SDL_RenderDrawRect(emu_renderer, &pixel);
            } else {
                SDL_Rect pixel = {x,y,PIXEL_WIDTH,PIXEL_HEIGHT};
                SDL_SetRenderDrawColor(emu_renderer, 0,0,0,0);
                SDL_RenderDrawRect(emu_renderer, &pixel); 
            }
            ++pp;
        }
    }
    SDL_RenderPresent(emu_renderer);
    return;
}

void clear_screen(SDL_Renderer * emu_renderer) {
    SDL_RenderClear(emu_renderer);
    return;
}

void store_key_input(Emu * emu, SDL_Event * event, unsigned int storage_register) {
    unsigned int event_key = event->key.keysym.sym;
    for(int i = 0; i < 16; ++i) {
        if(event_key == key_map[i]) {
            emu->V[storage_register] = i;
        }
    }
    return;
}