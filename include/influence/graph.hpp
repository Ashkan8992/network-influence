#pragma once

#include <cstddef>

namespace influence {

class Graph {
public:
    explicit Graph(std::size_t node_count);
    
    std::size_t node_count() const noexcept;
    std::size_t edge_count() const noexcept;
    
private:
    std::size_t node_count_;
};

} // namespace influence
