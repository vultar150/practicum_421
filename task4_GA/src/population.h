#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

using IndividualType = std::vector<bool>;


void print_individual(const IndividualType& individual, const int& size);

bool getRandomBool();


class IPopulation {
public:
    virtual void init() = 0;
    virtual void print() const = 0;
    virtual IPopulation* copy() const = 0;
    virtual ~IPopulation() = default;
};


class Population : public IPopulation {
public:
    Population(const int& pop_size=100, const int& sqrt_genom_size=50);
    void init() override;
    void print() const override;
    Population* copy() const override;

private:
    std::vector<IndividualType> population;
    int pop_size;
    int sqrt_genom_size;

};


#endif // POPULATION_H