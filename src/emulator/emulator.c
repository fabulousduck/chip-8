#include "emulator.h"
#include "../machine/machine.h"

void prepare_emulator(Emulator * emulator, Machine * machine) {
    prepare_machine(machine);
    emulator->machine = machine;
    emulator->game_loaded = 0;
    emulator->machine_thread_id = NULL;
    emulator->debugger_thread_id = NULL;
    return; 
}