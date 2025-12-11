#ifndef PERSON_H
#define PERSON_H

#include <vector>
#include <string>
#include <iostream>

class Person {
private:
    std::string name;
    std::string surname;
    std::vector<int> homework;
    int exam{0};

public:
    // Constructors
    Person() = default;
    Person(std::string n, std::string s, const std::vector<int>& hw, int ex);

    // Rule of zero for special members
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;
    ~Person() = default;

    // Stream operators
    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

    // Grade calculations
    double avg() const;
    double median() const;
    double finalAvg() const;
    double finalMed() const;

    // Accessors
    const std::string& getName() const { return name; }
    const std::string& getSurname() const { return surname; }
};

#endif
