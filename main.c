#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "emu.h"

SDL_Window * create_emu_window();
void emulate_cycle(Emu * emu);

int main()
{
    //setup variables for SDL2
    SDL_Event event;
    int sdl_running = 1;

    Emu *emu = malloc(sizeof(Emu));
    init_emu(emu, "./games/PONG");
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
        SDL_Delay(100);
    }
}

void emulate_cycle(Emu * emu)
{
    unsigned int opcode = emu->memory[emu->pc] << 8 | emu->memory[emu->pc +1];
    
    printf("%#04X\n", opcode);
    emu->pc += 2;
    return;
    switch(opcode & 0xF000) {
        case 0x000:
            switch(opcode & 0x000F) {
                case 0x000E:
                    //return from subroutine
                    break;
                case 0x0000:
                    //clear display;
                    break;
            }
            break;
        case 0x1000:
            //goto 0x0NNN;
            break;
        case 0x2000:
            //call subroutine at 0x0NNN;
            break;
        case 0x3000:
            //skips the next instruction if V[X] == NN
            break;
        case 0x4000:
            //skips the next instruction is V[X] != NN
            break;
        case 0x5000:
            //skips the next instruction if V[X] == V[Y]
            break;
        case 0x6000:
            //sets V[X] to value NN
            break;
        case 0x7000:
            //adds NN to V[X] (Carry flag is not changed)
            break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000:
                    //sets V[X] to the value of V[Y]
                    break;
                case 0x0001:
                    //sets V[X] to (V[X] OR V[Y]) 
                    break;
                case 0x0002:
                    //sets V[X] to (V[X] AND V[Y]) 
                    break;
                case 0x0003:
                    //sets V[X] to (V[X] XOR V[Y]) 
                    break;
                case 0x0004:
                    //adds V[X] to V[Y] use carry flag if needed
                    break;
                case 0x0005:
                    //subtracts V[Y] from V[X]. when there is a borrow carry flag is set to 0; and 1 when there is not a borrow
                    break;
                case 0x0006:
                    //shifts V[Y] right by one and copies the result to V[X]. V[0xF] is set to the least significant bit of V[Y] before the shift
                    break;
                case 0x0007:
                    //sets V[X] to (V[Y] - V[X]). V[0xF] is set to 0 when there is a borrow. it is set to 1 when there is no borrow
                    break;                
                case 0x000E:
                    //shifts V[Y] left by one and copies the result to V[X]. V[0xF] is set to the value of the most significant bit before the shift
                    break;
            }
            break;
        case 0x9000:
            //skips the next instruction if V[X] does not equal V[Y]
            break;
        case 0xA000:
            //Sets I to the address NNN
            break;
        case 0xB000:
            //jumps to address (NNN + V[0])
            break;
        case 0xC000:
            //sets V[X] to the result of a bitwise AND operation on a random number from 0 - 255 and NN
            break;
        case 0xD000:
            //draws sprite at at coordinates V[X] and V[Y] with a width of 8 pixels.
            //each row of 8 pixels is read as bit-encoded starting from memory location emu->I.
            //the value of emu->I does not change after the execution of these instructions
            //V[0xF] is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
            break;
        case 0xE000:
            switch (opcode & 0x000F) {
                case 0x000E:
                    //skips the next instruction of the key stored in V[X] is pressed
                    break;
                case 0x0001:
                    //skips the next instruction if the key stored in V[X] is not pressed
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    //sets V[X] to the value of the delay timer
                    break;
                case 0x000A:
                    //A key press is awaited and then stored in V[X]. (blocking operation. all execution is halted untill next key event)
                    break;
                case 0x0015:
                    //sets the delay timer to V[X]
                    break;
                case 0x0018:
                    //sets the sound timer to V[X]
                    break;
                case 0x001E:
                    //adds V[X] to emu->I
                    break;
                case 0x0029:
                    //sets emu->I to the location of the sprite for the character in V[X].
                    //characters 0-F (in hexadecimal) are represented by a 4x5 font.
                case 0x0033:
                    //stores V[X] as decimal number in its separate in at memory addresses emu->I / emu->I+1 / emu->I+2
                    //I.E
                    //decimal V[X] == 123 would be
                    //store 1 in emu->memory[emu->I]
                    //store 2 in emu->memory[emu->I + 1]
                    //store 3 in emu->memory[emu->I + 2]

                case 0x0055:
                    //stores V[0] to V[X] (including V[X]) in memory address starting at emu->I.
                    //emu->I is increased for every value written
                    break;
                case 0x0065:
                    //fills V[0] to V[X] (including V[X]) with the values starting at memory address emu->I.
                    //emu->I is increased for every value written
                    break;
            }

            break;
            
    }
    //decode opcode
    //execute opcode
    //update timers
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
