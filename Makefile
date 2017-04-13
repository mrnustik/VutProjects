CC=g++
CLIENT = ipk-client
C_FLAGS= -Wall -Wextra -Werror -std=c++11
LIBRARY=$(wildcard Library/*.cpp)
OBJECTS=$(LIBRARY:.cpp=.o)

all: $(CLIENT)

clean:
	rm -f main.o $(OBJECTS) $(CLIENT)

.cpp.o:
	$(CC) $(C_FLAGS) -c -o $@ $<

$(CLIENT): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lcrypto

