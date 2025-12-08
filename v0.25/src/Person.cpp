#include "Person.h"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <stdexcept>

Person::Person() : name(""), surname(""), exam(0) {}

Person::Person(std::string n, std::string s, const std::vector<int>& hw, int ex)
    : name(std::move(n)), surname(std::move(s)), homework(hw), exam(ex) {}

Person::Person(const Person& other)
    : name(other.name), surname(other.surname),
      homework(other.homework), exam(other.exam) {}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        name = other.name;
        surname = other.surname;
        homework = other.homework;
        exam = other.exam;
    }
    return *this;
}

Person::~Person() {}

std::istream& operator>>(std::istream& in, Person& p) {
    p.homework.clear();
    std::string line;
    if (!std::getline(in, line)) return in;

    std::istringstream ss(line);
    if (!(ss >> p.name >> p.surname)) {
        throw std::runtime_error("Invalid input: missing name or surname");
    }

    std::vector<int> data;
    int value;
    while (ss >> value) data.push_back(value);

    if (data.empty()) {
        throw std::runtime_error("Invalid input: no grades provided");
    }

    p.exam = data.back();
    data.pop_back();
    p.homework = std::move(data);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left << std::setw(15) << p.getName()
        << std::left << std::setw(15) << p.getSurname()
        << std::fixed << std::setprecision(2)
        << std::setw(15) << p.finalAvg()
        << std::setw(15) << p.finalMed();
    return out;
}

double Person::avg() const {
    if (homework.empty()) return 0.0;
    return static_cast<double>(
        std::accumulate(homework.begin(), homework.end(), 0)
    ) / homework.size();
}

double Person::median() const {
    if (homework.empty()) return 0.0;
    std::vector<int> tmp = homework;
    std::sort(tmp.begin(), tmp.end());
    const int size = static_cast<int>(tmp.size());
    if (size % 2 == 0)
        return (tmp[size/2 - 1] + tmp[size/2]) / 2.0;
    return tmp[size/2];
}

double Person::finalAvg() const { return 0.4 * avg() + 0.6 * exam; }
double Person::finalMed() const { return 0.4 * median() + 0.6 * exam; }
