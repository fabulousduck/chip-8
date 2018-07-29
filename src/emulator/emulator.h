#pragma once

#include <pthread.h>
#include "../machine/machine.h"

#define MACHINE_ON 1
#define MACHINE_OFF 0
#define EMULATOR_ON 1
#define EMULATOR_OFF 0

typedef struct emulator_t {
    Machine * machine;
    unsigned int emulator_active;
    unsigned int game_loaded;
    unsigned int debugger_active;
} Emulator;

void prepare_emulator(Emulator *, Machine *);
void start_emulator(Emulator *);