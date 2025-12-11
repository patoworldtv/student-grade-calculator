#include "Analyzer.h"
#include "Sorter.h"
#include "ExceptionHandlers.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

static constexpr double PASS_CUTOFF = 5.0;

static inline double ms(auto a, auto b){
    return duration<double, std::milli>(b - a).count();
}

namespace Analyzer {

std::vector<Student> readFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open())
        throw FileException("Cannot open: " + filename);

    std::vector<Student> data;
    Student s;
    while (in >> s)
        data.push_back(s);

    return data;
}

void writeToFile(const std::string& filename, const std::vector<Student>& v) {
    std::ofstream out(filename);
    if (!out.is_open())
        throw FileException("Cannot write: " + filename);

    for (const auto& s : v) out << s << "\n";
}

template<class Container>
static PerfResult runGeneric(const std::string& input,
                             const std::string& passFile,
                             const std::string& failFile,
                             SplitStrategy strategy)
{
    PerfResult r;

    // READ
    auto t0 = high_resolution_clock::now();
    auto vec = readFromFile(input);
    auto t1 = high_resolution_clock::now();
    r.read_ms = ms(t0, t1);
    r.total_students = vec.size();

    // Move into container
    Container cont(vec.begin(), vec.end());
    vec.clear();

    // SORT
    auto ts = high_resolution_clock::now();
    Sorter::sort(cont);
    auto te = high_resolution_clock::now();
    r.sort_ms = ms(ts, te);

    // SPLIT
    auto sp0 = high_resolution_clock::now();

    std::vector<Student> passed, failed;

    if (strategy == SplitStrategy::COPY) {
        // Strategy 1 — copy_if
        std::copy_if(cont.begin(), cont.end(),
                     std::back_inserter(passed),
                     [](const Student& s){ return s.finalAvg() >= PASS_CUTOFF; });

        std::remove_copy_if(cont.begin(), cont.end(),
                            std::back_inserter(failed),
                            [](const Student& s){ return s.finalAvg() >= PASS_CUTOFF; });
    }
    else {
        // Strategy 2 — stable_partition then move
        auto it = std::stable_partition(
            cont.begin(), cont.end(),
            [](const Student& s){ return s.finalAvg() >= PASS_CUTOFF; });

        passed.assign(cont.begin(), it);
        failed.assign(it, cont.end());

        cont.erase(it, cont.end());
    }

    auto sp1 = high_resolution_clock::now();
    r.split_ms = ms(sp0, sp1);

    // WRITE
    auto wr0 = high_resolution_clock::now();
    writeToFile(passFile, passed);
    writeToFile(failFile, failed);
    auto wr1 = high_resolution_clock::now();
    r.write_ms = ms(wr0, wr1);

    return r;
}

// PUBLIC WRAPPERS

PerfResult runVector(const std::string& input,
                     const std::string& passFile,
                     const std::string& failFile,
                     SplitStrategy strategy)
{
    return runGeneric<std::vector<Student>>(input, passFile, failFile, strategy);
}

PerfResult runDeque(const std::string& input,
                    const std::string& passFile,
                    const std::string& failFile,
                    SplitStrategy strategy)
{
    return runGeneric<std::deque<Student>>(input, passFile, failFile, strategy);
}

PerfResult runList(const std::string& input,
                   const std::string& passFile,
                   const std::string& failFile,
                   SplitStrategy strategy)
{
    return runGeneric<std::list<Student>>(input, passFile, failFile, strategy);
}

} // namespace
