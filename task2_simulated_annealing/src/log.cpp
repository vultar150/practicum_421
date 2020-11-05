#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <chrono>

#include "XDecision.h"
#include "log.h"


void 
log(char* fileName,  SPtrAbstractData bestSolution,   
    ParamsT& params, std::chrono::duration<double> time, std::string law)
{
    std::ofstream log;
    log.open("log.txt", std::ios::app);
    if (log.is_open())
    {
        log << "DATA FILE = " << fileName << std::endl;
        log << "NUM OF THREADS = " << params["NUM_OF_THREADS"] << std::endl;
        log << "\tINITIAL TEMPERATURE = " 
            << params["INITIAL_TEMPERATURE"] << std::endl;
        log << "\tNUM OF IT OUTER MOST LOOP WITHOUT CHANGE = " 
            << params["NUM_OF_IT_OUTER_MOST_LOOP_WITHOUT_CHANGE"] << std::endl;
        log << "\tNUM OF IT INNER MOST LOOP WITHOUT CHANGE = " 
            << params["NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE"] << std::endl;
        log << "\tNUM OF IT WITHOUT CHANGE TEMPERATUE = " 
            << params["NUM_OF_IT_WITHOUT_CHANGE_TEMPERATUE"] << std::endl;
        log << "\tDECREASE LAW = " << law << std::endl;
        log << "\t\tTARGET VALUE = " << bestSolution->targetFunc() << std::endl;
        log << "\t\tEXECUTION TIME (SUM) = " 
            << bestSolution->getExecTime() << std::endl;
        log << "\t\t" 
            << std::chrono::duration <double, std::milli> (time).count() 
            << " ms" << std::endl;
    }
    log.close();
}