#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>


// #include "const.h"
#include "survival_function.h"




CellularAutomaton::CellularAutomaton() : isStationary(true) {}


void CellularAutomaton::oneStep(IndividualType& individual) {

    int size = individual.size();
    int counts[size];
    isStationary = true;
    #pragma omp parallel shared(counts)
    {
        #pragma omp for
        for(int i = 0; i < sqrt_size; ++i) {
            for (int j = 0; j < sqrt_size; ++j) {
                int& count = counts[i * sqrt_size + j];
                count = 0;
                setCount(individual, i, j, count);
            }
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
            if (individual[i * sqrt_size + j]) {
                ++count;
            }
        }
    }
    if (individual[x * sqrt_size + y]) --count;
}


void CellularAutomaton::setSize(const int& size) { sqrt_size = size; }


void CellularAutomaton::outputByStep(const IndividualType& individual, 
                                     const int& num_it) {
    IndividualType result = individual;
    int sqrt_size = std::sqrt(individual.size());
    printIndividual(result, sqrt_size);
    std::cout << std::endl;
    setSize(sqrt_size);
    for (int i = 0; i < num_it; ++i) {
        isStationary = true;
        oneStep(result);
        printIndividual(result, sqrt_size);
        std::cout << std::endl;
        if (isStationary) {
            std::cout << "IS STATIONARY!" << std::endl;
            return; 
        }
    }
    isStationary = true;
    IndividualType checkRequirement = result;
    oneStep(checkRequirement);
    if (isStationary) std::cout << "IS STATIONARY!" << std::endl;
}


bool CellularAutomaton::checkStationary() const { return isStationary; }


IndividualType* 
CellularAutomaton::getLast(const IndividualType& individual, const int& num_it) {
    IndividualType* result = new IndividualType(individual);
    int sqrt_size = std::sqrt(individual.size());
    setSize(sqrt_size);
    for (int i = 0; i < num_it; ++i) {
        oneStep(*result);
    }
    return result;
}


Fitness::Fitness(const int& num_it): 
            automaton(), isStationary(true), num_it(num_it), fine(0.8) {}


int Fitness::fitness(const IndividualType& individual) {
    IndividualType result = individual;
    int sqrt_size = std::sqrt(individual.size());
    automaton.setSize(sqrt_size);
    fine = 0.8;
    for (int i = 0; i < num_it; ++i) {
        isStationary = true;
        automaton.oneStep(result);
        isStationary = automaton.checkStationary();
        if (isStationary) {
            fine *= (double)(i) / (double) (num_it); 
            return countAlive(result); 
        }
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


double Fitness::getFine() const { return fine; }

