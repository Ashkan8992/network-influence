#pragma once

#include "influence/graph.hpp"

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace influence {

class IndependentCascade {
public:
    using Probability = double;

    struct Configuration {
        Probability activation_probability = 0.1;
        std::size_t simulations = 1'000;
        std::uint64_t random_seed = 0; // Reproducibility Purposes: 12345 or 0 for not RNG
        std::size_t worker_count = 1;
    };

    IndependentCascade(
        const Graph& graph,
        Configuration configuration,
        const std::vector<NodeId> seeds
    );
    
private:
    const Graph& graph_;
    Configuration configuration_;
    std::vector<NodeId> seeds_;
    
    // Number of cascades each node was activated.
    std::vector<std::uint64_t> access_counts_;
    // Activation probability of each node.
    std::vector<Probability> access_probs_;
    
    /* ========================================
     Worker-State Abstraction (Multi-Thread)
    ======================================== */
    struct WorkerState {
        // Persistent random-number generator.
        std::mt19937_64 generator_;
        std::size_t simulations_;
        
        std::vector<std::uint8_t> active_;
        std::vector<NodeId> current_frontier_;
        std::vector<NodeId> next_frontier_;
        std::vector<std::uint64_t> partial_access_counts_;
        
    private:
        #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
        Metrics metrics_;
        #endif
    };
    
    // Run one stochastic Independent Cascade.
    void cascade(WorkerState& state);
    void run_worker(WorkerState& state);
    
    // Cascade Seed Function
    std::uint64_t cascade_seed(std::size_t simulation_index) const;
    
public:
    // Run the configured number of cascades.
    void run(); // Don't call it in the constructor as you might "override its results".
    
    // Access the nodes activation probabilities.
    const std::vector<Probability>& access_probabilities() const;
    
    // The rest is for profiling and metrics TODO: remove them
    struct Metrics {
        std::uint64_t cascades = 0;
        std::uint64_t activated_nodes = 0;
        std::uint64_t neighbor_examinations = 0;
        std::uint64_t activation_successes = 0;
        std::uint64_t activation_failures = 0;
    };

// For profiling and metrics
#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
    const Metrics& metrics() const;
#endif
};

}  // namespace influence
