#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include "cycle.h"
#include "machine.h"
#include "../emulator/timer/timer.h"
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
void update_timers(Machine *, Timespec *, Timespec *);

void update_draw_flag(Machine * machine, SDL_Renderer * renderer) {
    if(machine->drawflag == 1) {
        update_screen_pixels(machine,renderer);
        machine->drawflag = 0;
    }  
}

void update_timers(Machine * machine, Timespec * timer, Timespec * timer2) {
    if(machine->delay_timer != 0 || machine->sound_timer != 0) {
        nanosleep(timer, timer2);
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
    printf("machine pc at init %d\n", machine->pc);
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
    unsigned char * game_memory = machine->memory + PROGRAM_MEM_SPACE_START;
    memset(game_memory, 0, sizeof(machine->memory) - PROGRAM_MEM_SPACE_START);
    return;
}
void dumpMem(Machine * machine) {
    for(int i = 0; i < 4096; i++) {
        if(i % 16 == 0) {
            printf("\n %04X | ", i);
        }
        printf("%02X ", machine->memory[i]);
    }
}

void load_game(Machine * machine, char * file_path) {
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
    int bytes_read_error = fread((machine->memory + PROGRAM_MEM_SPACE_START) , bufferSize, 1, fp); // Read in the entire file into memory
    (void)bytes_read_error; //this is done so we dont get an unused variable warn with -Wall
    start_machine(machine);
}



unsigned int get_file_length(FILE * fp) {
    unsigned int length;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}

void start_machine(Machine * machine) {
    machine->power_state = MACHINE_ON;
}