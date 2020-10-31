#include <cmath>
#include "decrease.h"

// definition of methods for implementation of temperature
// decrease law classes. Implemented Boltzmann Law, 
// Cauchy Law and Third no named law.


BoltzmannLaw::BoltzmannLaw(double T0) : T0(T0) {}


CauchyLaw::CauchyLaw(double T0) : T0(T0) {}


ThirdLaw::ThirdLaw(double T0) : T0(T0) {}


double BoltzmannLaw::getDecreaseTemperature(int it) const
{
    return T0 / std::log1p(it);
}


double CauchyLaw::getDecreaseTemperature(int it) const
{
    return T0 / (1 + it);
}


double ThirdLaw::getDecreaseTemperature(int it) const
{
    return T0 * std::log1p(it) / (1 + it);
}