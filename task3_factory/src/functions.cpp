#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cmath>
#include <initializer_list>

#include "functions.h"

// for transformation to std::string

template<typename T>
std::string toStr(T value)
{
    std::ostringstream s;
    s.precision(std::numeric_limits<T>::digits10);
    s << value;
    return s.str();
}


IFunction::~IFunction() = default;


// Polynomial

Polynomial::~Polynomial() = default;


Polynomial::Polynomial(int x): coef(x, 0.)
{
    coef.emplace_back(1.);
}


Polynomial::Polynomial(const std::initializer_list<double>& init): coef(init) {}


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


std::string Polynomial::toString() const
{
    std::string result = this->getName();
    if (coef.size() <= 1) 
    {
        result += toStr(coef[0]);
        return result;
    }
    bool needPlus = false;
    result += (coef[0] != 0.) ? (needPlus = true, toStr(coef[0])) : "";
    result += (coef[1] != 0.) ? needPlus = true, ((coef[0] != 0. ? " + " : "") + 
                                (coef[1] != 1. ? toStr(coef[1]) + "*" : "") + 
                                "x") : "";
    auto it = coef.begin() + 2;
    auto f = [&](const double& cf, const int& pow) {
                if (cf == 0) return;
                result += (needPlus ? " + " : "") + 
                          (cf != 1. ? toStr(cf) + "*" : "") + 
                          "x^" + toStr(pow);
                needPlus = true;
             };
    for (int power = 2; it != coef.end(); it++, power++) { f(*it, power); }
    return result;
}


std::string Polynomial::getName() const { return std::string("Polynomial "); }

// end Polynomial


// Const

Const::Const(double x): Polynomial({x}) {}

std::string Const::getName() const { return std::string("Const "); }

Const::~Const() = default;

// end Const


// Ident

Ident::Ident(int x): Polynomial({0., 1.}) {}

std::string Ident::getName() const { return std::string("Ident "); }

Ident::~Ident() = default;

// end Ident


// Power

Power::Power(int x): Polynomial(x) {}

std::string Power::getName() const { return std::string("Power "); }

Power::~Power() = default;

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


std::string Exp::toString() const { return std::string("Exp"); }


Exp::~Exp() = default;

// end exp


// FuncFactory

template<typename CurrentFunction, typename ParamT>
IFunctionPtr TImpl::Creator<CurrentFunction, ParamT>::create(void* param) const 
{
    return std::make_shared<CurrentFunction>(*(static_cast<ParamT*>(param)));
}


template<typename CurrentFunction, typename ParamT>
IFunctionPtr TImpl::Creator<CurrentFunction, ParamT>::create() const 
{
    return std::make_shared<CurrentFunction>();
}


void TImpl::registerAll()
{
    registerCreator<Polynomial, std::initializer_list<double>>("polynomial");
    registerCreator<Const, double>("const");
    registerCreator<Ident, int>("ident");
    registerCreator<Power, int>("power");
    registerCreator<Exp, int>("exp");
}

TImpl::TImpl() { registerAll(); }


IFunctionPtr TImpl::createFunction(const std::string& type) const
{
    auto creator = registeredCreators.find(type);
    if (creator == registeredCreators.end()) // need to be fixed
    {
        return nullptr;
    }
    return creator->second->create();
}


std::vector<std::string> TImpl::getAvailableFunctions() const 
{
    std::vector<std::string> result;
    for(const auto& type : registeredCreators)
    {
        result.push_back(type.first);
    }
    return result;
}


FuncFactory::FuncFactory(): impl(std::make_unique<TImpl>()) {}


FuncFactory::~FuncFactory() = default;


IFunctionPtr 
FuncFactory::createFunction(const std::string& type,
                            std::initializer_list<double> param) const
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


