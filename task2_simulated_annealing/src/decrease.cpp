#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/math.h>

#include "decrease.h"


// definition of methods for implementation of temperature
// decrease law classes. Implemented Boltzmann Law, 
// Cauchy Law and Third no named law.


BoltzmannLaw::BoltzmannLaw(double T0) : _T0(T0) {}


CauchyLaw::CauchyLaw(double T0) : _T0(T0) {}


ThirdLaw::ThirdLaw(double T0) : _T0(T0) {}


AbstractTDecreaseLaw&
BoltzmannLaw::operator=(AbstractTDecreaseLaw& obj)
{
    if (this == &obj) return *this;
    _T0 = obj.getT0();
    return *this;
}


AbstractTDecreaseLaw&
CauchyLaw::operator=(AbstractTDecreaseLaw& obj)
{
    if (this == &obj) return *this;
    _T0 = obj.getT0();
    return *this;
}


AbstractTDecreaseLaw&
ThirdLaw::operator=(AbstractTDecreaseLaw& obj)
{
    if (this == &obj) return *this;
    _T0 = obj.getT0();
    return *this;
}


double BoltzmannLaw::getT0() const { return _T0; }

double CauchyLaw::getT0() const { return _T0; }

double ThirdLaw::getT0() const { return _T0; }


double BoltzmannLaw::getDecreaseTemperature(int it) const
{
    return _T0 / log1p(it);
}


double CauchyLaw::getDecreaseTemperature(int it) const
{
    return _T0 / (1 + it);
}


double ThirdLaw::getDecreaseTemperature(int it) const
{
    return _T0 * log1p(it) / (1 + it);
}