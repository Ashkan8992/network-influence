# Performance Profiling

## Build Configuration

Performance profiling is performed using a `RelWithDebInfo` build.

This provides compiler optimizations while retaining debug symbols,
allowing profiling tools to associate sampled CPU time with C++
functions.

The initial profiling stage is intended to identify the dominant
CPU costs before optimization.

Areas of interest include:

Independent Cascade propagation
graph adjacency traversal
random-number generation
frontier management
activation-state management
access-count updates

Optimization decisions should be based on measured profiling results
rather than assumptions.

```bash
cmake -S . -B build-profile \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
(or for metrics)
cmake -S . -B build-metrics \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_SIMULATION_METRICS=ON

cmake --build build-profile

./build-profile/independent_cascade_benchmark

xcrun xctrace record \
    --template 'Time Profiler' \
    --launch ./build-profile/independent_cascade_benchmark

Future performance experiments should report at least:

graph size
edge count
number of simulations
activation probability
execution time
simulations per second

release --> build-profile --> (build-profile) private --> (release) private
========================================
Small sparse graph
========================================
Independent Cascade Benchmark
------------------------------
Nodes:                  1000
Edges:                  5000
Simulations:            10000
Activation probability: 0.100000
Graph construction:     0.000632 s --> 0.000849 s --> 0.000315 s
IC simulation:          0.001874 s --> 0.005062 s --> 0.000806 s --> 0.002017
Simulations/second:     5650575.389472 --> 1975503.753457 --> 12414649.286158
Average access prob.:   0.001997

========================================
Medium sparse graph
========================================
Independent Cascade Benchmark
------------------------------
Nodes:                  10000
Edges:                  50000
Simulations:            10000
Activation probability: 0.100000
Graph construction:     0.003554 s --> 0.009569 s --> 0.003837 s
IC simulation:          0.002286 s --> 0.005253 s --> 0.001748 s --> 0.003768
Simulations/second:     4377211.467805 --> 1903840.083525 --> 5721504.618399
Average access prob.:   0.000177

========================================
Large sparse graph
========================================
Independent Cascade Benchmark
------------------------------
Nodes:                  100000
Edges:                  500000
Simulations:            10000
Activation probability: 0.100000
Graph construction:     0.041597 s --> 0.052321 s --> 0.050076 s
IC simulation:          0.015030 s --> 0.015587 s --> 0.015125 s --> 0.014089
Simulations/second:     672971.080064 --> 641565.419624 --> 661149.768538
Average access prob.:   0.000019

========================================
Medium dense graph - high propagation
========================================
Independent Cascade Benchmark
------------------------------
Nodes:                  10000
Edges:                  50000
Simulations:            10000
Activation probability: 1.000000
Graph construction:     0.003341 s --> 0.003504 s --> 0.004854 s
IC simulation:          2.232285 s --> 2.323884 s --> 2.598152 s --> 2.238506
Simulations/second:     4479.745993 --> 4303.141547 --> 3848.890197
Average access prob.:   0.995300

### Tool

Apple Instruments Time Profiler was used with a
`RelWithDebInfo` build.

### Findings

Top CPU-consuming functions:

within the propagation loop: adjacency-list traversal, iterator overhead, and random-number generation.
cascade count
activated nodes
neighbor examinations
successful activation attempts
failed activation attempts
1. random-number generation
2. adjacency iteration

### Interpretation

TODO
