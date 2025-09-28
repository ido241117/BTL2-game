@echo off
echo Building Space Ping Pong SDL3...
echo.

REM Check if source file exists
if not exist "space_pingpong_sdl3.cpp" (
    echo Error: space_pingpong_sdl3.cpp not found!
    pause
    exit /b 1
)

REM Check if SDL3 development libraries exist
if not exist "SDL3-devel-3.2.22-mingw\SDL3-3.2.22\x86_64-w64-mingw32\include\SDL3\SDL.h" (
    echo Error: SDL3 development libraries not found!
    echo Please extract SDL3 development libraries to ./SDL3-devel-3.2.22-mingw/
    pause
    exit /b 1
)

REM Compile the game
echo Compiling...
g++ -o space_pingpong_sdl3.exe space_pingpong_sdl3.cpp -I./SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/include -L./SDL3-devel-3.2.22-mingw/SDL3-3.2.22/x86_64-w64-mingw32/lib -lSDL3 -lmingw32 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

if %errorlevel% equ 0 (
    echo.
    echo Build successful! space_pingpong_sdl3.exe created.
    echo.
    echo To run the game:
    echo   - Double-click space_pingpong_sdl3.exe, or
    echo   - Run: run_game.bat, or
    echo   - Run: make run
) else (
    echo.
    echo Build failed! Check the error messages above.
)

echo.
pause
