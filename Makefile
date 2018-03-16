CC=gcc
OBJS=main.c emu.c

make: main.c
	$(CC) $(OBJS) -Wall -I/usr/local/include -L/usr/local/lib -lSDL2 -o chip-8
