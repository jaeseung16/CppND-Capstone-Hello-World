#include <opencv2/opencv.hpp>

class MandelbrotColor
{
public:
    enum Color { Red, Yellow, Green, Cyan, Blue, White };

    static cv::Vec3b convertToVec3b(const MandelbrotColor::Color color);
    static MandelbrotColor::Color convertToMandelbrotColor(const cv::Vec3b vec3b);
};
