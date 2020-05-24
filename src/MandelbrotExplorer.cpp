#include <iostream>
#include <chrono>
#include <thread>

#include <opencv2/imgproc.hpp>

#include "MandelbrotExplorer.h"

const cv::Rect_<float> MandelbrotExplorer::defaultRect = cv::Rect_<float>(-2.0, -1.5, 3.0, 3.0);
const int MandelbrotExplorer::defaultDisplaySize = 600;
const std::string MandelbrotExplorer::defaultDiplayWindowName = "Mandelbrot";
const std::string MandelbrotExplorer::zoomedDiplayWindowName = "Zoomed Mandelbrot";

MandelbrotExplorer::MandelbrotExplorer()
{
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

MandelbrotExplorer::~MandelbrotExplorer()
{

}

MandelbrotExplorer::MandelbrotExplorer(const MandelbrotExplorer &source)
{
    std::cout << "MandelbrotExplorer Copy Constructor" << std::endl;
    _regionToZoomed = source._regionToZoomed;
    _origin = source._origin;
    _colorForRegionToZoomed = source._colorForRegionToZoomed;
    _defaultDisplayRect = source._defaultDisplayRect;

    _staticDisplay = std::make_unique<MandelbrotDisplay>(*(source._staticDisplay));
    _zoomedDisplay = std::make_unique<MandelbrotDisplay>(*(source._zoomedDisplay));
}

MandelbrotExplorer &MandelbrotExplorer::operator=(const MandelbrotExplorer &source)
{
    std::cout << "MandelbrotExplorer Copy Assignment Operator" << std::endl;
    if (this == &source)
        return *this;
        
    _regionToZoomed = source._regionToZoomed;
    _origin = source._origin;
    _colorForRegionToZoomed = source._colorForRegionToZoomed;
    _defaultDisplayRect = source._defaultDisplayRect;

    _staticDisplay = std::make_unique<MandelbrotDisplay>(*(source._staticDisplay));
    _zoomedDisplay = std::make_unique<MandelbrotDisplay>(*(source._zoomedDisplay));

    return *this;
}

MandelbrotExplorer::MandelbrotExplorer(MandelbrotExplorer &&source)
{
    std::cout << "MandelbrotExplorer Move Constructor" << std::endl;
    _regionToZoomed = source._regionToZoomed;
    _origin = source._origin;
    _colorForRegionToZoomed = source._colorForRegionToZoomed;
    _defaultDisplayRect = source._defaultDisplayRect;

    _staticDisplay = std::move(_staticDisplay);
    _zoomedDisplay = std::move(_zoomedDisplay);
}

MandelbrotExplorer &MandelbrotExplorer::operator=(MandelbrotExplorer &&source){
    std::cout << "MandelbrotExplorer Move Assignment Operator" << std::endl;
    if (this == &source)
        return *this;
        
    _regionToZoomed = source._regionToZoomed;
    _origin = source._origin;
    _colorForRegionToZoomed = source._colorForRegionToZoomed;
    _defaultDisplayRect = source._defaultDisplayRect;

    _staticDisplay = std::move(_staticDisplay);
    _zoomedDisplay = std::move(_zoomedDisplay);

    return *this;
}


cv::Rect_<float> MandelbrotExplorer::convertZoomedRegionToRect(cv::Rect regionToZoomed)
{
    float xmin = defaultRect.x + defaultRect.width * (float)regionToZoomed.x / (defaultDisplaySize+1);
    float ymin = defaultRect.y + defaultRect.height * (float)regionToZoomed.y / (defaultDisplaySize+1);
    float range = defaultRect.width * (float)regionToZoomed.width / (defaultDisplaySize+1);
    return cv::Rect_<float>(xmin, ymin, range, range);
}

void MandelbrotExplorer::setRegionToZoomed(cv::Rect region)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _regionToZoomed = region;
}

cv::Rect MandelbrotExplorer::getRegionToZoomed()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _regionToZoomed;
}

