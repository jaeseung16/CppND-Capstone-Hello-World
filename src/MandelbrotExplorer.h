#include <opencv2/opencv.hpp>
#include "MandelbrotDisplay.h"

class MandelbrotExplorer
{
public:
    static const cv::Rect_<float> defaultRect;
    static const cv::Rect_<float> initialZoomedRect;
    static const int defaultDisplaySize;

    static void onMouse(int event, int x, int y, int flags, void *that);

    MandelbrotExplorer();
    void getRangeToZoom();
    void showMandelbrotSet();
    void mouseClick(int event, int x, int y, int flags);

private:
    MandelbrotDisplay _staticDisplay = MandelbrotDisplay(defaultRect, defaultDisplaySize);
    MandelbrotDisplay _zoomedDisplay = MandelbrotDisplay(initialZoomedRect, defaultDisplaySize);

    cv::Rect _regionToZoomed;
    cv::Point origin;
    cv::Vec3b seletionColor = cv::Vec3b(255,255,255);

    bool selectObject = false;
    bool trackObject = false;   

};