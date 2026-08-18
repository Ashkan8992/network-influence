#include "influence/independent_cascade.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using influence::Graph;
using influence::IndependentCascade;

TEST(IndependentCascadeTest, AcceptsValidConfiguration) { // + InvalidSeedThrows
    Graph graph(3, Graph::Direction::Directed);

    IndependentCascade::Configuration config;
    ASSERT_EQ(config.activation_probability, 0.1);
    ASSERT_EQ(config.simulations, 1'000);
    config.activation_probability = 0.25;
    config.simulations = 10'000;
    
    std::vector<influence::NodeId> seeds = {0};

    EXPECT_NO_THROW(IndependentCascade simulation(graph, config, seeds););
    
    config.activation_probability = -0.1;
    EXPECT_THROW(IndependentCascade simulation(graph, config, seeds), std::invalid_argument);
    
    config.activation_probability = 1.1;
    EXPECT_THROW(
        IndependentCascade simulation(graph, config, seeds),
        std::invalid_argument
    );
    
    config.activation_probability = 1.0;
    config.simulations = 0;
    EXPECT_THROW(
        IndependentCascade simulation(graph, config, seeds),
        std::invalid_argument
    );
    
    config.simulations = 1000;
    seeds.push_back(5);
    EXPECT_THROW(
        IndependentCascade simulation(graph, config, seeds),
        std::out_of_range
    );
}

TEST(IndependentCascadeTest, ProbabilityOneActivatesEntireReachableComponent) { // + DuplicateSeedsAreHandledOnce
    Graph graph(4, Graph::Direction::Directed);

    graph.add_edge(0, 1);
    graph.add_edge(1, 2);

    IndependentCascade::Configuration config;
    config.activation_probability = 1.0;
    
    std::vector<influence::NodeId> seeds = {0, 0};

    IndependentCascade simulation(graph, config, seeds);

    simulation.run();
    const auto& result = simulation.access_probabilities();

    ASSERT_EQ(result.size(), 4);

    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 1);
    EXPECT_EQ(result[2], 1);
    EXPECT_EQ(result[3], 0);
}

TEST(IndependentCascadeTest, ProbabilityZeroOnlyActivatesSeeds) { // + MultipleSeedsAreActivated
    Graph graph(3,Graph::Direction::Directed);

    graph.add_edge(0, 1);

    IndependentCascade::Configuration config;
    config.activation_probability = 0.0;
    
    std::vector<influence::NodeId> seeds = {0, 0, 2};

    IndependentCascade simulation(graph, config, seeds);

    simulation.run();
    const auto& result = simulation.access_probabilities();

    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 0);
    EXPECT_EQ(result[2], 1);
}

TEST(IndependentCascadeTest, IsolatedSeedRemainsActive) { // + SeedCannotActivatedInNodes
    Graph graph(3, Graph::Direction::Directed);

    graph.add_edge(1, 2);

    IndependentCascade::Configuration config;
    config.activation_probability = 1.0;
    
    std::vector<influence::NodeId> seeds = {0, 2};

    IndependentCascade simulation(graph, config, seeds);

    simulation.run();
    const auto& result = simulation.access_probabilities();

    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 0);
}

/* TEST(IndependentCascadeTest, CascadeEffectProbability) {
    Graph graph(4, Graph::Direction::Directed);

    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);

    IndependentCascade::Configuration config;
    config.activation_probability = 0.1;
    
    std::vector<influence::NodeId> seeds = {0};

    IndependentCascade simulation(graph, config, seeds);

    simulation.run();
    const auto& result = simulation.access_probabilities();

    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 0.1);
    EXPECT_EQ(result[2], 0.01);
    EXPECT_EQ(result[3], 0.001);
} */
