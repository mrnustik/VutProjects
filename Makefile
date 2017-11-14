CC=g++
CLIENT = isamon
C_FLAGS= -std=c++11 -Wextra
LIBRARY=$(wildcard src/*.cpp)
OBJ_DIR=obj
OBJECTS=$(LIBRARY:src/%.cpp=$(OBJ_DIR)/%.o)

all: $(CLIENT)

clean:
	rm -f $(OBJECTS) $(CLIENT)

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(C_FLAGS) -c -o $@ $< 

$(CLIENT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lpcap 
