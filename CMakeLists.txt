cmake_minimum_required(VERSION 3.7)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

project(MandelbrotExplorer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

add_executable(MandelbrotExplorer src/main.cpp src/MandelbrotExplorer.cpp src/MandelbrotSet.cpp src/MandelbrotDisplay.cpp src/MandelbrotColor.cpp)
target_link_libraries(MandelbrotExplorer ${OpenCV_LIBS})