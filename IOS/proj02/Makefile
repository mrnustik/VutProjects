
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS=-lrt -pthread
PROGRAM=proj2

SRC=helpers.c errors.c proj2.c 
OBJ=helpers.o errors.o proj2.o 
HEAD=helpers.h


$(PROGRAM): $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ) $(LDFLAGS)

all: $(PROGRAM)


clean: 
	rm -rf $(OBJ) $(PROGRAM)
