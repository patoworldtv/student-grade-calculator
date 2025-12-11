#include "Person.h"
#include <algorithm>
#include <numeric>
#include <sstream>

// Constructor
Person::Person(std::string n, std::string s, const std::vector<int>& hw, int ex)
    : name(std::move(n)), surname(std::move(s)), homework(hw), exam(ex) {}

// Stream input
std::istream& operator>>(std::istream& in, Person& p) {
    std::string line;
    if (!std::getline(in, line)) return in;
    if (line.empty()) return in;

    std::istringstream ss(line);
    ss >> p.name >> p.surname;

    p.homework.clear();
    p.exam = 0;
    int val;
    std::vector<int> nums;
    while (ss >> val) nums.push_back(val);
    if (!nums.empty()) {
        p.exam = nums.back();
        nums.pop_back();
        p.homework = std::move(nums);
    }
    return in;
}

// Stream output
std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << p.name << " " << p.surname;
    for (int hw : p.homework) out << " " << hw;
    out << " " << p.exam;
    return out;
}

// Average of homework
double Person::avg() const {
    if (homework.empty()) return 0.0;
    double sum = std::accumulate(homework.begin(), homework.end(), 0.0);
    return sum / static_cast<double>(homework.size());
}

// Median of homework
double Person::median() const {
    if (homework.empty()) return 0.0;
    std::vector<int> tmp = homework;
    std::sort(tmp.begin(), tmp.end());
    size_t n = tmp.size();
    if (n % 2 == 0) return (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
    return tmp[n / 2];
}

// Final grade using average
double Person::finalAvg() const {
    return 0.4 * avg() + 0.6 * static_cast<double>(exam);
}

// Final grade using median
double Person::finalMed() const {
    return 0.4 * median() + 0.6 * static_cast<double>(exam);
}
