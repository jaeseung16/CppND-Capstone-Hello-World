#include <complex>
#include <iostream>

#include "MandelbrotPoint.h"

MandelbrotPoint::MandelbrotPoint(std::complex<float> &point, unsigned int maxIteration) : _point(point)
{
    if (maxIteration > 0) {
        _maxIteration = maxIteration;
        evaluate();
    } else {
        std::cout << "The maximum iteration need to be positive." << std::endl;
    }
}

void MandelbrotPoint::evaluate()
{
    //std::cout << point << "\n";
    while (abs(_value) <= 2 && _iteration < _maxIteration) {
        _value = _value * _value + _point;
        _iteration ++;
    }
}