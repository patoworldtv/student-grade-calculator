#ifndef ANALYZER_H
#define ANALYZER_H

#include "Student.h"
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <cstddef>

struct PerfResult {
    double read_ms  = 0.0;
    double sort_ms  = 0.0;
    double split_ms = 0.0;
    double write_ms = 0.0;
    double total_ms = 0.0;
    std::size_t total_students = 0;
};

// v0.25 requirements: do full pipeline timing for vector, deque, list
namespace Analyzer {

// Read directly into each container (no “read vector then convert”).
std::vector<Student> readVectorFromFile(const std::string& filename);
std::deque<Student>  readDequeFromFile(const std::string& filename);
std::list<Student>   readListFromFile(const std::string& filename);

// Write any container (overloads)
void writeToFile(const std::string& filename, const std::vector<Student>& students);
void writeToFile(const std::string& filename, const std::deque<Student>& students);
void writeToFile(const std::string& filename, const std::list<Student>& students);

// Pipelines: read + sort + split + write (+ total)
PerfResult runVectorPipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail);
PerfResult runDequePipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail);
PerfResult runListPipeline (const std::string& inputFile, const std::string& outPass, const std::string& outFail);

// Utility: run all three pipelines and print results (optional)
void printPerf(const std::string& tag, const PerfResult& r);

} // namespace Analyzer

#endif // ANALYZER_H
