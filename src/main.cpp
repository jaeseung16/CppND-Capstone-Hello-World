#include <iostream>
#include <fstream>
#include <complex>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

float width = 600;
float height = 600;

int value(int x, int y) {
    std::complex<float> point(3.0*(float)x/width-2.0, 3.0*(float)y/height-1.5);
    std::complex<float> z(0,0);
    int nb_iter = 0;
    //std::cout << point << "\n";
    while (abs(z) <= 2 && nb_iter < 50) {
        z = z * z + point;
        nb_iter ++;
    }
    if (nb_iter < 50) {
        return (255 * nb_iter) / 50;
    } else {
        return 0;
    }
}

int main() {
    std::cout << "Hello World!" << "\n";

    std::ofstream my_Image("mandelbrot.ppm");

    cv::Mat imageMat(width, height, CV_8UC3, cv::Vec3b(0,0,0));

    if (my_Image.is_open()) {
        my_Image << "P3\n" << width << " " << height << " 255\n";
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int val = value(x, y);
                my_Image << val << ' ' << 0 << ' ' << 0 << "\n";
                imageMat.at<cv::Vec3b>(x, y) = cv::Vec3b(val,val,val);
            }
        }
        my_Image.close();
    } else {
        std::cout << "Could not open the file\n";
    }

    cv::imshow("My image", imageMat);

    cv::waitKey(0);

    return 0;
}