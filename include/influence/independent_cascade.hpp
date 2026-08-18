#pragma once

#include "influence/graph.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace influence {

class IndependentCascade {
public:
    using Probability = double;

    struct Configuration {
        Probability activation_probability = 0.1;
        std::size_t simulations = 1'000;
        std::uint64_t random_seed = 0; // Reproducibility Purposes
    };

    IndependentCascade(
        const Graph& graph,
        Configuration configuration,
        const std::vector<NodeId> seeds
    );
    
    void cascade();
    void run();
    
    const std::vector<Probability> access_probabilities() const;

private:
    const Graph& graph_;
    Configuration configuration_;
    std::vector<NodeId> seeds_;
    
    std::vector<std::size_t> access_counts_;
    std::vector<Probability> access_probs_;
};

}  // namespace influence
