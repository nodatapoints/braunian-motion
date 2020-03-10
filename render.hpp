#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include <opencv2/opencv.hpp>

void initSFML();
void setBackground(std::string filename);
bool handleEvents();
void drawBackground();
void drawPoint(const cv::Point2f &point);
void drawLine(const cv::Point2f &a, const cv::Point2f &b);
void display();
