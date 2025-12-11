#ifndef EXCEPTIONHANDLERS_H
#define EXCEPTIONHANDLERS_H

#include <stdexcept>
#include <string>

class FileException : public std::runtime_error {
public:
    explicit FileException(const std::string& msg)
        : std::runtime_error("FileException: " + msg) {}
};

class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& msg)
        : std::runtime_error("ParseException: " + msg) {}
};

class GeneratorException : public std::runtime_error {
public:
    explicit GeneratorException(const std::string& msg)
        : std::runtime_error("GeneratorException: " + msg) {}
};

#endif // EXCEPTIONHANDLERS_H
