#!/usr/bin/env bash

set -e
set -u

if [[ $# -lt 1 ]]; then
    echo "No build preset provided."
    echo "Usage: ./build.sh [clangdeb|clangrel|gdeb|grel] [clean]"
    exit 1
fi

case "${1,,}" in
    clangdeb)
        PRESET="x64-Clang-Debug"
        ;;
    clangrel)
        PRESET="x64-Clang-Release"
        ;;
    gdeb)
        PRESET="Linux-GCC-Debug"
        ;;
    grel)
        PRESET="Linux-GCC-Release"
        ;;
    *)
        echo "Invalid argument: $1"
        echo "Usage: ./build.sh [clangdeb|clangrel|gdeb|grel] [clean]"
        exit 1
        ;;
esac

if [[ "${2:-}" == "clean" ]]; then
    echo "Cleaning build directories..."
    rm -rf build
    rm -f out/main out/main_d out/main_d.pdb
    rm -rf out/Debug out/Release

    echo "Build directories cleaned."
fi

SOURCE_DIR="."
LOG_DIR="build/logs"
LOG_FILE="$LOG_DIR/build_log.txt"

echo "--- Starting CMake Configuration and Build ---"
echo "Using preset: $PRESET"
echo "Source Directory: \"$SOURCE_DIR\""
echo "All CMake output will be logged to: \"$LOG_FILE\""
echo

mkdir -p "$LOG_DIR"

echo "--- CMake Build Log - $(date) ---" > "$LOG_FILE"

echo "--- Configuring CMake ---"
echo "--- Configuring CMake ---" >> "$LOG_FILE" 2>&1
if ! cmake "$SOURCE_DIR" --preset "$PRESET" >> "$LOG_FILE" 2>&1; then
    echo
    echo "CMake configuration FAILED!"
    echo "Please check \"$LOG_FILE\" for detailed errors."
    exit 1
fi

echo "CMake configuration COMPLETED successfully."
echo

echo "--- Building Project ---"
echo "--- Building Project ---" >> "$LOG_FILE" 2>&1
if ! cmake -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT"/scripts/buildsystems/vcpkg.cmake --build --preset "$PRESET" >> "$LOG_FILE" 2>&1; then
    echo
    echo "Project build FAILED!"
    echo "Please check \"$LOG_FILE\" for detailed errors."
    exit 1
fi

echo
echo "Project build COMPLETED successfully."
echo
echo "Build process finished. Full log available at \"$LOG_FILE\"."