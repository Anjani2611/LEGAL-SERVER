cmake_minimum_required(VERSION 3.22)
project(LegalServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Boost 1.74 REQUIRED COMPONENTS system thread)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

set(SOURCES
    src/main.cpp
    src/utils/Logger.cpp
    src/server/Config.cpp
    src/server/HttpServer.cpp
    src/services/MLInferenceService.cpp
    src/services/LegalService.cpp
    src/handlers/RequestHandler.cpp
)

add_executable(legal-server ${SOURCES})

target_link_libraries(legal-server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    spdlog
    fmt
    pq
)

message(STATUS "===========================================")
message(STATUS "Legal Server - Build Configuration")
message(STATUS "===========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "===========================================")

install(TARGETS legal-server DESTINATION bin)
install(DIRECTORY config/ DESTINATION etc/legal-server)
