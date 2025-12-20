#include "Analyzer.h"
#include "ExceptionHandlers.h"
#include "Sorter.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std::chrono;

namespace Analyzer {

static constexpr double PASS_CUTOFF = 5.0;

static inline double msBetween(const high_resolution_clock::time_point& a,
                               const high_resolution_clock::time_point& b) {
    return duration<double, std::milli>(b - a).count();
}

static inline bool isPassed(const Student& s) {
    return s.finalAvgCached() >= PASS_CUTOFF;
}

template <typename It, typename Pred>
static It doPartition(It first, It last, Pred pred, PartitionMode pmode) {
    return (pmode == PartitionMode::StablePartition)
        ? std::stable_partition(first, last, pred)
        : std::partition(first, last, pred);
}

// -------------------- READERS --------------------

static void ensureCache(std::vector<Student>& v) {
    std::transform(v.begin(), v.end(), v.begin(), [](Student s) {
        s.computeCache();
        return s;
    });
}

static void ensureCache(std::deque<Student>& d) {
    std::transform(d.begin(), d.end(), d.begin(), [](Student s) {
        s.computeCache();
        return s;
    });
}

static void ensureCache(std::list<Student>& l) {
    std::list<Student> rebuilt;
    std::transform(l.begin(), l.end(), std::back_inserter(rebuilt), [](Student s) {
        s.computeCache();
        return s;
    });
    l.swap(rebuilt);
}

std::vector<Student> readVectorFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw FileException("Cannot open file: " + filename);

    std::vector<Student> out;
    while (in.good()) {
        Student s;
        in >> s;
        if (!in && in.eof()) break;
        if (!s.isValidBasic()) continue;
        out.push_back(std::move(s));
    }

    ensureCache(out);
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
        out.push_back(std::move(s));
    }

    ensureCache(out);
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
        out.push_back(std::move(s));
    }

    ensureCache(out);
    return out;
}

// -------------------- WRITERS --------------------

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

// -------------------- PERF PRINT --------------------

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

// -------------------- STRATEGY 1 (COPY to two containers) --------------------
// Requirement: original students container remains unchanged

static void splitVector_Strategy1(const std::vector<Student>& students,
                                  std::vector<Student>& passed,
                                  std::vector<Student>& failed) {
    passed.clear();
    failed.clear();
    passed.reserve(students.size() / 2);
    failed.reserve(students.size() / 2);

    // remove_copy_if required (used twice)
    std::remove_copy_if(students.begin(), students.end(),
                        std::back_inserter(failed),
                        [](const Student& s){ return isPassed(s); });

    std::remove_copy_if(students.begin(), students.end(),
                        std::back_inserter(passed),
                        [](const Student& s){ return !isPassed(s); });
}

static void splitDeque_Strategy1(const std::deque<Student>& students,
                                 std::deque<Student>& passed,
                                 std::deque<Student>& failed) {
    passed.clear();
    failed.clear();

    std::remove_copy_if(students.begin(), students.end(),
                        std::back_inserter(failed),
                        [](const Student& s){ return isPassed(s); });

    std::remove_copy_if(students.begin(), students.end(),
                        std::back_inserter(passed),
                        [](const Student& s){ return !isPassed(s); });
}

static void splitList_Strategy1(const std::list<Student>& students,
                                std::list<Student>& passed,
                                std::list<Student>& failed) {
    passed.clear();
    failed.clear();

    std::remove_copy_if(students.begin(), students.end(),
                        std::back_inserter(failed),
                        [](const Student& s){ return isPassed(s); });

    std::remove_copy_if(students.begin(), students.end(),
                        std::back_inserter(passed),
                        [](const Student& s){ return !isPassed(s); });
}

// -------------------- STRATEGY 2 (MOVE fails out, shrink base) --------------------
// Requirement: only ONE new container created (failed). Base container becomes passed.

static void splitVector_Strategy2(std::vector<Student>& students,
                                  std::vector<Student>& failed,
                                  PartitionMode pmode) {
    failed.clear();
    failed.reserve(students.size() / 2);

    // partition required
    auto it = doPartition(students.begin(), students.end(),
                          [](const Student& s){ return isPassed(s); }, pmode);

    // copy required: move tail into failed
    std::copy(std::make_move_iterator(it),
              std::make_move_iterator(students.end()),
              std::back_inserter(failed));

    // erase tail (shrink base)
    students.erase(it, students.end());
    students.shrink_to_fit();
}

static void splitDeque_Strategy2(std::deque<Student>& students,
                                 std::deque<Student>& failed,
                                 PartitionMode pmode) {
    failed.clear();

    auto it = doPartition(students.begin(), students.end(),
                          [](const Student& s){ return isPassed(s); }, pmode);

    std::copy(std::make_move_iterator(it),
              std::make_move_iterator(students.end()),
              std::back_inserter(failed));

    students.erase(it, students.end());
}

static void splitList_Strategy2(std::list<Student>& students,
                                std::list<Student>& failed,
                                PartitionMode pmode) {
    failed.clear();

    // stable_partition (optional) OR partition required
    auto it = doPartition(students.begin(), students.end(),
                          [](const Student& s){ return isPassed(s); }, pmode);

    // for list, best move is splice (moves nodes, no copying)
    failed.splice(failed.end(), students, it, students.end());
}

// -------------------- PIPELINES --------------------

