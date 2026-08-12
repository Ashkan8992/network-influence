#include "influence/graph.hpp"

#include <gtest/gtest.h>

TEST(GraphTest, NewGraph) {
    influence::Graph graph(5, influence::Graph::Direction::Directed);
    
    EXPECT_EQ(graph.node_count(), 5);
    EXPECT_EQ(graph.edge_count(), 0);
}

TEST(GraphTest, GraphDirection) {
    influence::Graph graph(5, influence::Graph::Direction::Directed);
    
    EXPECT_TRUE(graph.is_directed());
}

TEST(GraphTest, GraphDirectedEdge) {
    influence::Graph graph(3, influence::Graph::Direction::Directed);
    
    graph.add_edge(0, 1);
    
    const auto neighbors = graph.neighbors(0);
    
    ASSERT_EQ(neighbors.size(), 1);
    EXPECT_EQ(neighbors[0], influence::NodeId{1});
    
    EXPECT_TRUE(graph.neighbors(1).empty());
}

TEST(GraphTest, GraphEdgeCount) {
    influence::Graph graph(3, influence::Graph::Direction::Directed);
    
    graph.add_edge(0, 1);
    
    EXPECT_EQ(graph.edge_count(), 1);
}
