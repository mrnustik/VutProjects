CC=g++
CLIENT = isamon
C_FLAGS= -std=c++11
LIBRARY=$(wildcard *.cpp)
OBJ_DIR=obj
OBJECTS=$(LIBRARY:%.cpp=$(OBJ_DIR)/%.o)

all: $(CLIENT)

clean:
	rm -f $(OBJECTS) $(CLIENT)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(C_FLAGS) -c -o $@ $< -lpcap -lpthread

$(CLIENT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lpcap -lpthread

