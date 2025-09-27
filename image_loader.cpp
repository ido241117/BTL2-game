#include "image_loader.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Surface* loadImageWithSTB(const std::string& path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return nullptr;
    }
    
    // Tạo SDL_Surface từ dữ liệu ảnh
    SDL_Surface* surface = nullptr;
    
    if (channels == 3) {
        // RGB
        surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGB24);
    } else if (channels == 4) {
        // RGBA
        surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    } else {
        std::cerr << "Unsupported number of channels: " << channels << std::endl;
        stbi_image_free(data);
        return nullptr;
    }
    
    if (!surface) {
        std::cerr << "Failed to create SDL surface: " << SDL_GetError() << std::endl;
        stbi_image_free(data);
        return nullptr;
    }
    
    // Copy dữ liệu ảnh vào surface
    unsigned char* pixels = (unsigned char*)surface->pixels;
    int pitch = surface->pitch;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int src_index = (y * width + x) * channels;
            int dst_index = y * pitch + x * (channels == 3 ? 3 : 4);
            
            pixels[dst_index + 0] = data[src_index + 0]; // R
            pixels[dst_index + 1] = data[src_index + 1]; // G
            pixels[dst_index + 2] = data[src_index + 2]; // B
            
            if (channels == 4) {
                pixels[dst_index + 3] = data[src_index + 3]; // A
            }
        }
    }
    
    stbi_image_free(data);
    return surface;
}
