#include <iostream>
#include <memory>
#include <unordered_map>
#include <cmath>
#include <fstream>

#include "parameters.h"
#include "XDecision.h"
#include "decrease.h"
#include "mutation.h"
#include "sequential.h"


int main(int argc, char** argv)
{
    SequentialAlg<TypeDecision, MyOperation,
                  CauchyLaw, MyDataType> algorithm(argv[1], INITIAL_TEMPERATURE);


    std::shared_ptr<AbstractTypeDecision<MyDataType>> record = algorithm.startMainCycle();
    std::ofstream out;
    out.open("log.txt", std::ios::app);
    if (out.is_open())
    {
        out << "TARGET VALUE = " << record->targetFunc() << std::endl;
        out << "EXECUTION TIME (SUM) = " << record->getExecTime() << std::endl;
    }
    out.close();
    // record->print();
    return 0;
}