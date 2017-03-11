CC=g++
SERVER = ftrestd
CLIENT = ftrest
LIBRARY=$(wildcard Library/*.cpp)
OBJECTS=$(LIBRARY:.cpp=.o)

all: $(SERVER) $(CLIENT)

clean: 
	rm -f client.o server.o $(OBJECTS) $(CLIENT) $(SERVER)

.cpp.o:
	$(CC) $(C_FLAGS) -c -o $@ $<

$(CLIENT): client.o $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER): server.o $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^