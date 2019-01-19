CC=gcc


MACHINE_OBJS = \
	src/machine/machine.c \
 	src/machine/cycle.c

EMULATOR_OBJS = \
	src/emulator/emulator.c \
	src/emulator/timer/timer.c

DISPLAY_OBJS = \
	src/emulator/display/debugger.c \
	src/emulator/display/machine.c \
	src/emulator/display/window.c

OBJS=main.c \
	$(MACHINE_OBJS) \
	$(DEBUGGER_OBJS) \
	$(EMULATOR_OBJS) \
	$(DISPLAY_OBJS)

FLAGS = -Wall -g -O2

LIBS = -lSDL2 -lSDL2_ttf

make: 
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o chip-8
