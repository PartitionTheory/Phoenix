cmake_minimum_required(VERSION 3.10)
project(abr C)

#
# A: Mathematical description
#    The ABR build system composes three independent modules:
#      core   : byte → window extraction
#      sdk    : ABI boundary for plugins
#      plugins: structural + bitwise transforms
#
# B: Engineering description
#    Each module builds into a static library.
#    The CLI links all three.
#

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)



#
# Core library
#
add_library(abr_core
    core/core_extract.c
    core/core_slice.c
    core/core_dummy.c
)
target_include_directories(abr_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

#
# SDK library
#
add_library(abr_sdk
    sdk/sdk_dummy.c
)

#
# Plugins library
#
add_subdirectory(plugins)

#
# CLI executable
#
add_executable(abr
    cli/abr_cli.c
)

target_link_libraries(abr
    abr_core
    abr_sdk
    abr_plugins
)

#
# Tests
#
add_executable(core_tests tests/core_tests.c)
target_link_libraries(core_tests abr_core)

add_executable(plugin_tests tests/plugin_tests.c)
target_link_libraries(plugin_tests
    abr_core
    abr_sdk
    abr_plugins
)

