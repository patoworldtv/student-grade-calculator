#ifndef SORTER_H
#define SORTER_H

#include <vector>
#include <list>
#include <deque>
#include "Student.h"

namespace Sorter {

void sort(std::vector<Student>& v);
void sort(std::deque<Student>& d);
void sort(std::list<Student>& l);

}

#endif
