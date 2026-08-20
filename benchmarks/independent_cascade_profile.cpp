#include "influence/graph.hpp"
#include "influence/independent_cascade.hpp"

#include <chrono>
#include <cstddef>
#include <iomanip> // setprecision?
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

struct BenchmarkScenario {
    std::string name;
    std::size_t node_count;
    std::size_t edge_count;
    double activation_probability;
    std::size_t simulations;
};

void run_benchmark(const BenchmarkScenario& scenario) {
    /*
     * ---------------------------------------------------------
     * Graph construction
     * ---------------------------------------------------------
     */
    const auto graph_start = std::chrono::steady_clock::now();
    
    influence::Graph graph(scenario.node_count, influence::Graph::Direction::Directed);

    /*
     * Build a deterministic graph for reproducible benchmarking.
     *
     * This is deliberately simple for the first benchmark.
     * We'll introduce realistic network datasets later.
     */
    std::mt19937_64 graph_generator(12345);

    for (std::size_t i = 0; i < scenario.edge_count; ++i) {
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

    config.activation_probability = scenario.activation_probability;
    config.simulations = scenario.simulations;
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

    const double simulations_per_second = static_cast<double>(scenario.simulations) / simulation_elapsed.count();

    /*
     * ---------------------------------------------------------
     * Basic correctness/sanity metric
     * ---------------------------------------------------------
     */
    const auto& probabilities = simulation.access_probabilities();

    double total_probability = 0.0;

    for (double probability : probabilities) {
        total_probability += probability;
    }

    const double average_probability =
        total_probability / static_cast<double>(probabilities.size());
    
    /*
     * ---------------------------------------------------------
     * Results
     * ---------------------------------------------------------
     */
    
// #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS

    const auto& metrics = simulation.metrics();

    std::cout
        << "Cascades:               "
        << metrics.cascades
        << '\n'

        << "Activated nodes:        "
        << metrics.activated_nodes
        << '\n'

        << "Neighbor examinations:  "
        << metrics.neighbor_examinations
        << '\n'

        << "Activation successes:   "
        << metrics.activation_successes
        << '\n'

        << "Activation failures:    "
        << metrics.activation_failures
        << '\n';

// #endif
}

}  // namespace

int main() {
    const std::vector<BenchmarkScenario> scenarios = {
        {
            "Medium graph - high propagation",
            10'000,
            50'000,
            1.0,
            10'000
        }
    };

    for (const auto& scenario : scenarios) {
        run_benchmark(scenario);
    }

    return 0;
}
