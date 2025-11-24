#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>
#include <filesystem>
#include "Person.h"

void printMenu() {
    std::cout << "\n===== Student Grade Calculator =====\n";
    std::cout << "1. Read students from file\n";
    std::cout << "2. Enter new student data manually\n";
    std::cout << "3. Display highest and lowest scores (from all files)\n";
    std::cout << "4. Show random student (from all files)\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option: ";
}

void loadFromFile(std::vector<Person>& students) {
    std::string fileName;
    std::cout << "Enter file path (example: data/students10000.txt): ";
    std::cin >> fileName;

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "FAILED: Cannot open " << fileName << "\n";
        return;
    }

    Person temp;
    std::string line;
    students.clear(); // To remove old data before loading new

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> temp;
        students.push_back(temp);
    }

    std::cout << "SUCCESS: Loaded data from " << fileName << "\n";

    // PRINT STUDENTS IMMEDIATELY
    std::cout << "\n--- Loaded Students ---\n";
    std::cout << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::setw(15) << "Final (Avg)"
              << std::setw(15) << "Final (Med)" << "\n";
    std::cout << "--------------------------------------------------------\n";

    for (const auto& s : students) {
        std::cout << std::left << std::setw(15) << s.getName()
                  << std::setw(15) << s.getSurname()
                  << std::setw(15) << std::fixed << std::setprecision(2) << s.finalAvg()
                  << std::setw(15) << std::fixed << std::setprecision(2) << s.finalMed()
                  << "\n";
    }
}


void manualEntry(std::vector<Person>& students) {
    std::cin.ignore();

    while (true) {
        std::string name, surname;
        int hwCount, exam;

        std::cout << "\nEnter student name (or empty to finish): ";
        std::getline(std::cin, name);
        if (name.empty()) break;

        std::cout << "Enter surname: ";
        std::getline(std::cin, surname);

        std::cout << "How many homework grades? ";
        std::cin >> hwCount;

        std::vector<int> hw(hwCount);
        for (int i = 0; i < hwCount; i++) {
            std::cout << "HW" << (i + 1) << ": ";
            std::cin >> hw[i];
        }

        std::cout << "Exam grade: ";
        std::cin >> exam;
        std::cin.ignore();

        Person temp(name, surname, hw, exam);
        students.push_back(temp);

        std::cout << "\n--- Calculated Results ---\n";
        std::cout << "Average HW:  " << temp.avg() << "\n";
        std::cout << "Median HW:   " << temp.median() << "\n";
        std::cout << "Final (Avg): " << temp.finalAvg() << "\n";
        std::cout << "Final (Med): " << temp.finalMed() << "\n";
    }

    std::cout << "\nSUCCESS: Manual data added.\n";
}

std::vector<Person> loadAllStudentFiles(const std::string& folder) {
    std::vector<Person> students;

    // SAFETY CHECK — prevents crash
    if (!std::filesystem::exists(folder)) {
        std::cout << "ERROR: Folder \"" << folder << "\" does not exist.\n";
        return students;
    }

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;

        std::string fileName = entry.path().string();

        if (fileName.find("students") != std::string::npos &&
            fileName.find(".txt") != std::string::npos) 
        {
            std::ifstream file(fileName);
            if (!file.is_open()) continue;

            Person temp;
            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                ss >> temp;
                students.push_back(temp);
            }

            std::cout << "Loaded: " << fileName << "\n";
        }
    }
    return students;
}

void displayHighLowFromAllFiles(const std::string& folder) {
    auto students = loadAllStudentFiles(folder);
    if (students.empty()) {
        std::cout << "No valid student files found in \"" << folder << "\".\n";
        return;
    }

    auto max_student = *std::max_element(students.begin(), students.end(),
        [](const Person& a, const Person& b) { return a.finalAvg() < b.finalAvg(); });

    auto min_student = *std::min_element(students.begin(), students.end(),
        [](const Person& a, const Person& b) { return a.finalAvg() < b.finalAvg(); });

    std::cout << "\nHighest Score: " << max_student.getName() << " " 
              << max_student.getSurname() << " (" 
              << std::fixed << std::setprecision(2) << max_student.finalAvg() << ")\n";

    std::cout << "Lowest Score: " << min_student.getName() << " "
              << min_student.getSurname() << " ("
              << std::fixed << std::setprecision(2) << min_student.finalAvg() << ")\n";

    std::cout << "Total students loaded: " << students.size() << "\n";
}

void showRandomStudentFromAllFiles(const std::string& folder) {
    auto students = loadAllStudentFiles(folder);
    if (students.empty()) {
        std::cout << "No valid student files found in \"" << folder << "\".\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, students.size() - 1);

    const Person& s = students[dist(gen)];

    std::cout << "\n-----------------------------------------------\n";
    std::cout << std::left << std::setw(10) << "Name"
              << std::setw(12) << "Surname"
              << std::setw(15) << "Final (Avg.)"
              << " | "
              << std::setw(12) << "Final (Med.)"
              << "\n-----------------------------------------------\n";

    std::cout << std::left << std::setw(10) << s.getName()
              << std::setw(12) << s.getSurname()
              << std::setw(15) << std::fixed << std::setprecision(2) << s.finalAvg()
              << " | "
              << std::setw(12) << std::fixed << std::setprecision(2) << s.finalMed()
              << "\n";
}

int main() {
    std::vector<Person> students;
    int choice;

    while (true) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: loadFromFile(students); break;
            case 2: manualEntry(students); break;
            case 3: displayHighLowFromAllFiles("data"); break;
            case 4: showRandomStudentFromAllFiles("data"); break;
            case 5:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}
