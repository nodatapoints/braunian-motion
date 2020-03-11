#include "render.hpp"
#include "blob.hpp"
#include "matching.hpp"

#include <fstream>

int main() {
    initSFML();
    initCV();

    std::ofstream outfile{"output.dat"};

    outfile.seekp(0);

    char filename[32];

    std::vector<std::vector<cv::Point2f>> particleByFrame;

    for (int i = 1; i < 77; ++i) {
        std::sprintf(filename, "trueimg/%02d.png", i);

        const auto points = getFrameParticles(filename);
        setBackground(filename);
        for (const auto &point : points)
            drawPoint(point);

        if (!particleByFrame.empty()) {
            const auto &previous = particleByFrame.back();
            auto matching = solveBipartite(previous, points);

            for (const auto &edge : matching) {
                const auto &a = previous.at(edge.in), &b = points.at(edge.out);
                outfile << (b.x-a.x) << " " << (b.y-a.y) << std::endl;
                drawLine(a, b);
            }
        }
        particleByFrame.push_back(std::move(points));

        display();
    }
    outfile.close();
}
