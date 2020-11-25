#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class TFunction;

using TFunctionPtr = std::unique_ptr<TFunction>;


class TFunction
{
    public:
        virtual double operator()(const double x) const = 0;
        virtual double getDerive(const double x) const = 0;
        virtual ~TFunction();
};

class Const: public TFunction
{
    public:
        Const(const double x=7);

        virtual double operator()(const double x) const override;

        virtual double getDerive(const double x) const override;

        virtual ~Const();

    private:
        double value;
};

class Ident: public TFunction
{
    public:
        Ident();

        virtual double operator()(const double x) const override;

        virtual double getDerive(const double x) const override;

        virtual ~Ident();
};


class FuncFactory
{
    private:
        class TImpl;
        std::unique_ptr<TImpl> impl;

    public:
        FuncFactory();

        ~FuncFactory();

        TFunctionPtr createFunction(const std::string& id) const;

        std::vector<std::string> getAvailableFunctions() const;



};


#endif //FUNCTIONS_H