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
