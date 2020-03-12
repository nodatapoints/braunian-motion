#include "tracer.hpp"

static int getMatched(const std::vector<Edge> &edges, int i) {
    for (const auto &e : edges) 
        if (i == e.in) 
            return e.out;
    return -1;
}

static int getMatched(const std::vector<Edge> &edges, const Trace &trace) {
    return getMatched(edges, trace.last_i);
}

void Tracer::updateTraces(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes, const std::vector<Edge> &edges) {
    std::vector wasMatched(inNodes.size(), false);
    for (auto it = activeTraces.begin(); it != activeTraces.end(); ++it) {
        int outIndex = getMatched(edges, *it);
        if (outIndex < 0) {
            // close trace if no edge was found
            traces.push_back(std::move(*it));
            it = activeTraces.erase(it);
        } else {
            // when edge is fount extend trace
            wasMatched.at(it->last_i) = true;
            it->points.push_back(outNodes.at(outIndex));
            it->last_i = outIndex;
        }
    }

    // add new traces
    for (int i = 0; i < inNodes.size(); ++i) {
        int outIndex = getMatched(edges, i);
        // node has edge and was not previously matched
        if (outIndex >= 0 && !wasMatched.at(i)) {
            activeTraces.push_back({
                std::vector{inNodes.at(i), outNodes.at(outIndex)}, outIndex});
        }
    }
}

void Tracer::terminateTraces() {
    while (!activeTraces.empty()) {
        traces.push_back(std::move(activeTraces.back()));
        activeTraces.pop_back();
    }
}

void Tracer::dumpToFile(const char filename[]) const {
    std::ofstream out{filename};
    out.seekp(0);
    for (const auto &trace : traces) {
        for (const auto &point : trace.points)
            out << point.x << " ";
        out << std::endl;
        for (const auto &point : trace.points)
            out << point.y << " ";
        out << std::endl;
    }
    out.close();
}
