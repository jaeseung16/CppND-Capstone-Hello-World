#include <complex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

#include <opencv2/core/utility.hpp>
#include <opencv2/core/types.hpp>

#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet() {}

MandelbrotSet::~MandelbrotSet()
{
    _zs.clear();
    _values.clear();
}

MandelbrotSet::MandelbrotSet(const MandelbrotSet &source)
{
    for (auto it = std::begin(source._zs); it != std::end(source._zs); ++it)
    {
        _zs.push_back(*it);
    }

    for (auto it = std::begin(source._values); it != std::end(source._values); ++it)
    {
        _values.push_back(*it);
    }

    _maxIter = source._maxIter;
}

MandelbrotSet &MandelbrotSet::operator=(const MandelbrotSet &source)
{
    if (this == &source)
         return *this;

    for (auto it = std::begin(source._zs); it != std::end(source._zs); ++it)
    {
        _zs.push_back(*it);
    }

    for (auto it = std::begin(source._values); it != std::end(source._values); ++it)
    {
        _values.push_back(*it);
    }

    _maxIter = source._maxIter;

    return *this;
}

MandelbrotSet::MandelbrotSet(MandelbrotSet &&source)
{
    _zs.clear();
    _values.clear();

    for (auto it = std::begin(source._zs); it != std::end(source._zs); ++it)
    {
        _zs.push_back(std::move(*it));
    }

    for (auto it = std::begin(source._values); it != std::end(source._values); ++it)
    {
        _values.push_back(std::move(*it));
    }

    _maxIter = source._maxIter;
    
    source._zs.clear();
    source._values.clear();
    source._maxIter = 0;
}

MandelbrotSet &MandelbrotSet::operator=(MandelbrotSet &&source)
{
    if (this == &source)
         return *this;

    _zs.clear();
    _values.clear();

    for (auto it = std::begin(source._zs); it != std::end(source._zs); ++it)
    {
        _zs.push_back(std::move(*it));
    }

    for (auto it = std::begin(source._values); it != std::end(source._values); ++it)
    {
        _values.push_back(std::move(*it));
    }

    _maxIter = source._maxIter;
    
    source._zs.clear();
    source._values.clear();
    source._maxIter = 0;

    return *this;
}

MandelbrotSet::MandelbrotSet(std::vector<std::complex<float>> &&zs, int maxIter)
{
    _zs = std::move(zs);
    _maxIter = maxIter;
    _values = std::vector<int>(_zs.size(), 0);

    calculate();
}

void MandelbrotSet::recalculate(std::vector<std::complex<float>> &&zs)
{
    _zs.clear();
    _zs = std::move(zs);

    calculate();
}

void MandelbrotSet::calculate()
{
    auto start = std::chrono::high_resolution_clock::now();
    parallel_for_(cv::Range(0, _zs.size()), [&](const cv::Range& range)
        {
            for (int r = range.start; r < range.end; r++)
            {
                auto value = mandelbrotFormula(this->_zs[r], this->_maxIter);
                _values[r] = value;
            }
        }
    );
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Generating Mandelbrot values took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;  
}

int MandelbrotSet::mandelbrotFormula(const std::complex<float> &z0, const int &maxIter)
{
    std::complex<float> z = z0;
    int iter = 0;
    while (iter < maxIter)
    {
        if (std::norm(z) > 4.0f) break;
        z = z*z + z0;
        iter ++;
    }

    return (maxIter == iter) ? 0 : (int)((float)iter / (float)maxIter * 255.0);
}