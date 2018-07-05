CC=gcc

OBJS=main.c src/emu.c src/wm.c src/memory_mapper.c

FLAGS = -Wall -g -O2

SRC = main.c

LIBS = -lSDL2 -framework OpenGL -lm -lGLEW

make: SRC
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o chip-8
