#pragma once
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace gqe {

using NodeId = int;

struct Edge {
    NodeId to;
    double weight;
};

class Graph {
public:
    void addNode(NodeId id);
    void addEdge(NodeId from, NodeId to, double weight = 1.0);

    const std::vector<Edge>& neighbors(NodeId id) const;
    bool hasNode(NodeId id) const;
    const std::unordered_map<NodeId, std::vector<Edge>>& adjacencyList() const;

private:
    std::unordered_map<NodeId, std::vector<Edge>> adjList_;
};

} // namespace gqe