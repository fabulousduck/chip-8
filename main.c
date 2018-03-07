#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "opcodes.h"

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

void init_emu(Emu * emu)
{
    //init registers and memory once
}

void emulate_cycle(Emu * emu)
{
    unsigned int opcode = emu->memory[emu->pc] << 8 | emu->memory[emu->pc +1];
    //decode opcode
    //execute opcode
    //update timers
}

Emu * create_emu()
{
    Emu * emu = malloc(sizeof(Emu));
    Opcode opcodes[OPCODE_COUNT] = {
        {"0NNN", CALL}, {"00E0", DISPLAY}, {"00EE", FLOW},
        {"1NNN", FLOW}, {"2NNN", FLOW}, {"3XNN", COND},
        {"4XNN", COND}, {"5XY0", COND}, {"6XNN", CONST},
        {"7XNN", CONST}, {"8XY0", ASSIGN}, {"8XY1", BITOP},
        {"8XY2", BITOP}, {"8XY3", BITOP}, {"8XY4", MATH},
        {"8XY5", MATH}, {"8XY6", BITOP}, {"8XY7", MATH},
        {"8XYE", BITOP}, {"9XY0", COND}, {"ANNN", MEM},
        {"BNNN", FLOW}, {"CXNN", RAND}, {"DXYN", DISP},
        {"EX9E", KEYOP}, {"EXA1", KEYOP}, {"FX07", TIMER},
        {"FX0A", KEYOP}, {"FX15", TIMER}, {"FX18", SOUND},
        {"FX1E", MEM}, {"FX29", MEM}, {"FX33", BCD},
        {"FX55", MEM}, {"FX65", MEM}
    };
    memcpy(emu->opcodes, opcodes, OPCODE_COUNT);
    return emu;
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
