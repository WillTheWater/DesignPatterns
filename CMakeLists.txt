# =========================================================================
# BUILD SYSTEM CONFIGURATION: Game Design Patterns
# =========================================================================
# THE GOAL:
# Orchestrate the compilation, dependency management, and deployment 
# of the educational design patterns project.
#
# THE STRUCTURE:
# 1. CORE CONFIG:    Defines project identity and minimum requirements.
# 2. TOOLCHAIN:      Sets C++20 standards and compiler-specific flags.
# 3. DEPENDENCIES:   Automates the fetching of external libraries (JSON).
# 4. ORCHESTRATION:  Discovers source files and defines build targets.
# 5. DEPLOYMENT:     Handles post-build asset relocation.
# =========================================================================

cmake_minimum_required(VERSION 3.20) ######

# =========================================================================
# PROJECT IDENTITY
# ROLE: Establishes the project namespace, versioning, and purpose.
# =========================================================================
project(GameDesignPatterns 
    VERSION 1.0 
    DESCRIPTION "Educational project demonstrating Game Dev Design Patterns"
    LANGUAGES CXX
)

# =========================================================================
# TOOLCHAIN & STANDARDS
# ROLE: Enforces modern C++ standards and ensures cross-platform consistency.
# =========================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================================================================
# EXTERNAL DEPENDENCIES (JSON SUBSYSTEM)
# ROLE: Uses FetchContent to pull third-party code directly into the build.
# =========================================================================
include(FetchContent)

FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

# ======================== NLOHMANN_JSON ========================
FetchContent_MakeAvailable(json)

# =========================================================================
# OUTPUT DIRECTORY CONFIGURATION
# ROLE: Organizes binary, library, and archive output for clean deployment.
# =========================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =========================================================================
# SOURCE DISCOVERY & TARGET DEFINITION
# ROLE: Dynamically gathers all code assets and constructs the executable.
# =========================================================================
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

list(APPEND SOURCES ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES})

# =========================================================================
# LINKING & INCLUDE PATHS
# ROLE: Connects the project to headers and third-party compiled binaries.
# =========================================================================
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# ======================== BINDING JSON ========================
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# ======================== SAFETY & QUALITY ========================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# =========================================================================
# ASSET DEPLOYMENT (POST-BUILD)
# ROLE: Ensures the runtime data (save files, icons) follows the executable.
# =========================================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Deploying project data to binary directory..."
)