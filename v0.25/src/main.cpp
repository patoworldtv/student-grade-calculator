#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <random>
#include "Person.h"
#include "Student.h"
#include "FileGenerator.h"
#include "Analyzer.h"
#include "ExceptionHandlers.h"

namespace fs = std::filesystem;
using namespace std;

void printMenu() {
    cout << "\n====== Student Grade Calculator (v0.25) ======\n";
    cout << "1. Generate sample data files (10k,100k,1M,10M) [configurable]\n";
    cout << "2. Load & display students from folder\n";
    cout << "3. Add manual student(s) and optionally save\n";
    cout << "4. Display highest & lowest (across folder)\n";
    cout << "5. Show random student (across folder)\n";
    cout << "6. Run performance tests (vector/list/deque) on a chosen file\n";
    cout << "7. Exit\n";
    cout << "Choose an option: ";
}

void optionGenerate() {
    try {
        cout << "Output folder (default: data/generated): ";
        string folder; getline(cin, folder);
        if (folder.empty()) folder = "data/generated";

        cout << "Enter counts comma-separated (default: 10000,100000,1000000,10000000): ";
        string line; getline(cin, line);
        vector<uint64_t> counts;
        if (line.empty()) counts = {10000, 100000, 1000000, 10000000};
        else {
            istringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                try { counts.push_back(stoull(token)); } catch(...) {}
            }
            if (counts.empty()) counts = {10000, 100000, 1000000, 10000000};
        }

        cout << "HW per student (default 5): ";
        string hwline; getline(cin, hwline);
        int hw = 5;
        if (!hwline.empty()) {
            try { hw = stoi(hwline); } catch(...) { hw = 5; }
        }

        FileGenerator::generateFilesForCounts(folder, counts, hw);
    } catch (const std::exception& e) {
        cerr << "Generator error: " << e.what() << "\n";
    }
}

void optionLoadDisplay() {
    try {
        cout << "Enter folder to read (default: data/Students): ";
        string folder; getline(cin, folder);
        if (folder.empty()) folder = "data/Students";

        auto all = Analyzer::readAllFromFolder(folder);
        if (all.empty()) {
            cout << "No student records found in " << folder << "\n";
            return;
        }

        cout << left << setw(15) << "Name" << setw(15) << "Surname" << setw(12) << "FinalAvg" << setw(12) << "FinalMed" << "\n";
        cout << string(60, '-') << "\n";
        size_t toShow = min<size_t>(all.size(), 200);
        for (size_t i = 0; i < toShow; ++i) {
            const auto& s = all[i];
            cout << left << setw(15) << s.getName()
                 << setw(15) << s.getSurname()
                 << setw(12) << fixed << setprecision(2) << s.finalAvg()
                 << setw(12) << fixed << setprecision(2) << s.finalMed() << "\n";
        }
        cout << "Total students: " << all.size() << " (showing up to 200)\n";
    } catch (const FileException& fe) {
        cerr << fe.what() << "\n";
    }
}

void optionManualEntry() {
    vector<Student> students;
    while (true) {
        cout << "Enter name (empty to finish): ";
        string name; getline(cin, name);
        if (name.empty()) break;
        cout << "Enter surname: ";
        string surname; getline(cin, surname);

        cout << "How many homework grades? ";
        string hwc; getline(cin, hwc);
        int hwCount = 0;
        try { hwCount = stoi(hwc); } catch(...) { hwCount = 0; }

        vector<int> hw(hwCount);
        for (int i = 0; i < hwCount; ++i) {
            cout << "HW" << (i+1) << ": ";
            string g; getline(cin, g);
            int gi = 0;
            try { gi = stoi(g); } catch(...) { gi = 0; }
            hw[i] = gi;
        }

        cout << "Exam grade: ";
        string exs; getline(cin, exs);
        int exam = 0;
        try { exam = stoi(exs); } catch(...) { exam = 0; }

        students.emplace_back(name, surname, hw, exam);
        cout << "Added: " << name << " " << surname
             << " FinalAvg=" << fixed << setprecision(2) << students.back().finalAvg()
             << " FinalMed=" << fixed << setprecision(2) << students.back().finalMed() << "\n";
    }

    if (students.empty()) {
        cout << "No students entered.\n";
        return;
    }

    cout << "Save to file (press enter to skip): ";
    string fname; getline(cin, fname);
    if (!fname.empty()) {
        try {
            Analyzer::writeToFile(fname, students);
            cout << "Saved " << students.size() << " students to " << fname << "\n";
        } catch (const FileException& fe) {
            cerr << fe.what() << "\n";
        }
    }
}

