#pragma once

#include "influence/graph.hpp"

#include <cstddef>
#include <cstdint>

namespace influence {

class IndependentCascade {
public:
    using Probability = double;

    struct Configuration {
        Probability activation_probability = 0.1;
        std::size_t simulations = 1'000;
    };

    IndependentCascade(
        const Graph& graph,
        Configuration configuration
    );

private:
    const Graph& graph_;
    Configuration configuration_;
};

}  // namespace influence
