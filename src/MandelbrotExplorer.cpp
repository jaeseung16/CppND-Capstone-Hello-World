#include <iostream>
#include <chrono>
#include <thread>

#include "MandelbrotExplorer.h"

const cv::Rect_<float> MandelbrotExplorer::defaultRect = cv::Rect_(-2.0, -1.5, 3.0, 3.0);
const cv::Rect_<float> MandelbrotExplorer::initialZoomedRect = cv::Rect_(-1.5, -0.2, 0.4, 0.4);
const int MandelbrotExplorer::defaultDisplaySize = 800;

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

void MandelbrotExplorer::getRangeToZoom() {
    float range = MandelbrotExplorer::defaultRect.width;

    float xmin_zoomed = _regionToZoomed.x + 125 * range / (float) (defaultDisplaySize + 1);
    float ymin_zoomed = _regionToZoomed.y + 350 * range / (float) (defaultDisplaySize + 1);
    float range_zoomed = 100 * range / (float) (defaultDisplaySize + 1);
}

void MandelbrotExplorer::showMandelbrotSet() {
    std::string windowName = "Mandelbrot";
    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, MandelbrotExplorer::onMouse, this);
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cv::Mat image = _staticDisplay.getMat().clone();
        cv::rectangle(image, _regionToZoomed, seletionColor);
        cv::imshow(windowName, image);

        cv::imshow("zoomed Mandelbrot", _zoomedDisplay.getMat());

        //cv::waitKey(33);
        // Quit when ESC pressed
        if((char)27 == cv::waitKey(30)) {
            break;
        }  
    }
}

void MandelbrotExplorer::onMouse(int event, int x, int y, int flags, void *that) {
    MandelbrotExplorer* thiz = static_cast<MandelbrotExplorer*>(that);
    thiz -> mouseClick(event, x, y, flags);
}

void MandelbrotExplorer::mouseClick(int event, int x, int y, int flags)
{
    std::string windowName = "Mandelbrot";
    switch(event) {
        case cv::EVENT_MOUSEMOVE:
            {
                bool isLeftButtonDown = (flags & cv::EVENT_FLAG_LBUTTON) != 0;
                bool isRightButtonDown = (flags & cv::EVENT_FLAG_RBUTTON) != 0;
                if (trackObject == true && isLeftButtonDown) {
                    _regionToZoomed.x += x - origin.x;
                    _regionToZoomed.y += y - origin.y;
                    origin = cv::Point(x,y);

                    seletionColor = cv::Vec3b(255,255,0);
                }

                if (trackObject == false && isRightButtonDown) {
                    int dx = x - origin.x;
                    int dy = y - origin.y;
                    int delta = (dx + dy) / 2;

                    std::cout << "delta = " << delta << std::endl;

                    if ( delta > 0) {
                        if (_regionToZoomed.x > _regionToZoomed.y) {
                            if (_regionToZoomed.x + _regionToZoomed.width + delta < defaultDisplaySize) {
                                _regionToZoomed.width += delta;
                            } else {
                                _regionToZoomed.width = defaultDisplaySize - _regionToZoomed.x;
                            }
                            _regionToZoomed.height = _regionToZoomed.width;
                        } else {
                            if (_regionToZoomed.y + _regionToZoomed.height + delta < defaultDisplaySize) {
                                _regionToZoomed.height += delta;
                            } else {
                                _regionToZoomed.height = defaultDisplaySize - _regionToZoomed.y;
                            }
                            _regionToZoomed.width = _regionToZoomed.height;
                        }
                    } else if (delta < 0) {
                        if (_regionToZoomed.width + delta >= 10) {
                            _regionToZoomed.width += delta;
                        }
                        _regionToZoomed.height = _regionToZoomed.width;
                    }

                    origin = cv::Point(x, y);
                    seletionColor = cv::Vec3b(0,255,255);
                }
            }
            break;
        case cv::EVENT_LBUTTONDOWN:
            {
                if ( x > _regionToZoomed.x && x < (_regionToZoomed.x + _regionToZoomed.width) && y > _regionToZoomed.y && y < (_regionToZoomed.y + _regionToZoomed.height)) {
                    if (trackObject == false) {
                        origin = cv::Point(x,y);
                        trackObject = true;

                        seletionColor = cv::Vec3b(255,255,0);
                    }
                } 
            }
            break;
        case cv::EVENT_LBUTTONUP:
            if (trackObject == true) {
                _regionToZoomed.x += x - origin.x;
                _regionToZoomed.y += y - origin.y;
                trackObject = false;
                    
                seletionColor = cv::Vec3b(255,255,255);
            }
            break;
        
        case cv::EVENT_RBUTTONDOWN:
            if (trackObject == false) {
                origin = cv::Point(x, y);
                seletionColor = cv::Vec3b(0,255,255);
            }
            break;

        case cv::EVENT_RBUTTONUP:
            if (trackObject == false) {    
                seletionColor = cv::Vec3b(255,255,255);
            }
            break;
    }
/*
    cv::Mat image = _staticDisplay.getMat().clone();
    cv::rectangle(image, _regionToZoomed, seletionColor);
    cv::imshow(windowName, image);
*/
    std::cout << "Selection in image = " << _regionToZoomed << std::endl;
    float xmin_zoomed = -2.0 + 3.0 * (float)_regionToZoomed.x / (defaultDisplaySize+1);
    float ymin_zoomed = -1.5 + 3.0 * (float)_regionToZoomed.y / (defaultDisplaySize+1);
    float range_zoomed = 3.0 * (float)(_regionToZoomed.width - 1) / (defaultDisplaySize+1);
    std::cout << "Selection = " << cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed) << std::endl;
    _zoomedDisplay = MandelbrotDisplay(cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed), defaultDisplaySize);

    //cv::imshow("zoomed Mandelbrot", _zoomedDisplay.getMat());
    
}