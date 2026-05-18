# TOPTW + ALNS Solver

**Team Orienteering Problem with Time Windows** — C++17 heuristic solver with Adaptive Large Neighborhood Search.

## Project Structure

```
CPP/
├── toptw/
│   ├── problem.hpp              # Problem struct + random instance generator
│   ├── evaluate.hpp             # Route decoder: arrival time + feasibility
│   ├── construct.hpp            # GRASP greedy randomized construction
│   ├── local_search.hpp         # 2-opt / Relocate / Exchange operators
│   ├── solver.hpp               # Original multi-start solver
│   ├── alns_solver.hpp          # ALNS: 3 destroy + 3 repair + 3 meta-layers
│   └── instance_loader.hpp      # Benchmark instance file loader
├── benchmarks/
│   └── c_r_rc_100_100/          # 29 standard TOPTW instances (Righini & Salani 2006)
├── main.cpp                     # Entry point
├── CMakeLists.txt
├── REPORT.html                  # Full algorithm documentation (Chinese)
└── README.md
```

## Algorithm Overview

| Stage | Method |
|-------|--------|
| **Initial Construction** | GRASP + multi-start (10 restarts, α: 0→0.27) |
| **Local Search** | 2-opt → Relocate → Exchange (iterated until no improvement) |
| **ALNS Framework** | 3 metaheuristic layers |

### Destroy Operators
- **Random Removal** — randomly pick q nodes
- **Shaw Removal** — remove nodes most related to a random seed (distance + time window similarity)
- **Worst Removal** — remove nodes with worst score/cost ratio

### Repair Operators
- **Greedy Insertion** — best score/cost ratio first
- **Regret-k Insertion** — highest regret first (k=3)
- **Noise Insertion** — greedy with cost perturbation (±15%)

### Metaheuristic Layers
| Layer | Method | Role |
|-------|--------|------|
| 1 | **Simulated Annealing** | Accept/reject worse solutions |
| 2 | **Tabu Search** | Prevent re-inserting recently removed nodes |
| 3 | **Adaptive Weights** | Auto-select best operators via roulette wheel |

## Quick Start

### Linux
```bash
# Install dependencies
sudo apt install g++ cmake git    # Ubuntu/Debian
sudo dnf install gcc-c++ cmake git # Fedora

# Build & run
cd CPP
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/toptw                     # all 29 instances
```

### macOS
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/toptw
```

## Usage

```bash
./build/toptw                 # Full benchmark (29 instances)
```

## Benchmark Results (29 instances, 100 customers each)

| Type | Count | m | Tmax | Total Score | Achieved Range |
|------|-------|---|------|-------------|----------------|
| C (clustered) | 9 | 4 | 1236 | 9000 | 7380 ~ 8910 |
| R (random) | 12 | 4 | 230 | 1000 | 290 ~ 440 |
| RC (mixed) | 8 | 4 | 240 | 1000 | 290 ~ 370 |

**Total**: 81,950 across all 29 instances.

## Requirements

- C++17 compiler (GCC ≥9 or Clang ≥7)
- CMake ≥3.14
- No external libraries (standard library only)

## Reference

- Righini, G., Salani, M. (2006). *Dynamic programming for the orienteering problem with time windows.* Technical Report 91, Università degli Studi di Milano.
- Vansteenwegen, P., Souffriau, W., & Van Oudheusden, D. (2009). *The orienteering problem: A survey.* European Journal of Operational Research.
- Benchmark instances from [kodingway/TOPTW](https://github.com/kodingway/TOPTW)
