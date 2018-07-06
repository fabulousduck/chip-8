#include <SDL2/SDL.h>
#include "machine.h"
#include "../emulator/display/machine.h"

void emulate_cycle(Machine * machine, SDL_Renderer * renderer, SDL_Event * event)
{
    unsigned int opcode = machine->memory[machine->pc] << 8 | machine->memory[machine->pc +1];
    unsigned int X = (opcode & 0x0F00) >> 8;
    unsigned int Y = (opcode & 0x00F0) >> 4;
    unsigned int N = opcode & 0x000F;
    unsigned int NN = opcode & 0x00FF;
    unsigned int NNN = opcode & 0x0FFF;

    switch(opcode & 0xF000) {
        case 0x000:
            switch(opcode & 0x000F) {
                case 0x000E: //return from subroutine
                    machine->pc = machine->stack[machine->sp];
                    --machine->sp;
                    machine->pc += 2;
                    break;
                case 0x0000: //clear display
                    clear_screen(renderer);
                    machine->pc += 2;
                    break;
            }
            break;
        case 0x1000: //goto 0x0NNN;
            machine->pc = opcode & NNN;
            break;
        case 0x2000: //call subroutine at 0x0NNN;
            machine->sp++;
            machine->stack[machine->sp] = machine->pc;
            machine->pc = NNN;
            break;
        case 0x3000: //skips the next instruction if V[X] == NN
            if(machine->V[X] == NN)
                machine->pc += 2;
            machine->pc += 2;
            break;
        case 0x4000: //skips the next instruction is V[X] != NN
            if(machine->V[X] != NN)
                machine->pc += 2;
            machine->pc += 2;
            break;
        case 0x5000: //skips the next instruction if V[X] == V[Y]
            if(machine->V[X] == machine->V[Y])
                machine->pc += 2;
            machine->pc += 2;
            break;
        case 0x6000: //sets V[X] to value NN
            machine->V[X] = NN;
            machine->pc += 2;
            break;
        case 0x7000: //adds NN to V[X] (Carry flag is not changed)
            machine->V[X] += NN; 
            machine->pc += 2;
            break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000: //sets V[X] to the value of V[Y]
                    machine->V[X] = machine->V[Y];
                    machine->pc += 2;
                    break;
                case 0x0001: //sets V[X] to (V[X] OR V[Y]) 
                   machine->V[X] |= machine->V[Y];
                   machine->pc += 2;
                    break;
                case 0x0002: //sets V[X] to (V[X] AND V[Y]) 
                    machine->V[X] &= machine->V[Y];
                    machine->pc += 2; 
                    break;
                case 0x0003: //sets V[X] to (V[X] XOR V[Y]) 
                    machine->V[X] ^= machine->V[Y];
                    break;
                case 0x0004:
                        if(((int)machine->V[X] + (int)machine->V[Y]) < 256)
                            machine->V[0xF] &= 0;
                        else
                            machine->V[0xF] = 1;
                        machine->V[X] += machine->V[Y];
                        machine->pc += 2;
                    break;
                case 0x0005: //subtracts V[Y] from V[X]. when there is a borrow carry flag is set to 0; and 1 when there is not a borrow
                        if(((int)machine->V[X] - (int)machine->V[Y]) >= 0)
                            machine->V[0xF] = 1;
                        else
                            machine->V[0xF] &= 0;
                        machine->V[X] -= machine->V[Y];
                        machine->pc += 2;
                    break;
                case 0x0006: //shifts V[Y] right by one and copies the result to V[X]. V[0xF] is set to the least significant bit of V[Y] before the shift
                        machine->V[0xF] = machine->V[X] & 7;
                        machine->V[X] = machine->V[X] >> 1;
                        machine->pc += 2;
                    break;
                case 0x0007: //sets V[X] to (V[Y] - V[X]). V[0xF] is set to 0 when there is a borrow. it is set to 1 when there is no borrow
                        if(((int)machine->V[X] - (int)machine->V[Y]) > 0)
                            machine->V[0xF] = 1;
                        else
                            machine->V[0xF] &= 0;
                        machine->V[X] = machine->V[Y] - machine->V[X];
                        machine->pc += 2;
                    break;                
                case 0x000E:
                    //shifts V[Y] left by one and copies the result to V[X]. V[0xF] is set to the value of the most significant bit before the shift
                        machine->V[0xF] = machine->V[X] >> 7;
                        machine->V[X] = machine->V[X] << 1;
                        machine->pc += 2;
                    break;
            }
            break;
        case 0x9000: //skips the next instruction if V[X] does not equal V[Y]
            if(machine->V[X] != machine->V[Y])
                machine->pc += 2;
            machine->pc += 2;
            break;
        case 0xA000: //Sets I to the address NNN
            machine->I = NNN;
            machine->pc += 2;
            break;
        case 0xB000: //goto address (NNN + V[0])
            machine->pc = NNN + machine->V[0];
            machine->pc += 2;
            break;
        case 0xC000: //sets V[X] to the result of a bitwise AND operation on a random number from 0 - 255 and NN
            {
                time_t t;
                srand((unsigned) time(&t));
                machine->V[X] = ((rand() % 255) & NN);
                machine->pc += 2;
            }
            break;
        case 0xD000:
            {
                //draws sprite at at coordinates V[X] and V[Y] with a width of 8 pixels.
                //each row of 8 pixels is read as bit-encoded string starting from memory location machine->I.
                //the value of machine->I does not change after the execution of these instructions
                //V[0xF] is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
                unsigned short x = machine->V[X];
                unsigned short y = machine->V[Y];

                machine->V[0xF] = 0;
                for(unsigned short i = 0; i < N; i++) {
                    unsigned short pixel = machine->memory[machine->I + i];
                    for(int row_x_index = 0; row_x_index < 8; row_x_index++) {
                        //the 0x80 equals 1000 0000. every loop the 1 shifts 1 to the right. and then we take that and OR (mask) it with the bits we have
                        if((pixel & (0x80 >> row_x_index)) != 0) { 
                            if(machine->gfx[(x + row_x_index + ((y + i) * 64))] == 1) {
                                machine->V[0xF] = 1;
                            }
                            machine->gfx[x + row_x_index + ((y + i) * 64)] ^= 1; //if LHS XOR RHS. LHS = 1
                        }
                    }
                }
                machine->drawflag = 1;
                machine->pc += 2;
                break;
            }
        case 0xE000:
            switch (opcode & 0x000F) {
                case 0x000E:
                    //skips the next instruction of the key stored in V[X] is pressed
                    if(machine->keys[machine->V[X]]) {
                        machine->pc += 2;
                    }
                    machine->pc += 2;
                    break;
                case 0x0001:
                    //skips the next instruction if the key stored in V[X] is not pressed
                    if(!machine->keys[machine->V[X]]) {
                        machine->pc += 2;
                    }
                    machine->pc += 2;
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    //sets V[X] to the value of the delay timer
                    machine->V[X] = machine->delay_timer;
                    machine->pc += 2;  
                    break;
                case 0x000A:
                    //A key press is awaited and then stored in V[X]. (blocking operation. all execution is halted untill next key event)
                    //TODO should this not be done with a halt flag or something ?
                    {
                        printf("keypress awaited : \n");
                        SDL_Event event;
                        while(SDL_PollEvent(&event)) {
                            if(event.type == SDL_KEYDOWN) {
                                store_key_input(machine, &event, X);
                            }
                        }
                        machine->pc += 2;
                    }
                    break;
                case 0x0015:
                    //sets the delay timer to V[X]
                    machine->delay_timer = machine->V[X];
                    machine->pc += 2;
                    break;
                case 0x0018:
                    //sets the sound timer to V[X]
                    machine->sound_timer = machine->V[X];
                    machine->pc += 2;
                    break;
                case 0x001E:
                    //adds V[X] to machine->I
                    machine->I += machine->V[X];
                    machine->pc += 2;
                    break;
                case 0x0029:
                    //sets machine->I to the location of the sprite for the character currently in V[X].
                    //characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    machine->I = machine->V[X] * 5;
                    machine->pc += 2;
                    break;
                case 0x0033: //stores V[X] as decimal number in its separate in at memory addresses machine->I / machine->I+1 / machine->I+2
                    machine->memory[machine->I]     = machine->V[X] / 100;
                    machine->memory[machine->I + 1] = (machine->V[X] / 10) % 10;
                    machine->memory[machine->I + 2] = (machine->V[X] % 100) % 10;
                    machine->pc += 2;
                    break;
                case 0x0055: //stores V[0] to V[X] (including V[X]) in memory address starting at machine->I.
                    for(int i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
                        machine->memory[machine->I + i] = machine->V[i];
                    }
                    machine->pc += 2;
                    break;
                case 0x0065: //fills V[0] to V[X] (including V[X]) with the values starting at memory address machine->I.
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        machine->V[i] = machine->memory[machine->I + i];
                    }
                    machine->pc += 2;
                    break;
            }

            break;
            
    }
    return;
}