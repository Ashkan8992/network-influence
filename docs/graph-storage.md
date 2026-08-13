Data-Oriented Design (Graph Storage):

Current Representation:
The mutable Graph uses:
std::vector<std::vector<NodeId>>
Advantages: Simple implementation, Efficient neighbor iteration, Straightforward edge insertion (Immutable), Straightforward directed/undirected graph support, Easy unit testing.

Adjacency Matrix: Unsuitable for large sparse social networks, O(N²) storage.

Future Simulation Representation:
Build the graph in a convenient representation, then convert it into a compact immutable representation for simulation.
Use Compressed Sparse Row (CSR):
Advantages: Memory Locality, Cache Behavior, Allocation Efficiency, Sequential traversal Performance.
Disadvantages: Mutation
