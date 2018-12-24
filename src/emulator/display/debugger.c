#include <SDL2/SDL.h>
#include "debugger.h"
#include "window.h"
#include "../emulator.h"

void render_memory_map(Machine *, SDL_Renderer *);

void render_debugger(Machine * machine, SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
    SDL_RenderDrawLine(renderer, MACHINE_WIDTH, 0, MACHINE_WIDTH, MACHINE_HEIGHT);
    render_memory_map(machine, renderer);
    SDL_RenderPresent(renderer);

}

void render_memory_map(Machine * machine, SDL_Renderer * renderer) {
    int blocks_per_row = (DEBUGGER_WINDOW_MULTIPLIER * MACHINE_WIDTH) / MEM_BLOCK_WIDTH;
    int current_line = 0;
    int current_col = 0;
    for(int i = 0; i < EMU_MEMORY_SIZE; i++) {
        if(i%blocks_per_row == 0) {
            current_line++;
            current_col = 0;
        }
        SDL_Rect mem_block = {
            current_col * MEM_BLOCK_WIDTH + MACHINE_WIDTH,
            current_line * MEM_BLOCK_HEIGHT,
            MEM_BLOCK_WIDTH,
            MEM_BLOCK_HEIGHT
        };

        int R = machine->memory[i]/(256^2);
        int G = (machine->memory[i]/256) % 256;
        int B = machine->memory[i]%256;

        SDL_SetRenderDrawColor(renderer, R,G,B, 255);
        if(machine->memory[i] != 0) {
            SDL_RenderFillRect(renderer, &mem_block);
        } else {
            SDL_RenderDrawRect(renderer, &mem_block);
        }
        current_col++;
    }
}