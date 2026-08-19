#!/bin/bash

# Run once in terminal
#chmod +x scripts/build.sh

#TODO: Make Separate Files clean.sh, run.sh, test.sh

set -e

rm -rf build
cmake -S . -B build # cmake --preset debug (or release)
cmake --build build # cmake --build --preset debug (or release)
# ./build/network_influence
ctest --test-dir build --output-on-failure # ctest --preset debug (or release)
./build/graph_benchmark # ./build/debug (or release)/graph_benchmark
# ./build/tests/graph_tests (or for GoogleTest)

# Release Format:
# rm -rf build-release
# cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
# cmake --build build-release
# ./build/network_influence
# ctest --test-dir build-release --output-on-failure

# For XCode Project:
#
# rm -rf build-xcode
# cmake -S . -B build-xcode -G Xcode (instead)
# open build-xcode/NetworkInfluence.xcodeproj (or)
# open -a Xcode build-xcode/NetworkInfluence.xcodeproj

# Profiling:
# cmake -S . -B build-profile -DCMAKE_BUILD_TYPE=RelWithDebInfo
# cmake --build build-profile
# ./build-profile/independent_cascade_benchmark
# xcrun xctrace record \
#     --template 'Time Profiler' \
#     --launch ./build-profile/independent_cascade_benchmark
