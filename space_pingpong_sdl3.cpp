#include <SDL3/SDL.h>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Game constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int FPS = 60;

// Colors
struct Color {
    Uint8 r, g, b, a;
    Color(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255) : r(r), g(g), b(b), a(a) {}
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);
const Color CYAN(0, 255, 255);
const Color GREEN(50, 205, 50);
const Color RED(255, 69, 0);
const Color PURPLE(138, 43, 226);
const Color GOLD(255, 215, 0);
const Color BLUE(100, 149, 237);
const Color PINK(255, 20, 147);
const Color ORANGE(255, 165, 0);

// Game states
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    HIGH_SCORES
};

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

enum class PowerUpType {
    SPEED_BOOST,
    PADDLE_GROW,
    PADDLE_SHRINK,
    MULTI_BALL,
    SHIELD,
    FREEZE,
    LASER,
    MAGNET
};

// Vector2D class
class Vector2D {
public:
    float x, y;
    
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    
    Vector2D normalize() const {
        float mag = magnitude();
        if (mag == 0) return Vector2D(0, 0);
        return Vector2D(x / mag, y / mag);
    }
    
    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
};

// Helper functions
void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        int px = x + radius * cos(angle);
        int py = y + radius * sin(angle);
        SDL_RenderPoint(renderer, px, py);
    }
}

void drawFilledCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        SDL_RenderPoint(renderer, x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Simple text rendering functions
void drawChar(SDL_Renderer* renderer, char c, int x, int y, int size, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Simple 5x7 pixel font patterns
    switch (c) {
        case 'A':
            drawLine(renderer, x, y+size*6, x+size*2, y);
            drawLine(renderer, x+size*2, y, x+size*4, y+size*6);
            drawLine(renderer, x+size, y+size*3, x+size*3, y+size*3);
            break;
        case 'B':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x+size*3, y+size*6);
            break;
        case 'C':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case 'D':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y, x+size*2, y);
            drawLine(renderer, x, y+size*6, x+size*2, y+size*6);
            drawLine(renderer, x+size*3, y+size, x+size*3, y+size*5);
            drawLine(renderer, x+size*2, y, x+size*3, y+size);
            drawLine(renderer, x+size*2, y+size*6, x+size*3, y+size*5);
            break;
        case 'E':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x, y+size*3, x+size*2, y+size*3);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case 'F':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x, y+size*3, x+size*2, y+size*3);
            break;
        case 'G':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*3, x+size*3, y+size*6);
            drawLine(renderer, x+size*2, y+size*3, x+size*3, y+size*3);
            break;
        case 'H':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*6);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            break;
        case 'I':
            drawLine(renderer, x+size*1, y, x+size*2, y);
            drawLine(renderer, x+size*1, y+size*6, x+size*2, y+size*6);
            drawLine(renderer, x+size*1, y, x+size*1, y+size*6);
            break;
        case 'J':
            drawLine(renderer, x+size*2, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*5);
            drawLine(renderer, x+size*3, y+size*5, x, y+size*6);
            drawLine(renderer, x, y+size*6, x, y+size*5);
            break;
        case 'K':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*3, x+size*3, y);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*6);
            break;
        case 'L':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case 'M':
            drawLine(renderer, x, y+size*6, x, y);
            drawLine(renderer, x, y, x+size*2, y+size*3);
            drawLine(renderer, x+size*2, y+size*3, x+size*4, y);
            drawLine(renderer, x+size*4, y, x+size*4, y+size*6);
            break;
        case 'N':
            drawLine(renderer, x, y+size*6, x, y);
            drawLine(renderer, x, y, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x+size*3, y);
            break;
        case 'O':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x+size*3, y);
            break;
        case 'P':
            drawLine(renderer, x, y+size*6, x, y);
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x, y+size*3);
            break;
        case 'Q':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x+size*3, y);
            drawLine(renderer, x+size*2, y+size*4, x+size*4, y+size*6);
            break;
        case 'R':
            drawLine(renderer, x, y+size*6, x, y);
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x, y+size*3);
            drawLine(renderer, x+size*2, y+size*3, x+size*3, y+size*6);
            break;
        case 'S':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*3);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x, y+size*6);
            break;
        case 'T':
            drawLine(renderer, x+size*1, y, x+size*2, y);
            drawLine(renderer, x+size*1, y, x+size*1, y+size*6);
            break;
        case 'U':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case 'V':
            drawLine(renderer, x, y, x+size*1, y+size*6);
            drawLine(renderer, x+size*1, y+size*6, x+size*2, y);
            drawLine(renderer, x+size*2, y, x+size*3, y+size*6);
            break;
        case 'W':
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x+size*1, y+size*6, x+size*2, y+size*3);
            drawLine(renderer, x+size*2, y+size*3, x+size*3, y+size*6);
            drawLine(renderer, x+size*4, y, x+size*4, y+size*6);
            break;
        case 'X':
            drawLine(renderer, x, y, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y, x, y+size*6);
            break;
        case 'Y':
            drawLine(renderer, x, y, x+size*1, y+size*3);
            drawLine(renderer, x+size*1, y+size*3, x+size*2, y+size*3);
            drawLine(renderer, x+size*2, y+size*3, x+size*3, y);
            drawLine(renderer, x+size*1, y+size*3, x+size*1, y+size*6);
            break;
        case 'Z':
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case '0':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x+size*3, y);
            drawLine(renderer, x+size*3, y, x, y+size*6);
            break;
        case '1':
            drawLine(renderer, x+size*1, y, x+size*2, y);
            drawLine(renderer, x+size*1, y, x+size*1, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case '2':
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x, y+size*3);
            drawLine(renderer, x, y+size*3, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            break;
        case '3':
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x, y+size*6);
            drawLine(renderer, x, y+size*3, x+size*2, y+size*3);
            break;
        case '4':
            drawLine(renderer, x, y, x, y+size*3);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*6);
            break;
        case '5':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*3);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x, y+size*6);
            break;
        case '6':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x+size*3, y+size*3);
            drawLine(renderer, x+size*3, y+size*3, x, y+size*3);
            break;
        case '7':
            drawLine(renderer, x, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*6);
            break;
        case '8':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x, y, x, y+size*6);
            drawLine(renderer, x, y+size*6, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x+size*3, y);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            break;
        case '9':
            drawLine(renderer, x+size*3, y, x, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*6);
            drawLine(renderer, x+size*3, y+size*6, x, y+size*6);
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            drawLine(renderer, x, y, x, y+size*3);
            break;
        case ' ':
            // Space - do nothing
            break;
        case ':':
            drawLine(renderer, x+size*1, y+size*2, x+size*1, y+size*2);
            drawLine(renderer, x+size*1, y+size*4, x+size*1, y+size*4);
            break;
        case '-':
            drawLine(renderer, x, y+size*3, x+size*3, y+size*3);
            break;
        case '.':
            drawLine(renderer, x+size*1, y+size*5, x+size*1, y+size*5);
            break;
        case '!':
            drawLine(renderer, x+size*1, y, x+size*1, y+size*4);
            drawLine(renderer, x+size*1, y+size*6, x+size*1, y+size*6);
            break;
        case '?':
            drawLine(renderer, x, y+size*2, x+size*2, y);
            drawLine(renderer, x+size*2, y, x+size*3, y);
            drawLine(renderer, x+size*3, y, x+size*3, y+size*2);
            drawLine(renderer, x+size*3, y+size*2, x+size*2, y+size*3);
            drawLine(renderer, x+size*1, y+size*5, x+size*1, y+size*5);
            break;
    }
}

