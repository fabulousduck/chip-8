#include <stdio.h>
include "emu.h"

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

    //TODO load font set

    //load program into rom
    FILE * fp = fopen(fileName, "rb");
    unsigned int bufferSize = getFileLength(fp);
    unsigned int i;

    for(i = 0; i < bufferSize; ++i) {
        emu->memory[PROGRAM_MEM_SPACE_START + i] = fread()
    }

}

unsigned int getFileLength(FILE * fp) {
    unsigned int length;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}
