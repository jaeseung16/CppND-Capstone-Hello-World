#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

class MandelbrotDisplay {
public:
    MandelbrotDisplay(cv::Mat MandelbrotSet, int size, float scale, float xmin, float ymin);

    cv::Mat getMandelbrotSet() { return _mandelbrotSet; }

private:
    cv::Mat _mandelbrotSet;
    int _size;
    float _scale;
    float _xmin;
    float _xmax;
    float _ymin;
    float _ymax;
};