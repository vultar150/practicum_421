#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include <omp.h>
#include <SDL2/SDL.h>

#include "const.h"
#include "population.h"
#include "survival_function.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "main_cycle.h"
#include "print.h"


void outputResult(IndividualType* result, IndividualType* afterSteps, 
                  const int& record, const int& i, const int& num_series, 
                  std::chrono::duration<double> time) {
    std::string fileName = "series_" + std::to_string(num_series) + 
                           "_run_" + std::to_string(i) + "_sol.txt";

    fileName = "outputs/" + fileName;
    std::string fileNameAfter = "series_" + std::to_string(num_series) + 
                                "_run_" + std::to_string(i) + 
                                "_sol_after100.txt";
    fileNameAfter = "outputs/" + fileNameAfter;

    std::ofstream log(fileName, std::ios::app);
    std::ofstream logAfter(fileNameAfter, std::ios::app);
    std::ofstream tmp("record.txt", std::ios_base::out);
    std::ofstream times("time.txt", std::ios_base::out);

    int size = std::sqrt(result->size());
    if (log.is_open()) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                char sym = (*result)[i * size + j] ? 'X' : '-';
                log << sym;
            }
            log << std::endl;
        }
        log << "/////////////////////////" << std::endl << std::endl;

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                char sym = (*afterSteps)[i * size + j] ? 'X' : '-';
                logAfter << sym;
            }
            logAfter << std::endl;
        }
        logAfter << "/////////////////////////" << std::endl << std::endl;

        tmp << record << std::endl;

        times << std::chrono::duration <double, std::milli> (time).count()
              << " ms" << std::endl;
    }
    log.close();
    logAfter.close();
    tmp.close();
    times.close();
}


extern "C" int main(int argc, char *argv[]) {
    omp_set_num_threads(NUM_THREADS);
    int i = 0;
    int num_series = 0;

    if (argc >= 2) i = strtol(argv[1], 0, 10);
    if (argc >= 3) num_series = strtol(argv[2], 0, 10);
    double p_mut = P_MUT * (std::pow(1.5, i));

    // start
    auto start = std::chrono::steady_clock::now();
    IMutation*   mutation   = new Mutation(p_mut);
    ISelection*  selection  = new TournamentSelection(TOURNAMENT_SIZE);
    ICrossover*  crossover  = new SinglePointCrossover();
    IFitness*    F          = new Fitness(NUM_IT_FIT);
    IPopulation* population = new Population(NUM_POP, GENOM_SIZE, F, 
                                             MAX_NUM_IT_FOR_REQ);

    GA ga(PC, NUM_IT_WITHOUT_CHANGE, MAX_NUM_IT_FOR_REQ);

    IndividualType* result = ga.getResult(population, mutation, selection, 
                                          crossover, F);
    auto end = std::chrono::steady_clock::now();
    auto timeDiff = end - start;
    // finish
    delete mutation;
    delete selection;
    delete crossover;
    delete F;

    int size = std::sqrt(result->size());
    Picture* display = new Picture(size, size);
    display->show(*result, NUM_IT_FIT, NUM_PER_SEC);
    delete display;

    CellularAutomaton automaton;
    IndividualType* afterSteps = automaton.getLast(*result, NUM_IT_FIT);

    // std::cout << "result: " << std::endl;
    // printIndividual(*result, size);

    // std::cout << "after steps: " << std::endl;
    // printIndividual(*afterSteps, size);
    // outputResult(result, afterSteps, ga.getFitnessRecord(),
    //              i, num_series, timeDiff);

    delete result;
    delete afterSteps;
    std::cout << "time = " << std::chrono::duration <double, std::milli> (timeDiff).count()
              << " ms" << std::endl;
    std::cout << "Record = " << ga.getFitnessRecord() << std::endl;


    return 0;
}