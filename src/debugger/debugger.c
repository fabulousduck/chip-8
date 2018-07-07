#include <SDL2/SDL.h>
#include <GL/GLEW.h>
#include "../emulator/display/debugger.h"
#include "../emulator/display/window.h"
#include "../machine/machine.h"

void init_debugger(Machine * machine) {
    SDL_Window * window = create_window("debugger", DEBUGGER_WIDTH, DEBUGGER_HEIGHT,0,0);
    render_some_window(window);

}