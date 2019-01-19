#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../emulator.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

void apply_sdl_linux_renderer_fix(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


SDL_Rect * get_window_dims(Emulator * emulator) {
    SDL_Rect * dims = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    int debugger_x_space, debugger_y_space = 0;
    
    if(emulator->debugger_active) {
        debugger_x_space = DEBUGGER_X_WINDOW_MULTIPLIER * MACHINE_WIDTH;
        debugger_y_space = DEBUGGER_Y_WINDOW_MULTIPLIER * MACHINE_HEIGHT;
    }

    dims->w = MACHINE_WIDTH  + debugger_x_space;
    dims->h = MACHINE_HEIGHT + debugger_y_space;
    dims->x = 0;
    dims->y = 0;

    return dims;
}

SDL_Window * create_window(Emulator * emulator, char * name) {
    SDL_Window *window;
    SDL_Rect * dims = get_window_dims(emulator);
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow(name, dims->x, dims->y, dims->w, dims->h,0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }

    return window;
}

