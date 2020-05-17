#include <complex>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <iostream>

#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet() {
    
}

MandelbrotSet::MandelbrotSet(std::vector<std::complex<float>> &&zs, int nIteration) {
    //_zs = zs;
    _nIteration = nIteration;

    for (auto z : zs) {
        MandelbrotPoint point = MandelbrotPoint(std::move(z), (unsigned int) _nIteration);
        _iterations.push_back(point.getIteration());
        _isMandelbrotSet.push_back(point.isMandelBrotSet());
        _set.push_back(std::move(point));
    }
}
