# Graph Performance Baseline

## Environment

Record:

- Machine: MacBook Air M1, 2020
- CPU: Apple M1
- RAM: 8GB
- Operating system: MacOS Tahoe 26.5
- Compiler: Apple clang
- Compiler version: Apple clang version 21.0.0
- Build type: N/A (Debug or Release)

## Workload

- Nodes: 100,000
- Directed edges: 1,000,000
- Edges per node: 10

## Results

Record several runs rather than a single measurement. (Debug)

| Run | Construction (ms) | Traversal (ms) |
|-----|--------------------|----------------|
| 1   |       192 ms       |      31 ms     |
| 2   |       168 ms       |      31 ms     |
| 3   |       201 ms       |      31 ms     |
| 4   |       200 ms       |      30 ms     |
| 5   |       177 ms       |      31 ms     |

Record several runs rather than a single measurement. (Release)

| Run | Construction (ms) | Traversal (ms) |
|-----|--------------------|----------------|
| 1   |       37 ms        |      4 ms      |
| 2   |       16 ms        |      2 ms      |
| 3   |       32 ms        |      5 ms      |
| 4   |       35 ms        |      4 ms      |
| 5   |       36 ms        |      4 ms      |

## Notes

This benchmark establishes a baseline for the current
std::vector<std::vector<NodeId>> graph representation.

Future storage and performance changes should be evaluated
against this baseline.
