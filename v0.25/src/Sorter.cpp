#include "Sorter.h"

namespace Sorter {

void sortVectorDesc(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return a.finalAvg() > b.finalAvg();
    });
}

void sortDequeDesc(std::deque<Student>& d) {
    std::sort(d.begin(), d.end(), [](const Student& a, const Student& b) {
        return a.finalAvg() > b.finalAvg();
    });
}

void sortListDesc(std::list<Student>& l) {
    l.sort([](const Student& a, const Student& b) {
        return a.finalAvg() > b.finalAvg();
    });
}

} // namespace Sorter
