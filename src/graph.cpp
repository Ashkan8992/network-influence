#include "influence/graph.hpp"

namespace influence {

Graph::Graph(std::size_t node_count)
    : node_count_(node_count) {
}

std::size_t Graph::node_count() const noexcept {
    return node_count_;
}

std::size_t Graph::edge_count() const noexcept {
    return 0;
}

} // namespace influence
