# Makefile to compile the main.c file

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f main.o main

run:
	./main
