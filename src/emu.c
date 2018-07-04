#include <stdio.h>
#include <stdlib.h>
#include "emu.h"


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

Emu * create_emu()
{
    Emu * emu = malloc(sizeof(Emu));
    return emu;
}

void init_emu(Emu * emu, char * fileName)
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
    //load program into rom (read only memory)
    FILE * fp = fopen(fileName, "rb");
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

void set_stack_variable(Emu * emu, unsigned int index, unsigned char value) {
    printf("setting emu->V[%d] to %c\n", index, value);
    emu->V[index] = value;
    return;
}
