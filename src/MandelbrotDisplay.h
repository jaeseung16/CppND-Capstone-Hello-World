#include <memory>
#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MandelbrotSet.h"
#include "MandelbrotColor.h"

class MandelbrotDisplay {
public:
    MandelbrotDisplay();
    ~MandelbrotDisplay();
    MandelbrotDisplay(const MandelbrotDisplay &source);
    MandelbrotDisplay &operator=(const MandelbrotDisplay &source);
    MandelbrotDisplay(MandelbrotDisplay &&source);
    MandelbrotDisplay &operator=(MandelbrotDisplay &&source);

    MandelbrotDisplay(cv::Rect_<float> selection, int size, MandelbrotColor::Color color);
    void generateMandelbrotSet(std::promise<void> &&promise);

    cv::Mat getMat() { return _mat.clone(); };
    MandelbrotSet getMandelbrotSet() { return *_mandelbrotSet; };

    void updateRect(cv::Rect_<float> selection);
    bool isReadyToDisplay() { return _readyToDisplay; };

private:
    cv::Mat _mat;
    cv::Vec3b _color;
    std::unique_ptr<MandelbrotSet> _mandelbrotSet;
    int _size;
    float _scale;
    float _xmin;
    float _xmax;
    float _ymin;
    float _ymax;

    bool _readyToDisplay;

    void generateMat();

};