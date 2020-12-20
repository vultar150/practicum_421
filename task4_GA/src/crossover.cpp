#include <iostream>
#include <algorithm>

#include "population.h"
#include "crossover.h"


void SinglePointCrossover::crossing(IndividualType* copy_parent1,  
                                    IndividualType* copy_parent2) {
    int size = copy_parent1->size();
    int random = arc4random_uniform(size - 1) + 1;
    std::cout << "RANDOM = " << random << std::endl;
    std::swap_ranges(copy_parent1->begin(), 
                     copy_parent1->begin() + random, 
                     copy_parent2->begin());
    // std::copy(parent1->begin(), parent1->begin() + random, child1->begin());
    // std::copy(parent2->begin() + random, parent2->end(), child1->begin() + random);
    // std::copy(parent2->begin(), parent2->begin() + random, child2->begin());
    // std::copy(parent1->begin() + random, parent1->end(), child2->begin() + random);
}