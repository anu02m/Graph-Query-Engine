#include <iostream>
#include <sstream>
#include <string>
#include "graph/graph.hpp"
#include "graph/algorithms.hpp"

using namespace std;
using namespace gqe;

int main() {
    Graph g;
    string line;

    cout << "Graph Query Engine v0.1\n";
    cout << "Commands: add-node <id>, add-edge <from> <to> <weight>, bfs <start>, dijkstra <start>, exit\n\n";

    while (true) {
        cout << "gqe> ";
        if (!getline(cin, line)) break;

        istringstream iss(line);
        string cmd;
        iss >> cmd;

        if (cmd == "exit") {
            break;

        } else if (cmd == "add-node") {
            NodeId id;
            iss >> id;
            g.addNode(id);
            cout << "Node " << id << " added.\n";

        } else if (cmd == "add-edge") {
            NodeId from, to;
            double weight = 1.0;
            iss >> from >> to >> weight;
            try {
                g.addEdge(from, to, weight);
                cout << "Edge " << from << " -> " << to << " (weight " << weight << ") added.\n";
            } catch (const invalid_argument& e) {
                cout << "Error: " << e.what() << "\n";
            }

        } else if (cmd == "bfs") {
            NodeId start;
            iss >> start;
            auto result = bfs(g, start);
            if (result.empty()) {
                cout << "No nodes reachable (invalid start?)\n";
            } else {
                cout << "BFS: ";
                for (NodeId n : result) cout << n << " ";
                cout << "\n";
            }

        } else if (cmd == "dijkstra") {
            NodeId start;
            iss >> start;
            auto dist = dijkstra(g, start);
            if (dist.empty()) {
                cout << "No paths found (invalid start?)\n";
            } else {
                cout << "Dijkstra from " << start << ":\n";
                for (auto& [node, d] : dist)
                    cout << "  -> " << node << " : " << d << "\n";
            }

        } else {
            cout << "Unknown command. Try: add-node, add-edge, bfs, dijkstra, exit\n";
        }
    }

    return 0;
}