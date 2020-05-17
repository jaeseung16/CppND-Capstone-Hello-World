#include <iostream>
#include <fstream>
#include <complex>
#include <chrono>
#include <vector>
#include <algorithm>
#include <thread>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MandelbrotDisplay.h"

std::string windowName = "Mandelbrot";
float width = 800;
float height = 800;
int originalSize = 800;
bool selectObject = false;
bool trackObject = false;
int size = 100;
cv::Point origin;
cv::Rect selection(0, 0, 100, 100);
cv::Mat imageMat(width, height, CV_8UC3, cv::Vec3b(0,0,0));
cv::Mat zoomedImageMat(width, height, CV_8UC3, cv::Vec3b(0,0,0));
cv::Vec3b seletionColor = cv::Vec3b(255,255,255);

void onMouse(int event, int x, int y, int flags, void* )
{
    switch(event) {
        case cv::EVENT_MOUSEMOVE:
            {
                bool isLeftButtonDown = (flags & cv::EVENT_FLAG_LBUTTON) != 0;
                bool isRightButtonDown = (flags & cv::EVENT_FLAG_RBUTTON) != 0;
                if (trackObject == true && isLeftButtonDown) {
                    selection.x += x - origin.x;
                    selection.y += y - origin.y;
                    origin = cv::Point(x,y);

                    seletionColor = cv::Vec3b(255,255,0);
                }

                if (trackObject == false && isRightButtonDown) {
                    int dx = x - origin.x;
                    int dy = y - origin.y;
                    int delta = (dx + dy) / 2;

                    std::cout << "delta = " << delta << std::endl;

                    if ( delta > 0) {
                        if (selection.x > selection.y) {
                            if (selection.x + selection.width + delta < width) {
                                selection.width += delta;
                            } else {
                                selection.width = width - selection.x;
                            }
                            selection.height = selection.width;
                        } else {
                            if (selection.y + selection.height + delta < height) {
                                selection.height += delta;
                            } else {
                                selection.height = height - selection.y;
                            }
                            selection.width = selection.height;
                        }
                    } else if (delta < 0) {
                        if (selection.width + delta >= 10) {
                            selection.width += delta;
                        }
                        selection.height = selection.width;
                    }

                    origin = cv::Point(x, y);
                    seletionColor = cv::Vec3b(0,255,255);
                }
            }
            break;
        case cv::EVENT_LBUTTONDOWN:
            {
                if ( x > selection.x && x < (selection.x + selection.width) && y > selection.y && y < (selection.y + selection.height)) {
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
                selection.x += x - origin.x;
                selection.y += y - origin.y;
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

    cv::Mat image = imageMat.clone();
    cv::rectangle(image, selection, seletionColor);
    cv::imshow(windowName, image);

    std::cout << "Selection in image = " << selection << std::endl;
    float xmin_zoomed = -2.0 + 3.0 * (float)selection.x / (width+1);
    float ymin_zoomed = -1.5 + 3.0 * (float)selection.y / (height+1);
    float range_zoomed = 3.0 * (float)(selection.width - 1) / (width+1);
    std::cout << "Selection = " << cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed) << std::endl;
    MandelbrotDisplay zoomedDisplay = MandelbrotDisplay(cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed), originalSize);

    cv::imshow("zoomed Mandelbrot", zoomedDisplay.getMat());
}

void showMandelbrotSet() {
    //while(true) {
        float x_min = -2.0;
        float y_min = -1.5;
        float range = 3.0;

        MandelbrotDisplay display = MandelbrotDisplay(cv::Rect_<float>(x_min, y_min, range, range), originalSize);

        //auto durationToSleep = std::chrono::microseconds(2000) - std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        //std::this_thread::sleep_for(durationToSleep);

        cv::namedWindow(windowName);
        cv::setMouseCallback(windowName, onMouse, 0);
        
        selection.x = 125;
        selection.y = 350;
        selection.width = size;
        selection.height = size;
        
        imageMat = display.getMat().clone();
        cv::Mat image = display.getMat().clone();
        cv::rectangle(image, selection, cv::Vec3b(255,255,255));
        cv::imshow(windowName, image);

        float xmin_zoomed = x_min + 125 * range / (float) (originalSize + 1);
        float ymin_zoomed = y_min + 350 * range / (float) (originalSize + 1);
        float range_zoomed = 100 * range / (float) (originalSize + 1);

        MandelbrotDisplay zoomedDisplay = MandelbrotDisplay(cv::Rect_<float>(xmin_zoomed, ymin_zoomed, range_zoomed, range_zoomed), originalSize);
        zoomedImageMat = zoomedDisplay.getMat();
        cv::imshow("zoomed Mandelbrot", zoomedImageMat);

        cv::waitKey(0);
        //if((char)27 == cv::waitKey(30)) {
        //    break;
        //}  
    //}
    
}

int main() {
    std::cout << "### Mandelbrot Explorer ###" << "\n";
    showMandelbrotSet();
    return 0;
}