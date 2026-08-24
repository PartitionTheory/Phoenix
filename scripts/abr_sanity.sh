#!/usr/bin/env bash
set -e

echo "=== ABR Sanity Check ==="
echo "Location: $(pwd)"
echo

echo "[1] Cleaning build directory..."
rm -rf build
mkdir build
cd build

echo "[2] Running CMake configure..."
cmake ..

echo "[3] Building all targets..."
cmake --build .

echo
echo "=== Build Complete ==="
echo

echo "[4] Running core_tests..."
./core_tests > ../sanity_core.out
echo "core_tests output saved to sanity_core.out"

echo "[5] Running plugin_tests..."
./plugin_tests > ../sanity_plugin.out
echo "plugin_tests output saved to sanity_plugin.out"

echo "[6] Running CLI branching test..."
echo -ne '\xDE\xAD\xBE' > ../tc3.bin
./abr ../tc3.bin --plugin branch_merge_split > ../sanity_cli.out
echo "CLI output saved to sanity_cli.out"

echo
echo "=== Summary ==="
echo "core_tests:    PASS (output in sanity_core.out)"
echo "plugin_tests:  PASS (output in sanity_plugin.out)"
echo "cli test:      PASS (output in sanity_cli.out)"
echo
echo "All ABR sanity checks completed successfully."

