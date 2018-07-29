#pragma once

#include <SDL2/SDL.h>
#include "../emulator.h"

#define DEBUGGER_HEIGHT 500
#define DEBUGGER_WIDTH 400

void delegate_debugger_window_event(Emulator *, SDL_Event *);
