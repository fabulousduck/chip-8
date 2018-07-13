#pragma once

#include <pthread.h>
#include "../machine/machine.h"

#define MACHINE_ON 1
#define MACHINE_OFF 0

typedef struct emulator_t {
    Machine * machine;
    unsigned int game_loaded;
    pthread_t machine_thread_id;
    pthread_t debugger_thread_id;
} Emulator;

void prepare_emulator(Emulator *, Machine *);
