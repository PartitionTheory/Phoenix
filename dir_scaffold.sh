#!/usr/bin/env sh
set -e

echo "Scaffolding ABR‑v2 (pure C) project..."

# Root
mkdir -p abr
cd abr

# Top-level build files
touch CMakeLists.txt

# Core module
mkdir -p core
touch core/CMakeLists.txt
touch core/bitstream.h
touch core/window.h
touch core/windowset.h
touch core/widthset.h
touch core/core_extract.c
touch core/core_slice.c
touch core/core_dummy.c

# SDK module
mkdir -p sdk
touch sdk/CMakeLists.txt
touch sdk/flags.h
touch sdk/context.h
touch sdk/plugin.h

# Pipeline module
mkdir -p pipeline
touch pipeline/CMakeLists.txt
touch pipeline/pipeline.h
touch pipeline/pipeline_linear.c
touch pipeline/pipeline_branching.c
touch pipeline/pipeline_reverse.c

# VM module (pure C version)
mkdir -p vm
touch vm/CMakeLists.txt
touch vm/instruction.h
touch vm/vm_program.h
touch vm/vm_plugin.h
touch vm/vm_plugin.c

# System module
mkdir -p system
touch system/CMakeLists.txt
touch system/abr_system.h
touch system/abr_init.c
touch system/abr_run.c
touch system/abr_run_branching.c
touch system/abr_run_reverse.c

# Tests
mkdir -p tests
touch tests/CMakeLists.txt
touch tests/core_tests.c
touch tests/plugin_tests.c
touch tests/pipeline_tests.c
touch tests/vm_tests.c
touch tests/integration_tests.c

echo "ABR‑v2 scaffold complete."
