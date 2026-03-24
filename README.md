# Graph Query Engine

A high-performance graph database engine built in C++17 from scratch, featuring traversal algorithms, persistence, and parallel execution.

## Features

- ✅ Core graph structure with weighted directed adjacency list
- ✅ BFS and Dijkstra traversal algorithms
- ✅ Interactive CLI with token-based query parser
- ✅ Parallel task execution with thread pool
- ✅ Crash recovery with Write-Ahead Log (WAL)
- ✅ Graph persistence with snapshot save/load
- ✅ Benchmarking suite (BFS, Dijkstra, thread pool)
- ⬜ Thread-safe graph with reader-writer locks

## Tech Stack

- C++17
- CMake 3.20+
- GoogleTest v1.14.0
- Linux (Pop!_OS)

## Project Structure
```
graph-query-engine/
├── src/               # Engine source files
├── include/graph/     # Public headers
├── tests/             # GoogleTest suite
├── benchmarks/        # Performance benchmarks
└── third_party/       # GoogleTest (via FetchContent)
```

## Build
```bash
# Debug build
cmake --preset debug
cmake --build build/debug

# Release build (optimized)
cmake --preset release
cmake --build build/release
```

## Run
```bash
./build/debug/src/gqe
```

## CLI Usage
```
Graph Query Engine v0.1
Commands: add-node <id>, add-edge <from> <to> <weight>, bfs <start>,
          dijkstra <start>, save <file>, load <file>, exit

gqe> add-node 1
gqe> add-node 2
gqe> add-node 3
gqe> add-edge 1 2 1.0
gqe> add-edge 2 3 2.0
gqe> bfs 1
BFS: 1 2 3
gqe> dijkstra 1
Dijkstra from 1:
  -> 1 : 0
  -> 2 : 1
  -> 3 : 3
gqe> save mygraph.txt
Graph saved to mygraph.txt
gqe> load mygraph.txt
Graph loaded from mygraph.txt
gqe> exit
```

## Tests
```bash
cd build/debug && ./tests/gqe_tests
```

18 tests across 5 suites: Graph, BFS, Dijkstra, ThreadPool, WAL, Snapshot, Parser.

## Benchmarks
```bash
./build/debug/benchmarks/gqe_bench   # debug
./build/release/benchmarks/gqe_bench # release (recommended)
```

Sample results (release build):
```
BFS  [linear n=100000]: 4 ms
Dijkstra [dense n=50000]: 7 ms
ThreadPool [4 threads, 10k tasks]: 14 ms
```

## Status

🚧 Under active development — thread-safe graph with reader-writer locks coming next.