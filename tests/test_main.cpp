#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <fstream>
#include "graph/graph.hpp"
#include "graph/algorithms.hpp"
#include "graph/thread_pool.hpp"
#include "graph/wal.hpp"
#include "graph/parser.hpp"
#include "graph/snapshot.hpp"
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

// ----- Thread Pool tests ------

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

// ----- WAL tests -----

TEST(WALTest, LogAndReplay) {
    const std::string path = "/tmp/test_wal.log";
    std::remove(path.c_str()); // clean slate

    {
        WAL wal(path);
        wal.logAddNode(1);
        wal.logAddNode(2);
        wal.logAddEdge(1, 2, 5.0);
    }

    Graph g;
    WAL wal2(path);
    wal2.replay(g);

    EXPECT_TRUE(g.hasNode(1));
    EXPECT_TRUE(g.hasNode(2));
    EXPECT_EQ(g.neighbors(1)[0].to, 2);
    EXPECT_DOUBLE_EQ(g.neighbors(1)[0].weight, 5.0);

    std::remove(path.c_str()); // cleanup
}

TEST(WALTest, ReplayOnMissingFileDoesNotCrash) {
    Graph g;
    WAL wal("/tmp/nonexistent_wal.log");
    EXPECT_NO_THROW(wal.replay(g));
    std::remove("/tmp/nonexistent_wal.log");
}

// ----- Parser tests -----


TEST(ParserTest, ParseAddNode) {
    Parser p;
    Command cmd = p.parse("add-node 5");
    EXPECT_EQ(cmd.type, CommandType::ADD_NODE);
    EXPECT_EQ(cmd.args[0], "5");
}

TEST(ParserTest, ParseAddEdge) {
    Parser p;
    Command cmd = p.parse("add-edge 1 2 3.5");
    EXPECT_EQ(cmd.type, CommandType::ADD_EDGE);
    EXPECT_EQ(cmd.args[0], "1");
    EXPECT_EQ(cmd.args[1], "2");
    EXPECT_EQ(cmd.args[2], "3.5");
}

TEST(ParserTest, ParseBFS) {
    Parser p;
    Command cmd = p.parse("bfs 1");
    EXPECT_EQ(cmd.type, CommandType::BFS);
    EXPECT_EQ(cmd.args[0], "1");
}

TEST(ParserTest, ParseUnknown) {
    Parser p;
    Command cmd = p.parse("foobar");
    EXPECT_EQ(cmd.type, CommandType::UNKNOWN);
}

TEST(ParserTest, ParseEmptyLine) {
    Parser p;
    Command cmd = p.parse("");
    EXPECT_EQ(cmd.type, CommandType::UNKNOWN);
}

// ----- Snapshot tests -----

TEST(SnapshotTest, SaveAndLoad) {
    const std::string path = "/tmp/test_snapshot.txt";

    Graph g1;
    g1.addNode(1); g1.addNode(2); g1.addNode(3);
    g1.addEdge(1, 2, 2.0);
    g1.addEdge(2, 3, 3.0);
    Snapshot::save(g1, path);

    Graph g2;
    Snapshot::load(g2, path);

    EXPECT_TRUE(g2.hasNode(1));
    EXPECT_TRUE(g2.hasNode(2));
    EXPECT_TRUE(g2.hasNode(3));
    EXPECT_EQ(g2.neighbors(1)[0].to, 2);
    EXPECT_DOUBLE_EQ(g2.neighbors(1)[0].weight, 2.0);
    EXPECT_EQ(g2.neighbors(2)[0].to, 3);

    std::remove(path.c_str());
}

TEST(SnapshotTest, LoadMissingFileThrows) {
    Graph g;
    EXPECT_THROW(Snapshot::load(g, "/tmp/nonexistent.txt"), std::runtime_error);
    std::remove("/tmp/nonexistent.txt");
}