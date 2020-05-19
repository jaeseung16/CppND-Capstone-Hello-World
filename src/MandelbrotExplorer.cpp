#include <iostream>
#include <chrono>
#include <thread>

#include "MandelbrotExplorer.h"

const cv::Rect_<float> MandelbrotExplorer::defaultRect = cv::Rect_<float>(-2.0, -1.5, 3.0, 3.0);
const cv::Rect_<float> MandelbrotExplorer::initialZoomedRect = cv::Rect_<float>(-1.5, -0.2, 0.4, 0.4);
const int MandelbrotExplorer::defaultDisplaySize = 800;
const cv::Rect defaultDisplayRect = cv::Rect(0, 0, MandelbrotExplorer::defaultDisplaySize, MandelbrotExplorer::defaultDisplaySize);

MandelbrotExplorer::MandelbrotExplorer() {
    std::cout << "### MandelbrotExplorer ###" << std::endl;
    
    float scale = initialZoomedRect.width / defaultRect.width;
    float range = (float)defaultDisplaySize * scale ;
    float regionX = (int) ((initialZoomedRect.x - defaultRect.x) * (float)defaultDisplaySize / defaultRect.width);
    float regionY = (int) ((initialZoomedRect.y - defaultRect.y) * (float)defaultDisplaySize / defaultRect.width);
    
    _regionToZoomed = cv::Rect((int)regionX, (int)regionY, (int)range, (int)range);
    
    std::cout << "initialZoomedRect = " << initialZoomedRect << std::endl;
    std::cout << "_regionToZoomed = " << _regionToZoomed << std::endl;

    showMandelbrotSet();
}

void MandelbrotExplorer::getRangeToZoomed() {
    float range = MandelbrotExplorer::defaultRect.width;

    float xmin_zoomed = _regionToZoomed.x + 125 * range / (float) (defaultDisplaySize + 1);
    float ymin_zoomed = _regionToZoomed.y + 350 * range / (float) (defaultDisplaySize + 1);
    float range_zoomed = 100 * range / (float) (defaultDisplaySize + 1);
}

cv::Rect_<float> MandelbrotExplorer::convertRangeToZoomedToComplex() {
    float xmin_zoomed = -2.0 + 3.0 * (float)_regionToZoomed.x / (defaultDisplaySize+1);
    float ymin_zoomed = -1.5 + 3.0 * (float)_regionToZoomed.y / (defaultDisplaySize+1);
    float range_zoomed = 3.0 * (float)(_regionToZoomed.width - 1) / (defaultDisplaySize+1);
    return cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed);
}

void MandelbrotExplorer::showMandelbrotSet() {
    std::string windowName = "Mandelbrot";
    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, MandelbrotExplorer::onMouse, this);
    while(true) {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cv::Mat image = _staticDisplay.getMat().clone();
        cv::rectangle(image, _regionToZoomed, _colorForRegionToZoomed);
        cv::imshow(windowName, image);

        _zoomedDisplay.updateRect(convertRangeToZoomedToComplex());

        cv::imshow("zoomed Mandelbrot", _zoomedDisplay.getMat());

        //cv::waitKey(33);
        // Quit when ESC pressed
        if((char)27 == cv::waitKey(30)) {
            break;
        }  

        auto end = std::chrono::high_resolution_clock::now();
        //std::cout << "MandelbrotExplorer::showMandelbrotSet() took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    }
}

void MandelbrotExplorer::updateColor(Color color) {
    switch(color) {
        case Color::Red:
            _colorForRegionToZoomed = cv::Vec3b(0,0,255);
            break;
        case Color::Yellow:
            _colorForRegionToZoomed = cv::Vec3b(0,255,255);
            break;
        case Color::Green:
            _colorForRegionToZoomed = cv::Vec3b(0,255,0);
            break;
        case Color::Cyan:
            _colorForRegionToZoomed = cv::Vec3b(255,255,0);
            break;
        case Color::Blue:
            _colorForRegionToZoomed = cv::Vec3b(255,0,0);
            break;
        case Color::White:
            _colorForRegionToZoomed = cv::Vec3b(255,255,255);
            break;
    }
}

