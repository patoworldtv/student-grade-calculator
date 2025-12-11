#ifndef ANALYZER_H
#define ANALYZER_H

#include "Student.h"
#include <vector>
#include <deque>
#include <list>
#include <string>

enum class SplitStrategy {
    COPY,       // Strategy 1
    INPLACE     // Strategy 2
};

struct PerfResult {
    double read_ms{};
    double sort_ms{};
    double split_ms{};
    double write_ms{};
    std::size_t total_students{};
};

namespace Analyzer {

// Read
std::vector<Student> readFromFile(const std::string& filename);

// Write
void writeToFile(const std::string& filename, const std::vector<Student>& students);

// Strategy 1 & 2 for vector
PerfResult runVector(const std::string& input,
                     const std::string& passFile,
                     const std::string& failFile,
                     SplitStrategy strategy);

// same for deque
PerfResult runDeque(const std::string& input,
                    const std::string& passFile,
                    const std::string& failFile,
                    SplitStrategy strategy);

// same for list
PerfResult runList(const std::string& input,
                   const std::string& passFile,
                   const std::string& failFile,
                   SplitStrategy strategy);

}

#endif
