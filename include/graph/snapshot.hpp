#pragma once
#include <string>
#include "graph/graph.hpp"

namespace gqe {

class Snapshot {
public:
    static void save(const Graph& g, const std::string& path);
    static void load(Graph& g, const std::string& path);
};

} // namespace gqe