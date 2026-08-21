#include "influence/independent_cascade.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

namespace influence {

IndependentCascade::IndependentCascade(const Graph& graph, Configuration configuration, std::vector<NodeId> seeds) : graph_(graph), configuration_(configuration), seeds_(seeds), access_counts_(graph_.node_count(), 0), access_probs_(graph_.node_count(), 0.0), generator_(configuration.random_seed == 0 ? std::random_device{}() : configuration.random_seed), active_(graph.node_count(), 0) { // TODO: remove active_ initiation, and generator_ and access_count_?

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

void IndependentCascade::cascade(WorkerState& state) {
#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
    ++metrics_.cascades;
#endif
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
    
    //  Tracks whether a node has already been activated during this cascade.
    std::fill(state.active_.begin(), state.active_.end(), 0);
    // TODO: replace this later with something more optimize like the line below
    /* std::vector<uint8_t> active_(node_count, 0);
     // turned into private member -- moved to constructor */
    
    // Nodes activated during the current propagation step.
    state.current_frontier_.clear();
    /* std::vector<NodeId> current_frontier_;
     // turned into private member */
    
    // Nodes activated during the next propagation step.
    state.next_frontier_.clear();
    /* std::vector<NodeId> next_frontier_;
     // turned into private member */
    
    // state.current_frontier.reserve(seeds_.size());
    if (state.current_frontier_.capacity() < seeds_.size()) {
        state.current_frontier_.reserve(seeds_.size());
    }
    
    // Activate the initial seeds.
    for (NodeId seed : seeds_) {
        if (state.active_[seed] == 1) { continue; }
        state.active_[seed] = 1;
        ++state.access_counts_[seed];
        state.current_frontier_.push_back(seed);
#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
    ++metrics_.activated_nodes;
#endif
    }
    
    // Independent Cascade propagation.
    while (!state.current_frontier_.empty()) {
        state.next_frontier_.clear();
        
        for (NodeId node : state.current_frontier_) {
            graph_.for_each_neighbor(node, [&](NodeId neighbor) {
#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
        ++metrics_.neighbor_examinations;
#endif
                if (state.active_[neighbor] == 1) { return; }

                const Probability random_value = distribution(state.generator_);

                if (random_value < configuration_.activation_probability) {
#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
            ++metrics_.activation_successes;
            ++metrics_.activated_nodes;
#endif
                    state.active_[neighbor] = 1;
                    ++state.access_counts_[neighbor];
                    state.next_frontier_.push_back(neighbor);
                } else {
#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
            ++metrics_.activation_failures;
#endif
                }
            }
            );
        }
        state.current_frontier_.swap(state.next_frontier_);
    }
}

void IndependentCascade::run() {
    // TODO: Clean? Figure out if you want to run() multiple times or just create a new simulation? In that case call run() in the constructor?
    // TODO: might be dangerous for multi-threading
    // Reset results so run() represents one complete Monte Carlo experiment.
    std::fill(access_counts_.begin(), access_counts_.end(), 0);
    std::fill(access_probs_.begin(), access_probs_.end(), 0);
    
    WorkerState state;
    state.generator_.seed(configuration_.random_seed);
    state.active_.resize(graph_.node_count(), 0);
    state.current_frontier_.reserve(graph_.node_count());
    state.next_frontier_.reserve(graph_.node_count());
    state.access_counts_.assign(graph_.node_count(), 0);
    
    for (size_t simulation = 0; simulation < configuration_.simulations; ++simulation) {
        cascade(state);
    }
    
    access_counts_ = std::move(state.access_counts_);

    // Convert accumulated activation counts into access probabilities.
    const Probability simulation_count = static_cast<Probability>(configuration_.simulations);
    for (std::size_t node = 0; node < access_counts_.size(); ++node) {
        access_probs_[node] = static_cast<Probability>(access_counts_[node]) / simulation_count;
    }
}

const std::vector<IndependentCascade::Probability>& IndependentCascade::access_probabilities() const {
    return access_probs_;
}

#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS

const IndependentCascade::Metrics&
IndependentCascade::metrics() const {
    return metrics_;
}

#endif

}  // namespace influence
