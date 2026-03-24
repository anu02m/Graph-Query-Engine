#include "graph/wal.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace gqe {

WAL::WAL(const std::string& logPath) : logPath_(logPath) {
    logFile_.open(logPath, std::ios::app);
    if (!logFile_.is_open())
        throw std::runtime_error("Failed to open WAL file: " + logPath);
}

WAL::~WAL() {
    if (logFile_.is_open())
        logFile_.close();
}

void WAL::logAddNode(NodeId id) {
    logFile_ << "ADD_NODE " << id << "\n";
    flush();
}

void WAL::logAddEdge(NodeId from, NodeId to, double weight) {
    logFile_ << "ADD_EDGE " << from << " " << to << " " << weight << "\n";
    flush();
}

void WAL::flush() {
    logFile_.flush();
}

void WAL::replay(Graph& g) {
    std::ifstream file(logPath_);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "ADD_NODE") {
            NodeId id;
            iss >> id;
            if (!g.hasNode(id))
                g.addNode(id);

        } else if (cmd == "ADD_EDGE") {
            NodeId from, to;
            double weight;
            iss >> from >> to >> weight;
            if (g.hasNode(from) && g.hasNode(to))
                g.addEdge(from, to, weight);
        }
    }
}

} // namespace gqe