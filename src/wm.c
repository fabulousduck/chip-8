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