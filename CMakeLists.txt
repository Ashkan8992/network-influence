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
    include/influence/independent_cascade.hpp
    src/version.cpp
    src/graph.cpp
    src/independent_cascade.cpp
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

# BenchMarks:
# Graph Benchmark
add_executable(graph_benchmark
    benchmarks/graph_benchmark.cpp
)

target_link_libraries(graph_benchmark
    PRIVATE
        network_influence
)

# Independent Cascade Benchmark
add_executable(independent_cascade_benchmark
    benchmarks/independent_cascade_benchmark.cpp
)

target_link_libraries(independent_cascade_benchmark
    PRIVATE
        network_influence
)

#Testing
enable_testing()
add_subdirectory(tests)
