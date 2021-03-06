#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

namespace {
    cv::Ptr<cv::SimpleBlobDetector> detector;

    std::vector<cv::Point2f> findParticles(const cv::Mat &image) {
        std::vector<cv::KeyPoint> keypoints;

        detector->detect(image, keypoints);

        std::vector<cv::Point2f> points;
        for (const auto &kp : keypoints)
            points.emplace_back(std::move(kp.pt));

        return points;
    }
}

void initCV() {
    cv::SimpleBlobDetector::Params params;

    params.minThreshold = 0;
    params.maxThreshold = .6 * 255;
    params.thresholdStep = 3;

    params.filterByArea = true;
    params.minArea = 30;

    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    params.filterByConvexity = true;
    params.minConvexity = 0.5;

    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;

    detector = cv::SimpleBlobDetector::create(params);
}

std::vector<cv::Point2f> getFrameParticles(const char filename[]) {
    std::cout << "reading " << filename << std::endl;
    auto image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    std::cout << "processing ...";
    auto particles = findParticles(image);
    std::cout << " done. found " << particles.size() << " particles" << std::endl;
    return particles;
}
