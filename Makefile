CC=gcc

make: main.c
	$(CC) main.c -Wall -I/usr/local/include -L/usr/local/lib -lSDL2 -o chip-8
