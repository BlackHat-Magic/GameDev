add_library(GameDevC STATIC
    src/engine_core.c
)

target_include_directories(GameDevC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

find_package(SDL3 REQUIRED)

target_link_libraries(GameDevC
    PUBLIC
        SDL3::SDL3
)

target_include_directories(GameDevC
    PUBLIC
        ${SDL3_INCLUDE_DIRS}
)

install(FILES src/engine_core.h DESTINATION include)