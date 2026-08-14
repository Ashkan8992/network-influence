#include "influence/graph.hpp"

#include <algorithm>
#include <stdexcept>

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

bool Graph::edge_exist(NodeId from, NodeId to) const {
    validate_node(from);
    validate_node(to);
    
    const auto& neighbors = adjacency_[from];
    
    if (std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end()) {
        return true;
    } else { return false; }
}

void Graph::add_edge(NodeId from, NodeId to) {
    if (from == to) { throw std::invalid_argument("Self-loops are not allowed"); }
    
    if (edge_exist(from, to)) { return; }
    
    adjacency_[from].push_back(to);
    
    if (direction_ == Direction::Undirected) {
        adjacency_[to].push_back(from);
    }
    
    ++edge_count_;
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
