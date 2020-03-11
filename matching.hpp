#pragma once

#include <vector>
#include <glpk.h>
#include <iostream>

#include <opencv2/opencv.hpp>

using Point = cv::Point2f;

struct Edge {
    int in, out;
};


std::vector<Edge> solveBipartite(const std::vector<Point> &inNodes, const std::vector<Point> &outNodes);
