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


class IFunction;

using IFunctionPtr = std::shared_ptr<IFunction>;


template<typename T>
std::string toStr(T value)
{
    std::ostringstream s;
    s.precision(std::numeric_limits<T>::digits10);
    s << value;
    return s.str();
}


class IFunction {
public:
    // IFunction();
    // IFunction(IFunction& func);
    // IFunction(IFunction&& func);
    // virtual IFunction& operator=(IFunction& func) = 0;
    // virtual IFunction& operator=(IFunction&& func) = 0;
    virtual double operator()(const double& x) const = 0;
    virtual double getDerive(const double& x) const = 0;
    virtual std::string toString() const = 0;
    virtual ~IFunction() = default;
};


class Polynomial: public IFunction {
public:

    Polynomial(int x): coef(x, 0.) {
        coef.emplace_back(1.);
        str = "x^" + toStr(x);
    }

    Polynomial(const std::initializer_list<double>& init={0.}): coef(init) {
        if (coef.size() <= 1) {
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
        str.pop_back(), str.pop_back();
    }
    // virtual IFunction& operator=(IFunction& func) override;
    // virtual IFunction& operator=(IFunction&& func) override;

    virtual double operator()(const double& x) const override {
        double result = 0, newX = 1;
        for (const auto& c: coef) {
            result += c * newX;
            newX *= x;
        }
        return result;
    }

    virtual double getDerive(const double& x) const override {
        double result = 0, newX = 1;
        auto it = ++coef.begin();
        for (int i = 1; it != coef.end(); it++, i++) {
            result += (*it) * newX * i;
            newX *= x;
        }
        return result;
    }

    virtual std::string toString() const override { return str; }

    // virtual std::string getName() const { 
    //     return std::string("Polynomial"); 
    // }
    // virtual ~Polynomial();

protected:
    std::vector<double> coef;
    std::string str;
};


class Const: public Polynomial {
public:
    Const(double x=0.): Polynomial({x}) {}
    // virtual std::string getName() const override { 
    //     return std::string("Const"); 
    // }
    // virtual ~Const();
};


class Ident: public Polynomial {
public:
    Ident(int x=0): Polynomial({0., 1.}) {}
    // virtual std::string getName() const override {
    //     return std::string("Ident");
    // }
    // virtual ~Ident();
};


class Power: public Polynomial
{
    public:
        Power(int x=0): Polynomial(x) {}
        // virtual std::string getName() const override { 
        //     return std::string("Power");
        // }
        // virtual ~Power();
};

class Exp: public IFunction {
public:
    Exp(double x=1.): coef(x) { 
        str = coef != 1. ? (toStr(coef) +  " * e^x") : "e^x"; 
    }
    // virtual IFunction& operator=(IFunction& func) override;
    // virtual IFunction& operator=(IFunction&& func) override;
    virtual double operator()(const double& x) const override {
        return coef * std::exp(x);
    }

    virtual double getDerive(const double& x) const override {
        return this->operator()(x);
    }

    virtual std::string toString() const override {
        return str;
    }
    // virtual ~Exp();
private:
    double coef;
    std::string str;
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
        // virtual ~Creator() = default;
        virtual IFunctionPtr create(void* param) const override {
            return std::make_shared<CurrentFunction>(*(static_cast<ParamT*>(param)));
        }

        virtual IFunctionPtr create() const override {
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


#endif //FUNCTIONS_H