void drawText(SDL_Renderer* renderer, const std::string& text, int x, int y, int size, const Color& color) {
    int currentX = x;
    for (char c : text) {
        if (c != ' ') {
            drawChar(renderer, c, currentX, y, size, color);
        }
        currentX += size * 5; // Space between characters
    }
}

// Particle class
class Particle {
public:
    float x, y;
    Vector2D velocity;
    Color color;
    int lifetime;
    int maxLifetime;
    int size;
    
    Particle(float x, float y, const Color& color, const Vector2D& velocity, int lifetime = 60) 
        : x(x), y(y), color(color), velocity(velocity), lifetime(lifetime), maxLifetime(lifetime) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> sizeDist(2, 5);
        size = sizeDist(gen);
    }
    
    void update() {
        x += velocity.x;
        y += velocity.y;
        lifetime--;
        
        // Fade out effect
        float alpha = (float)lifetime / maxLifetime;
        color.a = (Uint8)(255 * alpha);
    }
    
    void draw(SDL_Renderer* renderer) const {
        if (lifetime > 0) {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            drawFilledCircle(renderer, (int)x, (int)y, size);
        }
    }
    
    bool isAlive() const {
        return lifetime > 0;
    }
};

// Star class for background
class Star {
public:
    float x, y;
    float speed;
    int size;
    int brightness;
    
    Star() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> xDist(0, SCREEN_WIDTH);
        static std::uniform_int_distribution<> yDist(0, SCREEN_HEIGHT);
        static std::uniform_real_distribution<> speedDist(0.1f, 1.0f);
        static std::uniform_int_distribution<> sizeDist(1, 3);
        static std::uniform_int_distribution<> brightnessDist(100, 255);
        
