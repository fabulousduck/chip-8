#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "cycle.h"
#include "machine.h"
#include "../emulator/emulator.h"
#include "../emulator/display/window.h"
#include "../emulator/display/machine.h"

unsigned char fontset[80] = 
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

unsigned int get_file_length(FILE *);
void load_game(Machine *, char *);
void update_draw_flag(Machine *, SDL_Renderer *);
void update_timers(Machine *, struct timespec, struct timespec);

void start_machine(Machine * machine) {
    SDL_Event event;
    struct timespec timer, timer2;

    timer.tv_sec = 0;
    timer.tv_nsec = 1600000; //if you want the real old hardware chip-8 expirience, set this to 160000000. This is basically downclocking your timer speed
    timer2.tv_sec = 0;
    timer2.tv_nsec = 0;

    SDL_Renderer * renderer = create_window_with_renderer("chip-8", MACHINE_WIDTH, MACHINE_HEIGHT,0,0);
    SDL_Delay(1);

    machine->power_state = MACHINE_ON;

    while(machine->power_state == MACHINE_ON) {
        while(SDL_PollEvent(&event)) {
            handle_sdl_event(machine, &event);
        }
        emulate_cycle(machine, renderer, &event);
        update_draw_flag(machine, renderer);
        update_timers(machine, timer, timer2);
    }
    return;
}

void update_draw_flag(Machine * machine, SDL_Renderer * renderer) {
    if(machine->drawflag == 1) {
        update_screen_pixels(machine,renderer);
        machine->drawflag = 0;
    }  
}

void update_timers(Machine * machine, struct timespec timer, struct timespec timer2) {
    if(machine->delay_timer != 0 || machine->sound_timer != 0) {
        nanosleep(&timer, &timer2);
        machine->delay_timer != 0 ? --machine->delay_timer : machine->delay_timer;
        machine->sound_timer != 0 ? --machine->sound_timer : machine->sound_timer;
    }
    return;
}

void prepare_machine(Machine * machine)
{
    //init registers and memory once
    machine->power_state = MACHINE_OFF;
    machine->pc = PROGRAM_MEM_SPACE_START;
    machine->opcode = 0;
    machine->I = 0;
    machine->sp = 0;
    machine->drawflag = 0;     
    //load font set
    for(int i = 0; i < 80; ++i) {
        machine->memory[i] = fontset[i];
    }
    return;
}

void reset_game_memory(Machine * machine) {
    unsigned char * game_memory =machine->memory + PROGRAM_MEM_SPACE_START;
    memset(game_memory, 0, sizeof(machine->memory) - PROGRAM_MEM_SPACE_START);
    return;
}

void load_game(Machine * machine, char * file_path) {
    //check if there is already a game loaded
    for(int i = 0; i < (EMU_MEMORY_SIZE - PROGRAM_MEM_SPACE_START); ++i) {
        if(machine->memory[PROGRAM_MEM_SPACE_START + i] != 0) {
            reset_game_memory(machine);
            break;
        }
    }
    //load program into rom (read only memory)
    FILE * fp = fopen(file_path, "rb");
    if(fp == NULL) {
        printf("could not find file at filepath %s", file_path);
        return;
    }
    
    unsigned int bufferSize = get_file_length(fp);
    fread((machine->memory + PROGRAM_MEM_SPACE_START) , bufferSize, 1, fp); // Read in the entire file into memory

    printf("read %d bytes into memory\n", bufferSize);
}

unsigned int get_file_length(FILE * fp) {
    unsigned int length;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}