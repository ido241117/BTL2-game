#include <SDL3/SDL.h>
// #include <SDL3/SDL_image.h> // Cần thiết để tải PNG, JPG, v.v. (không có trong SDL3 cơ bản)
#include <iostream>
#include <string>
#include "image_loader.h"

// Kích thước màn hình
const int SCREEN_WIDTH = 1920/2;
const int SCREEN_HEIGHT = 1080/2;

// Cửa sổ chúng ta sẽ render
SDL_Window* gWindow = NULL;

// Renderer để vẽ
SDL_Renderer* gRenderer = NULL;

// Texture sẽ chứa hình ảnh
SDL_Texture* gTexture = NULL;

// Hàm tải hình ảnh từ đường dẫn tệp
SDL_Texture* loadTexture( const std::string& path ) {
    // Texture cuối cùng
    SDL_Texture* newTexture = NULL;

    // Tải ảnh dưới dạng surface (hỗ trợ JPG/PNG/BMP)
    SDL_Surface* loadedSurface = loadImageWithSTB( path );
    
    if( loadedSurface == NULL ) {
        std::cerr << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    } else {
        // Tạo texture từ surface
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL ) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }

        // Giải phóng surface không cần thiết nữa
        SDL_DestroySurface( loadedSurface );
    }

    return newTexture;
}

// Hàm khởi tạo SDL và SDL_Image (nếu dùng)
bool init() {
    bool success = true;

    // Khởi tạo SDL Video
    if( !SDL_Init( SDL_INIT_VIDEO ) ) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        success = false;
    } else {
        // Tạo cửa sổ
        gWindow = SDL_CreateWindow( "SDL3 Show Image", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE );
        if( gWindow == NULL ) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            success = false;
        } else {
            // Tạo renderer
            gRenderer = SDL_CreateRenderer(gWindow, NULL); // Sử dụng cờ mặc định
            if( gRenderer == NULL ) {
                std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
                success = false;
            } else {
                // Khởi tạo màu vẽ renderer
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                /* // Khởi tạo SDL_Image cho PNG/JPG (Bỏ comment nếu bạn dùng IMG_Load)
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                    std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
                    // Không coi là lỗi fatal nếu chỉ muốn tải BMP
                }
                */
            }
        }
    }

    return success;
}

// Hàm tải media (hình ảnh)
bool loadMedia() {
    // Tải texture từ file BMP
    gTexture = loadTexture( "D:\\GG\\game\\BTL 2\\elaina.jpg" ); 
    // **Thay đổi:** nếu dùng PNG/JPG, đổi tên file và dùng IMG_Load trong loadTexture
    
    if( gTexture == NULL ) {
        std::cerr << "Failed to load texture image!" << std::endl;
        return false;
    }

    return true;
}

// Hàm đóng SDL
void close() {
    // Hủy texture
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;
    
    // Hủy renderer
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    
    // Hủy cửa sổ
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    // Thoát SDL_Image (Bỏ comment nếu bạn dùng)
    // IMG_Quit(); 
    
    // Thoát tất cả các hệ thống con của SDL
    SDL_Quit();
}

int main( int argc, char* args[] ) {
    // Khởi tạo
    if( !init() ) {
        std::cerr << "Failed to initialize!" << std::endl;
    } else {
        // Tải media
        if( !loadMedia() ) {
            std::cerr << "Failed to load media!" << std::endl;
        } else {
            // Vòng lặp chính
            bool quit = false;
            SDL_Event e;

            while( !quit ) {
                // Xử lý sự kiện trong hàng đợi
                while( SDL_PollEvent( &e ) != 0 ) {
                    // Người dùng yêu cầu thoát
                    if( e.type == SDL_EVENT_QUIT ) {
                        quit = true;
                    }
                    // Nhấn phím ESC để thoát
                    if ( e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
                        quit = true;
                    }
                }

                // --- Render ---
                
                // Tô màu trắng cho màn hình
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                // Render texture ra màn hình
                // Tham số: renderer, texture, srcRect (NULL = toàn bộ texture), dstRect (NULL = toàn bộ màn hình)
                SDL_RenderTexture( gRenderer, gTexture, NULL, NULL );

                // Cập nhật màn hình
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    // Đóng SDL và giải phóng tài nguyên
    close();

    return 0;
}