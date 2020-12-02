#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cmath>
#include <initializer_list>
#include <functional>
#include <type_traits>
#include <exception>


class IFunction;
class Expression;

using IFunctionPtr = std::shared_ptr<IFunction>;


template<typename T>
std::string toStr(T value) {
    std::ostringstream s;
    s.precision(std::numeric_limits<T>::digits10);
    s << value;
    return s.str();
}


class IFunction {
public:
    friend Expression operator+(const IFunction& f1, const IFunction& f2);
    friend Expression operator-(const IFunction& f1, const IFunction& f2);
    friend Expression operator*(const IFunction& f1, const IFunction& f2);
    friend Expression operator/(const IFunction& f1, const IFunction& f2);

    virtual double operator()(const double& x) const { return getValue(x); }
    virtual double getDerive(const double& x) const { return lgetDerive(x); }
    virtual std::string toString() const { return str; }
    virtual IFunction* clone() const { return new IFunction(*this); }
    virtual ~IFunction() = default;

protected:
    std::string str;
    bool moreThanOneTerms = false; // for string representation

    std::function<double(double)> getValue;
    std::function<double(double)> lgetDerive;
};


class Expression: public IFunction {
public:
    friend Expression operator+(const IFunction& f1, const IFunction& f2);
    friend Expression operator-(const IFunction& f1, const IFunction& f2);
    friend Expression operator*(const IFunction& f1, const IFunction& f2);
    friend Expression operator/(const IFunction& f1, const IFunction& f2);

    Expression(IFunction* a1, IFunction* a2, bool twoTerms=false):
                                                firstArg(std::move(a1)),
                                                secondArg(std::move(a2)) 
    { moreThanOneTerms = twoTerms; }
                                                

    Expression* clone() const override { return new Expression(*this); }

protected:
    std::shared_ptr<IFunction> firstArg;
    std::shared_ptr<IFunction> secondArg;
};


class Polynomial: public IFunction {
public:
    auto getValueLambdaInit() const {
        return [this] (const double& x) {
                    double result = 0, newX = 1;
                    for (const auto& c: this->coef) {
                        result += c * newX;
                        newX *= x;
                    }
                    return result;
                };
    }

    auto getDeriveLambdaInit() const {
        return [this] (const double& x) {
                    double result = 0, newX = 1;
                    auto it = ++(this->coef.begin());
                    for (int i = 1; it != this->coef.end(); it++, i++) {
                        result += (*it) * newX * i;
                        newX *= x;
                    }
                    return result;
                };
    }

    Polynomial(const std::initializer_list<double>& init={0.}): coef(init) {
        if (coef.size() == 0) { 
            throw std::logic_error("Empty parameters of polynomial"); 
            return;
        }
        if (coef.size() == 1) {
            getValue  = [this] (const double& x) { return this->coef[0]; };
            lgetDerive = [] (const double& x) { return 0.; };
            str = toStr(coef[0]);
            return;
        }
        str = (coef[0] != 0.) ? toStr(coef[0]) + " + " : "";
        str += (coef[1] != 0.) ? (coef[1] != 1. ? toStr(coef[1]) + "*" : "") + 
                        "x" + " + " : "";
        auto it = coef.begin() + 2;
        auto f = [&](const double& cf, const int& pow) {
                    if (cf == 0.) return;
                    str += (cf != 1. ? toStr(cf) + "*" : "") + "x^" + toStr(pow) + " + ";
                 };
        for (int power = 2; it != coef.end(); it++, power++) { f(*it, power); }
        str.pop_back(), str.pop_back(), str.pop_back();
        if (str.size() > 3) moreThanOneTerms = true;
        getValue  = getValueLambdaInit();
        lgetDerive = getDeriveLambdaInit();
    }

    Polynomial* clone() const override { return new Polynomial(*this); }

protected:
    std::vector<double> coef;
};


class Const: public IFunction {
public:
    Const(double x=0.): value(x) {
        str = toStr(value); 
        getValue = [this] (const double& x) { return this->value; };
        lgetDerive = [] (const double& x) { return 0.; };
    }

    Const* clone() const override { return new Const(*this); }

protected:
    double value;
};


