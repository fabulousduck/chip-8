#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "emu.h"

SDL_Window * create_emu_window();
void emulate_cycle(Emu * emu);

int main()
{
    //setup variables for SDL2
    SDL_Event event;
    int sdl_running = 1;

    Emu *emu = malloc(sizeof(Emu));
    SDL_Window * emu_window = create_emu_window();

    while(sdl_running == 1) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                sdl_running = 0;
            }
        }
        emulate_cycle(emu);
        //if drawflag, draw graphics
        //set keys

        //this is done so SDL does not exit early through pointer resets
        SDL_Delay(2000);
    }
}

void emulate_cycle(Emu * emu)
{
    unsigned int opcode = emu->memory[emu->pc] << 8 | emu->memory[emu->pc +1];


    //decode opcode
    //execute opcode
    //update timers
}

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
