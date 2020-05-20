#include <complex>
#include <vector>
#include <thread>

class MandelbrotSet
{
public:
    MandelbrotSet();
    MandelbrotSet(std::vector<std::complex<float>> &&zs, int nIteration);

    std::vector<int> getValues() { return _values; }

private:
    int mandelbrot(const std::complex<float> &z0, const int max);
    int mandelbrotFormula(const std::complex<float> &z0, const int maxIter);

    std::vector<std::complex<float>> _zs;
    std::vector<int> _values;
    int _nIteration = 50;
};