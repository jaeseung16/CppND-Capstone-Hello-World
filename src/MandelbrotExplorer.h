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
    static const cv::Rect_<float> initialZoomedRect;
    static const cv::Rect initialRegionToZoomed;
    static const int defaultDisplaySize;

    static void onMouse(int event, int x, int y, int flags, void *that);

    MandelbrotExplorer();
    void showMandelbrotSet();
    void mouseClick(int event, int x, int y, int flags);

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

    cv::Rect_<float> convertRangeToZoomedToComplex(cv::Rect regionToZoomed);
    void moveRegion(cv::Rect &region, const cv::Point origin, cv::Point &&point);
    void shrinkRegion(cv::Rect &region, int &&delta);
    void enlargeRegion(cv::Rect &region, int &&delta);
    int determineDelta(const cv::Point oldPoint, const cv::Point newPoint);
};