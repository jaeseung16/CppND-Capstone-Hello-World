#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>

#include "MandelbrotDisplay.h"

enum class Color { Red, Yellow, Green, Cyan, Blue, White };

class MandelbrotExplorer
{
public:
    static const cv::Rect_<float> defaultRect;
    static const cv::Rect_<float> initialZoomedRect;
    static const int defaultDisplaySize;
    const cv::Rect defaultDisplayRect;

    static void onMouse(int event, int x, int y, int flags, void *that);

    MandelbrotExplorer();
    void getRangeToZoomed();
    void showMandelbrotSet();
    void mouseClick(int event, int x, int y, int flags);

private:
    MandelbrotDisplay _staticDisplay = MandelbrotDisplay(defaultRect, defaultDisplaySize);
    MandelbrotDisplay _zoomedDisplay = MandelbrotDisplay(initialZoomedRect, defaultDisplaySize);

    cv::Rect _regionToZoomed;
    cv::Point _origin;
    cv::Vec3b _colorForRegionToZoomed = cv::Vec3b(255,255,255);

    bool _regionToZoomedSelected = false;
    bool _regionToZoomedTracked = false;  

    std::mutex _mutex;

    void updateColor(Color color); 
    cv::Rect_<float> convertRangeToZoomedToComplex();
    void moveRegion(cv::Point &&point);
    void updateOrigin(cv::Point &&point);
    void shrinkRegion(int &&delta);
    void enlargeRegion(int &&delta);
    int determineDelta(int x, int y);
};