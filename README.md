# Student Grade Calculator (v0.1)

Small C++ console application for storing students' homework and exam results
and calculating their final grade using either an average or a median of homework
scores. Implements the Rule of Three for the `Person` class and uses `std::vector`
for dynamically-sized homework lists.


# Final grade formulas

## Using average:
Final = 0.4 * Average(Homework) + 0.6 * Exam

## Using median:
Final = 0.4 * Median(Homework) + 0.6 * Exam

# Files
## v0.1/Person.h — class declaration
## v0.1/Person.cpp — class implementation (rule of three, calculations, IO)
## v0.1/main.cpp — program entry, user interaction
## Students.txt — example input file format
