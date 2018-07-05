#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "cycle.h"
#include "emu.h"
#include "wm.h"


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
void load_game(Emu *, char *);
void update_draw_flag(Emu *, SDL_Renderer *);
void update_timers(Emu *, struct timespec, struct timespec);

void start_emu(Emu * emu) {
    SDL_Event event;
    struct timespec timer, timer2;

    timer.tv_sec = 0;
    timer.tv_nsec = 1600000; //if you want the real old hardware chip-8 expirience, set this to 160000000. This is basically downclocking your CPU cycle speed
    timer2.tv_sec = 0;
    timer2.tv_nsec = 0;
    
    int * emulator_on_off_state = malloc(sizeof(int));
    *emulator_on_off_state = EMU_ON;
    
    SDL_Renderer * renderer = create_emu_window();
    SDL_Delay(1);    
    while(*emulator_on_off_state == EMU_ON) {
        while(SDL_PollEvent(&event)) {
            handle_sdl_event(emu, &event, emulator_on_off_state);
        }
        emulate_cycle(emu, renderer, &event);
        update_draw_flag(emu, renderer);
        update_timers(emu, timer, timer2);
    }
    free(emulator_on_off_state);
    return;
}

void update_draw_flag(Emu * emu, SDL_Renderer * renderer) {
    if(emu->drawflag == 1) {
        update_screen_pixels(emu,renderer);
        emu->drawflag = 0;
    }  
}

void update_timers(Emu * emu, struct timespec timer, struct timespec timer2) {
    if(emu->delay_timer != 0 || emu->sound_timer != 0) {
        nanosleep(&timer, &timer2);
        emu->delay_timer != 0 ? --emu->delay_timer : emu->delay_timer;
        emu->sound_timer != 0 ? --emu->sound_timer : emu->sound_timer;
    }
    return;
}

void prepare_emu(Emu * emu, char * file_path)
{
    //init registers and memory once
    emu->pc = 0x200;
    emu->opcode = 0;
    emu->I = 0;
    emu->sp = 0;
    emu->drawflag = 0;     
    //load font set
    for(int i = 0; i < 80; ++i) {
        emu->memory[i] = fontset[i];
    }

    load_game(emu, file_path);

}

void load_game(Emu * emu, char * file_path) {
    //load program into rom (read only memory)
    FILE * fp = fopen(file_path, "rb");
    unsigned int bufferSize = get_file_length(fp);
    fread((emu->memory + PROGRAM_MEM_SPACE_START) , bufferSize, 1, fp); // Read in the entire file into memory

    printf("read %d bytes into memory\n", bufferSize);
}

unsigned int get_file_length(FILE * fp) {
    unsigned int length;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}
