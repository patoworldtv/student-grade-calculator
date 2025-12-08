#include "Timer.h"

Timer::Timer() : start_(std::chrono::high_resolution_clock::now()) {}
void Timer::reset() { start_ = std::chrono::high_resolution_clock::now(); }
long long Timer::ms() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(high_resolution_clock::now() - start_).count();
}
