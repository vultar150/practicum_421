#include <iostream>

#include "population.h"
#include "survival_function.h"
#include "selection.h"

static const int SQRT_GENOM_SIZE = 10;


int main(int argc, char *argv[]) {
    IPopulation *population = new Population(10, SQRT_GENOM_SIZE);
    population->print();

    std::cout << std::endl;
    std::cout << "///////" << std::endl;

    IPopulation *population2 = population->copy();
    delete population;

    population2->print();

    // delete population2;

    IFitness *fitness = new Fitness(SQRT_GENOM_SIZE, 20);
    // CellularAutomaton automaton(size);
    // IndividualType individual(20 * 20, false);

    // individual[1] = true;
    // individual[20] = true;
    // individual[40] = true, individual[41] = true, individual[42] = true;

    // print_individual(individual, 20);
    // std::cout << std::endl;
    // automaton.getResult(individual, 4);

    std::cout << std::endl;
    std::cout << "NEXT//////////////////" << std::endl;

    // IndividualType individual2(size * size, false);
    // individual2[2 * size + 3] = true;
    // individual2[3 * size + 4] = true;
    // individual2[4 * size + 2] = true;
    // individual2[4 * size + 3] = true;
    // individual2[4 * size + 4] = true;

    // printIndividual(individual2, size);
    // std::cout << std::endl;
    // int targetValue = fitness->fitness(individual2);
    // // automaton.getResult(individual2, 23);

    // std::cout << "TARGET VALUE = " << targetValue << std::endl;
    // std::string state = fitness->checkRequirement() ? "YES" : "NO";
    // std::cout << "IS STATIONARY? " << state << std::endl;
    // std::cout << "NEXT//////////////////" << std::endl;

    ISelection *selection = new TournamentSelection();
    IPopulation *parents = selection->getParents(population2, fitness);

    parents->print();

    delete selection;
    delete parents;
    delete fitness;
    delete population2;


    return 0;
}