#pragma once       // over #ifndef

#include <cstddef> // size_t
#include <cstdint> // uint32_t
#include <vector>  // vector

namespace influence {

using NodeId = std::uint32_t; // TODO: limit on network size?

class Graph {
public:
    enum class Direction { // TODO: to Dir Undir?
        Directed,
        Undirected
    };
    
    explicit Graph(std::size_t node_count, Direction direction);
    
    std::size_t node_count() const noexcept;
    std::size_t edge_count() const noexcept;
    
    bool is_directed() const noexcept;
    
    void validate_node(NodeId node) const;
    bool edge_exist(NodeId from, NodeId to) const;
    void add_edge(NodeId from, NodeId to);
    
    std::size_t degree(NodeId node) const;
    std::vector<NodeId> neighbors(NodeId node) const;
    template <typename Function> // TODO: ...
    void for_each_neighbor(NodeId node, Function&& function) const;
    
private:
    std::size_t node_count_;
    std::size_t edge_count_{0};
    Direction direction_;
    std::vector<std::vector<NodeId>> adjacency_;
};

template <typename Function> // TODO: ...
void Graph::for_each_neighbor(
    NodeId node,
    Function&& function
) const {
    validate_node(node);

    for (NodeId neighbor : adjacency_[node]) {
        function(neighbor);
    }
}

} // namespace influence
