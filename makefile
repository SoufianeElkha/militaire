# Makefile to compile the main.c file

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -lpthread -g

all: main

main: main.o producer.o consumer.o shared_resources.o
	$(CC) $(CFLAGS) main.o producer.o consumer.o shared_resources.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

producer.o: producer.c producer.h shared_resources.h
	$(CC) $(CFLAGS) -c producer.c

consumer.o: consumer.c consumer.h shared_resources.h
	$(CC) $(CFLAGS) -c consumer.c

shared_resources.o: shared_resources.c shared_resources.h
	$(CC) $(CFLAGS) -c shared_resources.c

clean:
	rm -f main.o producer.o consumer.o shared_resources.o main

run:
	./main
