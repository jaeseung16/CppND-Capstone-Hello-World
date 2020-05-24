#include <memory>
#include <future>
#include <mutex>
#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MandelbrotSet.h"
#include "MandelbrotColor.h"
#include "MandelbrotQueue.h"

class MandelbrotDisplay {
public:
    static const int maxIterations;

    MandelbrotDisplay();
    ~MandelbrotDisplay();
    MandelbrotDisplay(const MandelbrotDisplay &source);
    MandelbrotDisplay &operator=(const MandelbrotDisplay &source);
    MandelbrotDisplay(MandelbrotDisplay &&source);
    MandelbrotDisplay &operator=(MandelbrotDisplay &&source);

    MandelbrotDisplay(cv::Rect_<float> region, int displaySize, MandelbrotColor::Color color);

    cv::Mat getMat();
    MandelbrotSet getMandelbrotSet() { return *_mandelbrotSet; };
    cv::Rect_<float> getRegion();
    void setRegion(cv::Rect_<float> region);

    void updateRect(cv::Rect_<float> region);
    void simulate();
    bool isUpdated();
    
private:
    int _displaySize;
    float _scale;
    cv::Mat _mat;
    cv::Vec3b _color;
    cv::Rect_<float> _region;
    std::mutex _mutex;
    std::vector<std::thread> threads;
    std::unique_ptr<MandelbrotSet> _mandelbrotSet;
    MandelbrotQueue<cv::Rect_<float>> _queue;
    
    enum Status { needToUpdate, readyToDisplay, waitForUpdate, done };
    Status _status;

    std::vector<std::complex<float>> generateComplexPointsFromRegion(cv::Rect_<float> region);
    MandelbrotDisplay::Status getStatus();
    void setStatus(MandelbrotDisplay::Status status);
    void cycleThroughPhases();
    void generateMandelbrotSet();
    void generateMat();
    float calculateScale();
    void updateMandelbrotSet();
};