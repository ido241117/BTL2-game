# Space Ping Pong SDL3 - Makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -I./SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/include
LIBS = -L./SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/lib -lSDL3 -lmingw32 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

# Target executable
TARGET = space_pingpong_sdl3.exe
SOURCE = space_pingpong_sdl3.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(INCLUDES) $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Run the game
run: $(TARGET)
	./$(TARGET)

# Install dependencies (for Windows with MinGW)
install-deps:
	@echo "Please download and extract SDL3 development libraries to ./SDL3-devel-3.2.22-mingw/"
	@echo "Download from: https://github.com/libsdl-org/SDL/releases"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the game (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  run          - Build and run the game"
	@echo "  install-deps - Show dependency installation instructions"
	@echo "  help         - Show this help message"

.PHONY: all clean run install-deps help