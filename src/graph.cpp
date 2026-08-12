#include "influence/graph.hpp"

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

void Graph::add_edge(NodeId from, NodeId to) {
    adjacency_[from].push_back(to);
    ++edge_count_;
}

std::vector<NodeId> Graph::neighbors(NodeId node) const {
    return adjacency_[node];
}

} // namespace influence
