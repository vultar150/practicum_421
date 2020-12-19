#include <iostream>
#include "population.h"
#include "survival_function.h"


int main(int argc, char *argv[]) {
    IPopulation *population = new Population(5, 3);
    population->print();

    std::cout << std::endl;
    std::cout << "///////" << std::endl;

    IPopulation *population2 = population->copy();
    delete population;

    population2->print();

    delete population2;

    int size = 10;
    ISurvivalFunc *survivalFunc = new SurvivalFunc(size, 1);
    // CellularAutomaton automaton(size);
    // IndividualType individual(20 * 20, false);

    // individual[1] = true;
    // individual[20] = true;
    // individual[40] = true, individual[41] = true, individual[42] = true;

    // print_individual(individual, 20);
    // std::cout << std::endl;
    // automaton.getResult(individual, 4);

    // std::cout << std::endl;
    std::cout << "NEXT//////////////////" << std::endl;

    IndividualType individual2(size * size, false);
    individual2[2 * size + 3] = true;
    individual2[3 * size + 4] = true;
    individual2[4 * size + 2] = true;
    individual2[4 * size + 3] = true;
    individual2[4 * size + 4] = true;

    print_individual(individual2, size);
    std::cout << std::endl;
    int targetValue = survivalFunc->getValue(individual2);
    // automaton.getResult(individual2, 23);

    std::cout << "TARGET VALUE = " << targetValue << std::endl;
    std::string state = survivalFunc->checkState() ? "YES" : "NO";
    std::cout << "IS STATIONARY? " << state << std::endl;
    std::cout << "NEXT//////////////////" << std::endl;

    return 0;
}