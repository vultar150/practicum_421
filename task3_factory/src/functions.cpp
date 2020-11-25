#include <vector>
#include <map>
#include <string>
#include <memory>

#include "functions.h"


TFunction::~TFunction() = default;


// Const

Const::Const(const double x): value(x) {}


double Const::operator()(const double x) const
{
    return value;
}


double Const::getDerive(const double x) const
{
    return 0.;
}


Const::~Const() = default;

// end Const



// Ident

Ident::Ident() {}


double Ident::operator()(const double x) const
{
    return x;
}


double Ident::getDerive(const double x) const
{
    return 1.;
}


Ident::~Ident() = default;

// end Ident


// FuncFactory

class FuncFactory::TImpl
{
    private:
        class CreatorInterface
        {
            public:
                virtual ~CreatorInterface() = default;

                virtual TFunctionPtr create() const = 0;
        };

        using CreatorInterfacePtr = std::shared_ptr<CreatorInterface>;
        using RegisteredCreators = std::map<std::string, CreatorInterfacePtr>;

        RegisteredCreators registeredCreators;

    public:

        template<typename CurrentFunction>
        class Creator: public CreatorInterface
        {
            public:
                virtual ~Creator() = default;

                virtual TFunctionPtr create() const override
                {
                    return std::make_unique<CurrentFunction>();
                }
        };

        TImpl() { registerAll(); }

        template<typename T>
        void registerCreator(const std::string& id)
        {
            registeredCreators[id] = std::make_shared<Creator<T>>();
        }


        void registerAll()
        {
            registerCreator<Const>("const");
            registerCreator<Ident>("ident");
        }


        TFunctionPtr createFunction(const std::string& id) const
        {
            auto creator = registeredCreators.find(id);
            if (creator == registeredCreators.end()) // need to be fixed
            {
                return nullptr;
            }
            return creator->second->create();
        }

        std::vector<std::string> getAvailableFunctions() const 
        {
            std::vector<std::string> result;
            for(const auto& id : registeredCreators)
            {
                result.push_back(id.first);
            }
            return result;
        }
};


FuncFactory::FuncFactory(): impl(std::make_unique<FuncFactory::TImpl>()) {}


FuncFactory::~FuncFactory() = default;


TFunctionPtr FuncFactory::createFunction(const std::string& id) const
{
    return impl->createFunction(id);
}


std::vector<std::string> FuncFactory::getAvailableFunctions() const
{
    return impl->getAvailableFunctions();
}


// end FuncFactory 


