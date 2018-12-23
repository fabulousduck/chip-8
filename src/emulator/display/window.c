#include <SDL2/SDL.h>


#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

void apply_sdl_linux_renderer_fix(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

//SDL for the machine
SDL_Window * create_window(char * name, int width, int height, int x, int y) {
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow(name, x, y, width, height,0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }

    return window;
}