        x = xDist(gen);
        y = yDist(gen);
        speed = speedDist(gen);
        size = sizeDist(gen);
        brightness = brightnessDist(gen);
    }
    
    void update() {
        y += speed;
        if (y > SCREEN_HEIGHT) {
            y = 0;
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> xDist(0, SCREEN_WIDTH);
            x = xDist(gen);
        }
    }
    
    void draw(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);
        drawFilledCircle(renderer, (int)x, (int)y, size);
    }
};

// PowerUp class
class PowerUp {
public:
    float x, y;
    PowerUpType powerType;
    int size;
    int lifetime;
    float floatOffset;
    
    PowerUp(float x, float y, PowerUpType type) 
        : x(x), y(y), powerType(type), size(30), lifetime(300), floatOffset(0) {}
    
    void update() {
        lifetime--;
        floatOffset += 0.1f;
        y += std::sin(floatOffset) * 0.5f;
    }
    
    void draw(SDL_Renderer* renderer) const {
        if (lifetime > 0) {
            Color color;
            switch (powerType) {
                case PowerUpType::SPEED_BOOST: color = CYAN; break;
                case PowerUpType::PADDLE_GROW: color = GREEN; break;
                case PowerUpType::PADDLE_SHRINK: color = RED; break;
                case PowerUpType::MULTI_BALL: color = PURPLE; break;
                case PowerUpType::SHIELD: color = GOLD; break;
                case PowerUpType::FREEZE: color = BLUE; break;
                case PowerUpType::LASER: color = ORANGE; break;
                case PowerUpType::MAGNET: color = PINK; break;
            }
            
            // Draw power-up with pulsing effect
            float pulse = std::abs(std::sin(floatOffset * 2)) * 5 + size;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            drawCircle(renderer, (int)x, (int)y, (int)pulse);
            drawFilledCircle(renderer, (int)x, (int)y, size / 2);
        }
    }
    
    bool isAlive() const {
        return lifetime > 0;
    }
    
    SDL_FRect getRect() const {
        return {x - size, y - size, size * 2, size * 2};
    }
    
    bool operator==(const PowerUp& other) const {
        return x == other.x && y == other.y && powerType == other.powerType;
    }
};

// Ball class
class Ball {
public:
    float x, y;
    Vector2D velocity;
    int size;
    float baseSpeed;
    float speedMultiplier;
    std::vector<Vector2D> trail;
    bool isMagnetic;
    float magneticForce;
    
    Ball(float x, float y, float speed = 8.0f) 
        : x(x), y(y), baseSpeed(speed), speedMultiplier(1.0f), size(8), 
          isMagnetic(false), magneticForce(0.0f) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> angleDist(-0.5, 0.5);
        static std::uniform_int_distribution<> directionDist(0, 1);
        
        float direction = directionDist(gen) == 0 ? -1 : 1;
        velocity = Vector2D(speed * direction, speed * angleDist(gen));
    }
    
    void update() {
        x += velocity.x * speedMultiplier;
        y += velocity.y * speedMultiplier;
        
        // Add to trail
        trail.push_back(Vector2D(x, y));
        if (trail.size() > 10) {
            trail.erase(trail.begin());
        }
        
        // Wall collision
        if (y <= size || y >= SCREEN_HEIGHT - size) {
            velocity.y *= -1;
            y = std::max((float)size, std::min((float)(SCREEN_HEIGHT - size), y));
        }
    }
    
    bool paddleCollision(const SDL_FRect& paddleRect, float paddleCenterY) {
        SDL_FRect ballRect = getRect();
        if (SDL_HasRectIntersectionFloat(&ballRect, &paddleRect)) {
            // Calculate hit position relative to paddle center
            float hitPos = (y - paddleCenterY) / (paddleRect.h / 2);
            hitPos = std::max(-1.0f, std::min(1.0f, hitPos));
            
            // Reverse horizontal direction
            velocity.x *= -1;
            
            // Adjust vertical velocity based on hit position
            velocity.y = hitPos * baseSpeed * 0.75f;
            
            // Increase speed slightly
            float currentSpeed = velocity.magnitude();
            if (currentSpeed < baseSpeed * 2) {
                velocity = velocity * 1.05f;
            }
            
            // Move ball away from paddle
            if (paddleRect.x < SCREEN_WIDTH / 2) {
                x = paddleRect.x + paddleRect.w + size;
            } else {
                x = paddleRect.x - size;
            }
            
            return true;
        }
        return false;
    }
    
    void resetPosition(int direction = 0) {
        x = SCREEN_WIDTH / 2;
        y = SCREEN_HEIGHT / 2;
        
        if (direction == 0) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> directionDist(0, 1);
            direction = directionDist(gen) == 0 ? -1 : 1;
        }
        
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> angleDist(-0.5, 0.5);
        
        velocity = Vector2D(baseSpeed * direction, baseSpeed * angleDist(gen));
        speedMultiplier = 1.0f;
        trail.clear();
        isMagnetic = false;
        magneticForce = 0.0f;
    }
    
    SDL_FRect getRect() const {
        return {x - size, y - size, size * 2, size * 2};
    }
    
    void draw(SDL_Renderer* renderer) const {
        // Draw trail
        for (size_t i = 0; i < trail.size(); i++) {
            float alpha = (float)i / trail.size() * 0.3f;
            SDL_SetRenderDrawColor(renderer, CYAN.r, CYAN.g, CYAN.b, (Uint8)(255 * alpha));
            drawFilledCircle(renderer, (int)trail[i].x, (int)trail[i].y, size);
        }
        
        // Draw ball
        Color ballColor = isMagnetic ? PINK : WHITE;
        SDL_SetRenderDrawColor(renderer, ballColor.r, ballColor.g, ballColor.b, ballColor.a);
        drawFilledCircle(renderer, (int)x, (int)y, size);
        SDL_SetRenderDrawColor(renderer, CYAN.r, CYAN.g, CYAN.b, CYAN.a);
        drawCircle(renderer, (int)x, (int)y, size);
    }
};

