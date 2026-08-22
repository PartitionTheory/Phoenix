cmake_minimum_required(VERSION 3.16)
project(abr C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -------------------------
# Core (pure bit reader/writer)
# -------------------------
add_library(abr_core
        core/bitstream.h
        core/window.h
        core/windowset.h
        core/widthset.h
        core/core_extract.c
        core/core_slice.c
        core/core_dummy.c
)
target_include_directories(abr_core PUBLIC core)

# -------------------------
# SDK (bit-level only)
# -------------------------
add_library(abr_sdk
        sdk/flags.h
        sdk/context.h
        sdk/plugin.h
        sdk/sdk_dummy.c
)
target_include_directories(abr_sdk PUBLIC sdk)

# -------------------------
# Plugins (bit-level transforms)
# -------------------------
add_subdirectory(plugins)

# -------------------------
# CLI tool
# -------------------------
add_executable(abr cli/abr_cli.c)
target_link_libraries(abr
        abr_core
        abr_sdk
        abr_plugins
)

# -------------------------
# Tests (only core + plugin tests)
# -------------------------
add_executable(core_tests tests/core_tests.c)
target_link_libraries(core_tests
        abr_core
        abr_sdk
        abr_plugins
)

add_executable(plugin_tests tests/plugin_tests.c)
target_link_libraries(plugin_tests
        abr_core
        abr_sdk
        abr_plugins
)
