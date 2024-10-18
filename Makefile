# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I ./ -std=c++11 \
           -I ./SuiteSparse/SuiteSparse-dev/UMFPACK/Include \
           -I ./SuiteSparse/SuiteSparse-dev/AMD/Include \
           -I ./SuiteSparse/SuiteSparse-dev/CHOLMOD/Include \
           -I ./SuiteSparse/SuiteSparse-dev/SuiteSparse_config \
           -O3

# Linker flags (path to the local SuiteSparse libraries)
LDFLAGS = -L./SuiteSparse/SuiteSparse-dev/lib \
          -lumfpack -lcholmod -lamd -lsuitesparseconfig

# Source and object files
SRC_DIR = src
OBJ_DIR = obj

# Add the main.cpp separately outside the src folder
MAIN_FILE = main.cpp

# Source files inside the src folder
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Object file for main.cpp
MAIN_OBJ = $(OBJ_DIR)/main.o

# Output executable
TARGET = solve
DEBUG_TARGET = solve_debug

# Default target (optimized build)
all: $(TARGET)

# Debug target (with -g for debugging)
debug: CXXFLAGS := $(filter-out -O3, $(CXXFLAGS)) -g
debug: $(DEBUG_TARGET)

# Link object files (optimized build)
$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(MAIN_OBJ) $(LDFLAGS)

# Link object files (debug build)
$(DEBUG_TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(DEBUG_TARGET) $(OBJS) $(MAIN_OBJ) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile the main.cpp file into an object file
$(MAIN_OBJ): $(MAIN_FILE) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_FILE) -o $(MAIN_OBJ)

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean target to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(DEBUG_TARGET)

# Phony targets to avoid conflicts with files named 'clean', 'all', etc.
.PHONY: all clean debug
