#include <SDL2/SDL.h>

SDL_Window * create_window(char * name, int width, int height, int x, int y) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow(name, x, y, width, height,0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    //fix for linux systems
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return window;
}

SDL_Renderer * create_window_with_renderer(char * name, int width, int height, int x, int y) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow(name, x, y, width, height,0);
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