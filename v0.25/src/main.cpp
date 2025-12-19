#include "Analyzer.h"
#include "FileGenerator.h"
#include "ExceptionHandlers.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>

namespace fs = std::filesystem;
using namespace std;

static void printMenu() {
    cout << "\n====== Student Grade Calculator (v0.25) ======\n";
    cout << "1) Generate data files (default: 1k,10k,100k,1M,10M)\n";
    cout << "2) Run performance test (vector/deque/list) on ONE file\n";
    cout << "3) Run performance test on ALL default sizes in folder\n";
    cout << "4) Exit\n";
    cout << "Choose: ";
}

static vector<uint64_t> parseCountsOrDefault(const string& line) {
    if (line.empty()) return {1000, 10000, 100000, 1000000, 10000000};
    vector<uint64_t> counts;
    istringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        try {
            // trim spaces
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            counts.push_back(stoull(token));
        } catch (...) {
            // ignore bad token
        }
    }
    if (counts.empty()) return {1000, 10000, 100000, 1000000, 10000000};
    return counts;
}

static int parseIntOrDefault(const string& s, int def) {
    if (s.empty()) return def;
    try { return stoi(s); } catch (...) { return def; }
}

static void optionGenerate() {
    try {
        cout << "Output folder (default: data/generated): ";
        string folder; getline(cin, folder);
        if (folder.empty()) folder = "data/generated";

        cout << "Counts comma-separated (default: 1000,10000,100000,1000000,10000000): ";
        string line; getline(cin, line);
        auto counts = parseCountsOrDefault(line);

        cout << "HW per student (default 5): ";
        string hwline; getline(cin, hwline);
        int hw = parseIntOrDefault(hwline, 5);

        FileGenerator::generateFilesForCounts(folder, counts, hw);
    } catch (const exception& e) {
        cerr << "Generator error: " << e.what() << "\n";
    }
}

static void runAllPipelinesOnFile(const string& input) {
    if (!fs::exists(input)) {
        cout << "File does not exist: " << input << "\n";
        return;
    }

    const string vecPass  = input + ".vector.passed.txt";
    const string vecFail  = input + ".vector.failed.txt";
    const string deqPass  = input + ".deque.passed.txt";
    const string deqFail  = input + ".deque.failed.txt";
    const string listPass = input + ".list.passed.txt";
    const string listFail = input + ".list.failed.txt";

    try {
        cout << "\n--- Running pipelines for: " << input << " ---\n";

        auto v = Analyzer::runVectorPipeline(input, vecPass, vecFail);
        auto d = Analyzer::runDequePipeline(input,  deqPass, deqFail);
        auto l = Analyzer::runListPipeline(input,  listPass, listFail);

        Analyzer::printPerf("Vector:", v);
        Analyzer::printPerf("Deque: ", d);
        Analyzer::printPerf("List:  ", l);
        cout << "Output files created next to input file.\n";

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}

static void optionPerformanceOne() {
    cout << "Enter input file path (e.g., data/generated/students_10000.txt): ";
    string input; getline(cin, input);
    if (input.empty()) { cout << "No file specified.\n"; return; }
    runAllPipelinesOnFile(input);
}

static void optionPerformanceAll() {
    cout << "Folder containing generated files (default: data/generated): ";
    string folder; getline(cin, folder);
    if (folder.empty()) folder = "data/generated";

    // Default required sizes
    vector<uint64_t> sizes = {1000, 10000, 100000, 1000000, 10000000};

    cout << "\n=== Batch performance (vector/deque/list) ===\n";
    for (auto s : sizes) {
        string input = folder + "/students_" + to_string(s) + ".txt";
        runAllPipelinesOnFile(input);
    }
}

int main() {
    // Ensure data folder exists
    fs::create_directories("data/generated");

    while (true) {
        printMenu();
        string choice; getline(cin, choice);
        if (choice.empty()) continue;

        int c = -1;
        try { c = stoi(choice); } catch (...) { c = -1; }

        switch (c) {
            case 1: optionGenerate(); break;
            case 2: optionPerformanceOne(); break;
            case 3: optionPerformanceAll(); break;
            case 4: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}
