#include <SDL3/SDL.h>
#include <iostream>

// Kích thước màn hình
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Paddle
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 80;
const int PADDLE_SPEED = 6;

// Ball
const int BALL_SIZE = 12;
const int BALL_SPEED = 5;

// SDL
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// Paddle struct
struct Paddle {
    int x, y;
    int vy;
};

// Ball struct
struct Ball {
    int x, y;
    int vx, vy;
};

// Khởi tạo SDL
bool init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("Pong Game", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!gWindow) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, NULL);
    if (!gRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// Đóng SDL
void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

// Vẽ paddle
void renderPaddle(const Paddle& p) {
    SDL_Rect rect = { p.x, p.y, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(gRenderer, &rect);
}

// Vẽ bóng
void renderBall(const Ball& b) {
    SDL_Rect rect = { b.x, b.y, BALL_SIZE, BALL_SIZE };
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0xFF);
    SDL_RenderFillRect(gRenderer, &rect);
}

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize!" << std::endl;
        return 1;
    }

    // Paddle trái (W/S)
    Paddle left = { 30, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, 0 };
    // Paddle phải (Up/Down)
    Paddle right = { SCREEN_WIDTH - 40, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, 0 };
    // Bóng
    Ball ball = { SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, BALL_SPEED, BALL_SPEED };

    int scoreLeft = 0, scoreRight = 0;
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
            if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: left.vy = -PADDLE_SPEED; break;
                    case SDLK_s: left.vy = PADDLE_SPEED; break;
                    case SDLK_UP: right.vy = -PADDLE_SPEED; break;
                    case SDLK_DOWN: right.vy = PADDLE_SPEED; break;
                }
            }
            if (e.type == SDL_EVENT_KEY_UP) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: case SDLK_s: left.vy = 0; break;
                    case SDLK_UP: case SDLK_DOWN: right.vy = 0; break;
                }
            }
        }

        // Di chuyển paddle
        left.y += left.vy;
        right.y += right.vy;
        if (left.y < 0) left.y = 0;
        if (left.y > SCREEN_HEIGHT - PADDLE_HEIGHT) left.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
        if (right.y < 0) right.y = 0;
        if (right.y > SCREEN_HEIGHT - PADDLE_HEIGHT) right.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

        // Di chuyển bóng
        ball.x += ball.vx;
        ball.y += ball.vy;

        // Va chạm biên trên/dưới
        if (ball.y < 0 || ball.y > SCREEN_HEIGHT - BALL_SIZE) ball.vy = -ball.vy;

        // Va chạm paddle trái
        if (ball.x <= left.x + PADDLE_WIDTH &&
            ball.y + BALL_SIZE > left.y &&
            ball.y < left.y + PADDLE_HEIGHT) {
            ball.vx = -ball.vx;
            ball.x = left.x + PADDLE_WIDTH; // tránh kẹt
        }
        // Va chạm paddle phải
        if (ball.x + BALL_SIZE >= right.x &&
            ball.y + BALL_SIZE > right.y &&
            ball.y < right.y + PADDLE_HEIGHT) {
            ball.vx = -ball.vx;
            ball.x = right.x - BALL_SIZE; // tránh kẹt
        }

        // Ghi bàn
        if (ball.x < 0) {
            scoreRight++;
            std::cout << "Score: Left " << scoreLeft << " - Right " << scoreRight << std::endl;
            ball = { SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, BALL_SPEED, BALL_SPEED };
        }
        if (ball.x > SCREEN_WIDTH - BALL_SIZE) {
            scoreLeft++;
            std::cout << "Score: Left " << scoreLeft << " - Right " << scoreRight << std::endl;
            ball = { SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, -BALL_SPEED, BALL_SPEED };
        }

        // Vẽ màn hình
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        renderPaddle(left);
        renderPaddle(right);
        renderBall(ball);
        SDL_RenderPresent(gRenderer);

        SDL_Delay(16); // ~60 FPS
    }

    close();
    return 0;
}