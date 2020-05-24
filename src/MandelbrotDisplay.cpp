#include <chrono>
#include <numeric>
#include <iostream>
#include <thread>
#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MandelbrotDisplay.h"

const int MandelbrotDisplay::maxIterations = 50;

MandelbrotDisplay::MandelbrotDisplay() {}

MandelbrotDisplay::~MandelbrotDisplay()
{
    std::cout << "MandelbrotDisplay Destructor" << std::endl;
    setStatus(MandelbrotDisplay::Status::done);
    if (threads.size() > 0)
    {
        std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });
    }
}

MandelbrotDisplay::MandelbrotDisplay(const MandelbrotDisplay &source)
{
    std::cout << "MandelbrotDisplay Copy Constructor" << std::endl;
    _mat = source._mat;
    _color = source._color;
    _region = source._region;
    _mandelbrotSet = std::make_unique<MandelbrotSet>(*(source._mandelbrotSet));
    _displaySize = source._displaySize;
    _scale = source._scale;
}

MandelbrotDisplay &MandelbrotDisplay::operator=(const MandelbrotDisplay &source)
{
    std::cout << "MandelbrotDisplay Copy Assignment Operator" << std::endl;
    if (this == &source)
        return *this;

    _mat = source._mat;
    _color = source._color;
    _region = source._region;
    _mandelbrotSet = std::make_unique<MandelbrotSet>(*(source._mandelbrotSet));
    _displaySize = source._displaySize;
    _scale = source._scale;
    
    return *this;
}

MandelbrotDisplay::MandelbrotDisplay(MandelbrotDisplay &&source)
{
    std::cout << "MandelbrotDisplay Move Constructor" << std::endl;
    _mat = std::move(source._mat);
    _color = std::move(source._color);
    _region = std::move(source._region);
    _mandelbrotSet = std::move(source._mandelbrotSet);
    _displaySize = source._displaySize;
    _scale = source._scale;

    source._displaySize = 0;
    source._scale = 0.0;
}

MandelbrotDisplay &MandelbrotDisplay::operator=(MandelbrotDisplay &&source)
{
    std::cout << "MandelbrotDisplay Move Assignment Operator" << std::endl;
    if (this == &source)
        return *this;

    _mat = std::move(source._mat);
    _color = std::move(source._color);
    _region = std::move(source._region);
    _mandelbrotSet = std::move(source._mandelbrotSet);
    _displaySize = source._displaySize;
    _scale = source._scale;

    source._displaySize = 0;
    source._scale = 0.0;

    return *this;
}

MandelbrotDisplay::MandelbrotDisplay(cv::Rect_<float> region, int displaySize, MandelbrotColor::Color color)
{
    _displaySize = displaySize;
    _scale = region.width / (float)(_displaySize+1);
    setRegion(region);

    _color = MandelbrotColor::convertToVec3b(color) / 255.0;
    _mat = cv::Mat(_displaySize, _displaySize, CV_8UC3, cv::Vec3b(0,0,0));

    generateMandelbrotSet();
    setStatus(MandelbrotDisplay::Status::readyToDisplay);
}

std::vector<std::complex<float>> MandelbrotDisplay::convertRectToVector(cv::Rect_<float> region)
{
    _scale = _region.width / (float)(_displaySize+1);

    std::vector<std::complex<float>> zs;
    for (int x = 0; x < _displaySize; x++)
    {
        for (int y = 0; y < _displaySize; y++)
        {
            zs.push_back(std::complex<float>(_region.x + _scale * (float)x, _region.y + _scale * (float)y));
        }
    }

    return zs;
}

void MandelbrotDisplay::generateMandelbrotSet() {
    auto start = std::chrono::high_resolution_clock::now();

    _mandelbrotSet = std::make_unique<MandelbrotSet>(std::move(convertRectToVector(_region)), maxIterations);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "MandelbrotDisplay::generateMandelbrotSet() took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    generateMat();
}

void MandelbrotDisplay::generateMat()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> values = _mandelbrotSet->getValues();

    cv::parallel_for_ (cv::Range(0, values.size()), [&](const cv::Range& range)
        {
            for (int count = range.start; count < range.end; count++)
            {
                int x = count / _displaySize;
                int y = count % _displaySize;
                _mat.at<cv::Vec3b>(y, x) = values[count] * _color;
            }
        }
    );

    auto end = std::chrono::high_resolution_clock::now();
}

void MandelbrotDisplay::updateRect(cv::Rect_<float> region)
{
    std::cout << "Input region = " << region << std::endl;
    std::cout << "_region = " << getRegion() << std::endl;
    if (MandelbrotDisplay::Status::waitForUpdate == getStatus())
    {
        _scale = region.width / (float)(_displaySize+1);
        setRegion(region);
        setStatus(Status::needToUpdate);
        return;
    }
}

void MandelbrotDisplay::simulate()
{
    setStatus(MandelbrotDisplay::Status::waitForUpdate);
    threads.emplace_back(std::thread(&MandelbrotDisplay::cycleThroughPhases, this));
}

void MandelbrotDisplay::cycleThroughPhases()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        MandelbrotDisplay::Status status = getStatus();
        if (MandelbrotDisplay::Status::done == status)
        {
            break;
        }
        else if (MandelbrotDisplay::Status::needToUpdate == status)
        {
            generateMandelbrotSet();
            setStatus(MandelbrotDisplay::Status::readyToDisplay);
        }
    }
}

MandelbrotDisplay::Status MandelbrotDisplay::getStatus()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _status;
}

void MandelbrotDisplay::setStatus(MandelbrotDisplay::Status status)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _status = status;
}

cv::Rect_<float> MandelbrotDisplay::getRegion()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _region;
}

void MandelbrotDisplay::setRegion(cv::Rect_<float> region)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _region = region;
}

bool MandelbrotDisplay::isUpdated()
{
    bool isUpdated = MandelbrotDisplay::Status::readyToDisplay == getStatus();
    if (isUpdated)
    {
        setStatus(MandelbrotDisplay::Status::waitForUpdate);
    }
    return isUpdated;
}

cv::Mat MandelbrotDisplay::getMat()
{ 
    return _mat.clone();
}