#include "graph/graph.hpp"
#include <mutex>

namespace gqe {

// --- internal (no locking) ---

bool Graph::hasNodeInternal(NodeId id) const {
    return adjList_.count(id) > 0;
}

const std::vector<Edge>& Graph::neighborsInternal(NodeId id) const {
    auto it = adjList_.find(id);
    if (it == adjList_.end())
        throw std::invalid_argument("Node not found");
    return it->second;
}

// --- public (with locking) ---

void Graph::addNode(NodeId id) {
    std::unique_lock lock(mutex_);
    adjList_.emplace(id, std::vector<Edge>{});
}

void Graph::addEdge(NodeId from, NodeId to, double weight) {
    std::unique_lock lock(mutex_);
    if (!hasNodeInternal(from) || !hasNodeInternal(to))
        throw std::invalid_argument("Node does not exist");
    adjList_[from].push_back({to, weight});
}

const std::vector<Edge>& Graph::neighbors(NodeId id) const {
    std::shared_lock lock(mutex_);
    return neighborsInternal(id);
}

bool Graph::hasNode(NodeId id) const {
    std::shared_lock lock(mutex_);
    return hasNodeInternal(id);
}

const std::unordered_map<NodeId, std::vector<Edge>>& Graph::adjacencyList() const {
    std::shared_lock lock(mutex_);
    return adjList_;
}

} // namespace gqe