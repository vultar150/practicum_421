#include <iostream>
#include <vector>
#include <random>
#include <functional>

#include "population.h"


void print_individual(const IndividualType& individual, const int& size) {
    auto it = individual.begin();
    for (int i = 0; i < size; ++i) {
        std::copy(it, it + size, 
                  std::ostream_iterator<bool>(std::cout, " "));
        std::cout << std::endl;
        it += size;
    }
}


bool getRandomBool() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static auto gen = std::bind(std::uniform_int_distribution<>(0, 1), generator);
    return gen();
}


Population::Population(const int& pop_size, const int& sqrt_genom_size) : 
            pop_size(pop_size), 
            sqrt_genom_size(sqrt_genom_size)
{
    init();
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
        print_individual(indiv, sqrt_genom_size);
        std::cout << std::endl;
    }
}


Population* Population::copy() const {
    return new Population(*this);
}

