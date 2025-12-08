# v0.25

This release extends v0.2 by benchmarking different container types (std::vector, std::list, std::deque) for the same pipeline: read, split (passed/failed), and write.

## Features
- Generate five datasets: 1,000; 10,000; 100,000; 1,000,000; 10,000,000 records.
- Parse records with exception handling.
- Split students into:
  - failed: final score < 5.0
  - passed: final score >= 5.0
- Measure and report timings for read, split, and write phases.
- Output results separated by container type:
  - ./out/vector
  - ./out/list
  - ./out/deque

## Input format
Each line:

NameX SurnameX hw1 hw2 ... hwN exam

Final scores:
- finalAvg = 0.4 * avg(homework) + 0.6 * exam
- finalMed = 0.4 * median(homework) + 0.6 * exam

## Usage
1. Build in Release mode for realistic performance.
2. Run the executable.
3. Choose:
   - Option 1: Generate data files into ./data
   - Option 2/3: Process using vector with avg/median
   - Option 5/6: Benchmark vector/list/deque with avg/median
4. Check outputs in ./out/{vector|list|deque}

## Notes
- For very large files (10M), disk speed and system memory impact runtime significantly.
- std::vector and std::deque generally outperform std::list due to cache locality.
- Output lines contain name, surname, finalAvg, finalMed (formatted).
