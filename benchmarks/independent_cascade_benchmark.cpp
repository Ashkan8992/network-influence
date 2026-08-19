#include "influence/graph.hpp"
#include "influence/independent_cascade.hpp"

#include <chrono>
#include <cstddef>
#include <iomanip> // setprecision?
#include <iostream>
#include <random>

int main() {
    constexpr std::size_t node_count = 10'000;
    constexpr std::size_t edge_count = 50'000;
    constexpr std::size_t simulations = 10'000;

    /*
     * ---------------------------------------------------------
     * Graph construction
     * ---------------------------------------------------------
     */
    const auto graph_start = std::chrono::steady_clock::now();
    
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
    
    const auto graph_end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> graph_elapsed = graph_end - graph_start;
    
    /*
     * ---------------------------------------------------------
     * Configure Independent Cascade
     * ---------------------------------------------------------
     */

    influence::IndependentCascade::Configuration config;

    config.activation_probability = 0.1;
    config.simulations = simulations;
    config.random_seed = 12345;

    influence::IndependentCascade simulation(graph, config, {0});

    /*
     * ---------------------------------------------------------
     * IC simulation
     * ---------------------------------------------------------
     */
    const auto simulation_start = std::chrono::steady_clock::now();

    simulation.run();

    const auto simulation_end = std::chrono::steady_clock::now();

    const std::chrono::duration<double> simulation_elapsed = simulation_end - simulation_start;

    const double simulations_per_second = static_cast<double>(simulations) / simulation_elapsed.count();

    /*
     * ---------------------------------------------------------
     * Results
     * ---------------------------------------------------------
     */
    const auto& probabilities = simulation.access_probabilities();

    double total_probability = 0.0;

    for (double probability : probabilities) {
        total_probability += probability;
    }

    const double average_probability =
        total_probability / static_cast<double>(probabilities.size());

    std::cout << std::fixed << std::setprecision(6);
    
    std::cout
        << "Independent Cascade Benchmark\n"
        << "------------------------------\n"
        << "Nodes:                  " << graph.node_count() << '\n'
        << "Edges:                  " << graph.edge_count() << '\n'
        << "Simulations:            " << simulations << '\n'
        << "Activation probability: " << config.activation_probability << '\n'
        << "Graph construction:     " << graph_elapsed.count() << " s\n"
        << "IC simulation:          " << simulation_elapsed.count() << " s\n"
        << "Simulations/second:     " << simulations_per_second << '\n'
        << "Average access prob.:   " << average_probability << '\n';

    return 0;
}
