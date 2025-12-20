#include "FileGenerator.h"
#include "ExceptionHandlers.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace fs = std::filesystem;

namespace FileGenerator {

void generateFile(const std::string& outPath, std::uint64_t count, int hwPerStudent) {
    fs::path p(outPath);
    if (p.has_parent_path()) fs::create_directories(p.parent_path());

    std::ofstream out(outPath);
    if (!out.is_open()) throw GeneratorException("Cannot open file for writing: " + outPath);

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int> grade(0, 10);

    for (std::uint64_t i = 1; i <= count; ++i) {
        out << "Name" << i << " " << "Surname" << i;
        for (int j = 0; j < hwPerStudent; ++j) out << " " << grade(rng);
        out << " " << grade(rng) << "\n";
    }

    std::cout << "Generated: " << outPath << " (" << count << " records)\n";
}

void generateFilesForCounts(const std::string& folder,
                            const std::vector<std::uint64_t>& counts,
                            int hwPerStudent) {
    fs::create_directories(folder);

    for (auto c : counts) {
        std::string path = folder + "/students_" + std::to_string(c) + ".txt";
        generateFile(path, c, hwPerStudent);
    }
}

}
