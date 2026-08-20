cmake_minimum_required(VERSION 3.16)
project(abr_v2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Core
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

# SDK
add_library(abr_sdk
        sdk/flags.h
        sdk/context.h
        sdk/plugin.h
        sdk/sdk_dummy.c
)
target_include_directories(abr_sdk PUBLIC sdk)

# Pipeline
add_library(abr_pipeline
        pipeline/pipeline.h
        pipeline/pipeline_linear.c
        pipeline/pipeline_branching.c
        pipeline/pipeline_reverse.c
        pipeline/pipeline_init.c
)
target_include_directories(abr_pipeline PUBLIC pipeline)
target_link_libraries(abr_pipeline PUBLIC abr_core abr_sdk)

# VM
add_library(abr_vm
        vm/instruction.h
        vm/vm_program.h
        vm/vm_plugin.h
        vm/vm_plugin.c
        vm/vm_program.c
)
target_include_directories(abr_vm PUBLIC vm)
target_link_libraries(abr_vm PUBLIC abr_core abr_sdk abr_pipeline)

# System
add_library(abr_system
        system/abr_system.h
        system/abr_init.c
        system/abr_run.c
        system/abr_run_branching.c
        system/abr_run_reverse.c
)
target_include_directories(abr_system PUBLIC system)
target_link_libraries(abr_system PUBLIC abr_core abr_sdk abr_pipeline abr_vm)

# Tests
add_subdirectory(tests)
