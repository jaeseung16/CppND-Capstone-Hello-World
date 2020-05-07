#include <complex>

class MandelbrotPoint
{
public:
    void setPoint(std::complex<float> point) { _point = point; }
    std::complex<float> getPoint() { return _point; }

    unsigned int getIteration() { return _iteration; }

    bool isMandelBrotSet() { return std::abs(_value) <= 2; }
    
private:
    std::complex<float> _point;
    std::complex<float> _value;
    unsigned int _iteration = 0;
};