#include "influence/graph.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using influence::Graph;

// Test New Graph Creation Nodes and Edges
TEST(GraphTest, NewGraphAddEdge) {
    Graph graph(3, Graph::Direction::Undirected);
    
    EXPECT_EQ(graph.node_count(), 3);
    EXPECT_EQ(graph.edge_count(), 0);
}

// Test Directed Graph, Edges, and EdgeCount
TEST(GraphTest, GraphDirectedEdge) {
    Graph graph(3, Graph::Direction::Directed);
    
    EXPECT_TRUE(graph.is_directed());
    
    graph.add_edge(0, 1);
    EXPECT_EQ(graph.edge_count(), 1);
    
    const auto neighbors = graph.neighbors(0);
    ASSERT_EQ(neighbors.size(), 1);
    EXPECT_EQ(neighbors[0], influence::NodeId{1});
    
    EXPECT_TRUE(graph.neighbors(1).empty());
    
    graph.add_edge(1, 0);
    EXPECT_EQ(graph.edge_count(), 2);
    graph.add_edge(0, 2);
    EXPECT_EQ(graph.degree(0), 2);
    EXPECT_EQ(graph.degree(2), 0);
}

// Test Undirected Graph, Edges, and EdgeCount
TEST(GraphTest, GraphUndirectedEdge) {
    Graph graph(3, Graph::Direction::Undirected);
    
    EXPECT_FALSE(graph.is_directed());
    
    graph.add_edge(0, 1);
    EXPECT_EQ(graph.edge_count(), 1);
    
    const auto neighbors_0 = graph.neighbors(0);
    ASSERT_EQ(neighbors_0.size(), 1);
    EXPECT_EQ(neighbors_0[0], influence::NodeId{1});
    
    const auto neighbors_1 = graph.neighbors(1);
    ASSERT_EQ(neighbors_1.size(), 1);
    EXPECT_EQ(neighbors_1[0], influence::NodeId{0});
    
    graph.add_edge(1, 0);
    EXPECT_EQ(graph.edge_count(), 1);
    graph.add_edge(0, 2);
    EXPECT_EQ(graph.degree(0), 2);
}

// Test Graph Duplicate Edges
TEST(GraphTest, GraphDuplicateEdge) {
    Graph graph(3, Graph::Direction::Directed);
    
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    
    EXPECT_EQ(graph.edge_count(), 1);
    
    ASSERT_EQ(graph.neighbors(0).size(), 1);
}

// Test Graph Self Loop
TEST(GraphTest, GraphSelfLoop) {
    Graph graph(3, Graph::Direction::Directed);
    
    EXPECT_THROW(graph.add_edge(0, 0), std::invalid_argument);
}

// Test Graph Out of Range
TEST(GraphTest, GraphOutOfRangeNode) {
    Graph graph(3, Graph::Direction::Directed);
    
    EXPECT_THROW(graph.add_edge(0, 3), std::out_of_range);
    EXPECT_THROW(graph.add_edge(3, 0), std::out_of_range);
    EXPECT_THROW(graph.add_edge(0, -1), std::out_of_range);
    EXPECT_THROW(graph.neighbors(3), std::out_of_range);
    EXPECT_THROW(graph.degree(3), std::out_of_range);
}

// ForEachNeighbor Template Function
TEST(GraphTest, ForEachNeighbor) {
    Graph graph(3, Graph::Direction::Directed);

    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<influence::NodeId> visited;

    graph.for_each_neighbor(0, [&](influence::NodeId neighbor) {
        visited.push_back(neighbor);
    });

    EXPECT_EQ(visited.size(), 2);
    EXPECT_EQ(visited[0], 1);
    EXPECT_EQ(visited[1], 2);
    
    EXPECT_THROW(graph.for_each_neighbor(3, [](influence::NodeId) {}), std::out_of_range);
}

TEST(GraphTest, AddRandomEdgeIncreasesEdgeCount) {
    Graph graph(100, Graph::Direction::Directed);

    std::mt19937_64 generator(12345);

    graph.add_random_edge(generator);

    EXPECT_EQ(graph.edge_count(), 1);
}

TEST(GraphTest, RandomEdgesAreUnique) {
    Graph graph(100, Graph::Direction::Directed);

    std::mt19937_64 generator(12345);

    constexpr std::size_t edge_count = 1'000;

    for (std::size_t i = 0; i < edge_count; ++i) {
        graph.add_random_edge(generator);
    }

    EXPECT_EQ(graph.edge_count(), edge_count);
}

TEST(GraphTest, RandomEdgesAreNotSelfLoops) {
    Graph graph(100, Graph::Direction::Directed);

    std::mt19937_64 generator(12345);

    constexpr std::size_t edge_count = 1'000;

    for (std::size_t i = 0; i < edge_count; ++i) {
        graph.add_random_edge(generator);
    }

    /*
     * We can't inspect the edge list here unless your Graph API
     * exposes it. If edge_list() is already available, verify:
     *
     * EXPECT_NE(from, to);
     *
     * for every edge.
     */
}
