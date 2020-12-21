#include "population.h"
#include "survival_function.h"
#include "selection.h"


TournamentSelection::TournamentSelection(int tournamentSize) : 
                                    tournamentSize(tournamentSize)
{}


IPopulation* TournamentSelection::getParents(IPopulation* population,
                                             IFitness *func, 
                                             int ids[]) {
    int size = population->size();
    IPopulation* parents = population->weakCopy();
    int current = 0;
    for (int i = 0; i < size; ++i) {
        int record = -1;
        for (int k = 0; k < tournamentSize; ++k) {
            current = arc4random_uniform(size);
            int targetCurr = population->getFitness()[current];
            if (record == -1 or (targetCurr > population->getFitness()[record])) // here
                record = current;
        }
        parents->addIndividual(population->getIndividual(record));
        ids[i] = record;
    }
    return parents;
}


