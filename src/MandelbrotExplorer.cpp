#include <iostream>
#include <chrono>
#include <thread>

#include "MandelbrotExplorer.h"

const cv::Rect_<float> MandelbrotExplorer::defaultRect = cv::Rect_<float>(-2.0, -1.5, 3.0, 3.0);
const int MandelbrotExplorer::defaultDisplaySize = 700;
const std::string MandelbrotExplorer::defaultDiplayWindowName = "Mandelbrot";
const std::string MandelbrotExplorer::zoomedDiplayWindowName = "Zoomed Mandelbrot";

MandelbrotExplorer::MandelbrotExplorer() {
    std::cout << "### Initialize MandelbrotExplorer ###" << std::endl;

    _defaultDisplayRect = cv::Rect(0, 0, defaultDisplaySize, defaultDisplaySize);
    
    // zoomed display initialized corresponding to cv::Rect_<float>(-1.5, -0.2, 0.4, 0.4)
    int xMin = defaultDisplaySize / 6;
    int yMin = defaultDisplaySize * 13 / 30;
    int range = defaultDisplaySize * 2 / 15;
    cv::Rect initialRegionToZoomed = cv::Rect(xMin, yMin, range, range);
    setRegionToZoomed(initialRegionToZoomed);

    _staticDisplay = std::unique_ptr<MandelbrotDisplay>(new MandelbrotDisplay(defaultRect, defaultDisplaySize, MandelbrotColor::Color::Red));
    _zoomedDisplay = std::unique_ptr<MandelbrotDisplay>(new MandelbrotDisplay(convertZoomedRegionToRect(_regionToZoomed), defaultDisplaySize, MandelbrotColor::Color::Green));
    _zoomedDisplay->simulate();
}


cv::Rect_<float> MandelbrotExplorer::convertZoomedRegionToRect(cv::Rect regionToZoomed) {
    float xmin = defaultRect.x + defaultRect.width * (float)regionToZoomed.x / (defaultDisplaySize+1);
    float ymin = defaultRect.y + defaultRect.height * (float)regionToZoomed.y / (defaultDisplaySize+1);
    float range = defaultRect.width * (float)(regionToZoomed.width - 1) / (defaultDisplaySize+1);
    return cv::Rect_<float>(xmin, ymin, range, range);
}

void MandelbrotExplorer::setRegionToZoomed(cv::Rect region) {
    std::lock_guard<std::mutex> lock(_mutex);
    _regionToZoomed = region;
}

cv::Rect MandelbrotExplorer::getRegionToZoomed() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _regionToZoomed;
}

void MandelbrotExplorer::showMandelbrotSet() {
    std::cout << "### Present Mandelbrot Set ###" << std::endl;

    cv::namedWindow(defaultDiplayWindowName);
    cv::setMouseCallback(defaultDiplayWindowName, MandelbrotExplorer::onMouse, this);
    cv::Mat zoomedImage = _zoomedDisplay->getMat();
    while(true) {
        //auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cv::Mat image = _staticDisplay->getMat();
        cv::rectangle(image, getRegionToZoomed(), _colorForRegionToZoomed);
        cv::imshow(defaultDiplayWindowName, image);

        if (_zoomedDisplay->isUpdated()) {
            std::cout << "Updating _zoomedDisplay " << (_zoomedDisplay->isUpdated()) << std::endl;
            zoomedImage = _zoomedDisplay->getMat();
        }
        cv::imshow(zoomedDiplayWindowName, zoomedImage);

        if((char)27 == cv::waitKey(30)) {
            std::cout << "Terminating MandelbrotExplorer" << std::endl;
            break;
        }  
    }
}

void MandelbrotExplorer::onMouse(int event, int x, int y, int flags, void *that) {
    MandelbrotExplorer* thiz = static_cast<MandelbrotExplorer*>(that);
    thiz -> mouseClick(event, x, y, flags);
}

void MandelbrotExplorer::moveRegion(cv::Rect &region, const cv::Point origin, cv::Point &&point) {
    region += point - origin;
    if (!_defaultDisplayRect.contains(region.tl()) || !_defaultDisplayRect.contains(region.br()))
    {
        if (region.x < 0) {
            region.x = 0;
        } else if (region.x >= (defaultDisplaySize - region.width)) {
            region.x = defaultDisplaySize - region.width;
        } 
        if (region.y < 0) {
            region.y = 0;
        } 
        if (region.y >= (defaultDisplaySize - region.height)) {
            region.y = defaultDisplaySize - region.height;
        } 
    }
}

