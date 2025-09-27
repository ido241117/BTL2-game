#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <SDL3/SDL.h>
#include <string>

// Hàm load ảnh JPG/PNG/BMP sử dụng stb_image
SDL_Surface* loadImageWithSTB(const std::string& path);

#endif
