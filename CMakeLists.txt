<<<<<<< HEAD
cmake_minimum_required(VERSION 3.16)
project(abr C)

set(CMAKE_C_STANDARD 11)

# Core
set(CORE_SRC
        core/interface/core_tape.c
)

# VM
set(VM_SRC
        vm/vm.c
        vm/program.c
        vm/instruction_registry.c
        vm/vm_plugin.c
)

# System
set(SYSTEM_SRC
        system/system.c
        system/system_modes.c
)

# ABR src (optional but recommended)
set(ABR_SRC
        src/abr_stream.c
        src/bigint.c
        src/bigint_list.c
        src/bitwindow.c
        src/reconstruct.c
        src/window_extract.c
        src/cli_join.c
        src/cli_split.c
        src/pipeline/pipeline_init.c
        src/pipeline/pipeline_linear.c
        src/pipeline/pipeline_reverse.c
        src/pipeline/pipeline_branching.c
)

add_library(abr
        ${CORE_SRC}
        ${VM_SRC}
        ${SYSTEM_SRC}
        ${ABR_SRC}
)

target_include_directories(abr PUBLIC
        core/interface
        vm
        system
        include
)

# -------------------------
# Tests
# -------------------------

enable_testing()

# VM tests
add_executable(test_vm vm/tests/test_vm.c)
target_link_libraries(test_vm abr)
add_test(NAME test_vm COMMAND test_vm)

add_executable(test_vm_integration vm/tests/test_vm_integration.c)
target_link_libraries(test_vm_integration abr)
add_test(NAME test_vm_integration COMMAND test_vm_integration)

add_executable(test_vm_program vm/tests/test_vm_program.c)
target_link_libraries(test_vm_program abr)
add_test(NAME test_vm_program COMMAND test_vm_program)

add_executable(test_vm_plugin vm/tests/test_vm_plugin.c)
target_link_libraries(test_vm_plugin abr)
add_test(NAME test_vm_plugin COMMAND test_vm_plugin)

# System tests
add_executable(test_system system/tests/test_system.c)
target_link_libraries(test_system abr)
add_test(NAME test_system COMMAND test_system)

add_executable(test_system_integration system/tests/test_system_integration.c)
target_link_libraries(test_system_integration abr)
add_test(NAME test_system_integration COMMAND test_system_integration)

add_executable(test_system_modes system/tests/test_system_modes.c)
target_link_libraries(test_system_modes abr)
add_test(NAME test_system_modes COMMAND test_system_modes)

add_executable(test_system_pipeline system/tests/test_system_pipeline.c)
target_link_libraries(test_system_pipeline abr)
add_test(NAME test_system_pipeline COMMAND test_system_pipeline)

add_executable(test_system_modes_pipeline system/tests/test_system_modes_pipeline.c)
target_link_libraries(test_system_modes_pipeline abr)
add_test(NAME test_system_modes_pipeline COMMAND test_system_modes_pipeline)

add_executable(test_system_modes_pipeline_integration system/tests/test_system_modes_pipeline_integration.c)
target_link_libraries(test_system_modes_pipeline_integration abr)
add_test(NAME test_system_modes_pipeline_integration COMMAND test_system_modes_pipeline_integration)

add_executable(test_system_modes_pipeline_integration_advanced system/tests/test_system_modes_pipeline_integration_advanced.c)
target_link_libraries(test_system_modes_pipeline_integration_advanced abr)
add_test(NAME test_system_modes_pipeline_integration_advanced COMMAND test_system_modes_pipeline_integration_advanced)

add_executable(test_system_modes_pipeline_integration_extreme system/tests/test_system_modes_pipeline_integration_extreme.c)
target_link_libraries(test_system_modes_pipeline_integration_extreme abr)
add_test(NAME test_system_modes_pipeline_integration_extreme COMMAND test_system_modes_pipeline_integration_extreme)
=======
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

>>>>>>> 5d15ab77bd13b9a0b3458d1385bb2270d7c45e61
