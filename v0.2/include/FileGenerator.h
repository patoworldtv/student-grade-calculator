#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include <string>
#include <vector>
#include <cstdint>

namespace FileGenerator {

// Generate a single file with 'count' student records at outPath.
// Each student: Name{index} Surname{index} hw1 hw2 ... exam
void generateFile(const std::string& outPath, std::uint64_t count, int hwPerStudent = 5);

// Generate multiple files inside folder according to counts vector.
void generateFilesForCounts(const std::string& folder, const std::vector<std::uint64_t>& counts, int hwPerStudent = 5);

} // namespace FileGenerator

#endif // FILEGENERATOR_H
