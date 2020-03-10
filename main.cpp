#include "render.hpp"
#include "blob.hpp"
#include "matching.hpp"

int main() {
    initSFML();
    initCV();

    char filename[32];

    std::vector<std::vector<cv::Point2f>> particleByFrame;

    for (int i = 1; i <= 78; ++i) {
        std::sprintf(filename, "img/%02d.png", i);

        const auto points = getFrameParticles(filename);
        setBackground(filename);
        for (const auto &point : points)
            drawPoint(point);

        if (!particleByFrame.empty()) {
            const auto &previous = particleByFrame.back();
            auto matching = solveBipartite(previous, points);
            std::cout << "matched edges: " << matching.size() << std::endl;

            for (const auto &edge : matching)
                drawLine(previous.at(edge.in), points.at(edge.out));
        }
        particleByFrame.push_back(std::move(points));

        display();
        sf::sleep(sf::milliseconds(100));
    }
}
