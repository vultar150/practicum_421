#include "population.h"
#include "mutation.h"
#include "functions.h"


Mutation::Mutation(const double& Pmut) : Pmut(Pmut) {}


void Mutation::mutate(IndividualType* individual) {
    int size = individual->size();
    for (int i = 0; i < size; ++i) {
        if (getRandomReal() < Pmut) { (*individual)[i] = !(*individual)[i]; }
    }
}