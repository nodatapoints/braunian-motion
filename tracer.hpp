#pragma once

#include "matching.hpp"
#include <list>
#include <forward_list>
#include <unordered_map>

struct Trace {
    std::forward_list<PointPtr> points;

    Trace() = default;
    Trace(std::forward_list<PointPtr> p);
    Trace(const Trace&) = default;
    Trace(Trace &&trace) = default;
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
