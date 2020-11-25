#ifndef LOG_H
#define LOG_H


using SPtrAbstractData = std::shared_ptr<AbstractTypeDecision<MyDataType>>;
using ParamsT = std::unordered_map<std::string, int>;

void 
log(char* fileName,  SPtrAbstractData bestSolution, 
    ParamsT& params, std::chrono::duration<double> time, std::string law);


#endif // LOG_H