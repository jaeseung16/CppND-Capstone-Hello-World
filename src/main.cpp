#include <iostream>
#include <fstream>
#include <complex>
#include <chrono> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

#include "MandelbrotPoint.h"

float width = 800;
float height = 800;

int value(int x, int y) {
    std::complex<float> point(3.0*(float)x/width-2.0, 3.0*(float)y/height-1.5);

    MandelbrotPoint mPoint = MandelbrotPoint(point, (unsigned int) 50);

    if (mPoint.isMandelBrotSet()) {
        return 0;
    } else {
        return 255 * (int)mPoint.getIteration() / 50;
    }
}

int main() {
    std::cout << "Hello World!" << "\n";

    std::ofstream my_Image("mandelbrot.ppm");

    cv::Mat imageMat(width, height, CV_8UC3, cv::Vec3b(0,0,0));

    if (my_Image.is_open()) {
        my_Image << "P3\n" << width << " " << height << " 255\n";

        auto start = std::chrono::high_resolution_clock::now();
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int val = value(x, y);
                my_Image << val << ' ' << 0 << ' ' << 0 << "\n";
                imageMat.at<cv::Vec3b>(x, y) = cv::Vec3b(0,val,0);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
        my_Image.close();
    } else {
        std::cout << "Could not open the file\n";
    }

    cv::imshow("My image", imageMat);

    cv::waitKey(0);

    return 0;
}