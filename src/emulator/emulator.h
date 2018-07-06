#pragma once

#include "../machine/machine.h"

#define MACHINE_ON 1
#define MACHINE_OFF 0

typedef struct emulator_t {
    Machine * machine;
    unsigned int game_loaded;
} Emulator;

void prepare_emulator(Emulator *, Machine *);
