#include "influence/graph.hpp"
#include "influence/independent_cascade.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>

int main() {
    constexpr std::size_t node_count = 10'000;
    constexpr std::size_t edge_count = 50'000;
    constexpr std::size_t simulations = 10'000;

    influence::Graph graph(node_count, influence::Graph::Direction::Directed);

    /*
     * Build a deterministic graph for reproducible benchmarking.
     *
     * This is deliberately simple for the first benchmark.
     * We'll introduce realistic network datasets later.
     */
    std::mt19937_64 graph_generator(12345);

    for (std::size_t i = 0; i < edge_count; ++i) {
        graph.add_random_edge(graph_generator);
    }

    influence::IndependentCascade::Configuration config;

    config.activation_probability = 0.1;
    config.simulations = simulations;
    config.random_seed = 12345;

    influence::IndependentCascade simulation(graph, config, {0});

    const auto start = std::chrono::steady_clock::now();

    simulation.run();

    const auto end = std::chrono::steady_clock::now();

    const std::chrono::duration<double> elapsed = end - start;

    const double simulations_per_second = static_cast<double>(simulations) / elapsed.count();

    std::cout
        << "Independent Cascade Benchmark\n"
        << "------------------------------\n"
        << "Nodes:                " << graph.node_count() << '\n'
        << "Edges:                " << graph.edge_count() << '\n'
        << "Simulations:          " << simulations << '\n'
        << "Elapsed time:         " << elapsed.count() << " s\n"
        << "Simulations/second:   "
        << simulations_per_second << '\n';

    return 0;
}
