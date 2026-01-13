@echo off
setlocal
echo ======================================================
echo Game Design Patterns: Windows Build Script
echo ======================================================

:: 1. Create and enter build directory
if not exist build (
    mkdir build
    echo [Log] Created build directory.
)

:: 2. Configure the project
echo [Log] Configuring project with CMake...
cmake -S . -B build
if %errorlevel% neq 0 (
    echo [Error] CMake configuration failed! Ensure Visual Studio is installed.
    pause
    exit /b %errorlevel%
)

:: 3. Build Debug version
echo [Log] Building Debug version...
cmake --build build --config Debug
if %errorlevel% neq 0 (
    echo [Error] Debug build failed!
    pause
    exit /b %errorlevel%
)

:: 4. Build Release version
echo [Log] Building Release version...
cmake --build build --config Release
if %errorlevel% neq 0 (
    echo [Error] Release build failed!
    pause
    exit /b %errorlevel%
)

echo ======================================================
echo BUILD COMPLETE!
echo Executables located in: build/bin/
echo ======================================================
pause