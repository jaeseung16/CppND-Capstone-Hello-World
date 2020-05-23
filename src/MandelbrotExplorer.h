#include <memory>
#include <thread>
#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>

#include "MandelbrotDisplay.h"

class MandelbrotExplorer
{
public:
    static const cv::Rect_<float> defaultRect;
    static const int defaultDisplaySize;
    static const std::string defaultDiplayWindowName;
    static const std::string zoomedDiplayWindowName;

    static void onMouse(int event, int x, int y, int flags, void *that);

    MandelbrotExplorer();
    void showMandelbrotSet();
    void mouseClick(int event, int x, int y, int flags);
    void setRegionToZoomed(cv::Rect region);
    cv::Rect getRegionToZoomed();
    void setColorForRegionToZoomed(cv::Vec3b color);
    cv::Vec3b getColorForRegionToZoomed();

private:
    std::unique_ptr<MandelbrotDisplay> _staticDisplay;
    std::unique_ptr<MandelbrotDisplay> _zoomedDisplay;

    cv::Rect _regionToZoomed;
    cv::Point _origin;
    cv::Vec3b _colorForRegionToZoomed = cv::Vec3b(255,255,255);
    cv::Rect _defaultDisplayRect;

    bool _regionToZoomedSelected = false;
    bool _regionToZoomedUpdated = false;

    std::mutex _mutex;

    cv::Rect_<float> convertZoomedRegionToRect(cv::Rect regionToZoomed);
    void moveRegion(cv::Rect &region, const cv::Point origin, cv::Point &&point);
    void shrinkRegion(cv::Rect &region, int &&delta);
    void enlargeRegion(cv::Rect &region, int &&delta);
    int determineDelta(const cv::Point oldPoint, const cv::Point newPoint);
    void putScale(cv::Mat &image);

    const cv::Point _scaleTextPosition = cv::Point(defaultDisplaySize * 90 / 100, defaultDisplaySize * 3 / 100);
    const double _fontScaleForScaleText = 0.7;
    const int _thicknessForScaleText = 2;
    const cv::Vec3b _fontColorForScaleText = MandelbrotColor::convertToVec3b(MandelbrotColor::Color::Yellow);

    const cv::Point _topleftTextPosition = cv::Point(0, defaultDisplaySize * 3 / 100);
    const cv::Point _bottomrightTextPosition = cv::Point(defaultDisplaySize * 85 / 100, defaultDisplaySize * 97 / 100);
    const double _fontScaleForLimitText = 0.5;
    const int _thicknessForLimitText = 1;

    const int _fontFace = cv::FONT_HERSHEY_SIMPLEX;
    const cv::Vec3b _fontColorForLimitText = MandelbrotColor::convertToVec3b(MandelbrotColor::Color::White);
};