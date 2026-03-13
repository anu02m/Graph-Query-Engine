#include <gtest/gtest.h>
#include "graph/graph.hpp"

using namespace gqe;

TEST(GraphTest, AddNodeAndCheck) {
    Graph g;
    g.addNode(1);
    EXPECT_TRUE(g.hasNode(1));
    EXPECT_FALSE(g.hasNode(99));
}

TEST(GraphTest, AddEdgeAndNeighbors) {
    Graph g;
    g.addNode(1); g.addNode(2);
    g.addEdge(1, 2, 3.5);
    const auto& nb = g.neighbors(1);
    EXPECT_EQ(nb.size(), 1);
    EXPECT_EQ(nb[0].to, 2);
    EXPECT_DOUBLE_EQ(nb[0].weight, 3.5);
}

TEST(GraphTest, EdgeToMissingNodeThrows) {
    Graph g;
    g.addNode(1);
    EXPECT_THROW(g.addEdge(1, 99, 1.0), std::invalid_argument);
}