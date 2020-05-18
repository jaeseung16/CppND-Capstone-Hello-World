#include <complex>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <iostream>

#include <opencv2/core/utility.hpp>
#include <opencv2/core/types.hpp>

#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet() {

}

MandelbrotSet::MandelbrotSet(std::vector<std::complex<float>> &&zs, int nIteration) {
    _zs = zs;
    _nIteration = nIteration;
/*
    for (auto z : zs) {
        MandelbrotPoint point = MandelbrotPoint(std::move(z), (unsigned int) _nIteration);
        _iterations.push_back(point.getIteration());
        _isMandelbrotSet.push_back(point.isMandelBrotSet());
        _set.push_back(std::move(point));
    }*/

    std::cout << "### MandelbrotSet ###" << "\n";

    _values = std::vector<int>(_zs.size(), 0);;

    //cv::Mat mandelbrotImg(800, 800, CV_8U);
    auto start = std::chrono::high_resolution_clock::now();
    parallel_for_(cv::Range(0, 800*800), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            /*
            int i = r / 800;
            int j = r % 800;

            float x0 = j * 3.0 / 800.0 - 2.0;
            float y0 = i * 3.0 / 800.0 - 1.5;

            std::complex<float> z0(x0, y0);*/
            uchar value = (uchar) mandelbrotFormula(this->_zs[r], this->_nIteration);
            _values[r] = value;
        }
    });

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "MandelbrotSet::MandelbrotSet took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    std::cout << "_values[0] = " << _values[0] << std::endl;
}

int MandelbrotSet::mandelbrot(const std::complex<float> &z0, const int max)
{
    std::complex<float> z = z0;
    for (int t = 0; t < max; t++)
    {
        if (z.real()*z.real() + z.imag()*z.imag() > 4.0f) return t;
        z = z*z + z0;
    }

    return max;
}

int MandelbrotSet::mandelbrotFormula(const std::complex<float> &z0, const int maxIter=50) {
    int value = mandelbrot(z0, maxIter);
    if(maxIter - value == 0)
    {
        return 0;
    }

    return cvRound(sqrt(value / (float) maxIter) * 255);
}