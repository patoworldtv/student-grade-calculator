#include "Analyzer.h"
#include "Sorter.h"
#include "ExceptionHandlers.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std::chrono;

namespace Analyzer {

static constexpr double PASS_CUTOFF = 5.0;

static inline double msBetween(const high_resolution_clock::time_point& a,
                               const high_resolution_clock::time_point& b) {
    return duration<double, std::milli>(b - a).count();
}

// ---------- READERS (direct to container) ----------

std::vector<Student> readVectorFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw FileException("Cannot open file: " + filename);

    std::vector<Student> out;
    std::size_t lineNo = 0;

    while (in.good()) {
        Student s;
        std::streampos before = in.tellg();
        in >> s; // uses getline inside operator>> in Person.cpp
        if (!in && in.eof()) break;

        ++lineNo;
        if (!s.isValidBasic()) {
            // Avoid infinite loop if stream is in a bad state
            if (in.fail() && !in.eof()) {
                in.clear();
                std::string junk;
                std::getline(in, junk);
            }
            // Protect against no-progress reads
            if (in.tellg() == before) {
                std::string junk;
                std::getline(in, junk);
            }
            continue;
        }

        // Cache is computed inside operator>> (Person), but safe to ensure:
        // (If you already do it in operator>>, this is redundant but harmless.)
        // s.computeCache();

        out.push_back(std::move(s));
    }
    return out;
}

std::deque<Student> readDequeFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw FileException("Cannot open file: " + filename);

    std::deque<Student> out;
    while (in.good()) {
        Student s;
        in >> s;
        if (!in && in.eof()) break;
        if (!s.isValidBasic()) continue;

        // Optional safety (usually redundant if operator>> computes cache)
        // s.computeCache();

        out.push_back(std::move(s));
    }
    return out;
}

std::list<Student> readListFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw FileException("Cannot open file: " + filename);

    std::list<Student> out;
    while (in.good()) {
        Student s;
        in >> s;
        if (!in && in.eof()) break;
        if (!s.isValidBasic()) continue;

        // Optional safety (usually redundant if operator>> computes cache)
        // s.computeCache();

        out.push_back(std::move(s));
    }
    return out;
}

// ---------- WRITERS ----------

void writeToFile(const std::string& filename, const std::vector<Student>& students) {
    std::ofstream out(filename);
    if (!out.is_open()) throw FileException("Cannot open file for writing: " + filename);
    for (const auto& s : students) out << s << "\n";
}

void writeToFile(const std::string& filename, const std::deque<Student>& students) {
    std::ofstream out(filename);
    if (!out.is_open()) throw FileException("Cannot open file for writing: " + filename);
    for (const auto& s : students) out << s << "\n";
}

void writeToFile(const std::string& filename, const std::list<Student>& students) {
    std::ofstream out(filename);
    if (!out.is_open()) throw FileException("Cannot open file for writing: " + filename);
    for (const auto& s : students) out << s << "\n";
}

// ---------- PERF PRINT ----------

void printPerf(const std::string& tag, const PerfResult& r) {
    std::cout
        << tag
        << " read="  << r.read_ms
        << " sort="  << r.sort_ms
        << " split=" << r.split_ms
        << " write=" << r.write_ms
        << " TOTAL=" << r.total_ms
        << " students=" << r.total_students
        << "\n";
}

// ---------- PIPELINES ----------

PerfResult runVectorPipeline(const std::string& inputFile,
                             const std::string& outPass,
                             const std::string& outFail) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_s = high_resolution_clock::now();
    auto students = readVectorFromFile(inputFile);
    auto t_read_e = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_s, t_read_e);
    r.total_students = students.size();

    auto t_sort_s = high_resolution_clock::now();
    Sorter::sortVectorDesc(students); // now sorts by finalAvgCached()
    auto t_sort_e = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_s, t_sort_e);

    auto t_split_s = high_resolution_clock::now();
    std::vector<Student> pass; pass.reserve(students.size()/2);
    std::vector<Student> fail; fail.reserve(students.size()/2);

    for (const auto& s : students) {
        if (s.finalAvgCached() >= PASS_CUTOFF) pass.push_back(s);
        else fail.push_back(s);
    }
    auto t_split_e = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_s, t_split_e);

    auto t_write_s = high_resolution_clock::now();
    writeToFile(outPass, pass);
    writeToFile(outFail, fail);
    auto t_write_e = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_s, t_write_e);

    auto t1 = high_resolution_clock::now();
    r.total_ms = msBetween(t0, t1);
    return r;
}

PerfResult runDequePipeline(const std::string& inputFile,
                            const std::string& outPass,
                            const std::string& outFail) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_s = high_resolution_clock::now();
    auto students = readDequeFromFile(inputFile);
    auto t_read_e = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_s, t_read_e);
    r.total_students = students.size();

    auto t_sort_s = high_resolution_clock::now();
    Sorter::sortDequeDesc(students); // now sorts by finalAvgCached()
    auto t_sort_e = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_s, t_sort_e);

    auto t_split_s = high_resolution_clock::now();
    std::deque<Student> pass;
    std::deque<Student> fail;

    for (const auto& s : students) {
        if (s.finalAvgCached() >= PASS_CUTOFF) pass.push_back(s);
        else fail.push_back(s);
    }
    auto t_split_e = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_s, t_split_e);

    auto t_write_s = high_resolution_clock::now();
    writeToFile(outPass, pass);
    writeToFile(outFail, fail);
    auto t_write_e = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_s, t_write_e);

    auto t1 = high_resolution_clock::now();
    r.total_ms = msBetween(t0, t1);
    return r;
}

PerfResult runListPipeline(const std::string& inputFile,
                           const std::string& outPass,
                           const std::string& outFail) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_s = high_resolution_clock::now();
    auto students = readListFromFile(inputFile);
    auto t_read_e = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_s, t_read_e);
    r.total_students = students.size();

    auto t_sort_s = high_resolution_clock::now();
    Sorter::sortListDesc(students); // now sorts by finalAvgCached()
    auto t_sort_e = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_s, t_sort_e);

    auto t_split_s = high_resolution_clock::now();
    std::list<Student> pass;
    std::list<Student> fail;

    for (const auto& s : students) {
        if (s.finalAvgCached() >= PASS_CUTOFF) pass.push_back(s);
        else fail.push_back(s);
    }
    auto t_split_e = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_s, t_split_e);

    auto t_write_s = high_resolution_clock::now();
    writeToFile(outPass, pass);
    writeToFile(outFail, fail);
    auto t_write_e = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_s, t_write_e);

    auto t1 = high_resolution_clock::now();
    r.total_ms = msBetween(t0, t1);
    return r;
}

} // namespace Analyzer
