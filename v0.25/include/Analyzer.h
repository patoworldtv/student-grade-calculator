#ifndef ANALYZER_H
#define ANALYZER_H

#include "Student.h"
#include <string>

struct PerfResult {
    double read_ms = 0.0;
    double sort_ms = 0.0;
    double split_ms = 0.0;
    double write_ms = 0.0;
    std::size_t total_students = 0;
};

// Provides file I/O, splitting, and three pipelines for vector/deque/list
namespace Analyzer {

// Read all students from a single file (each line one student)
std::vector<Student> readFromFile(const std::string& filename);

// Read all students from all .txt files in folder (flattened)
std::vector<Student> readAllFromFolder(const std::string& folder);

// Write vector<Student> to file (one line per student)
void writeToFile(const std::string& filename, const std::vector<Student>& students);

// Pipelines: read, sort, split (>=5 pass), write pass/fail
PerfResult runVectorPipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail);
PerfResult runDequePipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail);
PerfResult runListPipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail);

} // namespace Analyzer

#endif // ANALYZER_H
