#include "Person.h"
#include <algorithm>
#include <iomanip>
#include <numeric>

Person::Person() : name(""), surname(""), exam(0) {}

Person::Person(std::string n, std::string s, const std::vector<int>& hw, int ex)
    : name(n), surname(s), homework(hw), exam(ex) {}

// Constructor
Person::Person(const Person& other)
    : name(other.name), surname(other.surname),
      homework(other.homework), exam(other.exam) {}

// Assignment
Person& Person::operator=(const Person& other) {
    if (this != &other) {
        name = other.name;
        surname = other.surname;
        homework = other.homework;
        exam = other.exam;
    }
    return *this;
}

// Destructor
Person::~Person() {}

std::istream& operator>>(std::istream& in, Person& p) {
    p.homework.clear();

    int value;
    in >> p.name >> p.surname;

    // Read all homework until exam (last number)
    std::vector<int> data;
    while (in >> value) data.push_back(value);

    if (!data.empty()) {
        p.exam = data.back();
        data.pop_back();
        p.homework = data;
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left << std::setw(15) << p.name
        << std::left << std::setw(15) << p.surname
        << std::fixed << std::setprecision(2)
        << std::setw(15) << p.finalAvg()
        << std::setw(15) << p.finalMed();
    return out;
}

double Person::avg() const {
    if (homework.empty()) return 0;
    return (double) std::accumulate(homework.begin(), homework.end(), 0) / homework.size();
}

double Person::median() const {
    if (homework.empty()) return 0;
    std::vector<int> tmp = homework;
    std::sort(tmp.begin(), tmp.end());
    int size = tmp.size();
    if (size % 2 == 0)
        return (tmp[size/2 - 1] + tmp[size/2]) / 2.0;
    else 
        return tmp[size/2];
}

double Person::finalAvg() const {
    return 0.4 * avg() + 0.6 * exam;
}

double Person::finalMed() const {
    return 0.4 * median() + 0.6 * exam;
}
