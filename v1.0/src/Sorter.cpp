#include "Sorter.h"
#include <algorithm>

namespace Sorter {

void sort(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(),
        [](const Student& a, const Student& b){
            return a.finalAvg() > b.finalAvg();
        });
}

void sort(std::deque<Student>& d) {
    std::sort(d.begin(), d.end(),
        [](const Student& a, const Student& b){
            return a.finalAvg() > b.finalAvg();
        });
}

void sort(std::list<Student>& l) {
    l.sort([](const Student& a, const Student& b){
        return a.finalAvg() > b.finalAvg();
    });
}

}
