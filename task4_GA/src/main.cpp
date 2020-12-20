#include <iostream>

#include "const.h"
#include "population.h"
#include "survival_function.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"


int main(int argc, char *argv[]) {
    IPopulation *population = new Population(N_POP, SQRT_GENOM_SIZE);
    population->print();

    std::cout << std::endl;
    std::cout << "///////" << std::endl;
    // ICrossover *cross = new SinglePointCrossover();
    // IndividualType *child1 = new IndividualType(population->getIndividual(0));
    // IndividualType *child2 = new IndividualType(population->getIndividual(1));
    // child1 = new IndividualType(population->getIndividual(0));
    // child2 = new IndividualType(population->getIndividual(1));

    // cross->crossing(&population->getIndividual(0), &population->getIndividual(1), 
    //                 child1, child2);

    // std::copy(population->getIndividual(0).begin(), 
    //           population->getIndividual(0).end(), 
    //           std::ostream_iterator<int>(std::cout, " "));

  
    // std::cout << std::endl;
    // std::cout << "###########" << std::endl;
    // std::copy(population->getIndividual(1).begin(), 
    //           population->getIndividual(1).end(), 
    //           std::ostream_iterator<bool>(std::cout, " "));
    // std::cout << std::endl;
    // std::cout << "///////" << std::endl;
    // std::cout << std::endl;
    // std::copy(child1->begin(), 
    //           child1->end(), 
    //           std::ostream_iterator<int>(std::cout, " "));

    // std::cout << std::endl;
    // std::cout << "###########" << std::endl;
    // std::copy(child2->begin(), 
    //           child2->end(), 
    //           std::ostream_iterator<int>(std::cout, " "));
    // std::cout << std::endl;
    // std::cout << "///////" << std::endl;
    // std::cout << std::endl;





    // IPopulation *population2 = population->copy();
    // delete population;

    // population2->print();

    // delete population2;

    // IFitness *fitness = new Fitness(SQRT_GENOM_SIZE, 20);
    // CellularAutomaton automaton(size);
    // IndividualType individual(20 * 20, false);

    // individual[1] = true;
    // individual[20] = true;
    // individual[40] = true, individual[41] = true, individual[42] = true;

    // print_individual(individual, 20);
    // std::cout << std::endl;
    // automaton.getResult(individual, 4);

// test planer
    // std::cout << std::endl;
    // std::cout << "NEXT//////////////////" << std::endl;
    // int size = 10;
    int num_it = 2;

    IFitness *F = new Fitness(num_it);
    // IndividualType individual2(size * size, false);
    // individual2[2 * size + 3] = true;
    // individual2[3 * size + 4] = true;
    // individual2[4 * size + 2] = true;
    // individual2[4 * size + 3] = true;
    // individual2[4 * size + 4] = true;

    // printIndividual(individual2, size);
    // std::cout << std::endl;
    // int targetValue = F->fitness(individual2);
    // // automaton.getResult(individual2, 23);

    // std::cout << "TARGET VALUE = " << targetValue << std::endl;
    // std::string state = F->checkRequirement() ? "YES" : "NO";
    // std::cout << "IS STATIONARY? " << state << std::endl;
    // std::cout << "NEXT//////////////////" << std::endl;
//////////////

    ISelection *selection = new TournamentSelection();
    IPopulation *parents = selection->getParents(population, F);

    delete population;

    // parents->print();

    std::copy(parents->getIndividual(0).begin(), parents->getIndividual(0).end(), 
              std::ostream_iterator<bool>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "MUTATION CHECK/////" << std::endl;
    IMutation* mutation = new Mutation();
    mutation->mutate(&parents->getIndividual(0));
    std::copy(parents->getIndividual(0).begin(), parents->getIndividual(0).end(), 
              std::ostream_iterator<bool>(std::cout, " "));
    std::cout << std::endl;




    // delete selection;
    // delete parents;
    // delete fitness;
    // delete population2;


    return 0;
}