#include <iostream>
#include <algorithm>

#include "population.h"
#include "crossover.h"


void SinglePointCrossover::crossing(IndividualType* copy_parent1,  
                                    IndividualType* copy_parent2) {
    int size = copy_parent1->size();
    int random = arc4random_uniform(size - 1) + 1;
    std::swap_ranges(copy_parent1->begin(), 
                     copy_parent1->begin() + random, 
                     copy_parent2->begin());
}