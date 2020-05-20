#include <opencv2/opencv.hpp>

class MandelbrotColor
{
    public:
    enum Color { Red, Yellow, Green, Cyan, Blue, White };

    static cv::Vec3b convertToVec3b(MandelbrotColor::Color color);
};

