#include "influence/independent_cascade.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

#include <iostream> // test remove later

namespace influence {

IndependentCascade::IndependentCascade(const Graph& graph, Configuration configuration, std::vector<NodeId> seeds) : graph_(graph), configuration_(configuration), seeds_(seeds) {

    if (configuration_.activation_probability < 0.0 ||
        configuration_.activation_probability > 1.0) {
        throw std::invalid_argument(
            "activation probability must be between 0 and 1"
        );
    }

    if (configuration_.simulations == 0) {
        throw std::invalid_argument(
            "number of simulations must be greater than zero"
        );
    }
    
    // Validate all seed nodes
    for (NodeId seed : seeds_) {
        if (seed >= graph_.node_count()) {
            throw std::out_of_range("seed node is out of range");
        }
    }
    
    access_counts_.resize(graph_.node_count());
}

void IndependentCascade::run() {
    for (size_t i = 0; i < configuration_.simulations; ++i) {
        cascade();
    }
    
    access_probs_.resize(graph_.node_count());

    for (std::size_t node = 0; node < access_counts_.size(); ++node) {
        access_probs_[node] = static_cast<Probability>(access_counts_[node]) / static_cast<Probability>(configuration_.simulations);
    }
}

void IndependentCascade::cascade() {
    const std::size_t node_count = graph_.node_count();
    
    /* Reproducibility Purposes
    0 means: generate a seed automatically.
     non-zero seed makes the experiment reproducible. */
    std::uint64_t random_seed = configuration_.random_seed;
    if (random_seed == 0) {
        std::random_device random_device;
        random_seed = random_device();
    }
    std::mt19937_64 generator(random_seed);

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    
    std::vector<NodeId> current_frontier;
    std::vector<NodeId> next_frontier;
    
    current_frontier.reserve(seeds_.size());
    
    std::vector<uint8_t> active(node_count, 0);
    for (NodeId seed : seeds_) {
        if (active[seed] == 1) { continue; }
        active[seed] = 1;
        access_counts_[seed] += 1;
        current_frontier.push_back(seed);
    }
    
    while (!current_frontier.empty()) {
        next_frontier.clear();
        
        for (NodeId node : current_frontier) {
            graph_.for_each_neighbor(node, [&](NodeId neighbor) {
                if (active[neighbor] == 1) { return; }

                const Probability random_value = distribution(generator);

                if (random_value < configuration_.activation_probability) {
                    active[neighbor] = 1;
                    access_counts_[neighbor] += 1;
                    next_frontier.push_back(neighbor);
                }
            }
            );
        }
        current_frontier.swap(next_frontier);
    }
}

const std::vector<IndependentCascade::Probability> IndependentCascade::access_probabilities() const {
    return access_probs_;
}

}  // namespace influence