// Paddle class
class Paddle {
public:
    float x, y;
    int width;
    int baseHeight;
    int height;
    float speed;
    bool isPlayer;
    std::map<PowerUpType, int> effects;
    bool shieldActive;
    int shieldDuration;
    bool laserActive;
    int laserDuration;
    float laserY;
    
    Paddle(float x, float y, bool isPlayer = true) 
        : x(x), y(y), width(15), baseHeight(100), height(baseHeight), speed(8), 
          isPlayer(isPlayer), shieldActive(false), shieldDuration(0),
          laserActive(false), laserDuration(0), laserY(0) {}
    
    void update(const bool* keys, const Ball* ball = nullptr, Difficulty difficulty = Difficulty::MEDIUM) {
        // Update effects
        for (auto it = effects.begin(); it != effects.end();) {
            it->second--;
            if (it->second <= 0) {
                removeEffect(it->first);
                it = effects.erase(it);
            } else {
                ++it;
            }
        }
        
        // Update shield
        if (shieldDuration > 0) {
            shieldDuration--;
            if (shieldDuration <= 0) {
                shieldActive = false;
            }
        }
        
        // Update laser
        if (laserDuration > 0) {
            laserDuration--;
            if (laserDuration <= 0) {
                laserActive = false;
            }
        }
        
        // Player movement
        if (isPlayer && keys) {
            if (keys[SDL_SCANCODE_UP] && y > 0) {
                y -= speed;
            }
            if (keys[SDL_SCANCODE_DOWN] && y < SCREEN_HEIGHT - height) {
                y += speed;
            }
        }
        // AI movement
        else if (!isPlayer && ball) {
            aiMove(ball, difficulty);
        }
        
        // Keep paddle within bounds
        y = std::max(0.0f, std::min((float)(SCREEN_HEIGHT - height), y));
    }
    
    void aiMove(const Ball* ball, Difficulty difficulty) {
        float targetY = ball->y - height / 2;
        
        float speedFactor;
        int predictionError;
        
        switch (difficulty) {
            case Difficulty::EASY:
                speedFactor = 0.2f;
                predictionError = rand() % 61 - 30; // -30 to 30
                break;
            case Difficulty::MEDIUM:
                speedFactor = 0.5f;
                predictionError = rand() % 31 - 15; // -15 to 15
                break;
            case Difficulty::HARD:
                speedFactor = 1.0f;
                predictionError = rand() % 11 - 5; // -5 to 5
                break;
        }
        
        targetY += predictionError;
        
        if (std::abs(targetY - y) > 5) {
            if (targetY > y) {
                y += speed * speedFactor;
            } else {
                y -= speed * speedFactor;
            }
        }
    }
    
    void applyEffect(PowerUpType effectType, int duration = 300) {
        effects[effectType] = duration;
        
        switch (effectType) {
            case PowerUpType::PADDLE_GROW:
                height = std::min((int)(baseHeight * 1.5f), 150);
                break;
            case PowerUpType::PADDLE_SHRINK:
                height = std::max((int)(baseHeight * 0.5f), 50);
                break;
            case PowerUpType::SHIELD:
                shieldActive = true;
                shieldDuration = duration;
                break;
            case PowerUpType::LASER:
                laserActive = true;
                laserDuration = duration;
                laserY = y + height / 2;
                break;
        }
    }
    
    void removeEffect(PowerUpType effectType) {
        if (effectType == PowerUpType::PADDLE_GROW || effectType == PowerUpType::PADDLE_SHRINK) {
            height = baseHeight;
        }
    }
    
    SDL_FRect getRect() const {
        return {x, y, width, height};
    }
    
