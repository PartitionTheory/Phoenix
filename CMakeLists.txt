cmake_minimum_required(VERSION 3.16)
project(abr C)

set(CMAKE_C_STANDARD 11)

#
# Include directories
#
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(core/interface)
include_directories(vm)
include_directories(system)

#
# Core
#
set(CORE_SRC
        core/interface/core_tape.c
        core/core_extract.c
        core/core_slice.c
        core/core_dummy.c
)

#
# VM
#
set(VM_SRC
        vm/vm.c
        vm/program.c
        vm/instruction_registry.c
        vm/vm_plugin.c
)

#
# System
#
set(SYSTEM_SRC
        system/system.c
        system/system_modes.c
)

#
# ABR src
#
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
        src/abr_view.c
)

#
# Plugins
#
add_subdirectory(plugins)

#
# Build ABR core library
#
add_library(abr
        ${CORE_SRC}
        ${VM_SRC}
        ${SYSTEM_SRC}
        ${ABR_SRC}
)

#
# Viewer plugin (learnmath)
#
add_library(view_learnmath
        plugins/view_learnmath.c
)

target_link_libraries(abr PUBLIC view_learnmath)

#
# CLI executable
#
add_executable(abr_cli
        cli/abr_cli.c
)

target_link_libraries(abr_cli
        abr
        abr_plugins
)

#
# Tests
#
enable_testing()

add_executable(core_tests tests/core_tests.c)
target_link_libraries(core_tests abr)
add_test(NAME core_tests COMMAND core_tests)

add_executable(plugin_tests tests/plugin_tests.c)
target_link_libraries(plugin_tests abr abr_plugins)
add_test(NAME plugin_tests COMMAND plugin_tests)

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
