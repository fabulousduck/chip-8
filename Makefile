CC=gcc

OBJS=main.c src/emulator/emu.c src/emulator/wm.c src/emulator/cycle.c

FLAGS = -Wall -g -O2

SRC = main.c

LIBS = -lSDL2 -framework OpenGL -lm -lGLEW

make: SRC
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o chip-8
