#include "graph/parser.hpp"
#include <sstream>

namespace gqe {

Command Parser::parse(const std::string& line) {
    auto tokens = tokenize(line);
    if (tokens.empty()) return {CommandType::UNKNOWN, {}};

    CommandType type = resolveCommand(tokens[0]);
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());
    return {type, args};
}

std::vector<std::string> Parser::tokenize(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

CommandType Parser::resolveCommand(const std::string& token) {
    if (token == "add-node")  return CommandType::ADD_NODE;
    if (token == "add-edge")  return CommandType::ADD_EDGE;
    if (token == "bfs")       return CommandType::BFS;
    if (token == "dijkstra")  return CommandType::DIJKSTRA;
    if (token == "save")      return CommandType::SAVE;
    if (token == "load")      return CommandType::LOAD;
    if (token == "exit")      return CommandType::EXIT;
    return CommandType::UNKNOWN;
}

} // namespace gqe