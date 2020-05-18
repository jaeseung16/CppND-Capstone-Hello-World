#include <complex>
#include <vector>
#include <thread>
#include "MandelbrotPoint.h"

class MandelbrotSet
{
public:
    MandelbrotSet();
    MandelbrotSet(std::vector<std::complex<float>> &&zs, int nIteration);

    std::vector<MandelbrotPoint> getSet() { return _set; }
    std::vector<bool> getIsMandelbrotSet() { return _isMandelbrotSet; }
    std::vector<unsigned int> getIterations() { return _iterations; }
    std::vector<int> getValues() { return _values; }

private:
    int mandelbrot(const std::complex<float> &z0, const int max);
    int mandelbrotFormula(const std::complex<float> &z0, const int maxIter);

    std::vector<MandelbrotPoint> _set;
    std::vector<std::complex<float>> _zs;
    std::vector<bool> _isMandelbrotSet;
    std::vector<unsigned int> _iterations;
    std::vector<int> _values;
    int _nIteration = 50;
};