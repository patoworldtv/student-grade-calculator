#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include <string>
#include <vector>
#include <cstdint>

namespace FileGenerator {

// Generates one file with `count` students.
// Format: NameX SurnameX hw... exam
void generateFile(const std::string& outPath,
                  std::uint64_t count,
                  int hwPerStudent = 5);

// Generates multiple files in a folder for provided counts.
void generateFilesForCounts(const std::string& folder,
                            const std::vector<std::uint64_t>& counts,
                            int hwPerStudent = 5);

} // namespace FileGenerator

#endif // FILEGENERATOR_H