    float getCenterY() const {
        return y + height / 2;
    }
    
    void draw(SDL_Renderer* renderer) {
        Color color = WHITE;
        if (effects.find(PowerUpType::PADDLE_GROW) != effects.end()) {
            color = GREEN;
        } else if (effects.find(PowerUpType::PADDLE_SHRINK) != effects.end()) {
            color = RED;
        }
        
        SDL_FRect rect = getRect();
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
        
        // Draw shield effect
        if (shieldActive) {
            SDL_FRect shieldRect = {x - 5, y - 5, width + 10, height + 10};
            SDL_SetRenderDrawColor(renderer, GOLD.r, GOLD.g, GOLD.b, GOLD.a);
            SDL_RenderRect(renderer, &shieldRect);
        }
        
        // Draw laser
        if (laserActive) {
            SDL_SetRenderDrawColor(renderer, ORANGE.r, ORANGE.g, ORANGE.b, ORANGE.a);
            drawLine(renderer, (int)x, (int)laserY, (int)(x - 200), (int)laserY);
        }
    }
};

// Game class
class Game {
public:
    Game() : window(nullptr), renderer(nullptr), state(GameState::MENU), 
             running(true), gameMode("vs_computer"), difficulty(Difficulty::MEDIUM),
             paddle1(nullptr), paddle2(nullptr), player1Score(0), player2Score(0),
             powerUpTimer(0), powerUpSpawnInterval(600), screenShake(0), 
             freezeTimer(0), menuTime(0), menuPulse(0.0f) {
        
        // Initialize stars
        stars.resize(100);
        for (auto& star : stars) {
            star = Star();
        }
    }
    
