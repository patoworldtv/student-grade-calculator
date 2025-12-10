#include "Analyzer.h"
#include "ExceptionHandlers.h"
#include "Sorter.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <deque>
#include <list>

namespace fs = std::filesystem;
using namespace std::chrono;

namespace Analyzer {

static constexpr double PASS_CUTOFF = 5.0;

std::vector<Student> readFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw FileException("Cannot open file: " + filename);

    std::vector<Student> outVec;
    std::string line;
    size_t lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;
        std::istringstream ss(line);
        Student s;
        ss >> s;
        if (s.getName().empty() || s.getSurname().empty()) {
            std::cerr << "Warning: malformed line " << lineNo << " in " << filename << "\n";
            continue;
        }
        outVec.push_back(std::move(s));
    }
    return outVec;
}

std::vector<Student> readAllFromFolder(const std::string& folder) {
    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        throw FileException("Folder does not exist: " + folder);
    }
    std::vector<Student> all;
    for (const auto& e : fs::directory_iterator(folder)) {
        if (!e.is_regular_file()) continue;
        auto p = e.path();
        if (p.extension() != ".txt") continue;
        try {
            auto v = readFromFile(p.string());
            all.insert(all.end(), v.begin(), v.end());
            std::cout << "Loaded " << v.size() << " from " << p.filename().string() << "\n";
        } catch (const FileException& fe) {
            std::cerr << "Warning: " << fe.what() << "\n";
            continue;
        }
    }
    return all;
}

void writeToFile(const std::string& filename, const std::vector<Student>& students) {
    std::ofstream out(filename);
    if (!out.is_open()) throw FileException("Cannot open file for writing: " + filename);
    for (const auto& s : students) out << s << "\n";
}

static inline double msBetween(const high_resolution_clock::time_point& a, const high_resolution_clock::time_point& b) {
    return duration<double, std::milli>(b - a).count();
}

PerfResult runVectorPipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_start = high_resolution_clock::now();
    auto vec = readFromFile(inputFile);
    auto t_read_end = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_start, t_read_end);
    r.total_students = vec.size();

    auto t_sort_start = high_resolution_clock::now();
    Sorter::sortVectorDesc(vec);
    auto t_sort_end = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_start, t_sort_end);

    auto t_split_start = high_resolution_clock::now();
    std::vector<Student> pass; pass.reserve(vec.size()/2);
    std::vector<Student> fail; fail.reserve(vec.size()/2);
    for (const auto& s : vec) {
        if (s.finalAvg() >= PASS_CUTOFF) pass.push_back(s);
        else fail.push_back(s);
    }
    auto t_split_end = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_start, t_split_end);

    auto t_write_start = high_resolution_clock::now();
    writeToFile(outPass, pass);
    writeToFile(outFail, fail);
    auto t_write_end = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_start, t_write_end);

    return r;
}

PerfResult runDequePipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail) {
    PerfResult r;

    auto t_read_start = high_resolution_clock::now();
    auto vec = readFromFile(inputFile);
    auto t_read_end = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_start, t_read_end);
    r.total_students = vec.size();

    std::deque<Student> deq(std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()));
    vec.clear();

    auto t_sort_start = high_resolution_clock::now();
    Sorter::sortDequeDesc(deq);
    auto t_sort_end = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_start, t_sort_end);

    auto t_split_start = high_resolution_clock::now();
    std::vector<Student> pass; pass.reserve(deq.size()/2);
    std::vector<Student> fail; fail.reserve(deq.size()/2);
    for (const auto& s : deq) {
        if (s.finalAvg() >= PASS_CUTOFF) pass.push_back(s);
        else fail.push_back(s);
    }
    auto t_split_end = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_start, t_split_end);

    auto t_write_start = high_resolution_clock::now();
    writeToFile(outPass, pass);
    writeToFile(outFail, fail);
    auto t_write_end = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_start, t_write_end);

    return r;
}

PerfResult runListPipeline(const std::string& inputFile, const std::string& outPass, const std::string& outFail) {
    PerfResult r;

    auto t_read_start = high_resolution_clock::now();
    auto vec = readFromFile(inputFile);
    auto t_read_end = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_start, t_read_end);
    r.total_students = vec.size();

    std::list<Student> lst;
    lst.insert(lst.end(), std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()));
    vec.clear();

    auto t_sort_start = high_resolution_clock::now();
    Sorter::sortListDesc(lst);
    auto t_sort_end = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_start, t_sort_end);

    auto t_split_start = high_resolution_clock::now();
    std::vector<Student> pass; pass.reserve(lst.size()/2);
    std::vector<Student> fail; fail.reserve(lst.size()/2);
    for (const auto& s : lst) {
        if (s.finalAvg() >= PASS_CUTOFF) pass.push_back(s);
        else fail.push_back(s);
    }
    auto t_split_end = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_start, t_split_end);

    auto t_write_start = high_resolution_clock::now();
    writeToFile(outPass, pass);
    writeToFile(outFail, fail);
    auto t_write_end = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_start, t_write_end);

    return r;
}

} // namespace Analyzer
