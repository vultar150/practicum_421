#ifndef MAIN_CYCLE_H
#define MAIN_CYCLE_H

#include <cmath>

#include "functions.h"

class GA {
public:

    GA(const double& Pc, const int& numIt, const int& maxItReq) : 
       record(new IndividualType), fitnessRecord(-1), Pc(Pc), // here 
       maxNumItWithoutChanges(numIt), maxNumItForRequirements(maxItReq), 
       numItWithoutChanges(0) {}


    IndividualType* getResult(IPopulation* population, IMutation* mutation,
                              ISelection* selection, ICrossover* crossover,
                              IFitness* F) {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
        // population->updateFitnessValues(F);
        // std::cout << "FINISH UPDATE" << std::endl;
        updateRecord(population);
        int IT = 0;
        const int pop_size = population->size();
        // const int max_it_sel = 5;
        // int count_it_sel = 0;
        int ids[pop_size];       // for check repeat parents in new generation
        int was_added[pop_size]; // for check repeat parents in new generation

        while (numItWithoutChanges < maxNumItWithoutChanges) {
            for (int i = 0; i < pop_size; ++i) was_added[i] = 0;
            // std::cout << "INSIDE OUTER WHILE IT = " << IT << std::endl;
            // std::cout << "BEFORE SELECTION " << std::endl;
            // vector<int> fitnessValues = population->getFitness();
            IPopulation* parents = selection->getParents(population, F, ids);
            // std::cout << "AFTER SELECTION " << std::endl;
            delete population;
            // std::cout << "BEFORE WEAK COPY " << std::endl;
            IPopulation* newGeneration = parents->weakCopy();
            // std::cout << "AFTER WEAK COPY " << std::endl;
            // int count_num_pop = 0;
            // int count_in_while = 0;

            int rand2parent;
            for (int i = 0; i < pop_size; i += 2) {
                int rand1parent = arc4random_uniform(pop_size);
                // rand2parent = arc4random_uniform(pop_size);
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
            // std::cout << "COUNT IF = " << count_num_pop << std::endl;
            // std::cout << "COUNT IN INNER WHILE = " << count_in_while << std::endl;
            delete parents;
            // std::cout << "BEFORE MUTATION " << std::endl;
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
            // std::cout << "AFTER MUTATION " << std::endl;
            population = newGeneration;
            // std::cout << "BEFORE SECOND UPDATE FITNESS! " << std::endl;
            // population->print();
            // std::cout << "\tPOP SIZE = " << population->size() << std::endl;
            // population->updateFitnessValues(F);
            // population->print();
            // std::cout << "AFTER SECOND UPDATE FITNESS! " << std::endl;
            updateRecord(population);
            // printIndividual(*record, std::sqrt(record->size()));
            // std::cout << "END OUTER WHILE IT = " << IT << std::endl;
            IT++;
        }
        delete population;
        std::cout << "NUM OF IT = " << IT << std::endl;
        return record;
    }


    void updateRecord(IPopulation* population) {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
        int size = population->size();
        int max = fitnessRecord, it_max = 0;
        for (int i = 0; i < size; ++i) {
            int curr_fit = population->getFitness()[i].value;
            bool stationary = population->getFitness()[i].state;
            if (curr_fit > max and not stationary) { // here
                max = curr_fit;
                it_max = i;
            }
        }
        if (max > fitnessRecord) { // here
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
    // int numItForRequirements;
};

#endif // MAIN_CYCLE_H

