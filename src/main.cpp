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

#include "MandelbrotExplorer.h"

int main() {
    std::cout << "### Mandelbrot Explorer Initializing ###" << "\n";
    std::string windowName = "Mandelbrot";
    
    MandelbrotExplorer();
    
    return 0;
}