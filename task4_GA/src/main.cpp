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


    CellularAutomaton automaton(20);
    IndividualType individual(20 * 20, false);

    individual[1] = true;
    individual[20] = true;
    individual[40] = true, individual[41] = true, individual[42] = true;

    print_individual(individual, 20);
    std::cout << std::endl;
    automaton.getResult(individual, 4);

    return 0;
}