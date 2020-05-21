#include <chrono>
#include <numeric>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "MandelbrotDisplay.h"

MandelbrotDisplay::MandelbrotDisplay() {}

MandelbrotDisplay::~MandelbrotDisplay() {}

MandelbrotDisplay::MandelbrotDisplay(const MandelbrotDisplay &source) {
    std::cout << "MandelbrotDisplay Copy Constructor" << std::endl;
    _mat = source._mat;
    _color = source._color;
    _mandelbrotSet = std::make_unique<MandelbrotSet>(*(source._mandelbrotSet));
    _size = source._size;
    _scale = source._scale;
    _xmin = source._xmin;
    _xmax = source._xmax;
    _ymin = source._ymin;
    _ymax = source._ymax;
}

MandelbrotDisplay &MandelbrotDisplay::operator=(const MandelbrotDisplay &source) {
    std::cout << "MandelbrotDisplay Copy Assignment Operator" << std::endl;
    if (this == &source)
        return *this;

    _mat = source._mat;
    _color = source._color;
    _mandelbrotSet = std::make_unique<MandelbrotSet>(*(source._mandelbrotSet));
    _size = source._size;
    _scale = source._scale;
    _xmin = source._xmin;
    _xmax = source._xmax;
    _ymin = source._ymin;
    _ymax = source._ymax;
    
    return *this;
}

MandelbrotDisplay::MandelbrotDisplay(MandelbrotDisplay &&source) {
    std::cout << "MandelbrotDisplay Move Constructor" << std::endl;
    _mat = std::move(source._mat);
    _color = std::move(source._color);
    _mandelbrotSet = std::move(source._mandelbrotSet);
    _size = source._size;
    _scale = source._scale;
    _xmin = source._xmin;
    _xmax = source._xmax;
    _ymin = source._ymin;
    _ymax = source._ymax;

    source._size = 0;
    source._scale = 0.0;
    source._xmin = 0.0;
    source._xmax = 0.0;
    source._ymin = 0.0;
    source._ymax = 0.0;
}

MandelbrotDisplay &MandelbrotDisplay::operator=(MandelbrotDisplay &&source) {
    std::cout << "MandelbrotDisplay Move Assignment Operator" << std::endl;
    if (this == &source)
        return *this;

    _mat = std::move(source._mat);
    _color = std::move(source._color);
    _mandelbrotSet = std::move(source._mandelbrotSet);
    _size = source._size;
    _scale = source._scale;
    _xmin = source._xmin;
    _xmax = source._xmax;
    _ymin = source._ymin;
    _ymax = source._ymax;

    source._size = 0;
    source._scale = 0.0;
    source._xmin = 0.0;
    source._xmax = 0.0;
    source._ymin = 0.0;
    source._ymax = 0.0;

    return *this;
}

MandelbrotDisplay::MandelbrotDisplay(cv::Rect_<float> selection, int size, MandelbrotColor::Color color) {
    _size = size;
    _scale = selection.width / (float)(size+1);
    _xmin = selection.x;
    _ymin = selection.y;
    _xmax = _xmin + _scale * (float)(size-1);
    _ymax = _ymin + _scale * (float)(size-1);
    _color = MandelbrotColor::convertToVec3b(color) / 255.0;

    _mat = cv::Mat(_size, _size, CV_8UC3, cv::Vec3b(0,0,0));
    generateMandelbrotSet();
    generateMat();
}

void MandelbrotDisplay::generateMandelbrotSet() {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> xs(_size);
    std::iota(xs.begin(), xs.end(), 0);

    std::vector<int> ys(_size);
    std::iota(ys.begin(), ys.end(), 0);

    std::vector<std::complex<float>> zs;
    for (int x : xs) {
        for (int y : ys) {
            zs.push_back(std::complex<float>(_xmin + _scale * (float)x, _ymin + _scale * (float)y));
        }
    }

    _mandelbrotSet = std::make_unique<MandelbrotSet>(std::move(zs), 50);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "MandelbrotDisplay::generateMandelbrotSet() took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

void MandelbrotDisplay::generateMat() {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> values = _mandelbrotSet->getValues();

    cv::parallel_for_ (cv::Range(0, values.size()), [&](const cv::Range& range) {
        for (int count = range.start; count < range.end; count++) {
            int x = count / _size;
            int y = count % _size;
            _mat.at<cv::Vec3b>(y, x) = values[count] * _color;
        }
    });

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "MandelbrotDisplay::generateMat() took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

void MandelbrotDisplay::updateRect(cv::Rect_<float> selection) {
    _scale = selection.width / (float)(_size+1);
    _xmin = selection.x;
    _ymin = selection.y;
    _xmax = _xmin + _scale * (float)(_size-1);
    _ymax = _ymin + _scale * (float)(_size-1);

    generateMandelbrotSet();
    generateMat();
}