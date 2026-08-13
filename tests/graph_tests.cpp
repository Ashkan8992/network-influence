#include "influence/graph.hpp"

#include <gtest/gtest.h>

using influence::Graph;

// Test New Graph Creation Nodes and Edges
TEST(GraphTest, NewGraph) {
    Graph graph(5, Graph::Direction::Directed);
    
    EXPECT_EQ(graph.node_count(), 5);
    EXPECT_EQ(graph.edge_count(), 0);
}

// Test Graph Direction
TEST(GraphTest, GraphDirection) {
    Graph graph(5, Graph::Direction::Directed);
    
    EXPECT_TRUE(graph.is_directed());
}

// Test Graph Directed Edges
TEST(GraphTest, GraphDirectedEdge) {
    Graph graph(3, Graph::Direction::Directed);
    
    graph.add_edge(0, 1);
    
    const auto neighbors = graph.neighbors(0);
    
    ASSERT_EQ(neighbors.size(), 1);
    EXPECT_EQ(neighbors[0], influence::NodeId{1});
    
    EXPECT_TRUE(graph.neighbors(1).empty());
}

// Test Graph Undirected Edges
TEST(GraphTest, GraphUndirectedEdge) {
    Graph graph(3, Graph::Direction::Undirected);
    
    graph.add_edge(0, 1);
    
    const auto neighbors = graph.neighbors(1);
    
    ASSERT_EQ(neighbors.size(), 1);
    EXPECT_EQ(neighbors[0], influence::NodeId{0});
}

// Test Graph Edge Count
TEST(GraphTest, GraphEdgeCount) {
    Graph graph(3, Graph::Direction::Directed);
    
    graph.add_edge(0, 1);
    
    EXPECT_EQ(graph.edge_count(), 1);
}

// Test Graph Duplicate Edges
TEST(GraphTest, GraphDuplicateEdge) {
    Graph graph(3, Graph::Direction::Directed);
    
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    
    EXPECT_EQ(graph.edge_count(), 1);
    
    const auto neighbors = graph.neighbors(0);
    ASSERT_EQ(neighbors.size(), 1);
}
