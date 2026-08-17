#include "influence/independent_cascade.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using influence::Graph;
using influence::IndependentCascade;

TEST(IndependentCascadeTest, AcceptsValidConfiguration) {
    Graph graph(3, Graph::Direction::Directed);

    IndependentCascade::Configuration config;

    config.activation_probability = 0.25;
    config.simulations = 10'000;

    EXPECT_NO_THROW(
        IndependentCascade simulation(graph, config);
    );
    
    config.activation_probability = -0.1;
    EXPECT_THROW(
        IndependentCascade simulation(graph, config),
        std::invalid_argument
    );
    
    config.activation_probability = 1.1;
    EXPECT_THROW(
        IndependentCascade simulation(graph, config),
        std::invalid_argument
    );
    
    config.activation_probability = 1.0;
    config.simulations = 0;

    EXPECT_THROW(
        IndependentCascade simulation(graph, config),
        std::invalid_argument
    );
}