class Power: public IFunction {
public:
    Power(int x=0): power(x) {
        str = (x != 0) ? (x != 1 ? "x^" + toStr(x) : "x") : "1";
        getValue = [this](const double& x) { return std::pow(x, this->power); };
        lgetDerive = [this](const double& x) { return power * std::pow(x, this->power - 1); };
    }

    Power* clone() const override { return new Power(*this); }

protected:
    int power;
};


class Ident: public IFunction {
public:
    Ident(int x=1) { 
        str = "x";
        getValue = [] (const double& x) { return x; };
        lgetDerive = [] (const double& x) { return 1.; };
    }

    Ident* clone() const override { return new Ident(*this); }
};


class Exp: public IFunction {
public:
    Exp(double x=1.) {
        str = "e^x";
        getValue = [] (const double& x) { return std::exp(x); };
        lgetDerive = [this] (const double& x) { return this->getValue(x); };
    }

    Exp* clone() const override { return new Exp(*this); }
};


class TImpl {
private:
    class ICreator {
    public:
        virtual ~ICreator() = default;
        virtual IFunctionPtr create(void* param) const = 0;
        virtual IFunctionPtr create() const = 0;
    };

    template<typename CurrentFunction, typename ParamT>
    class Creator: public ICreator {
    public:
        
        IFunctionPtr create(void* param) const override {
            return std::make_shared<CurrentFunction>(*(static_cast<ParamT*>(param)));
        }

        
        IFunctionPtr create() const override {
            return std::make_shared<CurrentFunction>();
        }
    };

    using ICreatorPtr = std::shared_ptr<ICreator>;
    using RegisteredCreators = std::map<std::string, ICreatorPtr>;

    RegisteredCreators registeredCreators;

    template<typename T, typename U>
    void registerCreator(const std::string& type) {
        registeredCreators[type] = std::make_shared<Creator<T, U>>();
    }

    void registerAll() {
        registerCreator<Polynomial, std::initializer_list<double>>("polynomial");
        registerCreator<Const, double>("const");
        registerCreator<Ident, int>("ident");
        registerCreator<Power, int>("power");
        registerCreator<Exp, double>("exp");
    }

public:

    TImpl() { registerAll(); }

    template<typename T>
    IFunctionPtr createFunction(const std::string& type, T&& param) const {
        auto creator = registeredCreators.find(type);
        if (creator == registeredCreators.end()) { // need to be fixed
            return nullptr;
        }
        return creator->second->create(&param);
    }

    IFunctionPtr createFunction(const std::string& type) const {
        auto creator = registeredCreators.find(type);
        if (creator == registeredCreators.end()) { // need to be fixed
            return nullptr;
        }
        return creator->second->create();
    }

    std::vector<std::string> getAvailableFunctions() const {
        std::vector<std::string> result;
        for(const auto& type : registeredCreators) {
            result.push_back(type.first);
        }
        return result;
    }
};


class FuncFactory {
private:
    std::unique_ptr<TImpl> impl;

public:
    FuncFactory(): impl(std::make_unique<TImpl>()) {}

    ~FuncFactory() = default;

    template<typename T>
    IFunctionPtr createFunction(const std::string& type, T&& param) const {
        return impl->createFunction(type, std::forward<T>(param));
    }

    IFunctionPtr createFunction(const std::string& type,
                                std::initializer_list<double> param) const {
        return impl->createFunction(type, param);
    }

    IFunctionPtr createFunction(const std::string& type) const {
        return impl->createFunction(type);
    }

    std::vector<std::string> getAvailableFunctions() const {
        return impl->getAvailableFunctions();
    }
};


// operations

Expression operator+(const IFunction& f1, const IFunction& f2) {
    Expression* result = new Expression(f1.clone(), f2.clone(), true);
    result->getValue = [result] (const double& x) {
                          return (*result->firstArg)(x) + (*result->secondArg)(x);
                       };
    result->lgetDerive = [result] (const double& x) {
                             return result->firstArg->getDerive(x) + result->secondArg->getDerive(x);
                         };
    result->str = result->firstArg->toString() + " + " + result->secondArg->toString();
    return *result;
}


