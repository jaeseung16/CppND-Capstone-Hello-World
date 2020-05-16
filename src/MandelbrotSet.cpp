#include <complex>
#include <vector>

#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet(std::vector<std::complex<float>> zs, int nIteration) {
    _zs = zs;
    _nIteration = nIteration;

    for (auto z : _zs) {
        MandelbrotPoint point = MandelbrotPoint(z, (unsigned int) _nIteration);
        _iterations.push_back(point.getIteration());
        _isMandelbrotSet.push_back(point.isMandelBrotSet());
        _set.push_back(point);
    }
}
