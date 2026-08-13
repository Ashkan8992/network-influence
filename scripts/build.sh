#!/bin/bash

# Run once in terminal
#chmod +x scripts/build.sh

#TODO: Make Separate Files clean.sh, run.sh, test.sh

set -e

rm -rf build
cmake -S . -B build
cmake --build build
# ./build/network_influence
ctest --test-dir build --output-on-failure
# ./build/tests/graph_tests (or for GoogleTest)

# For XCode Project:
#
# rm -rf build-xcode
# cmake -S . -B build-xcode -G Xcode (instead)
# open build-xcode/NetworkInfluence.xcodeproj (or)
# open -a Xcode build-xcode/NetworkInfluence.xcodeproj
