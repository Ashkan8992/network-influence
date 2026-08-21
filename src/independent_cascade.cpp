#include "influence/independent_cascade.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>
#include <thread>
#include <vector>

namespace influence {

IndependentCascade::IndependentCascade(const Graph& graph, Configuration configuration, std::vector<NodeId> seeds) : graph_(graph), configuration_(configuration), seeds_(seeds), access_counts_(graph_.node_count(), 0), access_probs_(graph_.node_count(), 0.0) {

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
    ++state.metrics_.cascades;
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
    std::fill(state.active_.begin(), state.active_.end(), 0); // TODO: optimize like the line below
    
    // Nodes activated during the current propagation step.
    state.current_frontier_.clear();
    
    // Nodes activated during the next propagation step.
    state.next_frontier_.clear();
    
    // Activate the initial seeds.
    for (NodeId seed : seeds_) {
        if (state.active_[seed] == 1) { continue; }
        state.active_[seed] = 1;
        ++state.partial_access_counts_[seed];
        state.current_frontier_.push_back(seed);
        #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
        ++state.metrics_.activated_nodes;
        #endif
    }
    
    // Independent Cascade propagation.
    while (!state.current_frontier_.empty()) {
        state.next_frontier_.clear();
        
        for (NodeId node : state.current_frontier_) {
            graph_.for_each_neighbor(node, [&](NodeId neighbor) {
            #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
            ++state.metrics_.neighbor_examinations;
            #endif
                if (state.active_[neighbor] == 1) { return; }

                const Probability random_value = distribution(state.generator_);

                if (random_value < configuration_.activation_probability) {
                    #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
                    ++state.metrics_.activation_successes;
                    ++state.metrics_.activated_nodes;
                    #endif
                    state.active_[neighbor] = 1;
                    ++state.partial_access_counts_[neighbor];
                    state.next_frontier_.push_back(neighbor);
                } else {
                    #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
                    ++state.metrics_.activation_failures;
                    #endif
                }
            }
            );
        }
        state.current_frontier_.swap(state.next_frontier_);
    }
}

void IndependentCascade::run_worker(WorkerState& state){
    for (size_t simulation = 0; simulation < state.simulations_; ++simulation) {
        state.generator_.seed(cascade_seed(simulation)); // TODO: Commit 33
        cascade(state);
    }
}

void IndependentCascade::run() {
    std::vector<WorkerState> workers(configuration_.worker_count);
    std::vector<std::jthread> threads;
    threads.reserve(configuration_.worker_count);
    
    // Initialize worker state
    for (auto& worker : workers) {
        // state.generator_.seed(configuration_.random_seed);
        worker.active_.assign(graph_.node_count(), 0);
        worker.current_frontier_.reserve(graph_.node_count());
        worker.next_frontier_.reserve(graph_.node_count());
        worker.partial_access_counts_.assign(graph_.node_count(), 0);
    }
    
    // Divide the work evenly
    const std::size_t base = configuration_.simulations / configuration_.worker_count;
    const std::size_t remainder = configuration_.simulations % configuration_.worker_count;

    for (std::size_t worker_index = 0; worker_index < configuration_.worker_count; ++worker_index) {
        workers[worker_index].simulations_ = base + (worker_index < remainder ? 1 : 0);

        threads.emplace_back(
            [this, &worker = workers[worker_index]] {
                run_worker(worker);
            }
        );
    }
    
    threads.clear();
    
    // Reset results so run() represents one complete Monte Carlo experiment.
    std::fill(access_counts_.begin(), access_counts_.end(), 0);
    std::fill(access_probs_.begin(), access_probs_.end(), 0);
    
    for (const auto& worker : workers) {
        for (std::size_t node = 0; node < graph_.node_count(); ++node) {
            access_counts_[node] += worker.partial_access_counts_[node];
        }
    }

    // Convert accumulated activation counts into access probabilities.
    const Probability simulation_count = static_cast<Probability>(configuration_.simulations);
    for (std::size_t node = 0; node < access_counts_.size(); ++node) {
        access_probs_[node] = static_cast<Probability>(access_counts_[node]) / simulation_count;
    }
    
    #ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
    metrics_ = {};
    for (const auto& worker : workers) {
        metrics_.cascades += worker.metrics_.cascades;
        metrics_.activated_nodes += worker.metrics_.activated_nodes;
        metrics_.neighbor_examinations += worker.metrics_.neighbor_examinations;
        metrics_.activation_successes += worker.metrics_.activation_successes;
        metrics_.activation_failures += worker.metrics_.activation_failures;
    }

    #endif
}

const std::vector<IndependentCascade::Probability>& IndependentCascade::access_probabilities() const {
    return access_probs_;
}

std::uint64_t IndependentCascade::cascade_seed(std::size_t simulation_index) const {
    std::uint64_t value =
        configuration_.random_seed +
        static_cast<std::uint64_t>(simulation_index);

    value ^= value >> 30;
    value *= 0xbf58476d1ce4e5b9ULL;
    value ^= value >> 27;
    value *= 0x94d049bb133111ebULL;
    value ^= value >> 31;

    return value;
}

#ifdef INFLUENCE_ENABLE_SIMULATION_METRICS
const IndependentCascade::Metrics& IndependentCascade::metrics() const {
    return metrics_;
}
#endif

}  // namespace influence
