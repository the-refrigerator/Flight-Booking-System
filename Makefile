# Makefile

# Compiler
CXX := g++ # Change this to your compiler (e.g., g++ for GCC on Linux, or cl for Visual Studio on Windows)

# Flags
CXXFLAGS := -Wall

# Directories
SRC_DIR := src
BIN_DIR := bin

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRC_FILES))

# Executable
EXECUTABLE := $(BIN_DIR)/main

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -rf $(BIN_DIR)

# Ensure the bin directory exists
$(shell mkdir -p $(BIN_DIR))

