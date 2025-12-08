#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "StudentManager.h"

namespace fs = std::filesystem;

void printMenu() {
    std::cout << "\n===== v0.2 Student Processor =====\n";
    std::cout << "1. Generate data files (10k, 100k, 1M, 10M)\n";
    std::cout << "2. Process and time all data files (avg-based)\n";
    std::cout << "3. Process and time all data files (median-based)\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    StudentManager mgr;
    const std::string dataDir = "data";
    const std::string outDir = "out";
    const std::vector<size_t> sizes = {10000, 100000, 1000000, 10000000};

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) break;

        try {
            if (choice == 1) {
                std::cout << "Generating files...\n";
                mgr.generateDataFiles(dataDir, 5);
                std::cout << "Done.\n";
            } else if (choice == 2 || choice == 3) {
                bool useMedian = (choice == 3);
                for (size_t n : sizes) {
                    std::string filePath = dataDir + "/students_" + std::to_string(n) + ".txt";
                    if (!fs::exists(filePath)) {
                        std::cout << "Missing: " << filePath << " — generate first (option 1).\n";
                        continue;
                    }
                    std::cout << "\nProcessing " << filePath << " (" << n << " records)...\n";
                    auto t = mgr.processFile(filePath, outDir, useMedian);
                    std::cout << "Read:  " << t.read_ms  << " ms\n";
                    std::cout << "Split: " << t.split_ms << " ms\n";
                    std::cout << "Write: " << t.write_ms << " ms\n";
                    std::cout << "Total: " << (t.read_ms + t.split_ms + t.write_ms) << " ms\n";
                    std::cout << "Records: " << t.total_records << "\n";
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
