#include <SDL2/SDL.h>
#include "emulator.h"
#include "../machine/cycle.h"
#include "../machine/machine.h"
#include "display/window.h"
#include "display/machine.h"
#include "display/debugger.h"

void prepare_emulator(Emulator * emulator, Machine * machine) {
    prepare_machine(machine);
    emulator->machine = machine;
    emulator->game_loaded = 0;
    emulator->debugger_active = 0;
    emulator->emulator_active = 0;
    return; 
}

void start_emulator(Emulator * emulator) {
    Machine * machine = emulator->machine;
    struct timespec timer, timer2;
    SDL_Event event;
    timer.tv_sec = 0;
    timer.tv_nsec = 1600000; //if you want the real old hardware chip-8 expirience, set this to 160000000. This is basically downclocking your timer speed
    timer2.tv_sec = 0;
    timer2.tv_nsec = 0;

    //TODO maybe make a general window creastor with struct
    SDL_Window * machine_window = create_window("chip-8", MACHINE_WIDTH, MACHINE_HEIGHT,0,0);
    SDL_Renderer * machine_renderer = SDL_CreateRenderer(machine_window, -1, 0);
    apply_sdl_linux_renderer_fix(machine_renderer);
    int machine_window_id = SDL_GetWindowID(machine_window);
    int debugger_window_id = 0;


    if(emulator->debugger_active) {
        SDL_Window * debugger_window = create_window("chip-8 debugger", DEBUGGER_WIDTH, DEBUGGER_HEIGHT,0,0);
        SDL_Renderer * debugger_renderer = SDL_CreateRenderer(machine_window, -1, 0);   
        apply_sdl_linux_renderer_fix(debugger_renderer);
        debugger_window_id = SDL_GetWindowID(debugger_window);
    }

    emulator->emulator_active = EMULATOR_ON;

    while(emulator->emulator_active == EMULATOR_ON) {
        while(SDL_PollEvent(&event)) {
            int window_id = event.window.windowID;
            if(window_id == machine_window_id) {
                handle_machine_sdl_event(emulator->machine, &event);
            } else if(event.window.windowID == debugger_window_id) {
                delegate_debugger_window_event(emulator, &event);
            }
        }

        if(machine->power_state == MACHINE_ON) {
            emulate_cycle(machine, machine_renderer, &event);
            update_draw_flag(machine, machine_renderer);
            update_timers(machine, timer, timer2);
        }
        
    }
}