void MandelbrotExplorer::setColorForRegionToZoomed(cv::Vec3b color)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _colorForRegionToZoomed = color;
}

cv::Vec3b MandelbrotExplorer::getColorForRegionToZoomed()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _colorForRegionToZoomed;
}

std::string convertFloatToString(float number)
{
    return std::to_string(number).substr(0,4);
}

void MandelbrotExplorer::putScaleAndLimt(cv::Mat &image)
{
    std::string scale = "X" + convertFloatToString((float)defaultDisplaySize / (float)_regionToZoomed.width);
    cv::putText(image, scale, _scaleTextPosition, _fontFace, _fontScaleForScaleText, _fontColorForScaleText, _thicknessForScaleText);

    cv::Rect_<float> region = _zoomedDisplay->getRegion();
    std::string topleftText = "(" + convertFloatToString(region.x) + ", " + convertFloatToString(region.y) + ")";
    cv::putText(image, topleftText, _topleftTextPosition, _fontFace, _fontScaleForLimitText, _fontColorForLimitText, _thicknessForLimitText);

    std::string bottomrightText = "(" + convertFloatToString(region.x + region.width) + ", " + convertFloatToString(region.y + region.height) + ")";
    cv::putText(image, bottomrightText, _bottomrightTextPosition, _fontFace, _fontScaleForLimitText, _fontColorForLimitText, _thicknessForLimitText);
}

void MandelbrotExplorer::showMandelbrotSet()
{
    std::cout << "### Present Mandelbrot Set ###" << std::endl;

    cv::namedWindow(defaultDiplayWindowName);
    cv::moveWindow(defaultDiplayWindowName, defaultDisplaySize/10, defaultDisplaySize/10);
    cv::namedWindow(zoomedDiplayWindowName);
    cv::moveWindow(zoomedDiplayWindowName, defaultDisplaySize * 11/10, defaultDisplaySize/10);
    cv::setMouseCallback(defaultDiplayWindowName, MandelbrotExplorer::onMouse, this);

    cv::Mat zoomedImage = _zoomedDisplay->getMat();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cv::Mat image = _staticDisplay->getMat();
        cv::rectangle(image, getRegionToZoomed(), getColorForRegionToZoomed());
        cv::imshow(defaultDiplayWindowName, image);

        if (_zoomedDisplay->isUpdated())
        {
            std::cout << "Updating _zoomedDisplay" << std::endl;
            zoomedImage = _zoomedDisplay->getMat();
        }
        putScaleAndLimt(zoomedImage);
        cv::imshow(zoomedDiplayWindowName, zoomedImage);

        if((char)27 == cv::waitKey(30))
        {
            break;
        }  
    }

    std::cout << "Terminating MandelbrotExplorer" << std::endl;
}

void MandelbrotExplorer::onMouse(int event, int x, int y, int flags, void *that)
{
    MandelbrotExplorer* thiz = static_cast<MandelbrotExplorer*>(that);
    thiz -> mouseClick(event, x, y, flags);
}

void MandelbrotExplorer::moveRegion(cv::Rect &region, const cv::Point origin, cv::Point &&point)
{
    region += point - origin;
    if (!_defaultDisplayRect.contains(region.tl()) || !_defaultDisplayRect.contains(region.br()))
    {
        if (region.x < 0)
        {
            region.x = 0;
        }
        else if (region.x >= (defaultDisplaySize - region.width))
        {
            region.x = defaultDisplaySize - region.width;
        } 
        if (region.y < 0)
        {
            region.y = 0;
        } 
        if (region.y >= (defaultDisplaySize - region.height))
        {
            region.y = defaultDisplaySize - region.height;
        } 
    }
}

void MandelbrotExplorer::shrinkRegion(cv::Rect &region, int &&delta)
{
    if (delta < 0 && region.width + delta >= 10)
    {
        region.width += delta;
        region.height = region.width;
    }
}

