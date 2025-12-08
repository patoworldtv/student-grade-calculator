#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "StudentManager.h"
#include "ContainerManager.h"

namespace fs = std::filesystem;

void printMenu() {
    std::cout << "\n===== v0.25 Student Processor =====\n";
    std::cout << "1. Generate data files (1k, 10k, 100k, 1M, 10M)\n";
    std::cout << "2. Process and time all data files (avg-based, vector)\n";
    std::cout << "3. Process and time all data files (median-based, vector)\n";
    std::cout << "5. Benchmark vector/list/deque (avg-based)\n";
    std::cout << "6. Benchmark vector/list/deque (median-based)\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    StudentManager mgr;
    ContainerManager cmgr;
    const std::string dataDir = "data";
    const std::string outDir = "out";
    const std::vector<size_t> sizes = {1000, 10000, 100000, 1000000, 10000000};

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) break;

        try {
            if (choice == 1) {
                std::cout << "Generating files...\n";
                mgr.generateDataFilesWithSizes(dataDir, sizes, 5);
                std::cout << "Done.\n";

            } else if (choice == 2 || choice == 3) {
                bool useMedian = (choice == 3);
                for (size_t n : sizes) {
                    std::string filePath = dataDir + "/students_" + std::to_string(n) + ".txt";
                    if (!fs::exists(filePath)) {
                        std::cout << "Missing: " << filePath << " — generate first (option 1).\n";
                        continue;
                    }
                    std::cout << "\n[v0.25 vector] Processing " << filePath << " (" << n << " records)...\n";
                    auto t = mgr.processFile(filePath, outDir, useMedian);
                    std::cout << "Read:  " << t.read_ms  << " ms\n";
                    std::cout << "Split: " << t.split_ms << " ms\n";
                    std::cout << "Write: " << t.write_ms << " ms\n";
                    std::cout << "Total: " << (t.read_ms + t.split_ms + t.write_ms) << " ms\n";
                    std::cout << "Records: " << t.total_records << "\n";
                }

            } else if (choice == 5 || choice == 6) {
                bool useMedian = (choice == 6);
                for (size_t n : sizes) {
                    std::string filePath = dataDir + "/students_" + std::to_string(n) + ".txt";
                    if (!fs::exists(filePath)) {
                        std::cout << "Missing: " << filePath << " — generate first (option 1).\n";
                        continue;
                    }
                    auto tv = cmgr.processWithContainer(filePath, outDir, ContainerType::Vector, useMedian, mgr);
                    auto tl = cmgr.processWithContainer(filePath, outDir, ContainerType::List,   useMedian, mgr);
                    auto td = cmgr.processWithContainer(filePath, outDir, ContainerType::Deque,  useMedian, mgr);

                    std::cout << "\n[v0.25 benchmark] " << filePath << " (" << n << ")\n";
                    std::cout << "  vector: Read " << tv.read_ms << " ms, Split " << tv.split_ms
                              << " ms, Write " << tv.write_ms << " ms\n";
                    std::cout << "  list:   Read " << tl.read_ms << " ms, Split " << tl.split_ms
                              << " ms, Write " << tl.write_ms << " ms\n";
                    std::cout << "  deque:  Read " << td.read_ms << " ms, Split " << td.split_ms
                              << " ms, Write " << td.write_ms << " ms\n";
                }

            } else if (choice == 4) {
                std::cout << "Goodbye!\n";
                break;

            } else {
                std::cout << "Invalid choice.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
