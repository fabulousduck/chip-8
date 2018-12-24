#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>
#include<string.h>

#include "src/machine/machine.h"
#include "src/debugger/debugger.h"
#include "src/emulator/emulator.h"

int main(int argc, char * argv[])
{
    int c, opt_index;
    static struct option long_options[3] = {
        {"debug", 0, 0, 0},
        {"file", 1, 0, 0},
        { NULL, 0, NULL, 0 }
    };
    
    Machine * machine = malloc(sizeof(Machine));
    Emulator * emulator = malloc(sizeof(Emulator));
    prepare_emulator(emulator, machine);

    while ((c = getopt_long(argc, argv, "f:d", long_options, &opt_index)) != -1) {
        switch (c) {
        case 'd':
            emulator->debugger_active = 1;
            // init_debugger(machine);
            break;
        case 'f':
            load_game(emulator->machine, optarg);
            emulator->game_loaded = 1;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-d debug mode] [-f -file] filepath\n",argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    start_emulator(emulator);
    return 0;
}

