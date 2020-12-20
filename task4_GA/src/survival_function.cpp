#include <iostream>
#include <vector>
#include <cmath>

#include "population.h"
#include "survival_function.h"


CellularAutomaton::CellularAutomaton() : isStationary(true) {}


void CellularAutomaton::oneStep(IndividualType& individual) {
    int size = individual.size();
    std::vector<int> counts(size, 0);
    isStationary = true;

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


void CellularAutomaton::setSize(const int& size) { sqrt_size = size; }


bool CellularAutomaton::checkStationary() const { return isStationary; }



Fitness::Fitness(const int& num_it): 
            automaton(), isStationary(true), num_it(num_it) {}


int Fitness::fitness(const IndividualType& individual) {
    IndividualType result = individual;
    int sqrt_size = std::sqrt(individual.size());
    automaton.setSize(sqrt_size);
    for (int i = 0; i < num_it; ++i) {
        isStationary = true;
        automaton.oneStep(result);
        // printIndividual(result, sqrt_size);
        // std::cout << std::endl;
        isStationary = automaton.checkStationary();
        if (isStationary) { return countAlive(result); }
    }
    isStationary = true;
    IndividualType checkRequirement = result;
    automaton.oneStep(checkRequirement);
    isStationary = automaton.checkStationary();
    return countAlive(result);
}


int Fitness::countAlive(const IndividualType& individual) const {
    int size = individual.size();
    int count = 0;
    for (int i = 0; i < size; ++i) { if (individual[i]) ++count; }
    return count;
}


bool Fitness::checkRequirement() const { return isStationary; }



