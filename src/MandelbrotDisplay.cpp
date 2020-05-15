#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "MandelbrotDisplay.h"

MandelbrotDisplay::MandelbrotDisplay(cv::Mat MandelbrotSet, int size, float scale, float xmin, float ymin) {
    _mandelbrotSet = MandelbrotSet;
    _size = size;
    _scale = scale;
    _xmin = xmin;
    _ymin = ymin;
    _xmax = xmin + scale * (float)size;
    _ymax = ymin + scale * (float)size;
}