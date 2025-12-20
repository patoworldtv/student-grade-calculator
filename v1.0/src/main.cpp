#include "Analyzer.h"
#include "FileGenerator.h"
#include "ExceptionHandlers.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

static void printMenu() {
    std::cout << "\n====== Student Grade Calculator (v1.0) ======\n";
    std::cout << "1) Generate data files (default: 1k,10k,100k,1M,10M)\n";
    std::cout << "2) Run benchmark on ONE file (all containers, both strategies)\n";
    std::cout << "3) Run benchmark on ALL default sizes in folder (all containers, both strategies)\n";
    std::cout << "4) Exit\n";
    std::cout << "Choose: ";
}

static std::string strategyTag(SplitStrategy s) {
    return (s == SplitStrategy::Strategy1_CopyToTwoContainers) ? "S1" : "S2";
}

static std::string pmodeTag(PartitionMode p) {
    return (p == PartitionMode::StablePartition) ? "stable" : "partition";
}

static void runAllOnFile(const std::string& input, SplitStrategy strat, PartitionMode pmode) {
    std::string base = input + ".v1";

    std::string vPass = base + ".vector." + strategyTag(strat) + "." + pmodeTag(pmode) + ".passed.txt";
    std::string vFail = base + ".vector." + strategyTag(strat) + "." + pmodeTag(pmode) + ".failed.txt";

    std::string dPass = base + ".deque." + strategyTag(strat) + "." + pmodeTag(pmode) + ".passed.txt";
    std::string dFail = base + ".deque." + strategyTag(strat) + "." + pmodeTag(pmode) + ".failed.txt";

    std::string lPass = base + ".list." + strategyTag(strat) + "." + pmodeTag(pmode) + ".passed.txt";
    std::string lFail = base + ".list." + strategyTag(strat) + "." + pmodeTag(pmode) + ".failed.txt";

    std::cout << "\n--- Strategy " << strategyTag(strat) << " (" << pmodeTag(pmode) << ") for: " << input << " ---\n";

    auto rv = Analyzer::runVectorPipeline(input, vPass, vFail, strat, pmode);
    Analyzer::printPerf("Vector:", rv);

    auto rd = Analyzer::runDequePipeline(input, dPass, dFail, strat, pmode);
    Analyzer::printPerf("Deque: ", rd);

    auto rl = Analyzer::runListPipeline(input, lPass, lFail, strat, pmode);
    Analyzer::printPerf("List:  ", rl);

    std::cout << "Output files created next to input file.\n";
}

static void optionGenerate() {
    try {
        std::string folder = "data/generated";
        fs::create_directories(folder);

        std::vector<std::uint64_t> counts = {1000, 10000, 100000, 1000000, 10000000};
        FileGenerator::generateFilesForCounts(folder, counts, 5);
    } catch (const std::exception& e) {
        std::cerr << "Generate error: " << e.what() << "\n";
    }
}

static void optionRunOne() {
    std::cout << "Enter input file path (e.g., data/generated/students_10000.txt): ";
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) return;
    if (!fs::exists(input)) {
        std::cout << "File does not exist: " << input << "\n";
        return;
    }

    // run both strategies, both partition modes
    runAllOnFile(input, SplitStrategy::Strategy1_CopyToTwoContainers, PartitionMode::Partition);
    runAllOnFile(input, SplitStrategy::Strategy2_MoveFailAndShrinkBase, PartitionMode::Partition);
    runAllOnFile(input, SplitStrategy::Strategy2_MoveFailAndShrinkBase, PartitionMode::StablePartition);
}

static void optionRunAll() {
    std::cout << "Folder containing generated files (default: data/generated): ";
    std::string folder;
    std::getline(std::cin, folder);
    if (folder.empty()) folder = "data/generated";

    std::vector<std::uint64_t> counts = {1000, 10000, 100000, 1000000, 10000000};

    for (auto c : counts) {
        std::string file = folder + "/students_" + std::to_string(c) + ".txt";
        if (!fs::exists(file)) {
            std::cout << "Missing: " << file << " (skip)\n";
            continue;
        }

        runAllOnFile(file, SplitStrategy::Strategy1_CopyToTwoContainers, PartitionMode::Partition);
        runAllOnFile(file, SplitStrategy::Strategy2_MoveFailAndShrinkBase, PartitionMode::Partition);
        runAllOnFile(file, SplitStrategy::Strategy2_MoveFailAndShrinkBase, PartitionMode::StablePartition);
    }
}

int main() {
    fs::create_directories("data/generated");

    while (true) {
        printMenu();
        std::string choice;
        std::getline(std::cin, choice);

        int c = 0;
        try { c = std::stoi(choice); } catch (...) { c = 0; }

        if (c == 1) optionGenerate();
        else if (c == 2) optionRunOne();
        else if (c == 3) optionRunAll();
        else if (c == 4) {
            std::cout << "Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}
