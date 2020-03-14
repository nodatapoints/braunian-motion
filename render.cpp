#include "render.hpp"

/*
 * Render everything in pretty window.
 */
namespace {
    const int radius = 10;

    sf::Sprite bgSprite;
    sf::Texture bgTexture;

    sf::CircleShape circle;
    sf::VertexArray line{sf::Lines, 2};

    sf::RenderWindow window;
    sf::RenderTexture lineBuffer;
    sf::ContextSettings settings;

    const sf::Color circleColor{255, 77, 77};
    const sf::Color lineColor{255, 77, 77};

    // hardcoded dimensions
    const int width = 1280, height = 1024;
}

void initSFML() {
    circle.setRadius(radius);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(circleColor);
    circle.setFillColor(sf::Color::Transparent);

    settings.depthBits = 24;
    settings.antialiasingLevel = 5;

    window.create(sf::VideoMode(width, height), "[[float]] stuff", sf::Style::Default, settings);
    lineBuffer.create(width, height);
    lineBuffer.setSmooth(true);
    window.setActive();
}

void setBackground(std::string filename) {
    bgTexture.loadFromFile(filename);
    bgSprite.setTexture(bgTexture);
    window.draw(bgSprite);
}

bool handleEvents() {
    sf::Event event;
    while (true) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                return false;
            else if (event.type == sf::Event::KeyReleased)
                return (event.key.code != sf::Keyboard::Q);
        }
    }
}

void drawPoint(const cv::Point2f &point) {
    circle.setPosition(point.x-radius, point.y-radius);
    window.draw(circle);
}

void drawLine(const cv::Point2f &a, const cv::Point2f &b) {
    line[0].position = sf::Vector2f(a.x, height - a.y);
    line[1].position = sf::Vector2f(b.x, height - b.y);
    line[0].color = line[1].color = lineColor;

    lineBuffer.draw(line);
}
void display() {
    sf::Sprite lineSprite{lineBuffer.getTexture()};
    window.draw(lineSprite);
    window.display();
}
