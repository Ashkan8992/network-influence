#include "influence/independent_cascade.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

namespace influence {

IndependentCascade::IndependentCascade(const Graph& graph, Configuration configuration, std::vector<NodeId> seeds) : graph_(graph), configuration_(configuration), seeds_(seeds), access_counts_(graph_.node_count(), 0), access_probs_(graph_.node_count(), 0.0), generator_(configuration.random_seed == 0 ? std::random_device{}() : configuration.random_seed) {

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
}

void IndependentCascade::cascade() {
    const std::size_t node_count = graph_.node_count();
    
    /* Reproducibility Purposes
    0 means: generate a seed automatically.
     non-zero seed makes the experiment reproducible. */
    /* std::uint64_t random_seed = configuration_.random_seed;
    if (random_seed == 0) {
        std::random_device random_device;
        random_seed = random_device();
    }
    std::mt19937_64 generator(random_seed); TODO: put back for random (not RNG Persistence) simulation */

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    
    // Nodes activated during the current propagation step.
    std::vector<NodeId> current_frontier;
    // Nodes activated during the next propagation step.
    std::vector<NodeId> next_frontier;
    
    current_frontier.reserve(seeds_.size());
    
    //  Tracks whether a node has already been activated during this cascade.
    std::vector<uint8_t> active(node_count, 0);
    
    // Activate the initial seeds.
    for (NodeId seed : seeds_) {
        if (active[seed] == 1) { continue; }
        active[seed] = 1;
        ++access_counts_[seed];
        current_frontier.push_back(seed);
    }
    
    // Independent Cascade propagation.
    while (!current_frontier.empty()) {
        next_frontier.clear();
        
        for (NodeId node : current_frontier) {
            graph_.for_each_neighbor(node, [&](NodeId neighbor) {
                if (active[neighbor] == 1) { return; }

                const Probability random_value = distribution(generator_);

                if (random_value < configuration_.activation_probability) {
                    active[neighbor] = 1;
                    ++access_counts_[neighbor];
                    next_frontier.push_back(neighbor);
                }
            }
            );
        }
        current_frontier.swap(next_frontier);
    }
}

void IndependentCascade::run() {
    // TODO: Clean? Figure out if you want to run() multiple times or just create a new simulation? In that case call run() in the constructor?
    // TODO: might be dangerous for multi-threading
    // Reset results so run() represents one complete Monte Carlo experiment.
    std::fill(access_counts_.begin(), access_counts_.end(), 0);
    std::fill(access_probs_.begin(), access_probs_.end(), 0);
    
    for (size_t simulation = 0; simulation < configuration_.simulations; ++simulation) {
        cascade();
    }

    // Convert accumulated activation counts into access probabilities.
    const Probability simulation_count = static_cast<Probability>(configuration_.simulations);
    for (std::size_t node = 0; node < access_counts_.size(); ++node) {
        access_probs_[node] = static_cast<Probability>(access_counts_[node]) / simulation_count;
    }
}

const std::vector<IndependentCascade::Probability>& IndependentCascade::access_probabilities() const {
    return access_probs_;
}

}  // namespace influence
