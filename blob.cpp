#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

cv::SimpleBlobDetector::Params params;

std::vector<cv::Point2f> findParticles(const cv::Mat &image) {
    std::vector<cv::KeyPoint> keypoints;
    auto detector = cv::SimpleBlobDetector::create(params);

    detector->detect(image, keypoints);
    
    std::vector<cv::Point2f> points;
    for (const auto &kp : keypoints)
        points.push_back(std::move(kp.pt));

    return points;
}

int main() {
    params.filterByArea = true;
    params.minArea = 20;

    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    params.filterByConvexity = true;
    params.minConvexity = 0.87;

    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;


    char filename[32];
    for (int i = 1; i < 79; ++i) {
        std::sprintf(filename, "img/%02d.png", i);
        std::cout << "reading " << filename << ": ";
        auto image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        std::cout << "processing ...";
        auto particles = findParticles(image);
        std::cout << " done. found " << particles.size() << " particles" << std::endl;
    }
}
