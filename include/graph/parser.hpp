#pragma once
#include <string>
#include <vector>
#include "graph/graph.hpp"

namespace gqe {

enum class CommandType {
    ADD_NODE,
    ADD_EDGE,
    BFS,
    DIJKSTRA,
    EXIT,
    UNKNOWN
};

struct Command {
    CommandType type;
    std::vector<std::string> args;
};

class Parser {
public:
    Command parse(const std::string& line);

private:
    std::vector<std::string> tokenize(const std::string& line);
    CommandType resolveCommand(const std::string& token);
};

} // namespace gqe