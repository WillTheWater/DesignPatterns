cmake_minimum_required(VERSION 3.20)

project(GameDesignPatterns 
    VERSION 1.0 
    DESCRIPTION "Educational project demonstrating Game Dev Design Patterns"
    LANGUAGES CXX
)

# ------------------------------------------------------------------
# 1. C++ Standard Settings
# ------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------
# 2. Output Directories (Works on Win/Mac/Linux)
# ------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ------------------------------------------------------------------
# 3. Automating Source Discovery
# ------------------------------------------------------------------
# This automatically finds all .cpp files in src/ and subdirectories
file(GLOB_RECURSE SOURCES "src/*.cpp")

# This finds all headers in include/
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

list(APPEND SOURCES ${HEADERS})

# ------------------------------------------------------------------
# 4. Create Executable
# ------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# ------------------------------------------------------------------
# 5. Platform Specific Configuration
# ------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# ------------------------------------------------------------------
# 6. Compiler Warnings
# ------------------------------------------------------------------
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()