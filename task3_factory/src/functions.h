#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <map>

class IFunction;

using IFunctionPtr = std::unique_ptr<IFunction>;


class IFunction
{
    public:

        // virtual IFunction& operator=(const IFunction& func) = 0;
        // virtual IFunction& operator=(IFunction&& func) = 0;
        virtual double operator()(const double& x) const = 0;
        virtual double getDerive(const double& x) const = 0;
        virtual ~IFunction();
};


class Polynomial: public IFunction
{
    public:
        Polynomial(const std::initializer_list<int>& init={0});
        // virtual IFunction& operator=(const IFunction& func) override;
        // virtual IFunction& operator=(IFunction&& func) override;
        virtual double operator()(const double& x) const override;
        virtual double getDerive(const double& x) const override;
        virtual ~Polynomial();

    private:
        std::vector<int> coef;
};


class Const: public Polynomial
{
    public:
        Const(int x=0);
        // virtual double operator()(const double x) const override;
        // virtual double getDerive(const double x) const override;
        virtual ~Const();

    // private:
    //     double value;
};

class Ident: public Polynomial
{
    public:
        Ident(int x=0);

        // virtual double operator()(const double x) const override;

        // virtual double getDerive(const double x) const override;

        virtual ~Ident();
};


// class Power: public Polynomial
// {
//     public:
//         Power(int x=0);
//         virtual ~Power();
// }

class Exp: public IFunction
{
    public:
        Exp(int x=0);
        // virtual IFunction& operator=(const IFunction& func) override;
        // virtual IFunction& operator=(IFunction&& func) override;
        virtual double operator()(const double& x) const override;
        virtual double getDerive(const double& x) const override;
        virtual ~Exp();
};


class TImpl {
private:
    class ICreator
    {
        public:
            virtual ~ICreator() = default;
            virtual IFunctionPtr create(void* param) const = 0;
            virtual IFunctionPtr create() const = 0;
    };

    template<typename CurrentFunction, typename ParamT>
    class Creator: public ICreator
    {
        public:
            virtual ~Creator() = default;

            virtual IFunctionPtr create(void* param) const override
            {
                return std::make_unique<CurrentFunction>(*(static_cast<ParamT*>(param)));
            }

            virtual IFunctionPtr create() const override
            {
                return std::make_unique<CurrentFunction>();
            }
    };

    using ICreatorPtr = std::shared_ptr<ICreator>;
    using RegisteredCreators = std::map<std::string, ICreatorPtr>;

    RegisteredCreators registeredCreators;

    template<typename T, typename U>
    void registerCreator(const std::string& type)
    {
        registeredCreators[type] = std::make_shared<Creator<T, U>>();
    }


    void registerAll()
    {
        registerCreator<Polynomial, std::initializer_list<int>>("polynomial");
        registerCreator<Const, int>("const");
        registerCreator<Ident, int>("ident");
        // registerCreator<Power, int>("power");
        registerCreator<Exp, int>("exp");
    }

public:

    TImpl() { registerAll(); }

    template<typename T>
    IFunctionPtr createFunction(const std::string& type, T&& param) const
    {
        auto creator = registeredCreators.find(type);
        if (creator == registeredCreators.end()) // need to be fixed
        {
            return nullptr;
        }
        return creator->second->create(&param);
    }

    IFunctionPtr createFunction(const std::string& type) const
    {
        auto creator = registeredCreators.find(type);
        if (creator == registeredCreators.end()) // need to be fixed
        {
            return nullptr;
        }
        return creator->second->create();
    }

    std::vector<std::string> getAvailableFunctions() const 
    {
        std::vector<std::string> result;
        for(const auto& type : registeredCreators)
        {
            result.push_back(type.first);
        }
        return result;
    }
};


class FuncFactory {
private:
    std::unique_ptr<TImpl> impl;

public:
    FuncFactory();

    ~FuncFactory();

    template<typename T>
    IFunctionPtr createFunction(const std::string& type, T&& param) const
    {
        return impl->createFunction(type, std::forward<T>(param));
    }

    IFunctionPtr createFunction(const std::string& type,
                                std::initializer_list<int> param) const;

    IFunctionPtr createFunction(const std::string& type) const;

    std::vector<std::string> getAvailableFunctions() const;
};


#endif //FUNCTIONS_H