# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I ./Simplex -std=c++11 \
           -I ./Simplex/SuiteSparse/SuiteSparse-dev/UMFPACK/Include \
           -I ./Simplex/SuiteSparse/SuiteSparse-dev/AMD/Include \
           -I ./Simplex/SuiteSparse/SuiteSparse-dev/CHOLMOD/Include \
           -I ./Simplex/SuiteSparse/SuiteSparse-dev/SuiteSparse_config \
           -I ./Simplex/eigen \
           -O3

# Linker flags (path to the local SuiteSparse libraries)
LDFLAGS = -L./Simplex/SuiteSparse/SuiteSparse-dev/lib \
          -lumfpack -lcholmod -lamd -lsuitesparseconfig

# Source and object directories
SRC_DIR = Simplex/src
OBJ_DIR = obj

# Paths for main and extra files
MAIN_FILE = src/main.cpp
EXTRA_FILES = src/branchAndBound.cpp src/branchAndBound.h  

# Source files inside Simplex/src folder
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Object files for main.cpp and additional files in src/
MAIN_OBJ = $(OBJ_DIR)/main.o
EXTRA_OBJS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(EXTRA_FILES))

# Output executable
TARGET = solve
DEBUG_TARGET = solve_debug

# Default target (optimized build)
all: $(TARGET)

# Debug target (with -g for debugging)
debug: CXXFLAGS := $(filter-out -O3, $(CXXFLAGS)) -g
debug: $(DEBUG_TARGET)

# Link object files (optimized build)
$(TARGET): $(OBJS) $(MAIN_OBJ) $(EXTRA_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(MAIN_OBJ) $(EXTRA_OBJS) $(LDFLAGS)

# Link object files (debug build)
$(DEBUG_TARGET): $(OBJS) $(MAIN_OBJ) $(EXTRA_OBJS)
	$(CXX) $(CXXFLAGS) -o $(DEBUG_TARGET) $(OBJS) $(MAIN_OBJ) $(EXTRA_OBJS) $(LDFLAGS)

# Compile source files inside Simplex/src into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile the main.cpp file into an object file
$(MAIN_OBJ): $(MAIN_FILE) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_FILE) -o $(MAIN_OBJ)

# Compile additional files in src/ into object files
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean target to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(DEBUG_TARGET)

# Phony targets to avoid conflicts with files named 'clean', 'all', etc.
.PHONY: all clean debug
