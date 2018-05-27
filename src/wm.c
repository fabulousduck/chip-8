#include <SDL2/SDL.h>
#include "emu.h"
#include "wm.h"
#include "memory_mapper.h"

SDL_Window * create_emu_window()
{
    SDL_Window *window;
    SDL_Surface *surface = NULL;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow("chip-8", 0, 0, EMU_WIDTH, EMU_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    surface = SDL_GetWindowSurface(window);

    //fix for linux systems
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return window;
}

SDL_Window * create_visualizer_window()
{
    SDL_Window *window;
    SDL_Surface *surface = NULL;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow("chip-8 memory_map", 0, 0, MEM_MAP_WINDOW_WIDTH, MEM_MAP_WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    surface = SDL_GetWindowSurface(window);

    //fix for linux systems
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return window;
}

void updateScreenPixels(Emu * emu, SDL_Window * window) {
    SDL_Surface * surface = SDL_GetWindowSurface(window);
    unsigned short pp = 0;
    for(int row_number = 0; row_number < EMU_HEIGHT; row_number += PIXEL_HEIGHT) {
        for(int x_pixel_pos = 0; x_pixel_pos < EMU_WIDTH; x_pixel_pos += PIXEL_WIDTH) {
            SDL_Rect pixel = {x_pixel_pos,row_number,PIXEL_WIDTH,PIXEL_HEIGHT};
            if(emu->gfx[pp] == 1) {
                printf("drawing pixel gtx[%d]", pp);
                SDL_FillRect(surface, &pixel, SDL_MapRGB(surface->format, 255, 255, 255));
            }
            ++pp;
        }
    }
    return;
}