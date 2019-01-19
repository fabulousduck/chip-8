#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "debugger.h"
#include "window.h"
#include "../emulator.h"

void render_memory_map(Machine *, SDL_Renderer *, TTF_Font*);
char * build_memory_string(Machine *, int, int);


/*
    this function renders the lines and all the static things in the debugger
    this function exists so we dont have to redraw a lot of stuff every emulated cycle which
    saves us a lot of resources
*/  
void init_debugger_areas(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
    //right side line
    SDL_RenderDrawLine(renderer, MACHINE_WIDTH, 0, MACHINE_WIDTH, MACHINE_HEIGHT);
    //bottom line
    SDL_RenderDrawLine(renderer, 0, MACHINE_HEIGHT, MACHINE_WIDTH + (DEBUGGER_X_WINDOW_MULTIPLIER * MACHINE_WIDTH), MACHINE_HEIGHT);
    SDL_RenderPresent(renderer);
}

/*
    renders the debuggers memory map viewer.
    must be rerendered every cycle to remain accurate
*/
void render_memory_map(Machine * machine, SDL_Renderer * renderer, TTF_Font * font) {
    int blocks_per_row = ((DEBUGGER_X_WINDOW_MULTIPLIER * MACHINE_WIDTH) / MEM_BLOCK_WIDTH) / 2;
    printf("%d\n", blocks_per_row);
    int current_line = 0;


    for(int i = 0; i < 512; i+=6) {
        int w, h;
        char * render_string = build_memory_string(machine, blocks_per_row, i);
        
        TTF_SizeText(font, render_string, &w, &h);

        SDL_Rect mem_block = {
            0,
            MACHINE_HEIGHT + (current_line * MEM_BLOCK_HEIGHT) + 10,
            w,
            h
        };
        
        SDL_Color White = {255, 255, 255};
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, render_string, White);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        
        SDL_RenderCopy(renderer, Message, NULL, &mem_block); 
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
        current_line++;
    }

    
}

char * build_memory_string(Machine * machine, int blocks_per_row, int initial_offset) {
    char * render_string = (char*)malloc((5*blocks_per_row) * sizeof(char));
    int pos = 0;

    for (int k=0; k < blocks_per_row; k++) {
        pos += sprintf(&render_string[pos], "%04X ", machine->memory[initial_offset + k]);
    }


    return render_string;
}