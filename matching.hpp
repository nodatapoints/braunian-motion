#pragma once

#include <vector>
#include <glpk.h>
#include <iostream>

#include <opencv2/opencv.hpp>

using Point = cv::Point2f;

struct Edge {
    int in, out;

    Edge(int i, int o);
    Edge(const Edge&) = default;
    Edge(Edge&&) = default;
};

std::vector<Edge> solveBipartite(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes);
void setPenalty(float value);
