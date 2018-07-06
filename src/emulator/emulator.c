#include "emulator.h"
#include "../machine/machine.h"

void prepare_emulator(Emulator * emulator, Machine * machine) {
    prepare_machine(machine);
    emulator->machine = machine;
    emulator->game_loaded = 0;
    return; 
}