Expression operator-(const IFunction& f1, const IFunction& f2) {
    Expression* result = new Expression(f1.clone(), f2.clone(), true);
    result->getValue = [result] (const double& x) {
                          return (*result->firstArg)(x) - (*result->secondArg)(x);
                       };
    result->lgetDerive = [result] (const double& x) {
                             return result->firstArg->getDerive(x) - result->secondArg->getDerive(x);
                         };
    bool twoTerms = result->secondArg->moreThanOneTerms;
    result->str = result->firstArg->toString() + " - " + 
                  (twoTerms ? "(" + result->secondArg->toString() + ")" : result->secondArg->toString());
    return *result;
}


Expression operator*(const IFunction& f1, const IFunction& f2) {
    Expression* result = new Expression(f1.clone(), f2.clone());
    result->getValue = [result] (const double& x) {
                          return (*result->firstArg)(x) * (*result->secondArg)(x);
                       };
    result->lgetDerive = [result] (const double& x) {
                 return result->firstArg->getDerive(x) * (*result->secondArg)(x) + 
                        (*result->firstArg)(x) * result->secondArg->getDerive(x); // u' * v + u * v'
            };
    bool twoTerms1 = result->firstArg->moreThanOneTerms;
    bool twoTerms2 = result->secondArg->moreThanOneTerms;

    result->str = (twoTerms1 ? "(" + result->firstArg->toString() + ")" : result->firstArg->toString()) + 
                  " * " + 
                  (twoTerms2 ? "(" + result->secondArg->toString() + ")" : result->secondArg->toString());
    return *result;
}


Expression operator/(const IFunction& f1, const IFunction& f2) {
    Expression* result = new Expression(f1.clone(), f2.clone());
    result->getValue = [result] (const double& x) {
                          if ((*result->secondArg)(x) == 0.) throw std::logic_error("divide by zero!!");
                          return (*result->firstArg)(x) / (*result->secondArg)(x);
                       };
    result->lgetDerive = [result] (const double& x) {
                 return ( result->firstArg->getDerive(x) * (*result->secondArg)(x) - 
                        (*result->firstArg)(x) * result->secondArg->getDerive(x) ) / 
                        ((*result->secondArg)(x) * (*result->secondArg)(x)); // (u' * v - u * v') / v * v
            };
    bool twoTerms1 = result->firstArg->moreThanOneTerms;
    bool twoTerms2 = result->secondArg->moreThanOneTerms;
    result->str = (twoTerms1 ? "(" + result->firstArg->toString() + ")" : result->firstArg->toString()) + 
                  " / " + 
                  (twoTerms2 ? "(" + result->secondArg->toString() + ")" : result->secondArg->toString());
    return *result;
}


template<typename F1, typename F2>
std::enable_if_t<
std::is_base_of< IFunction, std::decay_t<F1> >::value xor
std::is_base_of< IFunction, std::decay_t<F2> >::value, IFunction>
operator+(F1&& f1, F2&& f2) {
    throw std::logic_error("invalid types of arguments");
}


template<typename F1, typename F2>
std::enable_if_t<
std::is_base_of< IFunction, std::decay_t<F1> >::value xor
std::is_base_of< IFunction, std::decay_t<F2> >::value, IFunction>
operator-(F1&& f1, F2&& f2) {
    throw std::logic_error("invalid types of arguments");
}


template<typename F1, typename F2>
std::enable_if_t<
std::is_base_of< IFunction, std::decay_t<F1> >::value xor
std::is_base_of< IFunction, std::decay_t<F2> >::value, IFunction>
operator*(F1&& f1, F2&& f2) {
    throw std::logic_error("invalid types of arguments");
}


template<typename F1, typename F2>
std::enable_if_t<
std::is_base_of< IFunction, std::decay_t<F1> >::value xor
std::is_base_of< IFunction, std::decay_t<F2> >::value, IFunction>
operator/(F1&& f1, F2&& f2) {
    throw std::logic_error("invalid types of arguments");
}

// end operations


double getRoot(IFunction& f, int it=1000, double x0 = -2.) {
    static const double EPS_FINISH = 0.00001;
    static const double LAMBDA = 0.01;
    double fx0 = f(x0);
    for (int i = 1; i <= it and std::abs(fx0) > EPS_FINISH; i++) {
        x0 = x0 + LAMBDA * (fx0 > 0 ? -f.getDerive(x0) : f.getDerive(x0));
        fx0 = f(x0);
    }
    return x0;
}


#endif //FUNCTIONS_H