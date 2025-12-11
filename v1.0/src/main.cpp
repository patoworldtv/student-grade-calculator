#include <iostream>
#include <filesystem>
#include "Analyzer.h"
#include "FileGenerator.h"

using namespace std;

void runPerformance(const string& file) {
    using enum SplitStrategy;

    cout << "\n=== VECTOR (COPY) ===\n";
    auto v1 = Analyzer::runVector(file, "v_pass.txt", "v_fail.txt", COPY);

    cout << "=== VECTOR (INPLACE) ===\n";
    auto v2 = Analyzer::runVector(file, "v_pass2.txt", "v_fail2.txt", INPLACE);

    cout << "\n=== LIST (COPY) ===\n";
    auto l1 = Analyzer::runList(file, "l_pass.txt", "l_fail.txt", COPY);

    cout << "\n=== LIST (INPLACE) ===\n";
    auto l2 = Analyzer::runList(file, "l_pass2.txt", "l_fail2.txt", INPLACE);

    cout << "\n=== DEQUE (COPY) ===\n";
    auto d1 = Analyzer::runDeque(file, "d_pass.txt", "d_fail.txt", COPY);

    cout << "\n=== DEQUE (INPLACE) ===\n";
    auto d2 = Analyzer::runDeque(file, "d_pass2.txt", "d_fail2.txt", INPLACE);
}

int main(){
    filesystem::create_directories("data");

    cout << "Input file: ";
    string f; getline(cin, f);

    if (!filesystem::exists(f)) {
        cout << "File doesn't exist.\n";
        return 0;
    }

    runPerformance(f);
}
