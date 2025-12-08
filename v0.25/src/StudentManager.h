#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include <string>
#include <vector>
#include "Person.h"

struct PhaseTiming {
    long long read_ms = 0;
    long long split_ms = 0;
    long long write_ms = 0;
    size_t total_records = 0;
};

class StudentManager {
public:
    // Generate files with specified sizes
    void generateDataFilesWithSizes(const std::string& folder,
                                    const std::vector<size_t>& sizes,
                                    int hwCount = 5);

    // Read a single file (throws on I/O or parse errors)
    std::vector<Person> readFile(const std::string& filePath);

    // Split into passed/failed by finalAvg or finalMed
    void splitPassedFailed(const std::vector<Person>& all,
                           std::vector<Person>& passed,
                           std::vector<Person>& failed,
                           bool useMedian = false);

    // Write a container of persons to a file
    void writeFile(const std::string& filePath, const std::vector<Person>& people);

    // Process file end-to-end and return timings (vector baseline)
    PhaseTiming processFile(const std::string& inputPath,
                            const std::string& outDir,
                            bool useMedian = false);

private:
    std::string makeRecord(size_t idx, int hwCount);
};

#endif
