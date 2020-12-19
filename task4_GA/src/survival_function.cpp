#include <iostream>
#include <vector>

#include "population.h"
#include "survival_function.h"


CellularAutomaton::CellularAutomaton(int sqrt_size) : 
    notStationary(true), 
    sqrt_size(sqrt_size) 
{}


IndividualType 
CellularAutomaton::getResult(const IndividualType& individual, 
                             const int& num_it) const {
    IndividualType newIndividual = individual;
    for (int i = 0; i < num_it; ++i) {
        oneStep(newIndividual);
        print_individual(newIndividual, sqrt_size);
        std::cout << std::endl;
    }
    return newIndividual;
}


void CellularAutomaton::oneStep(IndividualType& individual) const {
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
        }
        else if (individual[i] and (counts[i] < 2 or counts[i] > 3)) {
            individual[i] = false;
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


bool CellularAutomaton::isNotStationary() {
    return notStationary;
}



SurvivalFunc::SurvivalFunc(int sqrt_size): automaton(sqrt_size) {}

int SurvivalFunc::getValue(const IndividualType& individual) const {
    IndividualType result = automaton.getResult(individual, 100);
    // if (automaton.isNotStationary) {}
    return countAlive(result);
}


int SurvivalFunc::countAlive(const IndividualType& individual) const {
    int size = individual.size();
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (individual[i]) ++count;
    }
    return count;
}