cmake_minimum_required(VERSION 3.23)

project(NetworkInfluence
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(network-influence
    src/main.cpp
)

# cmake -S . -B build
# cmake --build build
# ./build/network_influence

# cmake -S . -B build-xcode -G Xcode
# open build-xcode/NetworkInfluence.xcodeproj