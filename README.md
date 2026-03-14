# Graph Query Engine

A high-performance graph database engine built in C++17.

## Features
- ✅ Core graph structure with adjacency list
- ✅ BFS and Dijkstra traversal
- ✅ Interactive CLI
- 🚧 Parallel query execution with thread pool
- ⬜ Disk persistence with Write-Ahead Log

## Tech Stack
- C++17
- CMake
- GoogleTest
- Linux (Pop!_OS)

## Build
```bash
cmake --preset debug
cmake --build build/debug
```

## Run
```bash
./build/debug/gqe
```

## CLI Usage
```
Graph Query Engine v0.1
Commands: add-node <id>, add-edge <from> <to> <weight>, bfs <start>, dijkstra <start>, exit

gqe> add-node 1
gqe> add-node 2
gqe> add-edge 1 2 5.0
gqe> dijkstra 1
Dijkstra from 1:
  -> 1 : 0
  -> 2 : 5
gqe> exit
```

## Status
🚧 Under active development