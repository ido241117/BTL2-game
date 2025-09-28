# Space Ping Pong SDL3

A modern, feature-rich ping pong game built with SDL3 and C++. Features include power-ups, particle effects, multiple game modes, and a beautiful space-themed interface.

## 🎮 Features

- **Multiple Game Modes**: Play against computer or human opponent
- **Power-ups**: Speed boost, paddle size changes, multi-ball, shield, freeze, laser, and magnet effects
- **Particle Effects**: Beautiful visual effects for collisions and power-ups
- **Difficulty Levels**: Easy, Medium, and Hard AI difficulty
- **High Scores**: Track your best performances
- **Space Theme**: Animated background with moving stars

## 🚀 Quick Start

### Prerequisites
- Windows with MinGW-w64 compiler
- SDL3 development libraries

### Installation

1. **Clone the repository:**
   ```bash
   git clone <your-repo-url>
   cd space-ping-pong-sdl3
   ```

2. **Install SDL3 dependencies:**
   ```bash
   make install-deps
   ```
   Then download SDL3 development libraries from [SDL Releases](https://github.com/libsdl-org/SDL/releases) and extract to `./SDL3-devel-3.2.22-mingw/`

3. **Build the game:**
   ```bash
   make
   ```

4. **Run the game:**
   ```bash
   make run
   ```

### Alternative: Direct compilation
```bash
g++ -o space_pingpong_sdl3.exe space_pingpong_sdl3.cpp -I./SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/include -L./SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/lib -lSDL3 -lmingw32 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid
```

## 🎯 Controls

### Menu Navigation
- **1**: Play vs Computer
- **2**: Play vs Human
- **3**: View High Scores
- **E/M/H**: Change difficulty (Easy/Medium/Hard)
- **ESC**: Quit

### In-Game Controls
- **Player 1 (Right)**: Arrow Keys (Up/Down)
- **Player 2 (Left)**: W/S keys (in vs Human mode)
- **SPACE**: Pause/Resume
- **ESC**: Return to menu

## 🎨 Game Features

### Power-ups
- **Cyan**: Speed Boost - Increases ball speed
- **Green**: Paddle Grow - Makes paddle larger
- **Red**: Paddle Shrink - Makes paddle smaller
- **Purple**: Multi-Ball - Spawns additional balls
- **Gold**: Shield - Protects paddle from ball
- **Blue**: Freeze - Temporarily stops ball movement
- **Orange**: Laser - Shoots laser beam
- **Pink**: Magnet - Ball is attracted to paddle

### Visual Effects
- Particle systems for collisions
- Screen shake effects
- Animated background stars
- Rainbow title animation
- Power-up visual indicators

## 📁 Project Structure

```
space-ping-pong-sdl3/
├── space_pingpong_sdl3.cpp    # Main game source code
├── Makefile                   # Build configuration
├── README.md                  # This file
├── .gitignore                 # Git ignore rules
├── SDL3.dll                   # SDL3 runtime library
├── SDL2_image.dll             # SDL2 image library
└── SDL3-devel-3.2.22-mingw/   # SDL3 development libraries
    └── SDL3-3.2.22/
        └── x86_64-w64-mingw32/
            ├── bin/
            ├── include/
            └── lib/
```

## 🔧 Development

### Building from source
```bash
# Clean previous build
make clean

# Build the game
make

# Run the game
make run
```

### Code Structure
- **Game Class**: Main game loop and state management
- **Ball Class**: Ball physics and rendering
- **Paddle Class**: Player and AI paddle logic
- **PowerUp Class**: Power-up effects and rendering
- **Particle Class**: Visual effects system
- **Star Class**: Background animation

## 🐛 Troubleshooting

### Common Issues

1. **"SDL3.dll not found"**
   - Ensure `SDL3.dll` is in the same directory as the executable
   - Download from SDL3 releases if missing

2. **Compilation errors**
   - Verify SDL3 development libraries are properly installed
   - Check that MinGW-w64 is installed and in PATH

3. **Game won't start**
   - Check that all required DLLs are present
   - Verify graphics drivers are up to date

## 📝 License

This project is open source. Feel free to modify and distribute.

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📞 Support

If you encounter any issues, please create an issue on GitHub with:
- Your operating system
- Compiler version
- Error messages
- Steps to reproduce
