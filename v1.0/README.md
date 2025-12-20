# Student Grade Calculator — v1.0

Student Grade Calculator — Performance Analysis

Project Overview

This project analyzes the performance of different C++ STL containers (std::vector, std::deque, std::list) when sorting and categorizing student records into passed and failed groups.

Two splitting strategies were evaluated:

Strategy 1 (Copy-based)
Students are copied into two new containers (passed and failed) while the original container remains unchanged.

Strategy 2 (In-place / Move-based)
Failed students are removed (or moved) from the base container, leaving only passed students. This strategy reduces memory usage but introduces container-specific costs.

Test Environment

Language: C++17

Compiler: GCC 13.x

OS: Linux (GitHub Codespaces)

Build Systems: Make & CMake

Dataset sizes:

10,000

100,000

1,000,000

10,000,000 students

Generated datasets are included in v0.25 for reproducibility.

Algorithms Used (C++ <algorithm>)

The optimized implementation (v1.0) uses standard algorithms as required:

std::sort

std::partition

std::stable_partition

std::remove_if

std::remove_copy_if

std::copy

std::transform

std::find

std::find_if

std::search

A cached final grade value is used to avoid repeated recomputation during partitioning.

Performance Results (Summary)
Strategy 1 — Copy to Two Containers
Container	Characteristics
vector	Fast sorting, moderate memory overhead
deque	Slightly slower sorting
list	Slowest due to node-based traversal

Observation:
Memory usage is higher because every student is copied into a new container.

Strategy 2 — In-place Split (Preferred)
Container	Result
vector	 Fastest overall
deque	Acceptable but slower erase operations
list	Efficient node movement but very slow sorting

Best Choice:
std::vector + Strategy 2 + std::partition

This combination achieved the best overall execution time for large datasets.

Final Conclusion

std::vector is the most efficient container for this task.

Strategy 2 significantly reduces memory usage.

Using <algorithm> functions improves clarity, correctness, and performance.

v0.25 demonstrates raw benchmarking.

v1.0 is a clean, optimized, final release.

How to Build and Run
Using Make
make
./student-grade-calculator

Using CMake
mkdir build_cmake
cd build_cmake
cmake ..
cmake --build .
./student-grade-calculator

 Repository Structure
student-grade-calculator/
├── v0.25/   # Benchmark version with generated datasets
├── v1.0/    # Final optimized release

Releases

v0.25 — Performance benchmarking version

v1.0 — Final optimized release
