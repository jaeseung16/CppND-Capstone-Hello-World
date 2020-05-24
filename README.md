# CPPND: Capstone MandelbrotExplorer

## How to build the project

- Download or clone the repository: `git clone https://github.com/jaeseung16/CppND-Capstone-MandelbrotExplorer.git`
- Build the project
  1. Change the directory: `cd CppND-Capstone-MandelbrotExplorer`
  2. Make the build directory: `mkdir build`
  3. Change the directory: `cd build`
  4. Run cmake: `cmake ..`
  5. Run make: `make`

## How to run the project

- Change the directory : `cd CppND-Capstone-MandelbrotExplorer/build`
- Run MandelbrotExplorer: `./MandelbrotExplorer`
  1. Two windows named **Mandelbrot** and **Zoomed Mandelbrot** will appear
  2. Move the white box in the window **Mandelbort**
     - Bring the mouse pointer inside the white box
     - Click the left button of the mouse. The box color will change.
     - Move the mouse while holding the left button of the mouse
     - Stop moving the mouse and release the left button. The box color will become white.
  3. Resize the white box in the window **Mandelbort**
     - Move the mouse pointer inside the window **Mandelbort**
     - While holding the CTRL key, click the left button of the mouse (or click and hold the right button of the mouse). The box color will change.
     - Move the mouse while holding the CTRL key and the left button (or holding the right button of the mouse).
     - The box will shrink (or expand) if the mouse moves toward the top-left (or bottom-right) corner.
     - Release the left button of the mouse while holding the CTRL key (or release the right button). The box color will become white.
  4. To end, press the `ESC` key

## Requirements

