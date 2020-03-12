#pragma once

#include "matching.hpp"
#include <list>

struct Trace {
    std::vector<Point> points;
    int last_i;
};

class Tracer {
private:
    std::vector<Trace> traces;
    std::list<Trace> activeTraces;

public:
    Tracer() = default;
    void updateTraces(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes, const std::vector<Edge> &edges);
    void dumpToFile(const char filename[]) const;
    void terminateTraces();
};
