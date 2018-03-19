#include <stdio.h>
#include "memory_mapper.h"
#include "emu.h"

void display_memory_access(
    Emu * emu, 
    unsigned int address, 
    enum access_type access)
{
    printf("memory visualising [%#04X] with operation [%d] ", address, access);
}

void display_register_access(
    Emu * Emu,
    unsigned int reg,
    enum access_type access)
{
    printf("register visualising [%#04X] with operation [%d] ", reg, access);
}