    ~Game() {
        cleanup();
    }
    
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        window = SDL_CreateWindow("Space Ping Pong SDL3", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (!window) {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        resetGame();
        return true;
    }
    
    void run() {
        Uint64 lastTime = SDL_GetTicks();
        const Uint64 targetFrameTime = 1000 / FPS;
        
        while (running) {
            Uint64 currentTime = SDL_GetTicks();
            Uint64 deltaTime = currentTime - lastTime;
            
            if (deltaTime >= targetFrameTime) {
                handleEvents();
                update();
                draw();
                lastTime = currentTime;
            }
        }
    }
    
    void cleanup() {
        if (paddle1) delete paddle1;
        if (paddle2) delete paddle2;
        
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        
        SDL_Quit();
    }
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    GameState state;
    bool running;
    std::string gameMode;
    Difficulty difficulty;
    
    std::vector<Star> stars;
    std::vector<Particle> particles;
    std::vector<PowerUp> powerUps;
    std::vector<Ball> balls;
    
    Paddle* paddle1; // Right paddle (Player 1)
    Paddle* paddle2; // Left paddle (Player 2/Computer)
    
    int player1Score;
    int player2Score;
    int powerUpTimer;
    int powerUpSpawnInterval;
    int screenShake;
    int freezeTimer;
    int menuTime;
    float menuPulse;
    
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (state == GameState::MENU) {
                    handleMenuInput(event.key.key);
                } else if (state == GameState::PLAYING) {
                    if (event.key.key == SDLK_SPACE) {
                        state = GameState::PAUSED;
                    }
                } else if (state == GameState::PAUSED) {
                    if (event.key.key == SDLK_SPACE) {
                        state = GameState::PLAYING;
                    } else if (event.key.key == SDLK_ESCAPE) {
                        state = GameState::MENU;
                    }
                } else if (state == GameState::GAME_OVER) {
                    if (event.key.key == SDLK_SPACE) {
                        resetGame();
                        state = GameState::PLAYING;
                    } else if (event.key.key == SDLK_ESCAPE) {
                        state = GameState::MENU;
                    }
                } else if (state == GameState::HIGH_SCORES) {
                    if (event.key.key == SDLK_ESCAPE) {
                        state = GameState::MENU;
                    }
                }
            }
        }
    }
    
    void handleMenuInput(SDL_Keycode key) {
        switch (key) {
            case SDLK_1:
                gameMode = "vs_computer";
                resetGame();
                state = GameState::PLAYING;
                break;
            case SDLK_2:
                gameMode = "vs_human";
                resetGame();
                state = GameState::PLAYING;
                break;
            case SDLK_3:
                state = GameState::HIGH_SCORES;
                break;
            case SDLK_E:
                difficulty = Difficulty::EASY;
                break;
            case SDLK_M:
                difficulty = Difficulty::MEDIUM;
                break;
            case SDLK_H:
                difficulty = Difficulty::HARD;
                break;
            case SDLK_ESCAPE:
                running = false;
                break;
        }
    }
    
    void update() {
        // Update background stars
        for (auto& star : stars) {
            star.update();
        }
        
        // Update particles
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return !p.isAlive(); }), particles.end());
        
        for (auto& particle : particles) {
            particle.update();
        }
        
        if (state == GameState::MENU) {
            menuTime++;
            menuPulse = std::abs(std::sin(menuTime * 0.05f)) * 0.3f + 0.7f;
        } else if (state == GameState::PLAYING) {
            updateGameplay();
        }
        
        // Update screen shake
        if (screenShake > 0) {
            screenShake--;
        }
    }
    
    void updateGameplay() {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        
        // Handle freeze effect
        if (freezeTimer > 0) {
            freezeTimer--;
            return;
        }
        
        // Update paddles
        paddle1->update(keys);
        
        if (gameMode == "vs_human") {
            // Player 2 controls (W/S keys)
            bool keysP2[SDL_SCANCODE_COUNT] = {false};
            keysP2[SDL_SCANCODE_W] = keys[SDL_SCANCODE_W];
            keysP2[SDL_SCANCODE_S] = keys[SDL_SCANCODE_S];
            paddle2->update(keysP2);
        } else {
            paddle2->update(nullptr, &balls[0], difficulty);
        }
        
        // Update balls
        for (auto it = balls.begin(); it != balls.end();) {
            it->update();
            
            // Paddle collisions
            if (it->paddleCollision(paddle1->getRect(), paddle1->getCenterY())) {
                addHitEffect(it->x, it->y);
            }
            
            if (it->paddleCollision(paddle2->getRect(), paddle2->getCenterY())) {
                addHitEffect(it->x, it->y);
            }
            
            // Score when ball goes off screen
            if (it->x < 0) {
                player1Score++;
                it = balls.erase(it);
                addScoreEffect();
                if (balls.empty()) {
                    balls.push_back(Ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
                }
            } else if (it->x > SCREEN_WIDTH) {
                player2Score++;
                it = balls.erase(it);
                addScoreEffect();
                if (balls.empty()) {
                    balls.push_back(Ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
                }
            } else {
                ++it;
            }
        }
        
        // Update power-ups
        updatePowerUps();
        
        // Check for game over
        if (player1Score >= 11 || player2Score >= 11) {
            state = GameState::GAME_OVER;
            saveHighScore();
        }
    }
    
    void updatePowerUps() {
        // Spawn new power-ups
        powerUpTimer++;
        if (powerUpTimer >= powerUpSpawnInterval) {
            spawnPowerUp();
            powerUpTimer = 0;
        }
        
        // Update existing power-ups
        powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
            [](const PowerUp& p) { return !p.isAlive(); }), powerUps.end());
        
        for (auto& powerUp : powerUps) {
            powerUp.update();
            
            // Check collisions with balls
            for (auto& ball : balls) {
                SDL_FRect powerUpRect = powerUp.getRect();
                SDL_FRect ballRect = ball.getRect();
                if (SDL_HasRectIntersectionFloat(&powerUpRect, &ballRect)) {
                    applyPowerUp(powerUp.powerType, &ball);
                    powerUps.erase(std::find(powerUps.begin(), powerUps.end(), powerUp));
                    addPowerUpEffect(powerUp.x, powerUp.y);
                    break;
                }
            }
        }
    }
    
    void spawnPowerUp() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> xDist(SCREEN_WIDTH / 4, 3 * SCREEN_WIDTH / 4);
        static std::uniform_int_distribution<> yDist(100, SCREEN_HEIGHT - 100);
        static std::uniform_int_distribution<> typeDist(0, 7);
        
        float x = xDist(gen);
        float y = yDist(gen);
        PowerUpType type = (PowerUpType)(typeDist(gen) + 1);
        
        powerUps.push_back(PowerUp(x, y, type));
    }
    
    void applyPowerUp(PowerUpType powerType, Ball* ball) {
        switch (powerType) {
            case PowerUpType::SPEED_BOOST:
                ball->speedMultiplier = 1.5f;
                break;
            case PowerUpType::MULTI_BALL:
                if (balls.size() < 3) {
                    Ball newBall(ball->x, ball->y);
                    newBall.velocity.y *= -1;
                    balls.push_back(newBall);
                }
                break;
            case PowerUpType::FREEZE:
                freezeTimer = 120; // 2 seconds
                break;
            case PowerUpType::MAGNET:
                ball->isMagnetic = true;
                ball->magneticForce = 0.5f;
                break;
            case PowerUpType::PADDLE_GROW:
            case PowerUpType::PADDLE_SHRINK:
            case PowerUpType::SHIELD:
            case PowerUpType::LASER:
                if (ball->x > SCREEN_WIDTH / 2) {
                    paddle1->applyEffect(powerType);
                } else {
                    paddle2->applyEffect(powerType);
                }
                break;
        }
    }
    
    void addHitEffect(float x, float y) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> velDist(-5, 5);
        
        for (int i = 0; i < 10; i++) {
            Vector2D velocity(velDist(gen), velDist(gen));
            particles.push_back(Particle(x, y, CYAN, velocity));
        }
        screenShake = 5;
    }
    
    void addScoreEffect() {
        screenShake = 10;
    }
    
    void addPowerUpEffect(float x, float y) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> velDist(-8, 8);
        
        for (int i = 0; i < 15; i++) {
            Vector2D velocity(velDist(gen), velDist(gen));
            particles.push_back(Particle(x, y, GOLD, velocity));
        }
    }
    
    void resetGame() {
        balls.clear();
        balls.push_back(Ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
        
        if (paddle1) delete paddle1;
        if (paddle2) delete paddle2;
        
        paddle1 = new Paddle(SCREEN_WIDTH - 45, SCREEN_HEIGHT / 2 - 50, true);
        paddle2 = new Paddle(30, SCREEN_HEIGHT / 2 - 50, gameMode == "vs_human");
        
        powerUps.clear();
        particles.clear();
        player1Score = 0;
        player2Score = 0;
        powerUpTimer = 0;
        freezeTimer = 0;
        screenShake = 0;
    }
    
    void draw() {
        // Screen shake effect
        float shakeX = (screenShake > 0) ? (rand() % (screenShake * 2) - screenShake) : 0;
        float shakeY = (screenShake > 0) ? (rand() % (screenShake * 2) - screenShake) : 0;
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_RenderClear(renderer);
        
        // Draw background stars
        for (const auto& star : stars) {
            star.draw(renderer);
        }
        
        switch (state) {
            case GameState::MENU:
                drawMenu();
                break;
            case GameState::PLAYING:
                drawGame(shakeX, shakeY);
                break;
            case GameState::PAUSED:
                drawGame(shakeX, shakeY);
                drawPauseOverlay();
                break;
            case GameState::GAME_OVER:
                drawGame(shakeX, shakeY);
                drawGameOver();
                break;
            case GameState::HIGH_SCORES:
                drawHighScores();
                break;
        }
        
        SDL_RenderPresent(renderer);
    }
    
    void drawMenu() {
        // Animated background gradient
        for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
            float gradientFactor = (float)y / SCREEN_HEIGHT;
            Uint8 colorR = (Uint8)(20 + (30 * gradientFactor * menuPulse));
            Uint8 colorG = (Uint8)(10 + (50 * gradientFactor));
            Uint8 colorB = (Uint8)(40 + (60 * gradientFactor));
            
            SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
            SDL_FRect rect = {0, (float)y, SCREEN_WIDTH, 4};
            SDL_RenderFillRect(renderer, &rect);
        }
        
        // Title with rainbow effect
        std::string title = "SPACE PING PONG SDL3";
        Color rainbowColors[] = {
            Color(255, 100, 100), Color(255, 150, 0), Color(255, 255, 0),
            Color(100, 255, 100), Color(100, 150, 255), Color(150, 100, 255),
            Color(255, 100, 255)
        };
        
        int titleX = SCREEN_WIDTH / 2 - (title.length() * 5 * 4) / 2;
        for (size_t i = 0; i < title.length(); i++) {
            if (title[i] != ' ') {
                int colorIndex = (i + menuTime / 10) % 7;
                Color color = rainbowColors[colorIndex];
                Color pulseColor(color.r * menuPulse, color.g * menuPulse, color.b * menuPulse);
                
                drawChar(renderer, title[i], titleX + i * 20, 100, 4, pulseColor);
            }
        }
        
        // Menu options
        std::vector<std::pair<std::string, Color>> menuItems = {
            {"1. PLAY VS COMPUTER", CYAN},
            {"2. PLAY VS HUMAN", PURPLE},
            {"3. HIGH SCORES", GOLD},
            {"", WHITE}, // Spacer
            {"DIFFICULTY: " + std::to_string((int)difficulty), GREEN},
            {"(PRESS E/M/H TO CHANGE)", WHITE},
            {"", WHITE}, // Spacer
            {"SPACE: PAUSE GAME", GOLD},
            {"ESC: QUIT", RED}
        };
        
        int y = 300;
        for (const auto& item : menuItems) {
            if (!item.first.empty()) {
                int textX = SCREEN_WIDTH / 2 - (item.first.length() * 5 * 2) / 2;
                drawText(renderer, item.first, textX, y, 2, item.second);
            }
            y += 40;
        }
        
        // Update and draw menu particles
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return !p.isAlive(); }), particles.end());
        
        for (auto& particle : particles) {
            particle.update();
            particle.draw(renderer);
        }
        
        // Add floating particles
        static std::random_device rd;
        static std::mt19937 gen(rd());
        if (gen() % 100 < 30) {
            float x = gen() % SCREEN_WIDTH;
            float y = gen() % SCREEN_HEIGHT;
            Color colors[] = {CYAN, PURPLE, GOLD, PINK};
            Color color = colors[gen() % 4];
            Vector2D velocity((gen() % 200 - 100) / 100.0f, (gen() % 150 - 200) / 100.0f);
            particles.push_back(Particle(x, y, color, velocity, 120));
        }
    }
    
    void drawGame(float shakeX, float shakeY) {
        // Draw center line
        for (int y = 0; y < SCREEN_HEIGHT; y += 20) {
            SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
            SDL_FRect lineRect = {SCREEN_WIDTH/2 - 2 + shakeX, y + shakeY, 4, 10};
            SDL_RenderFillRect(renderer, &lineRect);
        }
        
        // Draw paddles
        paddle1->draw(renderer);
        paddle2->draw(renderer);
        
        // Draw balls
        for (const auto& ball : balls) {
            ball.draw(renderer);
        }
        
        // Draw power-ups
        for (const auto& powerUp : powerUps) {
            powerUp.draw(renderer);
        }
        
        // Draw particles
        for (const auto& particle : particles) {
            particle.draw(renderer);
        }
        
        // Draw scores
        std::string score1 = std::to_string(player1Score);
        std::string score2 = std::to_string(player2Score);
        
        // Draw score backgrounds
        SDL_SetRenderDrawColor(renderer, CYAN.r, CYAN.g, CYAN.b, 100);
        SDL_FRect score1Rect = {SCREEN_WIDTH/2 + 20, 40, 50, 40};
        SDL_RenderFillRect(renderer, &score1Rect);
        SDL_SetRenderDrawColor(renderer, PINK.r, PINK.g, PINK.b, 100);
        SDL_FRect score2Rect = {SCREEN_WIDTH/2 - 70, 40, 50, 40};
        SDL_RenderFillRect(renderer, &score2Rect);
        
        // Draw score text
        drawText(renderer, score1, SCREEN_WIDTH/2 + 35, 50, 3, WHITE);
        drawText(renderer, score2, SCREEN_WIDTH/2 - 55, 50, 3, WHITE);
        
        // Draw freeze overlay
        if (freezeTimer > 0) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 50);
            SDL_FRect freezeRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &freezeRect);
            
            // Draw "FROZEN!" text
            drawText(renderer, "FROZEN!", SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 - 10, 4, BLUE);
        }
    }
    
    void drawPauseOverlay() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_FRect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);
        
        // Draw "PAUSED" text
        drawText(renderer, "PAUSED", SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT/2 - 20, 5, WHITE);
    }
    
    void drawGameOver() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_FRect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &gameOverRect);
        
        // Draw winner text
        std::string winner = (player1Score > player2Score) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
        int winnerX = SCREEN_WIDTH/2 - (winner.length() * 5 * 3) / 2;
        drawText(renderer, winner, winnerX, SCREEN_HEIGHT/2 - 50, 3, GOLD);
        
        // Draw final score
        std::string finalScore = std::to_string(player1Score) + " - " + std::to_string(player2Score);
        int scoreX = SCREEN_WIDTH/2 - (finalScore.length() * 5 * 2) / 2;
        drawText(renderer, finalScore, scoreX, SCREEN_HEIGHT/2, 2, WHITE);
        
        // Draw instructions
        drawText(renderer, "SPACE: PLAY AGAIN", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 50, 2, CYAN);
        drawText(renderer, "ESC: MENU", SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT/2 + 80, 2, CYAN);
    }
    
    void drawHighScores() {
        // Draw "HIGH SCORES" title
        drawText(renderer, "HIGH SCORES", SCREEN_WIDTH/2 - 80, 150, 4, CYAN);
        
        // Draw high scores (simplified)
        int y = 250;
        for (int i = 0; i < 5; i++) {
            std::string scoreText = std::to_string(i + 1) + ". PLAYER " + std::to_string(i % 2 + 1) + " - " + std::to_string(10 - i);
            drawText(renderer, scoreText, SCREEN_WIDTH/2 - 120, y, 2, WHITE);
            y += 40;
        }
        
        // Draw back instruction
        drawText(renderer, "ESC: BACK TO MENU", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 100, 2, GOLD);
    }
    
    void saveHighScore() {
        // Simplified high score saving
        // In a real implementation, you would save to a file
    }
    
    void loadHighScores() {
        // Simplified high score loading
        // In a real implementation, you would load from a file
    }
};

// Main function
int main(int argc, char* argv[]) {
    Game game;
    
    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return -1;
    }
    
    game.run();
    game.cleanup();
    
    return 0;
}
