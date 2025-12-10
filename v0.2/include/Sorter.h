#ifndef SORTER_H
#define SORTER_H

#include "Student.h"
#include <vector>
#include <deque>
#include <list>
#include <algorithm>

// Simple sorter utilities to sort different container types by finalAvg descending.

namespace Sorter {

// Sort vector in-place (descending by student.finalAvg()).
void sortVectorDesc(std::vector<Student>& v);

// Sort deque in-place (descending).
void sortDequeDesc(std::deque<Student>& d);

// Sort list in-place (descending).
void sortListDesc(std::list<Student>& l);

} // namespace Sorter

#endif // SORTER_H
