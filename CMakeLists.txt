cmake_minimum_required(VERSION 4.0)
project(GameDev.c C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(engine)
add_subdirectory(testbed)

# SDL 3
find_package(SDL3 CONFIG)
if(SDL3_FOUND)
    message(STATUS "Found SDL3 version: ${SDL3_VERSION}.")
else()
    message(WARNING "SDL3 not found by top-level CMake.")
endif()

message(STATUS "Top-level configuration complete.")