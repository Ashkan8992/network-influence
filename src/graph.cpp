#include "influence/graph.hpp"

namespace influence {

Graph::Graph(std::size_t node_count, Direction direction)
    : node_count_(node_count), direction_(direction) {
}

std::size_t Graph::node_count() const noexcept {
    return node_count_;
}

std::size_t Graph::edge_count() const noexcept {
    return 0;
}

bool Graph::is_directed() const noexcept {
    return direction_ == Direction::Directed;
}

} // namespace influence