void optionHighLow() {
    try {
        cout << "Enter folder to read (default: data/Students): ";
        string folder; getline(cin, folder);
        if (folder.empty()) folder = "data/Students";

        auto all = Analyzer::readAllFromFolder(folder);
        if (all.empty()) { cout << "No student records found.\n"; return; }

        auto maxIt = max_element(all.begin(), all.end(), [](const Student& a, const Student& b){ return a.finalAvg() < b.finalAvg(); });
        auto minIt = min_element(all.begin(), all.end(), [](const Student& a, const Student& b){ return a.finalAvg() < b.finalAvg(); });

        cout << "Highest: " << maxIt->getName() << " " << maxIt->getSurname() << " (" << fixed << setprecision(2) << maxIt->finalAvg() << ")\n";
        cout << "Lowest : " << minIt->getName() << " " << minIt->getSurname() << " (" << fixed << setprecision(2) << minIt->finalAvg() << ")\n";
        cout << "Total: " << all.size() << "\n";
    } catch (const FileException& fe) {
        cerr << fe.what() << "\n";
    }
}

void optionRandom() {
    try {
        cout << "Enter folder to read (default: data/Students): ";
        string folder; getline(cin, folder);
        if (folder.empty()) folder = "data/Students";

        auto all = Analyzer::readAllFromFolder(folder);
        if (all.empty()) { cout << "No student records found.\n"; return; }

        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, all.size()-1);
        const auto& s = all[dist(gen)];

        cout << left << setw(12) << "Name" << setw(12) << "Surname" << setw(12) << "FinalAvg" << setw(12) << "FinalMed" << "\n";
        cout << string(50, '-') << "\n";
        cout << left << setw(12) << s.getName() << setw(12) << s.getSurname()
             << setw(12) << fixed << setprecision(2) << s.finalAvg()
             << setw(12) << fixed << setprecision(2) << s.finalMed() << "\n";
    } catch (const FileException& fe) {
        cerr << fe.what() << "\n";
    }
}

void optionPerformance() {
    cout << "Enter single input file for test (e.g., data/generated/students_10000.txt): ";
    string input; getline(cin, input);
    if (input.empty()) { cout << "No file specified.\n"; return; }

    if (!fs::exists(input)) {
        cout << "File does not exist: " << input << "\n";
        return;
    }

    string vecPass = input + ".vector.pass.txt";
    string vecFail = input + ".vector.fail.txt";
    string deqPass = input + ".deque.pass.txt";
    string deqFail = input + ".deque.fail.txt";
    string listPass = input + ".list.pass.txt";
    string listFail = input + ".list.fail.txt";

    try {
        cout << "\nRunning vector pipeline...\n";
        auto v = Analyzer::runVectorPipeline(input, vecPass, vecFail);
        cout << "Vector: read(ms)=" << v.read_ms << " sort(ms)=" << v.sort_ms << " split(ms)=" << v.split_ms << " write(ms)=" << v.write_ms << " total=" << v.total_students << "\n";

        cout << "Running deque pipeline...\n";
        auto d = Analyzer::runDequePipeline(input, deqPass, deqFail);
        cout << "Deque:  read(ms)=" << d.read_ms << " sort(ms)=" << d.sort_ms << " split(ms)=" << d.split_ms << " write(ms)=" << d.write_ms << " total=" << d.total_students << "\n";

        cout << "Running list pipeline...\n";
        auto l = Analyzer::runListPipeline(input, listPass, listFail);
        cout << "List:   read(ms)=" << l.read_ms << " sort(ms)=" << l.sort_ms << " split(ms)=" << l.split_ms << " write(ms)=" << l.write_ms << " total=" << l.total_students << "\n";
    } catch (const std::exception& e) {
        cerr << "Performance pipelines failed: " << e.what() << "\n";
    }
}

int main() {
    // Ensure data folders exist
    fs::create_directories("data/Students");
    fs::create_directories("data/generated");

    while (true) {
        printMenu();
        string choice; getline(cin, choice);
        if (choice.empty()) continue;
        int c = -1;
        try { c = stoi(choice); } catch(...) { c = -1; }

        switch (c) {
            case 1: optionGenerate(); break;
            case 2: optionLoadDisplay(); break;
            case 3: optionManualEntry(); break;
            case 4: optionHighLow(); break;
            case 5: optionRandom(); break;
            case 6: optionPerformance(); break;
            case 7:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}
