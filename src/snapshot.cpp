#include "graph/snapshot.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace gqe {

void Snapshot::save(const Graph& g, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open snapshot file: " + path);

    for (const auto& [nodeId, edges] : g.adjacencyList()) {
        file << "NODE " << nodeId << "\n";
        for (const auto& e : edges)
            file << "EDGE " << nodeId << " " << e.to << " " << e.weight << "\n";
    }
}

void Snapshot::load(Graph& g, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open snapshot file: " + path);

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "NODE") {
            NodeId id;
            iss >> id;
            if (!g.hasNode(id))
                g.addNode(id);

        } else if (cmd == "EDGE") {
            NodeId from, to;
            double weight;
            iss >> from >> to >> weight;
            if (g.hasNode(from) && g.hasNode(to))
                g.addEdge(from, to, weight);
        }
    }
}

} // namespace gqe