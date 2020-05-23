#include <iostream>
#include <chrono>
#include <thread>

#include "MandelbrotExplorer.h"

const cv::Rect_<float> MandelbrotExplorer::defaultRect = cv::Rect_<float>(-2.0, -1.5, 3.0, 3.0);
const cv::Rect_<float> MandelbrotExplorer::initialZoomedRect = cv::Rect_<float>(-1.5, -0.2, 0.4, 0.4);
const cv::Rect MandelbrotExplorer::initialRegionToZoomed = cv::Rect(133, 346, 106, 106);
const int MandelbrotExplorer::defaultDisplaySize = 800;

MandelbrotExplorer::MandelbrotExplorer() {
    std::cout << "### MandelbrotExplorer ###" << std::endl;

    _staticDisplay = std::unique_ptr<MandelbrotDisplay>(new MandelbrotDisplay(defaultRect, defaultDisplaySize, MandelbrotColor::Color::Red));
    
    float scale = initialZoomedRect.width / defaultRect.width;
    float range = (float)defaultDisplaySize * scale ;
    float regionX = (int) ((initialZoomedRect.x - defaultRect.x) * (float)defaultDisplaySize / defaultRect.width);
    float regionY = (int) ((initialZoomedRect.y - defaultRect.y) * (float)defaultDisplaySize / defaultRect.width);
    
    _regionToZoomed = cv::Rect((int)regionX, (int)regionY, (int)range, (int)range);
    _defaultDisplayRect = cv::Rect(0, 0, defaultDisplaySize, defaultDisplaySize);

    _zoomedDisplay = std::unique_ptr<MandelbrotDisplay>(new MandelbrotDisplay(convertRangeToZoomedToComplex(initialRegionToZoomed), defaultDisplaySize, MandelbrotColor::Color::Green));
    _zoomedDisplay->simulate();

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


cv::Rect_<float> MandelbrotExplorer::convertRangeToZoomedToComplex(cv::Rect regionToZoomed) {
    float xmin_zoomed = defaultRect.x + defaultRect.width * (float)regionToZoomed.x / (defaultDisplaySize+1);
    float ymin_zoomed = defaultRect.y + defaultRect.height * (float)regionToZoomed.y / (defaultDisplaySize+1);
    float range_zoomed = defaultRect.width * (float)(regionToZoomed.width - 1) / (defaultDisplaySize+1);
    return cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed);
}

void MandelbrotExplorer::showMandelbrotSet() {
    std::string windowName = "Mandelbrot";
    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, MandelbrotExplorer::onMouse, this);
    cv::Mat zoomedImage = _zoomedDisplay->getMat();
    while(true) {
        //auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cv::Mat image = _staticDisplay->getMat();
        cv::rectangle(image, _regionToZoomed, _colorForRegionToZoomed);
        cv::imshow(windowName, image);

        if (_zoomedDisplay->isUpdated()) {
            std::cout << "Updating _zoomedDisplay " << (_zoomedDisplay->isUpdated()) << std::endl;
            zoomedImage = _zoomedDisplay->getMat();
        }
        cv::imshow("zoomed Mandelbrot", zoomedImage);

        //cv::waitKey(33);
        // Quit when ESC pressed
        if((char)27 == cv::waitKey(30)) {
            std::cout << "ESC PRESSED" << std::endl;
            break;
        }  

        //auto end = std::chrono::high_resolution_clock::now();
        //std::cout << "MandelbrotExplorer::showMandelbrotSet() took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
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
    _regionToZoomed = _regionToZoomedCandidate;
    _colorForRegionToZoomed = MandelbrotColor::convertToVec3b(_colorForRegionToZoomedCandidate);
    _zoomedDisplay->updateRect(convertRangeToZoomedToComplex(_regionToZoomedCandidate));

}