- gcc/g++ >= 5.4
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: [Install Xcode command line tools](https://developer.apple.com/xcode/resources/)
  - Windows: [MinGW](http://www.mingw.org/)
- make >= 4.1 (Linux), 3.8 (Mac)
  - Linux: make is installed by default on most Linux distros
  - Mac: [Install Xcode command line tools](https://developer.apple.com/xcode/resources/)
  - Windows: [Make for Windows](http://gnuwin32.sourceforge.net/packages/make.htm)
- cmake >= 3.17
  - All OSes: [Installing CMake](https://cmake.org/install/)
- OpenCV >= 3.3 (Linux), 4.3 (Mac)
  - Linux: [Installation in Linux](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)
  - Mac: [Installation in MacOS](https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html)
  - Windows: [Installation in Windows](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html)
- Tested under Ubuntu 16.04.6 LTS and macOS Catalina 10.15.4

## Project Description

The project is based on one of the suggestions in Capstone Option 1: To explore the [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set). One can move around within the interested part of the Mandelbrot set and investigate the details.

- Class structure
  - `MandelbrotExplorer`: Main class showing two Mandelbrot images (provided by `MandelbrotDisplay`) and processing user inputs
  - `MandelbrotDisplay` : Class providing Mandelbrot images given the range of complex numbers.
  - `MandelbrotSet`: Class providing `MandelbrotDisplay` with Mandelbrot data
  - `MandelbrotColor`: A support class for color conversion

### Project Rubric Points

#### README (All Rubric Points REQUIRED)

- [x] A README with instructions is included with the project
- [x] The README indicates which project is chosen.
- [x] The README includes information about each rubric point addressed.

### Compiling and Testing (All Rubric Points REQUIRED)

- [x] The submission must compile and run.

### Loops, Functions, I/O

- [x] The project demonstrates an understanding of C++ functions and control structures.
  - `MandelbrotExplorer.cpp`, line numbers 156-176, 187-239, 257-328
  - `MandelbrotDisplay.cpp`. line numbers 108-114, 136-141, 150-155, 166-180, 216-219
  - `MandelbrotSet.cpp`, line numbers 127-131, 142-147
  - `MandelbrotColor.cpp', line number 6-25, 33-48
- [ ] The project reads data from a file and process the data, or the program writes data to a file.
- [x] The project accepts user input and processes the input.
  - MandelbrotExplorer.cpp, line 247

### Object Oriented Programming

- [x] The project uses Object Oriented Programming techniques.
  - The main classes of the project are `MandelbrotExplorer`, `MandelbrotDisplay`, `MandelbrotSet`
    - `MandelbrotExplorer` presents a `MandelbrotDisplay` with a box. The box can be moved and resized. `MandelbrotExplorer` presents another `MandelbrotDisplay`, which depends on the position and size of the box.
    - `MandelbrotDisplay` provides the image of the Mandelbrot set within a certain range of complex numbers. `MandelbrotDisplay` ask `MandelbrotSet` for the data of the Mandelbrot set to generate the image.
    - `MandelbrotSet` calculates and holds tha data of the Mandelbrot set.
  - As a support class, `MandelbrotColor` provides conversion between enum and cv::Vec3b.
- [x] Classes use appropriate access specifiers for class members.
  - `MandelbrotExplorer.h`, line numbers 12, 22
  - `MandelbrotDisplay.h`. line numbers 14, 35
  - `MandelbrotSet.h`, line numbers 6, 19
  - `MandelbrotColor.h', line number 5
- [ ] Class constructors utilize member initialization lists.
- [x] Classes abstract implementation details from their interfaces.
   - `MandelbrotExplorer.cpp`, line numbers 187, 211, 220
   - `MandelbrotDisplay.cpp`. line number 148
   - `MandelbrotSet.cpp`. line numbers 111, 119
- [x] Classes encapsulate behavior.
  - Class `MandelbrotDisplay` provides the image of the Mandelbrot set within a certain range of complex numbers.
  - Class `MandelbrotSet` calculates and holds tha data of the Mandelbrot set.
- [ ] Classes follow an appropriate inheritance hierarchy.
- [x] Overloaded functions allow the same function to operate on different parameters.
  - `MandelbrotDisplay.h`. line number 24
  - `MandelbrotSet.h`, line number 14
- [ ] Derived class functions override virtual base class functions.
- [ ] Templates generalize functions in the project.

### Memory Management

- [x] The project makes use of references in function declarations.
  - `MandelbrotExplorer.h`, line numbers 15-18, 61-63, 67
  - `MandelbrotDisplay.h`. line numbers 19-22
  - `MandelbrotSet.h`, line numbers 9-12, 14, 16, 21
- [x] The project uses destructors appropriately.
  - `MandelbrotDisplay.cpp`, line numbers 19-27
  - `MandelbrotSet.cpp`, line numbers 15-19
- [x] The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
  - `MandelbrotExplorer.cpp`, line numbers 14-30
- [x] The project follows the Rule of 5.
  - `MandelbrotExplorer.cpp`, line numbers 14-92
  - `MandelbrotDisplay.cpp`. line numbers 17-91
  - `MandelbrotSet.cpp`, line numbers 13-100
- [x] The project uses move semantics to move data, instead of copying it, where possible.
  - `MandelbrotExplorer.cpp`, line numbers 74, 75, 88, 89, 273, 277
  - `MandelbrotDisplay.cpp`, line numbers 61-64, 79-82, 120, 125
  - `MandelbrotSet.cpp`, line numbers 63, 68, 88 ,93, 107, 117
- [x] The project uses smart pointers instead of raw pointers.
  - `MandelbrotExplorer.h`, line numbers 51, 52
  - `MandelbrotDisplay.h`, line number 43

### Concurrency

- [x] The project uses multithreading
  - `MandelbrotDisplay.cpp`, line numbers 161
- [ ] A promise and future is used in the project.
- [x] A mutex or lock is used in the project.
  - `MandelbrotExplorer.cpp`, line numbers 105, 111, 117, 123
  - `MandelbrotDisplay.cpp`, linu numbers 185, 191, 197 ,203
- [ ] A condition variable is used in the project.
