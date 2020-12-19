#include <iostream>
#include <vector>

#include "population.h"
#include "survival_function.h"


CellularAutomaton::CellularAutomaton(int sqrt_size) : 
                            isStationary(true), 
                            sqrt_size(sqrt_size) 
{}


IndividualType 
CellularAutomaton::getResult(const IndividualType& individual, 
                             const int& num_it) {
    IndividualType newIndividual = individual;
    for (int i = 0; i < num_it; ++i) {
        isStationary = true;
        oneStep(newIndividual);
        // printIndividual(newIndividual, sqrt_size);
        // std::cout << std::endl;
        if (isStationary) { return newIndividual; }
    }
    isStationary = true;
    IndividualType checkRequirement = newIndividual;
    oneStep(checkRequirement);
    return newIndividual;
}


void CellularAutomaton::oneStep(IndividualType& individual) {
    int size = individual.size();
    std::vector<int> counts(size, 0);

    for(int i = 0; i < sqrt_size; ++i) {
        for (int j = 0; j < sqrt_size; ++j) {
            setCount(individual, i, j, counts[i * sqrt_size + j]);
        }
    }

    for(int i = 0; i < size; ++i) {
        if (not individual[i] and counts[i] == 3) {
            individual[i] = true;
            isStationary = false;
        }
        else if (individual[i] and (counts[i] < 2 or counts[i] > 3)) {
            individual[i] = false;
            isStationary = false;
        }
    }
}


void CellularAutomaton::setCount(const IndividualType& individual, 
                                 int x, int y, int& count) const {
    int dx_up = x > 0 ? -1 : 0;
    int dx_down = x < sqrt_size - 1 ? 1 : 0;

    int dy_left = y > 0 ? -1 : 0;
    int dy_right = y < sqrt_size - 1 ? 1 : 0;

    for (int i = x + dx_up; i <= x + dx_down; ++i) {
        for (int j = y + dy_left; j <= y + dy_right; ++j) {
            if ((i != x or j != y) and individual[i * sqrt_size + j]) {
                ++count;
            }
        }
    }
}


bool CellularAutomaton::checkRequirement() const {
    return isStationary;
}



Fitness::Fitness(const int& sqrt_size, const int& num_it): 
            automaton(sqrt_size), 
            isStationary(true),
            num_it(num_it)
{}


int Fitness::fitness(const IndividualType& individual) {
    IndividualType result = automaton.getResult(individual, num_it);
    isStationary = automaton.checkRequirement();
    return countAlive(result);
}


int Fitness::countAlive(const IndividualType& individual) const {
    int size = individual.size();
    int count = 0;
    for (int i = 0; i < size; ++i) { if (individual[i]) ++count; }
    return count;
}


bool Fitness::checkRequirement() const {
    return isStationary;
}