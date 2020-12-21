#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

#include "population.h"
#include "functions.h"


void printIndividual(const IndividualType& individual, const int& size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            char sym = individual[i * size + j] ? 'X' : '-';
            std::cout << sym;
        }
        std::cout << std::endl;
    }
}


Population::Population(const int& pop_size, 
                       const int& genom_size,
                       IFitness* F,
                       const int& maxIt,
                       const bool& withInit) :
                pop_size(pop_size), 
                sqrt_genom_size(std::sqrt(genom_size)), 
                genom_size(genom_size) {
    fitnessValues.resize(pop_size);
    if (withInit) init(F, maxIt);
}


void Population::init(IFitness* F, const int& maxIt) {
    population.reserve(pop_size);
    for (int i = 0; i < pop_size; ++i) {
        IndividualType individ;
        bool stationary = true;
        int it = 0;
        while (stationary and it++ < maxIt) {
            individ.clear();
            individ.reserve(genom_size);
            for (int j = 0; j < genom_size; ++j) {
                individ.emplace_back(getRandomBool());
            }
            fitnessValues[i].value = F->fitness(individ);
            stationary = fitnessValues[i].state = F->checkRequirement();
        }
        if (stationary) fitnessValues[i].value *= F->getFine();
        population.emplace_back(individ);
    }
}


void Population::print() const {
    for (const auto& indiv : population) {
        printIndividual(indiv, sqrt_genom_size);
        std::cout << std::endl;
    }
    std::cout << "SIZE = " << population.size() << std::endl;
}


Population* Population::copy() const {
    return new Population(*this);
}


IPopulation* Population::weakCopy() const {
    return new Population(pop_size, genom_size, nullptr, 0, false);
}


void Population::updateFitnessValues(IFitness* F) {
    for (int i = 0; i < pop_size; ++i) {
        fitnessValues[i].value = F->fitness(population[i]);
        fitnessValues[i].state = F->checkRequirement();
        if (fitnessValues[i].state) fitnessValues[i].value *= F->getFine();
    }
}


std::vector<Fit>& Population::getFitness() { return fitnessValues; }


IndividualType& Population::getIndividual(const int& i) { return population[i]; }


void Population::addIndividual(const IndividualType& individual) {
    population.emplace_back(individual);
}


int Population::size() const { return pop_size; }


