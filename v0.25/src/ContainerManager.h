#ifndef CONTAINER_MANAGER_H
#define CONTAINER_MANAGER_H

#include <string>
#include "Person.h"
#include "StudentManager.h"

enum class ContainerType { Vector, List, Deque };

struct ContainerTiming {
    long long read_ms = 0;
    long long split_ms = 0;
    long long write_ms = 0;
    size_t total_records = 0;
};

class ContainerManager {
public:
    ContainerTiming processWithContainer(const std::string& inputPath,
                                         const std::string& outBaseDir,
                                         ContainerType type,
                                         bool useMedian,
                                         StudentManager& baseMgr);
};

#endif
