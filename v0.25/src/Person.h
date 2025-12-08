#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <vector>
#include <string>

class Person {
private:
    std::string name;
    std::string surname;
    std::vector<int> homework;
    int exam;

public:
    // Constructors
    Person();
    Person(std::string n, std::string s, const std::vector<int>& hw, int ex);

    // Rule of Three
    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    // Input & Output
    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

    // Methods
    double avg() const;
    double median() const;
    double finalAvg() const;
    double finalMed() const;

    // Getters
    std::string getName() const { return name; }
    std::string getSurname() const { return surname; }
    std::vector<int> getHomework() const { return homework; }
    int getExam() const { return exam; }
};

#endif
