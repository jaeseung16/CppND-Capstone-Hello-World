#include <complex>
#include <vector>
#include "MandelbrotPoint.h"

class MandelbrotSet
{
public:
    MandelbrotSet(std::vector<std::complex<float>> zs, int nIteration);

    std::vector<MandelbrotPoint> getSet() { return _set; }
    std::vector<bool> getIsMandelbrotSet() { return _isMandelbrotSet; }
    std::vector<unsigned int> getIterations() { return _iterations; }

private:
    std::vector<MandelbrotPoint> _set;
    std::vector<std::complex<float>> _zs;
    std::vector<bool> _isMandelbrotSet;
    std::vector<unsigned int> _iterations;
    //float _xmin = -2.0;
    //float _xmax = 1.0;
    //float _ymin = -1.0;
    //float _ymax = 1.0;
    int _nIteration = 50;
};