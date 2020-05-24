#include <complex>
#include <vector>

class MandelbrotSet
{
public:
    MandelbrotSet();
    ~MandelbrotSet();
    MandelbrotSet(const MandelbrotSet &source);
    MandelbrotSet &operator=(const MandelbrotSet &source);
    MandelbrotSet(MandelbrotSet &&source);
    MandelbrotSet &operator=(MandelbrotSet &&source);

    MandelbrotSet(std::vector<std::complex<float>> &&zs, int maxIter);

    std::vector<int> getValues() { return _values; }

private:
    int mandelbrotFormula(const std::complex<float> &z0, const int &maxIter);

    std::vector<std::complex<float>> _zs;
    std::vector<int> _values;
    int _maxIter = 50;
};