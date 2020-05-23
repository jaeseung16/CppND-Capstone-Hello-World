#include <complex>
#include <vector>
#include <thread>

class MandelbrotSet
{
public:
    MandelbrotSet();
    ~MandelbrotSet();
    MandelbrotSet(const MandelbrotSet &source);
    MandelbrotSet &operator=(const MandelbrotSet &source);
    MandelbrotSet(MandelbrotSet &&source);
    MandelbrotSet &operator=(MandelbrotSet &&source);

    MandelbrotSet(std::vector<std::complex<float>> &&zs, int nIteration, int size);

    std::vector<int> getValues() { return _values; }

private:
    int mandelbrot(const std::complex<float> &z0, const int max);
    int mandelbrotFormula(const std::complex<float> &z0, const int maxIter);

    std::vector<std::complex<float>> _zs;
    std::vector<int> _values;
    int _nIteration = 50;
};