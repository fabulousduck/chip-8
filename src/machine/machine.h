#pragma once

#include <SDL2/SDL.h>
#include "../emulator/timer/timer.h"

#define PIXEL_WIDTH 20
#define PIXEL_HEIGHT 20
#define MACHINE_WIDTH 64 * PIXEL_WIDTH
#define MACHINE_HEIGHT 32 * PIXEL_HEIGHT
#define PROGRAM_MEM_SPACE_START 512
#define EMU_MEMORY_SIZE 4096
#define SPRITE_WIDTH 8

//memory map for the machine
//  0x000 - 0x1FF : Machine interpreter (including the font set)
//  0x050 - 0x0A0 : Used for the 4x5 pixel font set ( 0 - F)
//  0x200 - 0xFFF : Program rom and work RAM

typedef struct machine_t
{
    unsigned int power_state; //this is just so it can turn itself off
    unsigned char gfx[MACHINE_WIDTH * MACHINE_HEIGHT];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char memory[EMU_MEMORY_SIZE];
    unsigned char keys[16];     //keypad state (this is hex based)
    unsigned char V[16];        //chip-8 only has 15 registers for use, F register is the carry flag
    unsigned short I;           //this is the index register
    unsigned short pc;          //this is the program counter
    unsigned short stack[16];   //16 stack levels
    unsigned short sp;          //the stack pointer
    unsigned short opcode;      //current opcode
    unsigned short drawflag;    //draw flag
} Machine;

void prepare_machine(Machine *);
void start_machine(Machine *);
void load_game(Machine *, char *);
void update_draw_flag(Machine *, SDL_Renderer *);
void update_timers(Machine *, Timespec *, Timespec *);



/*
0x200    a = 10
0x204    b = 20
0x208    for(a != b) {
0x20B      a++
0x20F    }



MOV 10 0x1
MOV 10 0x2
0x20F NEQ A B
ADD 0x1 1
JMP 0x20F
*/


