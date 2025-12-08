#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    void reset();
    long long ms() const;   // elapsed milliseconds

private:
    std::chrono::high_resolution_clock::time_point start_;
};

#endif
