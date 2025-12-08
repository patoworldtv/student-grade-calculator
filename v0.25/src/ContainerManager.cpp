#include "ContainerManager.h"
#include "Timer.h"
#include <filesystem>
#include <vector>
#include <list>
#include <deque>

namespace fs = std::filesystem;

ContainerTiming ContainerManager::processWithContainer(const std::string& inputPath,
                                                       const std::string& outBaseDir,
                                                       ContainerType type,
                                                       bool useMedian,
                                                       StudentManager& baseMgr) {
    ContainerTiming ct;
    Timer timer;

    // Read baseline using vector (consistent parsing and baseline timing)
    auto allVec = baseMgr.readFile(inputPath);
    ct.read_ms = timer.ms();
    ct.total_records = allVec.size();

    timer.reset();
    std::string stem = fs::path(inputPath).stem().string();

    if (type == ContainerType::Vector) {
        std::vector<Person> container = allVec;
        std::vector<Person> passed, failed;
        baseMgr.splitPassedFailed(container, passed, failed, useMedian);
        ct.split_ms = timer.ms();
        timer.reset();
        std::string outDir = outBaseDir + "/vector";
        baseMgr.writeFile(outDir + "/" + stem + "_passed.txt", passed);
        baseMgr.writeFile(outDir + "/" + stem + "_failed.txt", failed);
        ct.write_ms = timer.ms();

    } else if (type == ContainerType::List) {
        std::list<Person> container(allVec.begin(), allVec.end());
        std::list<Person> passed, failed;
        for (const auto& s : container) {
            double score = useMedian ? s.finalMed() : s.finalAvg();
            if (score >= 5.0) passed.push_back(s);
            else failed.push_back(s);
        }
        ct.split_ms = timer.ms();
        timer.reset();
        std::string outDir = outBaseDir + "/list";
        std::vector<Person> pvec(passed.begin(), passed.end());
        std::vector<Person> fvec(failed.begin(), failed.end());
        baseMgr.writeFile(outDir + "/" + stem + "_passed.txt", pvec);
        baseMgr.writeFile(outDir + "/" + stem + "_failed.txt", fvec);
        ct.write_ms = timer.ms();

    } else { // Deque
        std::deque<Person> container(allVec.begin(), allVec.end());
        std::deque<Person> passed, failed;
        for (const auto& s : container) {
            double score = useMedian ? s.finalMed() : s.finalAvg();
            if (score >= 5.0) passed.push_back(s);
            else failed.push_back(s);
        }
        ct.split_ms = timer.ms();
        timer.reset();
        std::string outDir = outBaseDir + "/deque";
        std::vector<Person> pvec(passed.begin(), passed.end());
        std::vector<Person> fvec(failed.begin(), failed.end());
        baseMgr.writeFile(outDir + "/" + stem + "_passed.txt", pvec);
        baseMgr.writeFile(outDir + "/" + stem + "_failed.txt", fvec);
        ct.write_ms = timer.ms();
    }

    return ct;
}
