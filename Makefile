CC = g++
CFLAGS = -std=c++17 -Wall -g -Iincludes/utils -Iincludes/graphics
LDFLAGS = -lCGAL -lboost_system

# Source directories
UTILS_DIR = includes/utils
GRAPHICS_DIR = includes/graphics

# Source and header files
SOURCES = $(UTILS_DIR)/utils.cpp $(GRAPHICS_DIR)/graphics.cpp ex.cpp
HEADERS = $(UTILS_DIR)/utils.hpp $(GRAPHICS_DIR)/graphics.hpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = Project1

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@echo "Linking executable $(EXEC)..."
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning project..."
	rm -f $(OBJECTS) $(EXEC)
