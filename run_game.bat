@echo off
echo Starting Space Ping Pong SDL3...
echo.

REM Check if executable exists
if not exist "space_pingpong_sdl3.exe" (
    echo Error: space_pingpong_sdl3.exe not found!
    echo Please run 'make' first to build the game.
    pause
    exit /b 1
)

REM Check if SDL3.dll exists
if not exist "SDL3.dll" (
    echo Error: SDL3.dll not found!
    echo Please ensure SDL3.dll is in the same directory.
    pause
    exit /b 1
)

REM Run the game
echo Launching game...
space_pingpong_sdl3.exe

REM If game exits, show message
echo.
echo Game ended. Press any key to exit.
pause >nul
