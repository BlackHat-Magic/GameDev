add_executable(testbed
    src/main.c
)

target_include_directories(testbed PRIVATE
    src
    # no need to explicitly add engine includes here
    # CMake automatically gets engine's public includes
)

target_compile_definitions(testbed PRIVATE
    # defines specific to testbed
)

target_link_libraries(testbed PRIVATE
    engine # links engine dll/so and brings public dependencies
)

message(STATUS "Configured testbed executable target.")