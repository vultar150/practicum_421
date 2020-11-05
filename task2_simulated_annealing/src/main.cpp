#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <chrono>
#include <omp.h>

#include "parameters.h"
#include "XDecision.h"
#include "decrease.h"
#include "mutation.h"
#include "sequential.h"

#include "log.h"


int main(int argc, char** argv)
{
    ParamsT params;
    setParameters(params);

    SPtrAbstractData bestSolution(new TypeDecision(argv[1]));
    std::vector<SPtrAbstractData> records(params["NUM_OF_THREADS"], bestSolution);
    int idBestRecord = 0, it = 0;

    omp_set_num_threads(params["NUM_OF_THREADS"]);
    auto start = std::chrono::steady_clock::now();

    while(it < params["NUM_OF_IT_OUTER_MOST_LOOP_WITHOUT_CHANGE"])
    {
        #pragma omp parallel shared(records, bestSolution, params)
        {
            SequentialAlg<TypeDecision, MyOperation,
                          ThirdLaw, MyDataType> algorithm(bestSolution, params);
            #pragma omp critical
            {
                algorithm.print();
            }
            int threadNum = omp_get_thread_num();
            records[threadNum] = algorithm.startMainCycle();
            #pragma omp barrier
        }
        int minOrMax = records[idBestRecord=0]->targetFunc();
        bool criterionIsMin = params["IS_MIN_CRITERION"];
        for (int i = 1; i < params["NUM_OF_THREADS"]; i++)
        {
            if ((criterionIsMin     and minOrMax > records[i]->targetFunc()) or 
                (not criterionIsMin and minOrMax < records[i]->targetFunc()))
            {
                idBestRecord = i;
                minOrMax = records[i]->targetFunc();
            }
        }
        if (bestSolution->targetFunc() == records[idBestRecord]->targetFunc())
            it++;
        else it = 0;
        bestSolution = records[idBestRecord];
    }

    auto end = std::chrono::steady_clock::now();
    auto timeDiff = end - start;
    log(argv[1], bestSolution, params, timeDiff, "ThirdLaw");
    // bestSolution->print();
    return 0;
}