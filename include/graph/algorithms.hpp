#pragma once
#include "graph/graph.hpp"
#include <vector>
#include <unordered_map>
#include <limits>

namespace gqe {

// Returns nodes in BFS visit order starting from `start`
std::vector<NodeId> bfs(const Graph& g, NodeId start);

// Returns shortest distances from `start` to all reachable nodes
// Unreachable nodes are absent from the map
std::unordered_map<NodeId, double> dijkstra(const Graph& g, NodeId start);

} // namespace gqe