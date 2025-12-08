#include "StudentManager.h"
#include "Timer.h"
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

std::string StudentManager::makeRecord(size_t idx, int hwCount) {
    std::mt19937 gen(static_cast<uint32_t>(idx));
    std::uniform_int_distribution<int> dist(1, 10);

    std::ostringstream line;
    line << "Name" << idx << " " << "Surname" << idx << " ";
    for (int i = 0; i < hwCount; ++i) line << dist(gen) << " ";
    line << dist(gen); // exam
    return line.str();
}

void StudentManager::generateDataFilesWithSizes(const std::string& folder,
                                                const std::vector<size_t>& sizes,
                                                int hwCount) {
    if (!fs::exists(folder)) fs::create_directories(folder);
    for (size_t n : sizes) {
        std::string path = folder + "/students_" + std::to_string(n) + ".txt";
        std::ofstream out(path);
        if (!out) throw std::runtime_error("Cannot create file: " + path);
        for (size_t i = 1; i <= n; ++i) out << makeRecord(i, hwCount) << "\n";
    }
}

std::vector<Person> StudentManager::readFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) throw std::runtime_error("Cannot open file: " + filePath);

    std::vector<Person> result;
    std::string line;
    size_t lineNo = 0;

    while (true) {
        if (!std::getline(in, line)) break;
        ++lineNo;
        try {
            Person p;
            std::istringstream ss(line);
            ss >> p; // may throw
            result.push_back(std::move(p));
        } catch (const std::exception& e) {
            throw std::runtime_error("Parse error at line " + std::to_string(lineNo) +
                                     " in " + filePath + ": " + e.what());
        }
    }
    return result;
}

void StudentManager::splitPassedFailed(const std::vector<Person>& all,
                                       std::vector<Person>& passed,
                                       std::vector<Person>& failed,
                                       bool useMedian) {
    passed.clear();
    failed.clear();
    passed.reserve(all.size());
    failed.reserve(all.size());

    for (const auto& s : all) {
        double score = useMedian ? s.finalMed() : s.finalAvg();
        if (score >= 5.0) passed.push_back(s);
        else failed.push_back(s);
    }
}

void StudentManager::writeFile(const std::string& filePath, const std::vector<Person>& people) {
    fs::create_directories(fs::path(filePath).parent_path());
    std::ofstream out(filePath);
    if (!out) throw std::runtime_error("Cannot open for write: " + filePath);
    for (const auto& p : people) out << p << "\n";
}

PhaseTiming StudentManager::processFile(const std::string& inputPath,
                                        const std::string& outDir,
                                        bool useMedian) {
    PhaseTiming t;
    Timer timer;

    // Read
    auto all = readFile(inputPath);
    t.read_ms = timer.ms();
    timer.reset();
    t.total_records = all.size();

    // Split
    std::vector<Person> passed, failed;
    splitPassedFailed(all, passed, failed, useMedian);
    t.split_ms = timer.ms();
    timer.reset();

    // Write
    std::string base = fs::path(inputPath).stem().string();
    std::string passedPath = outDir + "/" + base + "_passed.txt";
    std::string failedPath = outDir + "/" + base + "_failed.txt";
    writeFile(passedPath, passed);
    writeFile(failedPath, failed);
    t.write_ms = timer.ms();

    return t;
}
