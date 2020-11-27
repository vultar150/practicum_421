#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cmath>
#include <initializer_list>

#include "functions.h"


IFunction::~IFunction() = default;


// Polynomial

Polynomial::~Polynomial() = default;

Polynomial::Polynomial(const std::initializer_list<int>& init): coef(init) {}


double Polynomial::operator()(const double& x) const 
{
    double result = 0, newX = 1;
    for (const auto& c: coef)
    {
        result += c * newX;
        newX *= x;
    }
    return result;
}

double Polynomial::getDerive(const double& x) const
{
    double result = 0, newX = 1;
    auto it = ++coef.begin();
    for (int i = 1; it != coef.end(); it++, i++)
    {
        result += (*it) * newX * i;
        newX *= x;
    }
    return result;
}

// end Polynomial

// Const

Const::Const(int x): Polynomial({x}) {}


// double Const::operator()(const double x) const
// {
//     return value;
// }


// double Const::getDerive(const double x) const
// {
//     return 0.;
// }


Const::~Const() = default;

// end Const



// Ident

Ident::Ident(int x): Polynomial({0, 1}) {}


// double Ident::operator()(const double x) const { return x;
// }


// double Ident::getDerive(const double x) const
// {
//     return 1.;
// }


Ident::~Ident() = default;

// end Ident


// Power

// Power::Power(int x)
// {

// }

// Power::~Power() = default;

// end Power


// exp

Exp::Exp(int x) {}


double Exp::operator()(const double& x) const
{
    return std::exp(x);
}


double Exp::getDerive(const double& x) const
{
    return (*this)(x);
}


Exp::~Exp() = default;

// end exp


// FuncFactory


FuncFactory::FuncFactory(): impl(std::make_unique<TImpl>()) {}


FuncFactory::~FuncFactory() = default;


IFunctionPtr 
FuncFactory::createFunction(const std::string& type,
                            std::initializer_list<int> param) const
{
    return impl->createFunction(type, param);
}


IFunctionPtr FuncFactory::createFunction(const std::string& type) const
{
    return impl->createFunction(type);
}


std::vector<std::string> FuncFactory::getAvailableFunctions() const
{
    return impl->getAvailableFunctions();
}

// end FuncFactory 


