# C compiler we use
CXX = gcc

CXXFLAGS = -Wall -pthread -Iinc

# Output filename after compilation 
TARGET = server

TARGET1 = client

# Directory for object files
OBJ_DIR = obj

# Source files (without server.c)
SRC = src/list.c\
		src/read_funs.c\
		src/sema.c\
		src/server_fun.c\
		src/shmemmo.c

# Create object file names for each source file
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

# Main target
all: $(TARGET) $(TARGET1)

# Build (server) executable
$(TARGET): server.c $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) server.c $(OBJ)

$(TARGET1): client.c $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) client.c $(OBJ)

# Build object files (without server.c)
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run (server) executable
run: $(TARGET)
	./$(TARGET)

# Run valgrind for (server) executable
valgrind_server: $(TARGET)
	@echo "Running the build with Valgrind..."
	valgrind --leak-check=full --child-silent-after-fork=yes --show-reachable=yes --show-leak-kinds=all --track-origins=yes --trace-children=yes --verbose ./$(TARGET) $(ARGS)

# Clean up object files and executables
clean:
	rm -rf $(TARGET) $(TARGET1) $(OBJ_DIR)