void MandelbrotExplorer::enlargeRegion(cv::Rect &region, int &&delta)
{
    if (delta > 0)
    {
        region += cv::Size_<int>(delta, delta);
        if (!_defaultDisplayRect.contains(region.br()))
        {
            if (region.x > region.y)
            {
                region.width = defaultDisplaySize - region.x;
                region.height = region.width;
            }
            else
            {
                region.height = defaultDisplaySize - region.y;
                region.width = region.height;
            }
        }
    }
}

int MandelbrotExplorer::determineDelta(const cv::Point oldPoint, const cv::Point newPoint)
{
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

    switch(event)
    {
        case cv::EVENT_MOUSEMOVE:
            {
                if (_regionToZoomedCanMove == true && isLeftButtonDown)
                {
                    moveRegion(_regionToZoomedCandidate, _originCandidate, cv::Point(x,y));
                    _originCandidate = cv::Point(x,y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Cyan;
                }

                if (_regionToZoomedCanResize == true && _regionToZoomedCanMove == false && (isRightButtonDown || (isCtrlKeyDown && isLeftButtonDown)))
                {
                    int delta = determineDelta(_originCandidate, cv::Point(x,y));
                    if (delta > 0)
                    {
                        enlargeRegion(_regionToZoomedCandidate, std::move(delta));
                    }
                    else if (delta < 0)
                    {
                        shrinkRegion(_regionToZoomedCandidate, std::move(delta));
                    }
                    _originCandidate = cv::Point(x, y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Yellow;
                }
            }
            break;
        case cv::EVENT_LBUTTONDOWN:
            {
                if (!isCtrlKeyDown && _regionToZoomedCandidate.contains(cv::Point(x,y)) && _regionToZoomedCanMove == false)
                {
                    _originCandidate = cv::Point(x,y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Cyan;
                    _regionToZoomedCanMove = true;
                } 
                if (isCtrlKeyDown && _regionToZoomedCandidate.contains(cv::Point(x,y)) && _regionToZoomedCanResize == false && _regionToZoomedCanMove == false)
                {
                    _originCandidate = cv::Point(x, y);
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Yellow;
                    _regionToZoomedCanResize = true;
                }
            }
            break;
        case cv::EVENT_LBUTTONUP:
            {
                if (!isCtrlKeyDown && _regionToZoomedCanMove == true) {
                    moveRegion(_regionToZoomedCandidate, _originCandidate, cv::Point(x,y));
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::White;
                    _regionToZoomedCanMove = false;
                }
                if (isCtrlKeyDown && _regionToZoomedCanResize == true && _regionToZoomedCanMove == false) {    
                    _colorForRegionToZoomedCandidate = MandelbrotColor::Color::White;
                    _regionToZoomedCanMove = false;
                }
            }
            break;
        case cv::EVENT_RBUTTONDOWN:
            if (_regionToZoomedCandidate.contains(cv::Point(x,y)) && _regionToZoomedCanResize == false && _regionToZoomedCanMove == false)
            {
                _originCandidate = cv::Point(x, y);
                _colorForRegionToZoomedCandidate = MandelbrotColor::Color::Yellow;
                _regionToZoomedCanResize = true;
            }
            break;
        case cv::EVENT_RBUTTONUP:
            if (_regionToZoomedCanResize == true && _regionToZoomedCanMove == false)
            {    
                _colorForRegionToZoomedCandidate = MandelbrotColor::Color::White;
                _regionToZoomedCanResize = false;
            }
            break;
    }

    _origin = _originCandidate;
    setColorForRegionToZoomed(MandelbrotColor::convertToVec3b(_colorForRegionToZoomedCandidate));

    if (_regionToZoomedCandidate != getRegionToZoomed()) {
        setRegionToZoomed(_regionToZoomedCandidate);
        _zoomedDisplay->updateRect(convertZoomedRegionToRect(_regionToZoomedCandidate));
    }
}