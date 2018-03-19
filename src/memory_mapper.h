#include "emu.h"

#define MEM_MAP_WINDOW_WIDTH 400
#define MEM_MAP_WINDOW_HEIGHT 500

enum access_type{READ, WRITE};

void display_memory_access(Emu * emu, unsigned int address, enum access_type access);
void display_register_access(Emu * Emu, unsigned int reg, enum access_type access);