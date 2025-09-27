# Makefile cho dự án SDL3
CXX = g++
TARGET = hellosdl
SOURCE = hellosdl.cpp
SHOWIMAGE_TARGET = showimage
SHOWIMAGE_SOURCE = showimage.cpp image_loader.cpp

# Đường dẫn SDL3 (64-bit)
SDL_INCLUDE = -I"SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/include"
SDL_LIB = -L"SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/lib"
SDL_LIBS = -lSDL3

# Flags
CXXFLAGS = -std=c++17 -Wall -Wextra

# Biên dịch
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) $(SDL_LIB) $(SOURCE) $(SDL_LIBS) -o $(TARGET).exe

# Biên dịch showimage
$(SHOWIMAGE_TARGET): $(SHOWIMAGE_SOURCE)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) $(SDL_LIB) $(SHOWIMAGE_SOURCE) $(SDL_LIBS) -o $(SHOWIMAGE_TARGET).exe

# Chạy chương trình
run: $(TARGET)
	./$(TARGET).exe

# Dọn dẹp
clean:
	del $(TARGET).exe

# Copy DLL
dll:
	copy "SDL3-devel-3.2.22-mingw\SDL3-3.2.22\x86_64-w64-mingw32\bin\SDL3.dll" .

.PHONY: run clean dll
