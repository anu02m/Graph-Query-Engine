#pragma once
#include <string>
#include <fstream>
#include "graph/graph.hpp"

namespace gqe {

class WAL {
public:
    explicit WAL(const std::string& logPath);
    ~WAL();

    void logAddNode(NodeId id);
    void logAddEdge(NodeId from, NodeId to, double weight);
    void replay(Graph& g);

private:
    std::ofstream logFile_;
    std::string logPath_;

    void flush();
};

} // namespace gqe