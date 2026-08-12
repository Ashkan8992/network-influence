#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace influence {

using NodeId = std::uint32_t;

class Graph {
public:
    enum class Direction {
        Directed,
        Undirected
    };
    
    explicit Graph(std::size_t node_count, Direction direction);
    
    std::size_t node_count() const noexcept;
    std::size_t edge_count() const noexcept;
    
    bool is_directed() const noexcept;
    
    void add_edge(NodeId from, NodeId to);
    
    std::vector<NodeId> neighbors(NodeId node) const;
    
private:
    std::size_t node_count_;
    std::size_t edge_count_{0};
    Direction direction_;
    std::vector<std::vector<NodeId>> adjacency_;
};

} // namespace influence
