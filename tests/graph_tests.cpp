#include "influence/graph.hpp"

#include <gtest/gtest.h>

TEST(GraphTest, NewGraph) {
    influence::Graph graph(5);
    
    EXPECT_EQ(graph.node_count(), 5);
    EXPECT_EQ(graph.edge_count(), 0);
}
