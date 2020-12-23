#ifndef MAIN_CYCLE_H
#define MAIN_CYCLE_H

#include <cmath>

#include "functions.h"

class GA {
public:

    GA(const double& Pc, const int& numIt, const int& maxItReq) : 
       record(new IndividualType), fitnessRecord(-1), Pc(Pc), // max or min
       maxNumItWithoutChanges(numIt), maxNumItForRequirements(maxItReq), 
       numItWithoutChanges(0) {}


    IndividualType* getResult(IPopulation* population, IMutation* mutation,
                              ISelection* selection, ICrossover* crossover,
                              IFitness* F) {
        updateRecord(population);
        const int pop_size = population->size();
        int ids[pop_size];       // for check repeat parents in new generation
        int was_added[pop_size]; // for check repeat parents in new generation

        while (numItWithoutChanges < maxNumItWithoutChanges) {
            for (int i = 0; i < pop_size; ++i) was_added[i] = 0;
            IPopulation* parents = selection->getParents(population, F, ids);
            delete population;
            IPopulation* newGeneration = parents->weakCopy();

            int rand2parent;
            for (int i = 0; i < pop_size; i += 2) {
                int rand1parent = arc4random_uniform(pop_size);
                // get rand2parent
                for (rand2parent = (rand1parent + 1) % pop_size;
                     rand2parent != rand1parent and ids[rand1parent] == ids[rand2parent];
                     rand2parent = (rand2parent + 1) % pop_size);
                //////////////////

                newGeneration->addIndividual(parents->getIndividual(rand1parent));
                newGeneration->addIndividual(parents->getIndividual(rand2parent));

                if (getRandomReal() < Pc or was_added[ids[rand1parent]] or
                                            was_added[ids[rand2parent]]) {
                    crossover->crossing(&newGeneration->getIndividual(i),
                                        &newGeneration->getIndividual(i + 1));
                } else {
                    was_added[ids[rand1parent]] = was_added[ids[rand2parent]] = 1;
                }
            }
            delete parents;
            for (int i = 0; i < pop_size; ++i) {
                int numItForRequirements = 0;
                do {
                    mutation->mutate(&newGeneration->getIndividual(i));
                    newGeneration->getFitness()[i].value = F->fitness(newGeneration->getIndividual(i));
                    newGeneration->getFitness()[i].state = F->checkRequirement();
                }
                while (F->checkRequirement() and numItForRequirements++ < maxNumItForRequirements);
                if (newGeneration->getFitness()[i].state) 
                    newGeneration->getFitness()[i].value *= F->getFine(); 
            }
            population = newGeneration;
            updateRecord(population);
        }
        delete population;
        return record;
    }


    void updateRecord(IPopulation* population) {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
        int size = population->size();
        int max = fitnessRecord, it_max = 0;
        for (int i = 0; i < size; ++i) {
            int curr_fit = population->getFitness()[i].value;
            bool stationary = population->getFitness()[i].state;
            if (curr_fit > max and not stationary) { // max or min
                max = curr_fit;
                it_max = i;
            }
        }
        if (max > fitnessRecord) { // max or min
            fitnessRecord = max;
            delete record;
            record = new IndividualType(population->getIndividual(it_max));
            numItWithoutChanges = 0;
        }
        else {
            ++numItWithoutChanges;
        }
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    int getFitnessRecord() const { return fitnessRecord; }


private:
    IndividualType* record;
    int fitnessRecord;
    double Pc;
    int maxNumItWithoutChanges;
    int maxNumItForRequirements;
    int numItWithoutChanges;
};

#endif // MAIN_CYCLE_H