PerfResult runVectorPipeline(const std::string& inputFile,
                             const std::string& outPass,
                             const std::string& outFail,
                             SplitStrategy strat,
                             PartitionMode pmode) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_s = high_resolution_clock::now();
    auto students = readVectorFromFile(inputFile);
    auto t_read_e = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_s, t_read_e);
    r.total_students = students.size();

    auto t_sort_s = high_resolution_clock::now();
    Sorter::sortVectorDesc(students);
    auto t_sort_e = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_s, t_sort_e);

    std::vector<Student> passed;
    std::vector<Student> failed;

    auto t_split_s = high_resolution_clock::now();
    if (strat == SplitStrategy::Strategy1_CopyToTwoContainers) {
        splitVector_Strategy1(students, passed, failed);
    } else {
        splitVector_Strategy2(students, failed, pmode);
        // Base container is now passed students
        passed = students; // convenience; you can also write students directly
    }
    auto t_split_e = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_s, t_split_e);

    auto t_write_s = high_resolution_clock::now();
    writeToFile(outPass, passed);
    writeToFile(outFail, failed);
    auto t_write_e = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_s, t_write_e);

    r.total_ms = msBetween(t0, high_resolution_clock::now());
    return r;
}

PerfResult runDequePipeline(const std::string& inputFile,
                            const std::string& outPass,
                            const std::string& outFail,
                            SplitStrategy strat,
                            PartitionMode pmode) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_s = high_resolution_clock::now();
    auto students = readDequeFromFile(inputFile);
    auto t_read_e = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_s, t_read_e);
    r.total_students = students.size();

    auto t_sort_s = high_resolution_clock::now();
    Sorter::sortDequeDesc(students);
    auto t_sort_e = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_s, t_sort_e);

    std::deque<Student> passed;
    std::deque<Student> failed;

    auto t_split_s = high_resolution_clock::now();
    if (strat == SplitStrategy::Strategy1_CopyToTwoContainers) {
        splitDeque_Strategy1(students, passed, failed);
    } else {
        splitDeque_Strategy2(students, failed, pmode);
        passed = students;
    }
    auto t_split_e = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_s, t_split_e);

    auto t_write_s = high_resolution_clock::now();
    writeToFile(outPass, passed);
    writeToFile(outFail, failed);
    auto t_write_e = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_s, t_write_e);

    r.total_ms = msBetween(t0, high_resolution_clock::now());
    return r;
}

PerfResult runListPipeline(const std::string& inputFile,
                           const std::string& outPass,
                           const std::string& outFail,
                           SplitStrategy strat,
                           PartitionMode pmode) {
    PerfResult r;
    auto t0 = high_resolution_clock::now();

    auto t_read_s = high_resolution_clock::now();
    auto students = readListFromFile(inputFile);
    auto t_read_e = high_resolution_clock::now();
    r.read_ms = msBetween(t_read_s, t_read_e);
    r.total_students = students.size();

    auto t_sort_s = high_resolution_clock::now();
    Sorter::sortListDesc(students);
    auto t_sort_e = high_resolution_clock::now();
    r.sort_ms = msBetween(t_sort_s, t_sort_e);

    std::list<Student> passed;
    std::list<Student> failed;

    auto t_split_s = high_resolution_clock::now();
    if (strat == SplitStrategy::Strategy1_CopyToTwoContainers) {
        splitList_Strategy1(students, passed, failed);
    } else {
        splitList_Strategy2(students, failed, pmode);
        passed = students;
    }
    auto t_split_e = high_resolution_clock::now();
    r.split_ms = msBetween(t_split_s, t_split_e);

    auto t_write_s = high_resolution_clock::now();
    writeToFile(outPass, passed);
    writeToFile(outFail, failed);
    auto t_write_e = high_resolution_clock::now();
    r.write_ms = msBetween(t_write_s, t_write_e);

    r.total_ms = msBetween(t0, high_resolution_clock::now());
    return r;
}

// -------------------- REQUIRED ALGORITHM DEMOS: find/find_if/search --------------------

static bool containsSubstring(const std::string& text, const std::string& pattern) {
    auto it = std::search(text.begin(), text.end(), pattern.begin(), pattern.end());
    return it != text.end();
}

void demoAlgorithmSearch_Vector(const std::vector<Student>& students) {
    if (students.empty()) return;

    auto it = std::find_if(students.begin(), students.end(),
                           [](const Student& s){ return s.getSurname() == "Surname1"; });
    if (it != students.end()) {
        std::cout << "[find_if] Found Surname1: " << it->getName() << " " << it->getSurname() << "\n";
    }

    std::vector<std::string> names;
    names.reserve(std::min<std::size_t>(students.size(), 1000));
    for (std::size_t i = 0; i < students.size() && i < 1000; ++i)
        names.push_back(students[i].getName());

    auto it2 = std::find(names.begin(), names.end(), "Name2");
    if (it2 != names.end()) std::cout << "[find] Found Name2 in first 1000\n";

    if (containsSubstring(students.front().getName(), "Name")) {
        std::cout << "[search] 'Name' is substring of first student's name\n";
    }
}

void demoAlgorithmSearch_Deque(const std::deque<Student>& students) {
    if (students.empty()) return;
    auto it = std::find_if(students.begin(), students.end(),
                           [](const Student& s){ return s.getSurname() == "Surname1"; });
    if (it != students.end()) std::cout << "[find_if] deque found Surname1\n";
}

void demoAlgorithmSearch_List(const std::list<Student>& students) {
    if (students.empty()) return;
    auto it = std::find_if(students.begin(), students.end(),
                           [](const Student& s){ return s.getSurname() == "Surname1"; });
    if (it != students.end()) std::cout << "[find_if] list found Surname1\n";
}

} // namespace Analyzer
