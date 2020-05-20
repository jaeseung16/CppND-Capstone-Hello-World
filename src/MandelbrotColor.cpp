#include "MandelbrotColor.h"

cv::Vec3b MandelbrotColor::convertToVec3b(MandelbrotColor::Color color) {
    cv::Vec3b colorToVec3b = cv::Vec3b(0,0,0);
    switch(color) {
        case MandelbrotColor::Color::Red:
            colorToVec3b = cv::Vec3b(0,0,255);
            break;
        case MandelbrotColor::Color::Yellow:
            colorToVec3b = cv::Vec3b(0,255,255);
            break;
        case MandelbrotColor::Color::Green:
            colorToVec3b = cv::Vec3b(0,255,0);
            break;
        case MandelbrotColor::Color::Cyan:
            colorToVec3b = cv::Vec3b(255,255,0);
            break;
        case MandelbrotColor::Color::Blue:
            colorToVec3b = cv::Vec3b(255,0,0);
            break;
        case MandelbrotColor::Color::White:
            colorToVec3b = cv::Vec3b(255,255,255);
            break;
    }
    return colorToVec3b;
}