#include "graph/algorithms.hpp"
#include <queue>
#include <unordered_set>

namespace gqe {

std::vector<NodeId> bfs(const Graph& g, NodeId start) {
    if (!g.hasNode(start)) return {};

    std::vector<NodeId> visited;
    std::unordered_set<NodeId> seen;
    std::queue<NodeId> q;

    q.push(start);
    seen.insert(start);

    while (!q.empty()) {
        NodeId curr = q.front(); q.pop();
        visited.push_back(curr);

        for (const Edge& e : g.neighbors(curr)) {
            if (!seen.count(e.to)) {
                seen.insert(e.to);
                q.push(e.to);
            }
        }
    }
    return visited;
}

std::unordered_map<NodeId, double> dijkstra(const Graph& g, NodeId start) {
    if (!g.hasNode(start)) return {};

    using P = std::pair<double, NodeId>; // {distance, node}
    std::priority_queue<P, std::vector<P>, std::greater<P>> minHeap;
    std::unordered_map<NodeId, double> dist;

    dist[start] = 0.0;
    minHeap.push({0.0, start});

    while (!minHeap.empty()) {
        auto [d, u] = minHeap.top(); minHeap.pop();

        if (d > dist[u]) continue; // stale entry, skip

        for (const Edge& e : g.neighbors(u)) {
            double newDist = dist[u] + e.weight;
            if (!dist.count(e.to) || newDist < dist[e.to]) {
                dist[e.to] = newDist;
                minHeap.push({newDist, e.to});
            }
        }
    }
    return dist;
}

} // namespace gqe