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

private:
    std::vector<MandelbrotPoint> _set;
    std::vector<bool> _isMandelbrotSet;
    std::vector<unsigned int> _iterations;
    int _nIteration = 50;
};