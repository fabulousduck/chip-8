#include <SDL2/SDL.h>
#include "emu.h"
#include "wm.h"
#include "memory_mapper.h"

SDL_Renderer * create_emu_window()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow("chip-8", 0, 0, EMU_WIDTH, EMU_HEIGHT, 0);
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


void updateScreenPixels(Emu * emu, SDL_Renderer * emu_renderer) {


    unsigned short pp = 0;
    for(int y = 0; y < EMU_HEIGHT; y += PIXEL_HEIGHT) {
        for(int x = 0; x < EMU_WIDTH; x += PIXEL_WIDTH) {
            if(emu->gfx[pp] == 1) {
                SDL_Rect pixel = {x,y,PIXEL_WIDTH,PIXEL_HEIGHT};
                SDL_SetRenderDrawColor(emu_renderer, 255,255,255, 255);
                SDL_RenderDrawRect(emu_renderer, &pixel);
            }
            ++pp;
        }
    }
    SDL_RenderPresent(emu_renderer);
    return;
}