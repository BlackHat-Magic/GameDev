cmake_minimum_required(VERSION 3.16)
# Project name likely matches your game folder
project(Dani2025 C) # Use the actual game name

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# --- Find SDL3 ---
find_package(SDL3 REQUIRED CONFIG)
if (NOT SDL3_FOUND)
    message(FATAL_ERROR "SDL3 not found. Set SDL3_DIR or install SDL3 correctly.")
endif()
message(STATUS "Found SDL3: ${SDL3_INCLUDE_DIRS}")

# --- Add GLAD ---
# Assuming glad source/headers are in a 'glad' subdirectory relative to this CMakeLists.txt
add_library(glad STATIC
    glad/src/glad.c
)
target_include_directories(glad PUBLIC
    glad/include # Contains glad/glad.h and khr/khrplatform.h
)

# --- Your Executable ---
add_executable(${PROJECT_NAME} # Use project name for executable
    src/main.c
    src/utils/events.c
    src/utils/shaders.c # Add the new shader utility source file
)

# --- Copy Shader Files ---
# Define the source directory for shaders
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/assets/shaders)
# Define the destination directory relative to the executable
set(SHADER_DEST_DIR ${CMAKE_BINARY_DIR}/shaders) # Or ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders

# Create the destination directory if it doesn't exist
file(MAKE_DIRECTORY ${SHADER_DEST_DIR})

# Find all .vert and .frag files in the source directory
file(GLOB SHADER_FILES "${SHADER_SRC_DIR}/*.vert" "${SHADER_SRC_DIR}/*.frag")

# Copy each shader file to the destination
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    # Use configure_file to copy, or add_custom_command/target
    # configure_file is simpler for basic copying
    configure_file(${SHADER_FILE} ${SHADER_DEST_DIR}/${SHADER_NAME} COPYONLY)
    message(STATUS "Copying shader: ${SHADER_NAME} to ${SHADER_DEST_DIR}")
endforeach()


# --- Link Libraries ---
target_include_directories(${PROJECT_NAME} PRIVATE
    src # Allow includes like "utils/events.h"
    src/vendor
    ${SDL3_INCLUDE_DIRS}
    glad/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    glad
    m
)
