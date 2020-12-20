#include <random>
#include <iostream>
#include <functional>

#include "population.h"
#include "mutation.h"


double getRandomReal() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static auto gen = std::bind(std::uniform_real_distribution<>(0., 1.), generator);
    return gen();
}


Mutation::Mutation(double pMut) : pMut(pMut) {}


void Mutation::mutate(IndividualType* individual) {
    int size = individual->size();
    for (int i = 0; i < size; ++i) {
        if (getRandomReal() < pMut) { (*individual)[i] = !(*individual)[i]; }
    }
}