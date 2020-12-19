#include <iostream>
#include <vector>
#include <random>
#include <functional>

#include "population.h"


void printIndividual(const IndividualType& individual, const int& size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            char sym = individual[i * size + j] ? 'X' : '-';
            std::cout << sym;
        }
        std::cout << std::endl;
    }
}


bool getRandomBool() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static auto gen = std::bind(std::uniform_int_distribution<>(0, 1), generator);
    return gen();
}


Population::Population(const int& pop_size, 
                       const int& sqrt_genom_size,
                       const bool& withInit) : 
                pop_size(pop_size), 
                sqrt_genom_size(sqrt_genom_size) {
    if (withInit) init();
}


void Population::init() {
    population.reserve(pop_size);
    int genom_size = sqrt_genom_size * sqrt_genom_size;
    for (int i = 0; i < pop_size; ++i) {
        IndividualType individ;
        individ.reserve(genom_size);
        for (int j = 0; j < genom_size; ++j) {
            individ.emplace_back(getRandomBool());
        }
        population.emplace_back(individ);
    }
}


void Population::print() const {
    for (const auto& indiv : population) {
        printIndividual(indiv, sqrt_genom_size);
        std::cout << std::endl;
    }
}


Population* Population::copy() const {
    return new Population(*this);
}

IPopulation* Population::weakCopy() const {
    return new Population(0, sqrt_genom_size, false);
}


IndividualType& Population::getIndividual(const int& i) {
    return population[i];
}


void Population::addIndividual(const IndividualType& individual) {
    population.emplace_back(individual);
    pop_size++;
}


int Population::size() const { return pop_size; }


// int Population::getPopulationSize() const { return pop_size; }


// void Population::setPopulationSize(int size) { pop_size = size; }


// int Population::getSqrtGenomSize() const { return sqrt_genom_size; }


// void Population::setSqrtGenomSize(int size) { sqrt_genom_size = size; }

