#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "src/emu.h"
#include "src/wm.h"
#include "src/memory_mapper.h"
#include "src/helpers.h"

void emulate_cycle(Emu * emu);

int main(int argc, char * argv[])
{
    //setup variables for SDL2
    SDL_Event event;
    int sdl_running = 1;

    Emu *emu = malloc(sizeof(Emu));
    init_emu(emu, "./games/PONG");


    SDL_Renderer * emu_renderer = create_emu_window();
    
    while(sdl_running == 1) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                sdl_running = 0;
            }
        }
        emulate_cycle(emu);
        if(emu->drawflag == 1) {
            updateScreenPixels(emu,emu_renderer);
        }
        //set keys

        //this is done so SDL does not exit early through pointer resets
        SDL_Delay(100);
    }
}

void emulate_cycle(Emu * emu)
{
    unsigned int opcode = emu->memory[emu->pc] << 8 | emu->memory[emu->pc +1];
    
    /*
        0x2F5B


    */

    printf("%#04X\n", opcode);
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
            emu->stack[emu->sp] = emu->pc;
            ++emu->sp;
            emu->pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            //skips the next instruction if V[X] == NN
            if(emu->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                emu->pc += 4;
                break;
            }
            emu->pc += 2;
            break;
        case 0x4000:
            //skips the next instruction is V[X] != NN
            if(emu->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                emu->pc += 4;
                break;
            }
            emu->pc += 2;
            break;
        case 0x5000:
            //skips the next instruction if V[X] == V[Y]
            if(emu->V[(opcode & 0x0F00) >> 8] == emu->V[(opcode & 0x00F0) >> 4]) {
                emu->pc += 4;
                break;
            }
            emu->pc += 2;
            break;
        case 0x6000:
            //sets V[X] to value NN
            emu->V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            emu->pc += 2;
            break;
        case 0x7000:
            //adds NN to V[X] (Carry flag is not changed)
            emu->V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF); 
            emu->pc += 2;
            break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000:
                    //sets V[X] to the value of V[Y]
                    emu->V[(opcode & 0x0F00) >> 8] = emu->V[(opcode & 0x00F0) >> 4];
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
            emu->I = opcode & 0x0FFF;
            emu->pc += 2;
            break;
        case 0xB000:
            //jumps to address (NNN + V[0])
            break;
        case 0xC000:
            //sets V[X] to the result of a bitwise AND operation on a random number from 0 - 255 and NN
            break;
        case 0xD000:
            {

                //draws sprite at at coordinates V[X] and V[Y] with a width of 8 pixels.
                //each row of 8 pixels is read as bit-encoded string starting from memory location emu->I.
                //the value of emu->I does not change after the execution of these instructions
                //V[0xF] is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
                unsigned short num_rows = opcode & 0x000F;
                unsigned short x = emu->V[(opcode & 0x0F00) >> 8];
                unsigned short y = emu->V[(opcode & 0x00F0) >> 4];

                emu->V[0xF] = 0;
                for(unsigned short i = 0; i < num_rows; i++) {
                    unsigned short pixel = emu->memory[emu->I + i];
                    for(int row_x_index = 0; row_x_index < 8; row_x_index++) {
                        //the 0x80 equals 1000 0000. every loop the 1 shifts 1 to the right. and then we take that and OR (mask) it with the bits we have
                        if((pixel & (0x80 >> row_x_index)) != 0) { 
                            if(emu->gfx[(x + row_x_index + ((y + i) * 64))] == 1) {
                                emu->V[0xF] = 1;
                            }
                            emu->gfx[x + row_x_index + ((y + i) * 64)] ^= 1; //if LHS XOR RHS. LHS = 1
                            emu->drawflag = 1;
                        }
                    }
                }   
                emu->pc += 2;
                break;
            }
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
                    {
                        int digits = emu->V[(opcode & 0x0F00) >> 8];
                        int i = 0;
                        while(digits != 0){
                            emu->memory[emu->I + i] = digits % 10;
                            digits /= 10;
                            ++i;
                        }
                        emu->pc += 2;
                    }
                    
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
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        emu->V[i] = emu->memory[emu->I];
                        emu->I++;
                    }
                    emu->pc += 2;
                    break;
            }

            break;
            
    }
    return;
    //update timers
}


