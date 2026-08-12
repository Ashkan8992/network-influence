#pragma once

#include <cstddef>

namespace influence {

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
    
private:
    std::size_t node_count_;
    Direction direction_;
};

} // namespace influence
