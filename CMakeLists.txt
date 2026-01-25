cmake_minimum_required(VERSION 3.20)
project(legal-server
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Production Legal Document Processing Server")

# C++20 configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Release optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")

# Default to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required packages
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Main executable
add_executable(legal-server ${SOURCES})

# Link libraries
target_link_libraries(legal-server
    Boost::system
    Boost::thread
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    pthread
)
