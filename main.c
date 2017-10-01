#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define EMU_WIDTH 64
#define EMU_HEIGHT 32

typedef struct opcode_t
{
    unsigned int code;
} Opcode;

//memory map for the emu
//  0x000 - 0x1FF : Emu interpreter (including the font set)
//  0x050 - 0x0A0 : Used for the 4x5 pixel font set ( 0 - F)
//  0x200 - 0xFFF : Program rom and work RAM
typedef struct emu_t
{
    unsigned char gfx[EMU_WIDTH * EMU_HEIGHT];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char memory[4096];
    unsigned char key[16];      //keypad state (this is hex based)
    unsigned char V[16];        //chip-8 only has 15 registers, E register is the carry flag
    unsigned short I;           //this is the index register
    unsigned short pc;          //this is the program counter
    unsigned short stack[16];   //16 stack levels
    unsigned short sp;          //the stack pointer
    Opcode opcodes[35];         //general opcode storage
} Emu;

SDL_Window * create_emu_window();


int main()
{
    Emu *emu = malloc(sizeof(Emu));
    printf("%d\n%d\n", EMU_WIDTH, EMU_HEIGHT);
    SDL_Window * emu_window = create_emu_window();
}

SDL_Window * create_emu_window()
{
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("chip-8", 0, 0, EMU_WIDTH, EMU_HEIGHT, 0);

    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }

    return window;
}
