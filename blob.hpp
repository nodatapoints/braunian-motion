#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

void initCV();
std::vector<cv::Point2f> getFrameParticles(const char filename[]);
