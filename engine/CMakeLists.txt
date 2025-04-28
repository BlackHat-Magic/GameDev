include(GenerateExportHeader)

# library definition
add_library(engine SHARED
    src/test.c
)

# add header files; helps IDEs find them
target_sources(engine PRIVATE
    src/defines.h
    src/test.h
)

# generate export header
generate_export_header(engine
    BASE_NAME ENGINE
    EXPORT_FILE_NAME src/engine_export.h
)

target_include_directories(engine
    PUBLIC
        # directories for using the engine
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
    PRIVATE
        # directories for building the engine
)

target_compile_definitions(engine
    PRIVATE
        # defines only nwhen building engine dll/so itself
        $<$<CXX_COMPILER_ID:MSVC>:CRT_SECURE_NO_WARNINGS>
    PUBLIC
        # defines needed by code using the engine
)

target_compile_options(engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(engine PUBLIC
    $<$<PLATFORM_ID:Windows>:user32>
)

message(STATUS "Configured engine library target.")