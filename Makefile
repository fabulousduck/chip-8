CC=gcc


DEBUGGER_OBJS = src/debugger/debugger.c src/debugger/window.c
MACHINE_OBJS = src/machine/machine.c src/machine/wm.c src/machine/cycle.c
EMULATOR_OBJS = src/emulator/emulator.c
OBJS=main.c $(MACHINE_OBJS) $(DEBUGGER_OBJS) $(EMULATOR_OBJS)

FLAGS = -Wall -g -O2

SRC = main.c

LIBS = -lSDL2 -framework OpenGL -lm -lGLEW

make: SRC
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o chip-8
