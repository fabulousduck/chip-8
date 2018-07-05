#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "src/emulator/emu.h"
#include "src/emulator/wm.h"

int main(int argc, char * argv[])
{
    Emu *emu = malloc(sizeof(Emu));    
    prepare_emu(emu, "./games/TETRIS");
    start_emu(emu);
}

