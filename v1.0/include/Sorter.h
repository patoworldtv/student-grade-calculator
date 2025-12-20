#ifndef SORTER_H
#define SORTER_H

#include "Student.h"
#include <vector>
#include <deque>
#include <list>

namespace Sorter {
    void sortVectorDesc(std::vector<Student>& v);
    void sortDequeDesc(std::deque<Student>& d);
    void sortListDesc(std::list<Student>& l);
}

#endif
