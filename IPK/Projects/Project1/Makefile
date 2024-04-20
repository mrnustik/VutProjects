CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
SERVER = ftrestd
CLIENT = ftrest
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)


all: $(CLIENT) $(SERVER)

clean: 
	rm -f client.o server.o $(OBJECTS) $(CLIENT) $(SERVER)


.c.o:
	$(CC) $(C_FLAGS) -c -o $@ $<

$(CLIENT): client.o $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER): server.o $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^