void MandelbrotExplorer::onMouse(int event, int x, int y, int flags, void *that) {
    MandelbrotExplorer* thiz = static_cast<MandelbrotExplorer*>(that);
    thiz -> mouseClick(event, x, y, flags);
}

void MandelbrotExplorer::moveRegion(cv::Point &&point) {
    cv::Rect tempRegion = _regionToZoomed + point - _origin;
    if (!defaultDisplayRect.contains(_regionToZoomed.tl() + point - _origin) || !defaultDisplayRect.contains(_regionToZoomed.br() + point - _origin))
    {
        if (tempRegion.x < 0) {
            tempRegion.x = 0;
        } else if (tempRegion.x >= (defaultDisplaySize - tempRegion.width)) {
            tempRegion.x = defaultDisplaySize - tempRegion.width;
        } 
        if (tempRegion.y < 0) {
            tempRegion.y = 0;
        } 
        if (tempRegion.y >= (defaultDisplaySize - tempRegion.height)) {
            tempRegion.y = defaultDisplaySize - tempRegion.height;
        } 
    }
    _regionToZoomed = tempRegion;
}

void MandelbrotExplorer::updateOrigin(cv::Point &&point) {
    _origin = point;
}

void MandelbrotExplorer::shrinkRegion(int &&delta) {
    if (delta < 0 && _regionToZoomed.width + delta >= 10) {
        _regionToZoomed.width += delta;
        _regionToZoomed.height = _regionToZoomed.width;
    }
}

void MandelbrotExplorer::enlargeRegion(int &&delta) {
    if (delta > 0) {
        cv::Rect tempRegion = _regionToZoomed + cv::Size_<int>(delta, delta);
        if (!defaultDisplayRect.contains(tempRegion.br())) {
            if (tempRegion.x > tempRegion.y) {
                tempRegion.width = defaultDisplaySize - tempRegion.x;
                tempRegion.height = tempRegion.width;
            } else {
                tempRegion.height = defaultDisplaySize - tempRegion.y;
                tempRegion.width = tempRegion.height;
            }
            _regionToZoomed = tempRegion;
        }
    }
}

int MandelbrotExplorer::determineDelta(int x, int y) {
    int dx = x - _origin.x;
    int dy = y - _origin.y;
    return (dx + dy) / 2;
}

void MandelbrotExplorer::mouseClick(int event, int x, int y, int flags)
{
    std::lock_guard<std::mutex> lock(_mutex);
    switch(event) {
        case cv::EVENT_MOUSEMOVE:
            {
                bool isLeftButtonDown = (flags & cv::EVENT_FLAG_LBUTTON) != 0;
                bool isRightButtonDown = (flags & cv::EVENT_FLAG_RBUTTON) != 0;

                if (_regionToZoomedTracked == true && isLeftButtonDown) {
                    moveRegion(cv::Point(x,y));
                    updateOrigin(cv::Point(x,y));
                    updateColor(Color::Cyan);
                }

                if (_regionToZoomedTracked == false && isRightButtonDown) {
                    int delta = determineDelta(x, y);
                    if (delta > 0) {
                        enlargeRegion(std::move(delta));
                    } else if (delta < 0) {
                        shrinkRegion(std::move(delta));
                    }
                    updateOrigin(cv::Point(x, y));
                    updateColor(Color::Yellow);
                }
            }
            break;
        case cv::EVENT_LBUTTONDOWN:
            if (_regionToZoomed.contains(cv::Point(x,y)) && _regionToZoomedTracked == false) {
                updateOrigin(cv::Point(x,y));
                updateColor(Color::Cyan);
                _regionToZoomedTracked = true;
            } 
            break;
        case cv::EVENT_LBUTTONUP:
            if (_regionToZoomedTracked == true) {
                moveRegion(cv::Point(x,y));
                updateColor(Color::White);
                 _regionToZoomedTracked = false;
            }
            break;
        case cv::EVENT_RBUTTONDOWN:
            if (_regionToZoomed.contains(cv::Point(x,y)) && _regionToZoomedTracked == false) {
                updateOrigin(cv::Point(x, y));
                updateColor(Color::Yellow);
            }
            break;
        case cv::EVENT_RBUTTONUP:
            if (_regionToZoomedTracked == false) {    
                updateColor(Color::White);
            }
            break;
    }
}