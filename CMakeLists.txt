cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add executable
file(GLOB SOURCES "src/*.cpp")
add_executable(dungeon_crawler ${SOURCES})

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 