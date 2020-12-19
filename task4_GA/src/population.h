#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

using IndividualType = std::vector<bool>;


void printIndividual(const IndividualType& individual, const int& size);

bool getRandomBool();


class IPopulation {
public:
    virtual void init() = 0;
    virtual void print() const = 0;
    virtual IPopulation* copy() const = 0;
    virtual IPopulation* weakCopy() const = 0;
    virtual IndividualType& getIndividual(const int& i) = 0;
    virtual void addIndividual(const IndividualType& individual) = 0;
    virtual int size() const = 0;
    // virtual int getPopulationSize() const = 0;
    // virtual void setPopulationSize(int size) = 0;
    // virtual int getSqrtGenomSize() const = 0;
    // virtual void setSqrtGenomSize(int size) = 0;
    virtual ~IPopulation() = default;
};


class Population : public IPopulation {
public:
    Population(const int& pop_size, 
               const int& sqrt_genom_size=50, 
               const bool& withInit=true);

    void init() override;
    void print() const override;
    Population* copy() const override;
    IPopulation* weakCopy() const override;
    IndividualType& getIndividual(const int& i) override;
    void addIndividual(const IndividualType& individual) override;
    int size() const override;
    // int getPopulationSize() const override;
    // void setPopulationSize(int size) override;
    // int getSqrtGenomSize() const override;
    // void setSqrtGenomSize(int size) override;

private:
    std::vector<IndividualType> population;
    int pop_size;
    int sqrt_genom_size;

};


#endif // POPULATION_H