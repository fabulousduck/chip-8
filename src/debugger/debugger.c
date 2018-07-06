#include <SDL2/SDL.h>
#include "window.h"
#include "../machine/machine.h"

void init_debugger(Machine * machine) {
    SDL_Window * window = create_debugger_window();
}