#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

class Person {
private:
    std::string name_;
    std::string surname_;
    std::vector<int> homework_;
    int exam_ = 0;

    // Cached final average (computed once after reading / building the student)
    double finalAvgCached_ = -1.0;

public:
    Person();
    Person(std::string n, std::string s, const std::vector<int>& hw, int ex);

    ~Person() = default;
    Person(const Person&) = default;
    Person(Person&&) noexcept = default;
    Person& operator=(const Person&) = default;
    Person& operator=(Person&&) noexcept = default;

    // Record format: Name Surname hw1 hw2 ... exam
    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

    double avg() const;
    double median() const;
    double finalAvg() const;
    double finalMed() const;

    // cache helpers
    void computeCache();
    double finalAvgCached() const { return finalAvgCached_; }

    const std::string& getName() const { return name_; }
    const std::string& getSurname() const { return surname_; }
    const std::vector<int>& getHomework() const { return homework_; }
    int getExam() const { return exam_; }

    bool isValidBasic() const { return !name_.empty() && !surname_.empty(); }
};

#endif // PERSON_H
