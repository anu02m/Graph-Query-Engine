#include <iostream>
#include <chrono>
#include <string>
#include "graph/graph.hpp"
#include "graph/algorithms.hpp"
#include "graph/thread_pool.hpp"

using namespace std;
using namespace gqe;

using Clock = chrono::high_resolution_clock;

// Builds a linear chain: 1->2->3->...->n
void buildLinearGraph(Graph &g, int n) {
    for (int i = 1; i <= n; i++) g.addNode(i);
    for (int i = 1; i < n; i++)  g.addEdge(i, i + 1, 1.0);
}

// Builds a dense graph where every node connects to next 10 nodes
void buildDenseGraph(Graph &g, int n) {
    for (int i = 1; i <= n; i++) g.addNode(i);
    for (int i = 1; i <= n; i++)
        for (int j = i + 1; j <= min(i + 10, n); j++)
            g.addEdge(i, j, 1.0);
}

template<typename Fn>
long long timeMs(Fn&& fn) {
    auto start = Clock::now();
    fn();
    auto end = Clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

void benchBFS(const string& label, Graph& g, NodeId start) {
    auto ms = timeMs([&] { bfs(g, start); });
    cout << "BFS  [" << label << "]: " << ms << " ms\n";
}

void benchDijkstra(const string& label, Graph& g, NodeId start) {
    auto ms = timeMs([&] { dijkstra(g, start); });
    cout << "Dijkstra [" << label << "]: " << ms << " ms\n";
}

void benchThreadPool(const string& label, int numThreads, int numTasks) {
    ThreadPool pool(numThreads);
    atomic<int> counter{0};

    auto ms = timeMs([&] {
        for (int i = 0; i < numTasks; i++)
            pool.enqueue([&counter] { counter++; });
        // wait for all tasks
        while (counter.load() < numTasks) {}
    });

    cout << "ThreadPool [" << label << "]: " << ms << " ms\n";
}

int main() {
    cout << "=== Graph Query Engine Benchmarks ===\n\n";

    // BFS benchmarks
    cout << "-- BFS --\n";
    for (int n : {1000, 10000, 100000}) {
        Graph g;
        buildLinearGraph(g, n);
        benchBFS("linear n=" + to_string(n), g, 1);
    }

    cout << "\n-- Dijkstra --\n";
    for (int n : {1000, 10000, 50000}) {
        Graph g;
        buildDenseGraph(g, n);
        benchDijkstra("dense n=" + to_string(n), g, 1);
    }

    cout << "\n-- Thread Pool --\n";
    benchThreadPool("2 threads, 1k tasks",  2, 1000);
    benchThreadPool("4 threads, 1k tasks",  4, 1000);
    benchThreadPool("8 threads, 1k tasks",  8, 1000);
    benchThreadPool("4 threads, 10k tasks", 4, 10000);

    return 0;
}