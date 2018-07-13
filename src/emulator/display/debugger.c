#include <SDL2/SDL.h>
#include "debugger.h"
#include "window.h"
#include "../../../nuklear/nuklear.h"

//we use the SDL bindings for nuklear so we need to provide it a SDL window
void init_debugger_window(SDL_Window * window) {
    struct nk_context * context = create_nk_context(window);
    // create_nk_window(context);
}