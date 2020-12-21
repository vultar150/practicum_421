#include <iostream>
#include <functional>
#include <random>


#include "functions.h"


bool getRandomBool() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static auto gen = std::bind(std::uniform_int_distribution<>(0, 1), generator);
    return gen();
}


double getRandomReal() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static auto gen = std::bind(std::uniform_real_distribution<>(0., 1.), generator);
    return gen();
}