CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -lpthread -g -Iheader
OBJ_DIR=obj
HEADER_DIR=header

all: main

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/producer.o $(OBJ_DIR)/consumer.o $(OBJ_DIR)/shared_resources.o
	$(CC) $(CFLAGS) $^ -o main

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/producer.o: producer.c $(HEADER_DIR)/producer.h $(HEADER_DIR)/shared_resources.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c producer.c -o $(OBJ_DIR)/producer.o

$(OBJ_DIR)/consumer.o: consumer.c $(HEADER_DIR)/consumer.h $(HEADER_DIR)/shared_resources.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c consumer.c -o $(OBJ_DIR)/consumer.o

$(OBJ_DIR)/shared_resources.o: shared_resources.c $(HEADER_DIR)/shared_resources.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c shared_resources.c -o $(OBJ_DIR)/shared_resources.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o main

run:
	./main
