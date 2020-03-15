#include "tracer.hpp"

namespace {
PointPtr getMatched(const std::vector<Edge> &edges, PointPtr ptr) {
    for (const auto &e : edges) 
        if (ptr == e.in) 
            return e.out;
    return nullptr;
}

PointPtr getMatched(const std::vector<Edge> &edges, const Trace &trace) {
    return getMatched(edges, trace.points.front());
}
}

Trace::Trace(std::forward_list<PointPtr> p) : points(p) {}

void Tracer::updateTraces(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes, const std::vector<Edge> &edges) {
    std::unordered_map<PointPtr, bool> wasMatched;
    for (auto it = activeTraces.begin(); it != activeTraces.end();) {
        // addr bodge sry not sry
        wasMatched[it->points.front()] = true;
        auto out = getMatched(edges, *it);

        if (out) {
            // close trace if no edge was found
            traces.emplace_back(std::move(*it));
            it = activeTraces.erase(it);
        } else {
            // when edge is found extend trace
            it->points.emplace_front(std::move(out));
            ++it;
        }
    }

    // add new traces
    for (int i = 0; i < inNodes.size(); ++i) {
        auto in = &inNodes.at(i);
        auto out = getMatched(edges, in);
        // node has edge and was not previously matched
        if (out && !wasMatched[in])
            activeTraces.emplace_back(std::forward_list{in, out});
    }
}

void Tracer::terminateTraces() {
    while (!activeTraces.empty()) {
        traces.emplace_back(std::move(activeTraces.back()));
        activeTraces.pop_back();
    }
}

void Tracer::dumpToFile(const char filename[]) const {
    std::ofstream out{filename};
    out.seekp(0);
    for (const auto &trace : traces) {
        for (const auto &point : trace.points)
            out << point->x << " ";
        out << std::endl;
        for (const auto &point : trace.points)
            out << point->y << " ";
        out << std::endl;
    }
    out.close();
}
