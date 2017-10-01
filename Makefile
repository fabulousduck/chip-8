CC=gcc

make: main.c
	$(CC) main.c -Wall -lSDL2 -o chip-8
