#ifndef DECREASE_H
#define DECREASE_H

// declaration of abstrast class for temperature decrease laws
// and classes for it implementation. Implemented Boltzmann Law, 
// Cauchy Law and Third no named law.


class AbstractTDecreaseLaw 
{
    public:

        virtual AbstractTDecreaseLaw& operator=(AbstractTDecreaseLaw& obj)=0;
        virtual double getDecreaseTemperature(int it) const=0;
        virtual double getT0() const=0;
        virtual ~AbstractTDecreaseLaw() = default;
};


// T = T0 / ln(1 + i)

class BoltzmannLaw : public AbstractTDecreaseLaw 
{
    private:
        double _T0;

    public:

        BoltzmannLaw(double T0=1.);

        virtual AbstractTDecreaseLaw& operator=(AbstractTDecreaseLaw& obj) override;
        virtual double getDecreaseTemperature(int it) const override;
        virtual double getT0() const override;
};


// T = T0 / (1 + i)

class CauchyLaw : public AbstractTDecreaseLaw
{
    private:
        double _T0;

    public:

        CauchyLaw(double T0=1.);

        virtual AbstractTDecreaseLaw& operator=(AbstractTDecreaseLaw& obj) override;
        virtual double getDecreaseTemperature(int it) const override;
        virtual double getT0() const override;
};


// T = T0 * ln(1 + i) / (1 + i)

class ThirdLaw : public AbstractTDecreaseLaw
{
    private:
        double _T0;

    public:

        ThirdLaw(double T0=1.);

        virtual AbstractTDecreaseLaw& operator=(AbstractTDecreaseLaw& obj) override;
        virtual double getDecreaseTemperature(int it) const override;
        virtual double getT0() const override;
};

#endif // DECREASE_H