#ifndef ANALYZER_H
#define ANALYZER_H

#include "Student.h"
#include <cstddef>
#include <deque>
#include <list>
#include <string>
#include <vector>

struct PerfResult {
    double read_ms = 0.0;
    double sort_ms = 0.0;
    double split_ms = 0.0;
    double write_ms = 0.0;
    double total_ms = 0.0;
    std::size_t total_students = 0;
};

enum class SplitStrategy {
    Strategy1_CopyToTwoContainers = 1,
    Strategy2_MoveFailAndShrinkBase = 2
};

enum class PartitionMode {
    Partition = 1,
    StablePartition = 2
};

namespace Analyzer {

std::vector<Student> readVectorFromFile(const std::string& filename);
std::deque<Student>  readDequeFromFile (const std::string& filename);
std::list<Student>   readListFromFile  (const std::string& filename);

void writeToFile(const std::string& filename, const std::vector<Student>& students);
void writeToFile(const std::string& filename, const std::deque<Student>& students);
void writeToFile(const std::string& filename, const std::list<Student>& students);

void printPerf(const std::string& tag, const PerfResult& r);

PerfResult runVectorPipeline(const std::string& inputFile,
                             const std::string& outPass,
                             const std::string& outFail,
                             SplitStrategy strat,
                             PartitionMode pmode);

PerfResult runDequePipeline(const std::string& inputFile,
                            const std::string& outPass,
                            const std::string& outFail,
                            SplitStrategy strat,
                            PartitionMode pmode);

PerfResult runListPipeline(const std::string& inputFile,
                           const std::string& outPass,
                           const std::string& outFail,
                           SplitStrategy strat,
                           PartitionMode pmode);

// Demonstrate required algorithms: find/find_if/search on loaded container
void demoAlgorithmSearch_Vector(const std::vector<Student>& students);
void demoAlgorithmSearch_Deque(const std::deque<Student>& students);
void demoAlgorithmSearch_List(const std::list<Student>& students);

} // namespace Analyzer

#endif
