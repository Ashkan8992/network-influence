#include "influence/graph.hpp"

#include <algorithm> // find
#include <random>    // random edge
#include <stdexcept> // throw

namespace influence {

Graph::Graph(std::size_t node_count, Direction direction)
    : node_count_(node_count), direction_(direction), adjacency_(node_count) {
}

std::size_t Graph::node_count() const noexcept {
    return node_count_;
}

std::size_t Graph::edge_count() const noexcept {
    return edge_count_;
}

bool Graph::is_directed() const noexcept {
    return direction_ == Direction::Directed;
}

void Graph::validate_node(NodeId node) const {
    if (static_cast<std::size_t>(node) >= node_count()) {
        throw std::out_of_range("Graph node ID is out of range");
    }
}

bool Graph::edge_exists(NodeId from, NodeId to) const {
    validate_node(from);
    validate_node(to);
    
    const auto& neighbors = adjacency_[from];
    
    if (std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end()) {
        return true;
    } else { return false; }
}

void Graph::add_edge(NodeId from, NodeId to) {
    if (from == to) { throw std::invalid_argument("Self-loops are not allowed"); }
    
    if (edge_exists(from, to)) { return; }
    
    adjacency_[from].push_back(to);
    
    if (direction_ == Direction::Undirected) {
        adjacency_[to].push_back(from);
    }
    
    ++edge_count_;
}

void Graph::add_random_edge(std::mt19937_64& generator) {
    if (node_count() < 2) { throw std::invalid_argument("cannot add a random edge to a graph with fewer than two nodes"); }

    std::uniform_int_distribution<NodeId> distribution(0, static_cast<NodeId>(node_count() - 1));

    // TODO: prevent it from running infinitly or for a long time.
    // TODO: e.g. add condition if graph is full then throw invalid_argument
    while (true) {
        const NodeId from = distribution(generator);
        const NodeId to = distribution(generator);

        if (from == to) { continue; }

        if (edge_exists(from, to)) { continue;}

        add_edge(from, to);
        return;
    }
}

std::vector<NodeId> Graph::neighbors(NodeId node) const {
    validate_node(node);
    return adjacency_[node];
}

std::size_t Graph::degree(NodeId node) const {
    validate_node(node);
    return adjacency_[node].size();
}

} // namespace influence
