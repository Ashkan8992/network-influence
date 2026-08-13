cmake_minimum_required(VERSION 3.23)

project(NetworkInfluence
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(network_influence
    include/influence/version.hpp
    include/influence/graph.hpp
    src/version.cpp
    src/graph.cpp
)

target_include_directories(network_influence
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(network_influence_app
    src/main.cpp
)

target_link_libraries(network_influence_app
    PRIVATE
        network_influence
)

enable_testing()
add_subdirectory(tests)

# rm -rf build
# cmake -S . -B build
# rm -rf build-xcode
# cmake -S . -B build-xcode -G Xcode (instead)
# open build-xcode/NetworkInfluence.xcodeproj (or)
# open -a Xcode build-xcode/NetworkInfluence.xcodeproj
# cmake --build build
# ./build/network_influence
# ctest --test-dir build --output-on-failure (or)
# ./build/tests/graph_tests

# cmake -S . -B build-xcode -G Xcode
# open build-xcode/NetworkInfluence.xcodeproj
