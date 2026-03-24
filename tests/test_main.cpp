#include <gtest/gtest.h>
#include "graph/graph.hpp"
#include "graph/algorithms.hpp"
#include <atomic>
#include <chrono>
#include <thread>
#include "graph/thread_pool.hpp"
using namespace gqe;

// ---- Graph tests ----

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

// ---- BFS tests ----

TEST(BFSTest, VisitOrder) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addNode(i);
    g.addEdge(1, 2); g.addEdge(1, 3); g.addEdge(2, 4);

    auto result = bfs(g, 1);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result.size(), 4);
}

TEST(BFSTest, InvalidStartReturnsEmpty) {
    Graph g;
    EXPECT_TRUE(bfs(g, 99).empty());
}

// ---- Dijkstra tests ----

TEST(DijkstraTest, ShortestPaths) {
    Graph g;
    for (int i = 1; i <= 4; i++) g.addNode(i);
    g.addEdge(1, 2, 1.0);
    g.addEdge(1, 3, 4.0);
    g.addEdge(2, 3, 2.0);
    g.addEdge(2, 4, 6.0);
    g.addEdge(3, 4, 1.0);

    auto dist = dijkstra(g, 1);
    EXPECT_DOUBLE_EQ(dist[1], 0.0);
    EXPECT_DOUBLE_EQ(dist[2], 1.0);
    EXPECT_DOUBLE_EQ(dist[3], 3.0); // 1->2->3 cheaper than 1->3
    EXPECT_DOUBLE_EQ(dist[4], 4.0); // 1->2->3->4
}

TEST(DijkstraTest, UnreachableNodeAbsent) {
    Graph g;
    g.addNode(1); g.addNode(2); // no edge between them
    auto dist = dijkstra(g, 1);
    EXPECT_EQ(dist.count(2), 0);
}

TEST(ThreadPoolTest, TasksExecute) {
    ThreadPool pool(2);
    std::atomic<int> counter{0};

    for (int i = 0; i < 10; i++)
        pool.enqueue([&counter] { counter++; });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(counter.load(), 10);
}

TEST(ThreadPoolTest, MultipleThreadsWork) {
    ThreadPool pool(4);
    std::atomic<int> counter{0};

    for (int i = 0; i < 100; i++)
        pool.enqueue([&counter] { counter++; });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_EQ(counter.load(), 100);
}