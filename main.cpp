#include "render.hpp"
#include "blob.hpp"
#include "matching.hpp"

#include <fstream>

int main() {
    initSFML();
    initCV();

    std::ofstream outfile{"output.dat"};
    // rewind file
    outfile.seekp(0);

    setPenalty(25);

    char filename[32];

    // vector of particle vectors for each frame
    std::vector<std::vector<cv::Point2f>> particleByFrame;

    // another magic number: 77
    // hardcoded iteration over all image files
    for (int i = 1; i < 77; ++i) {
        std::sprintf(filename, "img/%02d.png", i);

        // detect particles
        const auto points = getFrameParticles(filename);

        // draw background and markers
        setBackground(filename);
        for (const auto &point : points)
            drawPoint(point);

        if (!particleByFrame.empty()) {
            const auto &previous = particleByFrame.back();

            // match them to the particles of previous frame
            const auto matching = solveBipartite(previous, points);

            // draw edges and log to outfile
            for (const auto &edge : matching) {
                const auto &a = previous.at(edge.in), &b = points.at(edge.out);
                outfile << (b.x-a.x) << " " << (b.y-a.y) << std::endl;
                drawLine(a, b);
            }
        }
        // add the particles
        particleByFrame.push_back(std::move(points));

        display();
    }
    outfile.close();
}
