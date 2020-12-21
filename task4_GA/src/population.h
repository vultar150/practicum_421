#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#include "survival_function.h"


void printIndividual(const IndividualType& individual, const int& size);


struct Fit {
    Fit(int value=0): value(value), state(true) {}
    int  value;
    bool state;
    operator int() { return value; }
};


class IPopulation {
public:
    virtual void init(IFitness* F, const int& maxIt = 5) = 0;
    virtual void print() const = 0;
    virtual IPopulation* copy() const = 0;
    virtual IPopulation* weakCopy() const = 0;
    virtual void updateFitnessValues(IFitness* F) = 0;
    virtual std::vector<Fit>& getFitness() = 0;
    virtual IndividualType& getIndividual(const int& i) = 0;
    virtual void addIndividual(const IndividualType& individual) = 0;
    virtual int size() const = 0;
    virtual ~IPopulation() = default;
};



class Population : public IPopulation {
public:
    Population(const int& pop_size,
               const int& genom_size=2500,
               IFitness* F = nullptr,
               const int& maxIt = 5,
               const bool& withInit=true);

    void init(IFitness* F, const int& maxIt = 5) override;
    void print() const override;
    Population* copy() const override;
    IPopulation* weakCopy() const override;
    void updateFitnessValues(IFitness* F) override;
    std::vector<Fit>& getFitness() override;
    IndividualType& getIndividual(const int& i) override;
    void addIndividual(const IndividualType& individual) override;
    int size() const override;

private:
    std::vector<IndividualType> population;
    std::vector<Fit> fitnessValues;
    int pop_size;
    int sqrt_genom_size;
    int genom_size;
};


#endif // POPULATION_H