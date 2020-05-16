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
#include "MandelbrotSet.h"

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

                    cv::Mat image = imageMat.clone();
                    cv::rectangle(image, selection, cv::Vec3b(255,255,0));
                    cv::imshow(windowName, image);
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
                    cv::Mat image = imageMat.clone();
                    cv::rectangle(image, selection, cv::Vec3b(0,255,255));
                    cv::imshow(windowName, image);
                }
            }
            break;
        case cv::EVENT_LBUTTONDOWN:
            {
                if ( x > selection.x && x < (selection.x + selection.width) && y > selection.y && y < (selection.y + selection.height)) {
                    if (trackObject == false) {
                        origin = cv::Point(x,y);
                        trackObject = true;

                        cv::Mat image = imageMat.clone();
                        cv::rectangle(image, selection, cv::Vec3b(255,255,0));
                        cv::imshow(windowName, image);
                    }
                } 
            }
            break;
        case cv::EVENT_LBUTTONUP:
            if (trackObject == true) {
                selection.x += x - origin.x;
                selection.y += y - origin.y;
                trackObject = false;
                    
                cv::Mat image = imageMat.clone();
                cv::rectangle(image, selection, cv::Vec3b(255,255,255));
                cv::imshow(windowName, image);
            }
            break;
        
        case cv::EVENT_RBUTTONDOWN:
            if (trackObject == false) {
                origin = cv::Point(x, y);

                cv::Mat image = imageMat.clone();
                cv::rectangle(image, selection, cv::Vec3b(0,255,255));
                cv::imshow(windowName, image);
            }
            break;

        case cv::EVENT_RBUTTONUP:
            if (trackObject == false) {    
                cv::Mat image = imageMat.clone();
                cv::rectangle(image, selection, cv::Vec3b(255,255,255));
                cv::imshow(windowName, image);
            }
            break;
    }

    std::vector<int> xs(width);
    std::iota(xs.begin(), xs.end(), 0);
    std::vector<int> ys(height);
    std::iota(ys.begin(), ys.end(), 0);
    cv::Mat zoomedImageMat(width, height, CV_8UC3, cv::Vec3b(0,0,0));

    std::vector<std::complex<float>> zs;
    for (int x : xs) {
        for (int y : ys) {
            zs.push_back(std::complex<float>(3.0 * ((float)x * selection.width / width + selection.x) / width - 2.0, 3.0 * ((float)y * selection.height / height + selection.y) / height - 1.5));
        }
    }

    auto start2 = std::chrono::high_resolution_clock::now();
    MandelbrotSet set = MandelbrotSet(zs, 50);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::cout << "It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << " ms" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<MandelbrotPoint> points = set.getSet();
    std::vector<bool> isMandelbrotSet = set.getIsMandelbrotSet();
    std::vector<unsigned int> iterations = set.getIterations();

    for (int count = 0; count < points.size(); count++) {
        int x = count / originalSize;
        int y = count % originalSize;

        int val = isMandelbrotSet.at(count) ? 0 : 255 * (int)iterations.at(count) / 50;
        zoomedImageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(0, val, 0);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Selection = " << selection << std::endl;
    std::cout << "Zoomed Image: It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    MandelbrotDisplay zoomedDisplay = MandelbrotDisplay(zoomedImageMat, width, 3.0 * selection.width / width, 3.0 * selection.x / width - 2.0, 3.0 * selection.y / height - 1.5);

    cv::imshow("zoomed Mandelbrot", zoomedDisplay.getMandelbrotSet());
}

void showMandelbrotSet() {
    //while(true) {
        float x_min = -2.0;
        float y_min = -1.5;

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<int> xs(originalSize);
        std::iota(xs.begin(), xs.end(), 0);

        std::vector<int> ys(originalSize);
        std::iota(ys.begin(), ys.end(), 0);

        std::vector<std::complex<float>> zs;
        for (int x : xs) {
            for (int y : ys) {
                zs.push_back(std::complex<float>(3.0*(float)x/originalSize+x_min, 3.0*(float)y/originalSize+y_min));
            }
        }

        auto start2 = std::chrono::high_resolution_clock::now();
        MandelbrotSet set = MandelbrotSet(zs, 50);
        auto end2 = std::chrono::high_resolution_clock::now();
        std::cout << "It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << " ms" << std::endl;

        std::vector<MandelbrotPoint> points = set.getSet();
        std::vector<bool> isMandelbrotSet = set.getIsMandelbrotSet();
        std::vector<unsigned int> iterations = set.getIterations();

        for (int count = 0; count < points.size(); count++) {
            int x = count / originalSize;
            int y = count % originalSize;

            int val = isMandelbrotSet.at(count) ? 0 : 255 * (int)iterations.at(count) / 50;
            imageMat.at<cv::Vec3b>(y, x) = cv::Vec3b(0, val, 0);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        MandelbrotDisplay display = MandelbrotDisplay(imageMat, width, 3.0 / width, x_min, y_min);

        //auto durationToSleep = std::chrono::microseconds(2000) - std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        //std::this_thread::sleep_for(durationToSleep);
        cv::namedWindow(windowName);

        cv::setMouseCallback(windowName, onMouse, 0);
        
        selection.x = 125;
        selection.y = 350;
        selection.width = size;
        selection.height = size;
        
        cv::Mat image = display.getMandelbrotSet().clone();
        cv::rectangle(image, selection, cv::Vec3b(255,255,255));
        cv::imshow(windowName, image);

        cv::waitKey(0);
        //if((char)27 == cv::waitKey(30)) {
        //    break;
        //}  
    //}
    
}

int main() {
    std::cout << "Hello World!" << "\n";
    showMandelbrotSet();
    return 0;
}