#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/math.h>
#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/__cxx_version>

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


using SPtrAbstractData = std::shared_ptr<AbstractTypeDecision<MyDataType>>;


int main(int argc, char** argv)
{
    std::unordered_map<std::string, int> params;
    setParameters(params);

    SPtrAbstractData bestSolution(new TypeDecision(argv[1]));
    std::vector<SPtrAbstractData> records(params["NUM_OF_THREADS"], bestSolution);

    int idBestRecord = 0, it = 0;

    omp_set_num_threads(params["NUM_OF_THREADS"]);
    auto start = std::chrono::steady_clock::now();

    while(it < params["NUM_OF_IT_OUTER_MOST_LOOP_WITHOUT_CHANGE"])
    {
        // std::cout << std::endl;
        // std::cout << std::endl;
        // std::cout << "############ NEXT IT OF MULTITHREAD ##################" << std::endl;
        // std::cout << "IT WITHOUT CHANGE = " << it << std::endl;
        #pragma omp parallel shared(records, bestSolution, params)
        {
            // #pragma omp critical
            // {
            //     bestSolution->print();
            // }
            // std::cout << "TUT!!!!" << std::endl << std::endl << std::endl;
            SequentialAlg<TypeDecision, MyOperation,
                          CauchyLaw, MyDataType> algorithm(bestSolution, params);
            #pragma omp critical
            {
                algorithm.print();
            }
            // std::cout << "NUM THREADS = " << omp_get_num_threads() << std::endl;
            int threadNum = omp_get_thread_num();
            records[threadNum] = algorithm.startMainCycle();
            // #pragma omp critical
            // {
            //     std::cout << "//////////////////////////////####!!!!!!!!!!!!" << std::endl;
            //     std::cout << "THREAD = " << threadNum << std::endl;
            //     std::cout << "TARGET VALUE = " << records[threadNum]->targetFunc() << std::endl;
            //     std::cout << "EXECUTION TIME (SUM) = " << records[threadNum]->getExecTime() << std::endl;
            //     std::cout << "ID MAX PROC = " << records[threadNum]->getIdMaxProc() << std::endl;
            //     std::cout << "ID MIN PROC = " << records[threadNum]->getIdMinProc() << std::endl;
            //     std::cout << "//////////////////////////////####!!!!!!!!!!!!" << std::endl;
            // }
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
        // std::cout << "BEST SOLUTION: " << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;
        // // std::cout << "THREAD = " << threadNum << std::endl;
        // std::cout << "TARGET VALUE = " << bestSolution->targetFunc() << std::endl;
        // std::cout << "EXECUTION TIME (SUM) = " << bestSolution->getExecTime() << std::endl;
        // std::cout << "ID MAX PROC = " << bestSolution->getIdMaxProc() << std::endl;
        // std::cout << "ID MIN PROC = " << bestSolution->getIdMinProc() << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;
        // std::cout << "//////////////////////////////" << std::endl;

        // std::cout << "############ END IT OF MULTITHREAD ##################" << std::endl;
        // std::cout << std::endl;
        // std::cout << std::endl;
    }

    auto end = std::chrono::steady_clock::now();
    auto timeDiff = end - start;
    // std::shared_ptr<AbstractTypeDecision<MyDataType>> record = algorithm.startMainCycle();
    std::ofstream log;
    log.open("log.txt", std::ios::app);
    if (log.is_open())
    {
        log << "DATA FILE = " << argv[1] << std::endl;
        log << "NUM OF THREADS = " << params["NUM_OF_THREADS"] << std::endl;
        log << "\tINITIAL TEMPERATURE = " << params["INITIAL_TEMPERATURE"] << std::endl;
        log << "\tNUM OF IT OUTER MOST LOOP WITHOUT CHANGE = " << params["NUM_OF_IT_OUTER_MOST_LOOP_WITHOUT_CHANGE"] << std::endl;
        log << "\tNUM OF IT INNER MOST LOOP WITHOUT CHANGE = " << params["NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE"] << std::endl;
        log << "\tNUM OF IT WITHOUT CHANGE TEMPERATUE = " << params["NUM_OF_IT_WITHOUT_CHANGE_TEMPERATUE"] << std::endl;
        log << "\t\tTARGET VALUE = " << bestSolution->targetFunc() << std::endl;
        log << "\t\tEXECUTION TIME (SUM) = " << bestSolution->getExecTime() << std::endl;
        log << "\t\t" << std::chrono::duration <double, std::milli> (timeDiff).count() << " ms" << std::endl;
    }
    log.close();
    // record->print();
    return 0;
}