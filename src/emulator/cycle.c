#include <SDL2/SDL.h>
#include "emu.h"
#include "wm.h"

void emulate_cycle(Emu * emu, SDL_Renderer * renderer, SDL_Event * event)
{
    unsigned int opcode = emu->memory[emu->pc] << 8 | emu->memory[emu->pc +1];
    unsigned int X = (opcode & 0x0F00) >> 8;
    unsigned int Y = (opcode & 0x00F0) >> 4;
    unsigned int N = opcode & 0x000F;
    unsigned int NN = opcode & 0x00FF;
    unsigned int NNN = opcode & 0x0FFF;

    printf("%#04X\n", opcode);
    switch(opcode & 0xF000) {
        case 0x000:
            switch(opcode & 0x000F) {
                case 0x000E: //return from subroutine
                    emu->pc = emu->stack[emu->sp];
                    --emu->sp;
                    emu->pc += 2;
                    break;
                case 0x0000: //clear display
                    clear_screen(renderer);
                    emu->pc += 2;
                    break;
            }
            break;
        case 0x1000: //goto 0x0NNN;
            emu->pc = opcode & NNN;
            break;
        case 0x2000: //call subroutine at 0x0NNN;
            emu->sp++;
            emu->stack[emu->sp] = emu->pc;
            emu->pc = NNN;
            break;
        case 0x3000: //skips the next instruction if V[X] == NN
            if(emu->V[X] == NN)
                emu->pc += 2;
            emu->pc += 2;
            break;
        case 0x4000: //skips the next instruction is V[X] != NN
            if(emu->V[X] != NN)
                emu->pc += 2;
            emu->pc += 2;
            break;
        case 0x5000: //skips the next instruction if V[X] == V[Y]
            if(emu->V[X] == emu->V[Y])
                emu->pc += 2;
            emu->pc += 2;
            break;
        case 0x6000: //sets V[X] to value NN
            emu->V[X] = NN;
            emu->pc += 2;
            break;
        case 0x7000: //adds NN to V[X] (Carry flag is not changed)
            emu->V[X] += NN; 
            emu->pc += 2;
            break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000: //sets V[X] to the value of V[Y]
                    emu->V[X] = emu->V[Y];
                    emu->pc += 2;
                    break;
                case 0x0001: //sets V[X] to (V[X] OR V[Y]) 
                   emu->V[X] |= emu->V[Y];
                   emu->pc += 2;
                    break;
                case 0x0002: //sets V[X] to (V[X] AND V[Y]) 
                    emu->V[X] &= emu->V[Y];
                    emu->pc += 2; 
                    break;
                case 0x0003: //sets V[X] to (V[X] XOR V[Y]) 
                    emu->V[X] ^= emu->V[Y];
                    break;
                case 0x0004:
                        if(((int)emu->V[X] + (int)emu->V[Y]) < 256)
                            emu->V[0xF] &= 0;
                        else
                            emu->V[0xF] = 1;
                        emu->V[X] += emu->V[Y];
                        emu->pc += 2;
                    break;
                case 0x0005: //subtracts V[Y] from V[X]. when there is a borrow carry flag is set to 0; and 1 when there is not a borrow
                        if(((int)emu->V[X] - (int)emu->V[Y]) >= 0)
                            emu->V[0xF] = 1;
                        else
                            emu->V[0xF] &= 0;
                        emu->V[X] -= emu->V[Y];
                        emu->pc += 2;
                    break;
                case 0x0006: //shifts V[Y] right by one and copies the result to V[X]. V[0xF] is set to the least significant bit of V[Y] before the shift
                        emu->V[0xF] = emu->V[X] & 7;
                        emu->V[X] = emu->V[X] >> 1;
                        emu->pc += 2;
                    break;
                case 0x0007: //sets V[X] to (V[Y] - V[X]). V[0xF] is set to 0 when there is a borrow. it is set to 1 when there is no borrow
                        if(((int)emu->V[X] - (int)emu->V[Y]) > 0)
                            emu->V[0xF] = 1;
                        else
                            emu->V[0xF] &= 0;
                        emu->V[X] = emu->V[Y] - emu->V[X];
                        emu->pc += 2;
                    break;                
                case 0x000E:
                    //shifts V[Y] left by one and copies the result to V[X]. V[0xF] is set to the value of the most significant bit before the shift
                        emu->V[0xF] = emu->V[X] >> 7;
                        emu->V[X] = emu->V[X] << 1;
                        emu->pc += 2;
                    break;
            }
            break;
        case 0x9000: //skips the next instruction if V[X] does not equal V[Y]
            if(emu->V[X] != emu->V[Y])
                emu->pc += 2;
            emu->pc += 2;
            break;
        case 0xA000: //Sets I to the address NNN
            emu->I = NNN;
            emu->pc += 2;
            break;
        case 0xB000: //goto address (NNN + V[0])
            emu->pc = NNN + emu->V[0];
            emu->pc += 2;
            break;
        case 0xC000: //sets V[X] to the result of a bitwise AND operation on a random number from 0 - 255 and NN
            {
                time_t t;
                srand((unsigned) time(&t));
                emu->V[X] = ((rand() % 255) & NN);
                emu->pc += 2;
            }
            break;
        case 0xD000:
            {
                //draws sprite at at coordinates V[X] and V[Y] with a width of 8 pixels.
                //each row of 8 pixels is read as bit-encoded string starting from memory location emu->I.
                //the value of emu->I does not change after the execution of these instructions
                //V[0xF] is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
                unsigned short x = emu->V[X];
                unsigned short y = emu->V[Y];

                emu->V[0xF] = 0;
                for(unsigned short i = 0; i < N; i++) {
                    unsigned short pixel = emu->memory[emu->I + i];
                    for(int row_x_index = 0; row_x_index < 8; row_x_index++) {
                        //the 0x80 equals 1000 0000. every loop the 1 shifts 1 to the right. and then we take that and OR (mask) it with the bits we have
                        if((pixel & (0x80 >> row_x_index)) != 0) { 
                            if(emu->gfx[(x + row_x_index + ((y + i) * 64))] == 1) {
                                emu->V[0xF] = 1;
                            }
                            emu->gfx[x + row_x_index + ((y + i) * 64)] ^= 1; //if LHS XOR RHS. LHS = 1
                        }
                    }
                }
                emu->drawflag = 1;
                emu->pc += 2;
                break;
            }
        case 0xE000:
            switch (opcode & 0x000F) {
                case 0x000E:
                    //skips the next instruction of the key stored in V[X] is pressed
                    if(emu->keys[emu->V[X]]) {
                        emu->pc += 2;
                    }
                    emu->pc += 2;
                    break;
                case 0x0001:
                    //skips the next instruction if the key stored in V[X] is not pressed
                    if(!emu->keys[emu->V[X]]) {
                        emu->pc += 2;
                    }
                    emu->pc += 2;
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    //sets V[X] to the value of the delay timer
                    emu->V[X] = emu->delay_timer;
                    emu->pc += 2;  
                    break;
                case 0x000A:
                    //A key press is awaited and then stored in V[X]. (blocking operation. all execution is halted untill next key event)
                    {
                        printf("keypress awaited : \n");
                        SDL_Event event;
                        while(SDL_PollEvent(&event)) {
                            if(event.type == SDL_KEYDOWN) {
                                store_key_input(emu, &event, X);
                            }
                        }
                        emu->pc += 2;
                    }
                    break;
                case 0x0015:
                    //sets the delay timer to V[X]
                    emu->delay_timer = emu->V[X];
                    emu->pc += 2;
                    break;
                case 0x0018:
                    //sets the sound timer to V[X]
                    emu->sound_timer = emu->V[X];
                    emu->pc += 2;
                    break;
                case 0x001E:
                    //adds V[X] to emu->I
                    emu->I += emu->V[X];
                    emu->pc += 2;
                    break;
                case 0x0029:
                    //sets emu->I to the location of the sprite for the character currently in V[X].
                    //characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    emu->I = emu->V[X] * 5;
                    emu->pc += 2;
                    break;
                case 0x0033: //stores V[X] as decimal number in its separate in at memory addresses emu->I / emu->I+1 / emu->I+2
                    emu->memory[emu->I]     = emu->V[X] / 100;
                    emu->memory[emu->I + 1] = (emu->V[X] / 10) % 10;
                    emu->memory[emu->I + 2] = (emu->V[X] % 100) % 10;
                    emu->pc += 2;
                    break;
                case 0x0055: //stores V[0] to V[X] (including V[X]) in memory address starting at emu->I.
                    for(int i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
                        emu->memory[emu->I + i] = emu->V[i];
                    }
                    emu->pc += 2;
                    break;
                case 0x0065: //fills V[0] to V[X] (including V[X]) with the values starting at memory address emu->I.
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        emu->V[i] = emu->memory[emu->I + i];
                    }
                    emu->pc += 2;
                    break;
            }

            break;
            
    }
    return;
}