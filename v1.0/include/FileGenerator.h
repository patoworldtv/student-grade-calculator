#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include <cstdint>
#include <string>
#include <vector>

namespace FileGenerator {

void generateFile(const std::string& outPath,
                  std::uint64_t count,
                  int hwPerStudent = 5);

void generateFilesForCounts(const std::string& folder,
                            const std::vector<std::uint64_t>& counts,
                            int hwPerStudent = 5);

} // namespace FileGenerator

#endif