void MandelbrotExplorer::shrinkRegion(cv::Rect &region, int &&delta) {
    if (delta < 0 && region.width + delta >= 10) {
        region.width += delta;
        region.height = region.width;
    }
}

void MandelbrotExplorer::enlargeRegion(cv::Rect &region, int &&delta) {
    if (delta > 0) {
        region += cv::Size_<int>(delta, delta);
        std::cout << "_defaultDisplayRect = " << _defaultDisplayRect << std::endl;
        if (!_defaultDisplayRect.contains(region.br())) {
            if (region.x > region.y) {
                region.width = defaultDisplaySize - region.x;
                region.height = region.width;
            } else {
                region.height = defaultDisplaySize - region.y;
                region.width = region.height;
            }
        }
    }
}

int MandelbrotExplorer::determineDelta(const cv::Point oldPoint, const cv::Point newPoint) {
    auto dp = newPoint - oldPoint;
    return (dp.x + dp.y) / 2;
}

void MandelbrotExplorer::mouseClick(int event, int x, int y, int flags)
{
    cv::Rect _regionToZoomedCandidate = _regionToZoomed;
    cv::Point _originCandidate = _origin;
    MandelbrotColor::Color _colorForRegionToZoomedCandidate = MandelbrotColor::convertToMandelbrotColor(_colorForRegionToZoomed);

    bool isLeftButtonDown = (flags & cv::EVENT_FLAG_LBUTTON) != 0;
    bool isRightButtonDown = (flags & cv::EVENT_FLAG_RBUTTON) != 0;
    bool isCtrlKeyDown = (flags & cv::EVENT_FLAG_CTRLKEY) != 0 ;

    switch(event) {
        case cv::EVENT_MOUSEMOVE:
            {
                if (_regionToZoomedSelected == true && isLeftButtonDown) {
                    moveRegion(_regionToZoomedCandidate, _originCandidate, cv::Point(x,y));
                    _originCandidate = cv::Point(x,y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Cyan;
                }

                if (_regionToZoomedSelected == false && (isRightButtonDown || isCtrlKeyDown)) {
                    int delta = determineDelta(_originCandidate, cv::Point(x,y));
                    if (delta > 0) {
                        enlargeRegion(_regionToZoomedCandidate, std::move(delta));
                    } else if (delta < 0) {
                        shrinkRegion(_regionToZoomedCandidate, std::move(delta));
                    }
                    _originCandidate = cv::Point(x, y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Yellow;
                }
            }
            break;
        case cv::EVENT_LBUTTONDOWN:
            {
                if (!isCtrlKeyDown && _regionToZoomedCandidate.contains(cv::Point(x,y)) && _regionToZoomedSelected == false) {
                    _originCandidate = cv::Point(x,y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Cyan;
                    _regionToZoomedSelected = true;
                } 
                if (isCtrlKeyDown && _regionToZoomedCandidate.contains(cv::Point(x,y)) && _regionToZoomedSelected == false) {
                    _originCandidate = cv::Point(x, y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Yellow;
                }
            }
            
            break;
        case cv::EVENT_LBUTTONUP:
            {
                if (!isCtrlKeyDown && _regionToZoomedSelected == true) {
                    moveRegion(_regionToZoomedCandidate, _originCandidate, cv::Point(x,y));
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::White;
                    _regionToZoomedSelected = false;
                }
                if (isCtrlKeyDown && _regionToZoomedSelected == false) {    
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::White;
                }
            }
            break;
        case cv::EVENT_RBUTTONDOWN:
            if (_regionToZoomedCandidate.contains(cv::Point(x,y)) && _regionToZoomedSelected == false) {
                _originCandidate = cv::Point(x, y);
                _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Yellow;
            }
            break;
        case cv::EVENT_RBUTTONUP:
            if (_regionToZoomedSelected == false) {    
                _colorForRegionToZoomedCandidate = MandelbrotColor::Color::White;
            }
            break;
    }

    _origin = _originCandidate;
    setRegionToZoomed(_regionToZoomedCandidate);
    _colorForRegionToZoomed = MandelbrotColor::convertToVec3b(_colorForRegionToZoomedCandidate);
    _zoomedDisplay->updateRect(convertZoomedRegionToRect(_regionToZoomedCandidate));

}