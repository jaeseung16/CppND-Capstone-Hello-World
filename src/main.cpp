#include <iostream>
#include <fstream>
#include <complex>
#include <chrono> 
#include <thread>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

#include "MandelbrotPoint.h"

float width = 800;
float height = 800;

int value(int x, int y, float x_min, float y_min) {
    std::complex<float> point(3.0*(float)x/width+x_min, 3.0*(float)y/height+y_min);

    MandelbrotPoint mPoint = MandelbrotPoint(point, (unsigned int) 50);

    if (mPoint.isMandelBrotSet()) {
        return 0;
    } else {
        return 255 * (int)mPoint.getIteration() / 50;
    }
}

void showMandelbrotSet() {
    while(true) {
        std::random_device rd; 
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);

        float x_min = -2.0 + uniform_dist(gen);
        float y_min = -1.5 + uniform_dist(gen);

        cv::Mat imageMat(width, height, CV_8UC3, cv::Vec3b(0,0,0));
        auto start = std::chrono::high_resolution_clock::now();
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    
                int val = value(x, y, x_min, y_min);
                imageMat.at<cv::Vec3b>(x, y) = cv::Vec3b(0,val,0);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        auto durationToSleep = std::chrono::microseconds(2000) - std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::this_thread::sleep_for(durationToSleep);

        cv::imshow("My image", imageMat);
        if((char)27 == cv::waitKey(30)) {
            break;
        }  
    }
    
}

int main() {
    std::cout << "Hello World!" << "\n";
    showMandelbrotSet();
    return 0;
}