CC=gcc
OBJS=main.c src/emu.c src/wm.c src/memory_mapper.c

make: main.c
	$(CC) $(OBJS) -Wall -I/usr/local/include -L/usr/local/lib -lSDL2 -o chip-8
