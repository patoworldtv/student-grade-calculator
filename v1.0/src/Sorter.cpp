#include "Sorter.h"
#include <algorithm>

namespace Sorter {

void sortVectorDesc(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return a.finalAvgCached() > b.finalAvgCached();
    });
}

void sortDequeDesc(std::deque<Student>& d) {
    std::sort(d.begin(), d.end(), [](const Student& a, const Student& b) {
        return a.finalAvgCached() > b.finalAvgCached();
    });
}

void sortListDesc(std::list<Student>& l) {
    l.sort([](const Student& a, const Student& b) {
        return a.finalAvgCached() > b.finalAvgCached();
    });
}

}
