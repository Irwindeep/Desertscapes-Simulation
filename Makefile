# Compiler
CXX = c++
CXXFLAGS = -O3 -std=c++17 -fPIC -fopenmp

# Python + pybind11 includes
PYTHON_INCLUDES = $(shell python3 -m pybind11 --includes)
PYTHON_SUFFIX = $(shell python3-config --extension-suffix)

# Source files
SRC = bind.cpp $(wildcard Code/Source/*.cpp)
INC = -ICode/Include

# Output
TARGET = dune$(PYTHON_SUFFIX)

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	@$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) $(INC) -shared $(SRC) -o $(TARGET)

# Clean build
clean:
	@rm -f $(TARGET)
