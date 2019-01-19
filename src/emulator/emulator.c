#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "emulator.h"
#include "../machine/cycle.h"
#include "../machine/machine.h"
#include "display/window.h"
#include "display/machine.h"
#include "display/debugger.h"
#include "timer/timer.h"

void prepare_emulator(Emulator * emulator, Machine * machine) {
    prepare_machine(machine);
    emulator->machine = machine;
    emulator->game_loaded = 0;
    emulator->debugger_active = 0;
    emulator->emulator_active = 0;
    return; 
}

void start_emulator(Emulator * emulator) {
    printf("fuck");

    Machine * machine = emulator->machine;
    Timespec * timer = (Timespec*)malloc(sizeof(Timespec));
    Timespec * timer2 = (Timespec*)malloc(sizeof(Timespec));
    SDL_Event event;

    //init timers
    init_timers(timer, timer2);

    //init fonts
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("fonts/ProggyClean.ttf", DEBUGGER_FONT_SIZE); 

    //init machine window
    SDL_Window * emu_window = create_window(emulator, "chip-8");
    SDL_Renderer * emu_renderer = SDL_CreateRenderer(emu_window, -1, 0);

    apply_sdl_linux_renderer_fix(emu_renderer);

    //init debugger areas if debugger is on
    if(emulator->debugger_active) {
       init_debugger_areas(emu_renderer);
    }

    //turn the emulator on
    emulator->emulator_active = EMULATOR_ON;

    int v = 0;
    while(emulator->emulator_active == 1) {
        atexit(SDL_Quit);

        while(SDL_PollEvent(&event)) {
            handle_machine_sdl_event(emulator->machine, &event);
        }  

        if(emulator->debugger_active) {
            render_memory_map(emulator->machine, emu_renderer, font);
        }

        /*
            we check here if the machine is off or not as
            I want to be able to add a feature later that
            allows the machine to be on, but no game loaded

            this is usefull when games need to be ejected
            because they are really crashing the machine
        */
        if(machine->power_state == MACHINE_ON) {
            emulate_cycle(machine, emu_renderer, &event);
            update_draw_flag(machine, emu_renderer);
            update_timers(machine, timer, timer2);

        }
    }
}