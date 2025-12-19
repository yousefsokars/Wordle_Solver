🟩 Wordle Solver (C++)

A high-performance command-line Wordle solver written in modern C++.
The project focuses on algorithmic efficiency, search-space reduction, and clean software architecture, making it suitable as a systems / algorithms portfolio project.

🚀 Features

Strategy Design Pattern – Easily switch solving strategies at runtime.

Aggressive Mode (Greedy Heuristic) – Fast frequency-based guess selection.

Safe Mode (Minimax) – Information-theoretic approach minimizing the worst-case outcome.

Custom Simulation Engine – Accurately simulates Wordle feedback to prune invalid candidates.

🛠️ Tech Stack

Language: C++ (STL: vector, unordered_map, set)

Paradigms: Object-Oriented Design, Strategy Pattern

Build System: CMake

Tooling: Git (feature-branch workflow), Visual Studio / GCC

🧠 Algorithms
1. Aggressive Strategy (Greedy)

Computes letter frequencies across the remaining valid words.

Scores each candidate by summing letter probabilities.

Time complexity: O(N × L) per guess (N = candidates, L = word length).

Optimized for fast early-game pruning.

2. Safe Strategy (Minimax / Information Theory)

Designed for difficult or constrained states.

Iterates through all allowed guesses.

Simulates feedback against every remaining possible answer.

Groups results into outcome “buckets”.

Measures the worst-case bucket size.

Selects the guess that minimizes the worst possible remaining search space.

Hybrid Optimization:
To keep runtime reasonable, the solver switches to Minimax only when the candidate pool drops below a threshold (e.g. N < 50).

This guarantees strong performance without sacrificing responsiveness.

💻 Build & Run
Prerequisites

C++17-compatible compiler (GCC, Clang, or MSVC)

CMake ≥ 3.15

Build Instructions
git clone https://github.com/yousefsokars/Wordle_Solver.git
cd Wordle_Solver

mkdir build
cd build

cmake ..
cmake --build .

Run
./WordleSolver


(On Windows, run the generated .exe from the build directory.)

📌 Why This Project Matters

This project demonstrates:

Algorithmic thinking (Minimax, entropy reduction)

Clean C++ architecture

Practical optimization tradeoffs

Real-world problem modeling

📄 License

MIT License
