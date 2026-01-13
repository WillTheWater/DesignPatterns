#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

echo "======================================================"
echo "Game Design Patterns: Unix Build Script (Mac/Linux)"
echo "======================================================"

# 1. Create build directory
mkdir -p build

# 2. Configure the project
echo "[Log] Configuring project..."
cmake -S . -B build

# 3. Build Debug version
echo "[Log] Building Debug..."
cmake --build build --config Debug

# 4. Build Release version
echo "[Log] Building Release..."
cmake --build build --config Release

echo "======================================================"
echo "BUILD COMPLETE!"
echo "Executables located in: build/bin/"
echo "======================================================"