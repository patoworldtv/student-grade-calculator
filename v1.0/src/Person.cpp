#include "Person.h"
#include <algorithm>
#include <numeric>
#include <sstream>

Person::Person() : name_(), surname_(), homework_(), exam_(0), finalAvgCached_(-1.0) {}

Person::Person(std::string n, std::string s, const std::vector<int>& hw, int ex)
    : name_(std::move(n)), surname_(std::move(s)), homework_(hw), exam_(ex), finalAvgCached_(-1.0) {
    computeCache();
}

void Person::computeCache() {
    finalAvgCached_ = finalAvg();
}

std::istream& operator>>(std::istream& in, Person& p) {
    std::string line;
    if (!std::getline(in, line)) return in;

    if (line.empty()) {
        p = Person{};
        return in;
    }

    std::istringstream ss(line);

    p.name_.clear();
    p.surname_.clear();
    p.homework_.clear();
    p.exam_ = 0;
    p.finalAvgCached_ = -1.0;

    ss >> p.name_ >> p.surname_;

    int x;
    std::vector<int> nums;
    while (ss >> x) nums.push_back(x);

    if (!nums.empty()) {
        p.exam_ = nums.back();
        nums.pop_back();
        p.homework_ = std::move(nums);
    }

    if (p.isValidBasic()) p.computeCache();
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << p.getName() << " " << p.getSurname();
    for (int hw : p.getHomework()) out << " " << hw;
    out << " " << p.getExam();
    return out;
}

double Person::avg() const {
    if (homework_.empty()) return 0.0;
    double sum = std::accumulate(homework_.begin(), homework_.end(), 0.0);
    return sum / static_cast<double>(homework_.size());
}

double Person::median() const {
    if (homework_.empty()) return 0.0;
    std::vector<int> tmp = homework_;
    std::sort(tmp.begin(), tmp.end());
    const size_t n = tmp.size();
    if (n % 2 == 0) return (tmp[n/2 - 1] + tmp[n/2]) / 2.0;
    return tmp[n/2];
}

double Person::finalAvg() const {
    return 0.4 * avg() + 0.6 * static_cast<double>(exam_);
}

double Person::finalMed() const {
    return 0.4 * median() + 0.6 * static_cast<double>(exam_);
}
