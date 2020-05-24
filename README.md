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
     - Release the left button of the mouse while holding the CTRL key (or release the right button). The box color will become white.
  4. To end, press the `ESC` key

## Requirements 
- gcc/g++ >= 5.4
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: [Install Xcode command line tools](https://developer.apple.com/xcode/resources/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)
- make >= 4.1 (Linux), 3.8 (Mac)
  - Linux: make is installed by default on most Linux distros
  - Mac: [Install Xcode command line tools to get make](https://developer.apple.com/xcode/resources/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- cmake >= 3.17
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- OpenCV >= 3.3 (Linux), 4.3 (Mac)
  - Linux: [Installation in Linux](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)
  - Mac: [Installation in MacOS](https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html)
  - Windows: [Installation in Windows](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html)
- Tested under Ubuntu 16.04.6 LTS and macOS Catalina 10.15.4

## Project Description

Let's explore the [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set)!



## Project Rubric Points

#### README (All Rubric Points REQUIRED)
- [x] A README with instructions is included with the project
- [x] The README indicates which project is chosen.
- [x] The README includes information about each rubric point addressed.

### Compiling and Testing (All Rubric Points REQUIRED)
- [x] The submission must compile and run.

### Loops, Functions, I/O
- [x] The project demonstrates an understanding of C++ functions and control structures.

* A variety of control structures are used in the project.

* The project code is clearly organized into functions.

- [ ] The project reads data from a file and process the data, or the program writes data to a file.

* The project reads data from an external file or writes data to a file as part of the necessary operation of the program.

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
  - Please see `MandelbrotExplorer.h`, `MandelbrotDisplay.h`, `MandelbrotSet.h`, and `MandelbrotColor.h`.

- [ ] Class constructors utilize member initialization lists.

* All class members that are set to argument values are initialized through member initialization lists.

- [ ] Classes abstract implementation details from their interfaces.

* All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.

- [x] Classes encapsulate behavior.

* Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.

- [ ] Classes follow an appropriate inheritance hierarchy.

* Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.

- [x] Overloaded functions allow the same function to operate on different parameters.

* One function is overloaded with different signatures for the same function name.

- [ ] Derived class functions override virtual base class functions.

* One member function in an inherited class overrides a virtual base class member function.

- [ ] Templates generalize functions in the project.

* One function is declared with a template that allows it to accept a generic parameter.

### Memory Management

- [x] The project makes use of references in function declarations.

* At least two variables are defined as references, or two functions use pass-by-reference in the project code.

- [x] The project uses destructors appropriately.
  - `MandelbrotDisplay.cpp`, line number 25

* At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.

- [ ] The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.

* The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.

- [x] The project follows the Rule of 5.
  - Classe `MandelbrotExplorer`, `MandelbrotDisplay`, and `MandelbrotSet` implement the Rule of 5.

- [x] The project uses move semantics to move data, instead of copying it, where possible.

* For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.

- [x] The project uses smart pointers instead of raw pointers.

* The project uses at least one smart pointer: `unique_ptr`, `shared_ptr`, or `weak_ptr`. The project does not use raw pointers.

### Concurrency

- [x] The project uses multithreading.

* The project uses multiple threads in the execution.

- [ ] A promise and future is used in the project.

* A promise and future is used to pass data from a worker thread to a parent thread in the project code.

- [x] A mutex or lock is used in the project.

* A mutex or lock (e.g. `std::lock_guard` or `std::unique_lock`) is used to protect data that is shared across multiple threads in the project code.

- [ ] A condition variable is used in the project.

* A `std::condition_variable` is used in the project code to synchronize thread execution.
