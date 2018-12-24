#include <SDL2/SDL.h>
#include "../../machine/machine.h"
#include "../emulator.h"
#include "machine.h"

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


void handle_machine_sdl_event(Machine * machine, SDL_Event * event) {

    switch(event->type) {
        case SDL_QUIT:
            machine->power_state = MACHINE_OFF;
            SDL_Quit();
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            for(int i = 0; i < 16; ++i) {
                if(event->key.keysym.sym == key_map[i]) {
                    const unsigned char state = (event->type == SDL_KEYDOWN) ? 1 : 0;
                    machine->keys[i] = state;
                }
            }
            break;
    }
    return;
}

void update_screen_pixels(Machine * machine, SDL_Renderer * emu_renderer) {
    unsigned short pp = 0;
    for(int y = 0; y < MACHINE_HEIGHT; y += PIXEL_HEIGHT) {
        for(int x = 0; x < MACHINE_WIDTH; x += PIXEL_WIDTH) {
            if(machine->gfx[pp] == 1) {
                SDL_Rect pixel = {x,y,PIXEL_WIDTH,PIXEL_HEIGHT};
                SDL_SetRenderDrawColor(emu_renderer, 255,255,255, 255);
                SDL_RenderDrawRect(emu_renderer, &pixel);
            } else{
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

void store_key_input(Machine * machine, SDL_Event * event, unsigned int storage_register) {
    unsigned int event_key = event->key.keysym.sym;
    for(int i = 0; i < 16; ++i) {
        if(event_key == key_map[i]) {
            machine->V[storage_register] = i;
        }
    }
    return;
}