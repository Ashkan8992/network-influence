#include "influence/graph.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>

using influence::Graph;
using influence::NodeId;

int main() {
    constexpr std::size_t node_count = 100'000;
    constexpr std::size_t edges_per_node = 10;

    Graph graph(node_count, Graph::Direction::Directed);

    const auto construction_start = std::chrono::steady_clock::now();

    for (NodeId node = 0; node < node_count; ++node) {
        for (NodeId offset = 1; offset <= edges_per_node; ++offset) {
            const NodeId target = (node + offset) % node_count;
            graph.add_edge(node, target);
        }
    }

    const auto construction_end = std::chrono::steady_clock::now();

    const auto construction_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            construction_end - construction_start
        );

    std::size_t visited_edges = 0;

    const auto traversal_start = std::chrono::steady_clock::now();

    for (NodeId node = 0; node < node_count; ++node) {
        for (const NodeId neighbor : graph.neighbors(node)) {
            ++visited_edges;
        }
    }

    const auto traversal_end = std::chrono::steady_clock::now();

    const auto traversal_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            traversal_end - traversal_start
        );

    std::cout << "Nodes: " << graph.node_count() << '\n';
    std::cout << "Edges: " << graph.edge_count() << '\n';
    std::cout << "Construction: "
              << construction_time.count()
              << " ms\n";

    std::cout << "Traversal: "
              << traversal_time.count()
              << " ms\n";

    std::cout << "Visited edges: "
              << visited_edges
              << '\n';
}
