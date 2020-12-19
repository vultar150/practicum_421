#include "population.h"
#include "survival_function.h"
#include "selection.h"


TournamentSelection::TournamentSelection(int tournamentSize) : 
                                    tournamentSize(tournamentSize)
{}


int TournamentSelection::getRandomIndividual(int size) {
    return arc4random_uniform(size);
}


IPopulation* TournamentSelection::getParents(IPopulation* population, 
                                             IFitness *func) {
    int size = population->size();
    IPopulation* parents = population->weakCopy();
    std::vector<int> targetValues(size);
    for (int i = 0; i < size; ++i) {
        targetValues[i] = func->fitness(population->getIndividual(i));
    }
    int current = 0;
    for (int i = 0; i < size; ++i) {
        int record = -1;
        for (int k = 0; k < tournamentSize; ++k) {
            current = getRandomIndividual(size);
            if (record == -1 or (targetValues[current] > targetValues[record]))
                record = current;
        }
        parents->addIndividual(population->getIndividual(record));
    }
    return parents;
}


