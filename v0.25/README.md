# Student Grade Calculator — v0.25

## Version Purpose
Version **v0.25** is the **benchmark and experimental version** of the Student Grade Calculator.

This version focuses on:
- Measuring performance of different STL containers
- Comparing `std::vector`, `std::deque`, and `std::list`
- Establishing baseline results before algorithm-level optimization (v1.0)

---

##  Functionality
v0.25 provides the following features:

1. Generate large student datasets
2. Load and display students from files
3. Add manual students
4. Find highest and lowest grades
5. Show random student
6. Run performance tests on:
   - `std::vector`
   - `std::deque`
   - `std::list`

---

##  Performance Test Description
The benchmark tests measure:

- **Read time** — loading students from file
- **Sort time** — sorting by final grade (descending)
- **Split time** — dividing students into passed / failed groups
- **Write time** — saving results to output files

### Passing rule
```text
Final grade ≥ 5.0 → PASSED
Final grade < 5.0 → FAILED
Observations (Summary)
std::vector performs best overall for large datasets

std::deque performs moderately well but slower on erase operations

std::list performs worst due to poor cache locality and slow sorting

This version does not use STL partitioning algorithms yet.
All operations are implemented manually to serve as a baseline.

 Data Files
Generated student data is stored in:

bash
Copy code
v0.25/data/Students/
For GitHub and grading purposes, only a small sample dataset is committed:

students10000.txt

Larger datasets (1M, 10M) are generated locally and excluded from Git.

 Build & Run Instructions
Using Make
bash
Copy code
make
./student-grade-calculator
 Relation to Other Versions
v0.25 → Benchmark & baseline implementation

v1.0 → Optimized version using <algorithm> (partition, remove_if, etc.)

