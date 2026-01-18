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
# 2. External Dependencies (nlohmann/json)
# ------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

# This makes the nlohmann_json::nlohmann_json target available
FetchContent_MakeAvailable(json)

# ------------------------------------------------------------------
# 3. Output Directories
# ------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ------------------------------------------------------------------
# 4. Automating Source Discovery
# ------------------------------------------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")
list(APPEND SOURCES ${HEADERS})

# ------------------------------------------------------------------
# 5. Create Executable
# ------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# ------------------------------------------------------------------
# 6. Linking and Include Directories
# ------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# Link the JSON library to your project
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# ------------------------------------------------------------------
# 7. Compiler Warnings
# ------------------------------------------------------------------
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# ------------------------------------------------------------------
# 8. Copy Data to Output Directory
# ------------------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)