#include "graph/graph.hpp"

namespace gqe {

void Graph::addNode(NodeId id) {
    adjList_.emplace(id, std::vector<Edge>{});
}

void Graph::addEdge(NodeId from, NodeId to, double weight) {
    if (!hasNode(from) || !hasNode(to))
        throw std::invalid_argument("Node does not exist");
    adjList_[from].push_back({to, weight});
}

const std::vector<Edge>& Graph::neighbors(NodeId id) const {
    auto it = adjList_.find(id);
    if (it == adjList_.end())
        throw std::invalid_argument("Node not found");
    return it->second;
}

bool Graph::hasNode(NodeId id) const {
    return adjList_.count(id) > 0;
}

const std::unordered_map<NodeId, std::vector<Edge>>& Graph::adjacencyList() const {
    return adjList_;
}

} // namespace gqe