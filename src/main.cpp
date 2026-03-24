#include <iostream>
#include <string>
#include "graph/graph.hpp"
#include "graph/algorithms.hpp"
#include "graph/wal.hpp"
#include "graph/parser.hpp"
#include "graph/snapshot.hpp"

using namespace std;
using namespace gqe;

int main() {
    Graph g;
    WAL wal("gqe.log");
    Parser parser;

    wal.replay(g);

    cout << "Graph Query Engine v0.1\n";
    cout << "Commands: add-node <id>, add-edge <from> <to> <weight>, bfs <start>,\n";
    cout << "          dijkstra <start>, save <file>, load <file>, exit\n\n";

    string line;
    while (true) {
        cout << "gqe> ";
        if (!getline(cin, line)) break;

        Command cmd = parser.parse(line);

        switch (cmd.type) {

        case CommandType::EXIT:
            return 0;

        case CommandType::ADD_NODE: {
            if (cmd.args.empty()) { cout << "Usage: add-node <id>\n"; break; }
            NodeId id = stoi(cmd.args[0]);
            wal.logAddNode(id);
            g.addNode(id);
            cout << "Node " << id << " added.\n";
            break;
        }

        case CommandType::ADD_EDGE: {
            if (cmd.args.size() < 2) { cout << "Usage: add-edge <from> <to> <weight>\n"; break; }
            NodeId from = stoi(cmd.args[0]);
            NodeId to   = stoi(cmd.args[1]);
            double weight = cmd.args.size() >= 3 ? stod(cmd.args[2]) : 1.0;
            try {
                wal.logAddEdge(from, to, weight);
                g.addEdge(from, to, weight);
                cout << "Edge " << from << " -> " << to << " (weight " << weight << ") added.\n";
            } catch (const invalid_argument& e) {
                cout << "Error: " << e.what() << "\n";
            }
            break;
        }

        case CommandType::BFS: {
            if (cmd.args.empty()) { cout << "Usage: bfs <start>\n"; break; }
            NodeId start = stoi(cmd.args[0]);
            auto result = bfs(g, start);
            if (result.empty()) { cout << "No nodes reachable.\n"; break; }
            cout << "BFS: ";
            for (NodeId n : result) cout << n << " ";
            cout << "\n";
            break;
        }

        case CommandType::DIJKSTRA: {
            if (cmd.args.empty()) { cout << "Usage: dijkstra <start>\n"; break; }
            NodeId start = stoi(cmd.args[0]);
            auto dist = dijkstra(g, start);
            if (dist.empty()) { cout << "No paths found.\n"; break; }
            cout << "Dijkstra from " << start << ":\n";
            for (auto& [node, d] : dist)
                cout << "  -> " << node << " : " << d << "\n";
            break;
        }

        case CommandType::SAVE: {
            if (cmd.args.empty()) { cout << "Usage: save <file>\n"; break; }
            try {
                Snapshot::save(g, cmd.args[0]);
                cout << "Graph saved to " << cmd.args[0] << "\n";
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
            break;
        }

        case CommandType::LOAD: {
            if (cmd.args.empty()) { cout << "Usage: load <file>\n"; break; }
            try {
                Snapshot::load(g, cmd.args[0]);
                cout << "Graph loaded from " << cmd.args[0] << "\n";
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
            break;
        }

        case CommandType::UNKNOWN:
        default:
            cout << "Unknown command. Try: add-node, add-edge, bfs, dijkstra, save, load, exit\n";
            break;
        }
    }

    return 0;
}