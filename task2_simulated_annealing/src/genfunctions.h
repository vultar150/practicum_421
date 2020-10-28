#ifndef GENFUNCTIONS_H
#define GENFUNCTIONS_H

#include "tinyxml2.h" // additional library (XML Parser)


using generatorParamType = std::unordered_map<std::string, std::string>;


// prototypes of functions for generator

void fillDataMap (char* fileName, generatorParamType& inputData);


void generateData (generatorParamType& dataVector, char* filename);


#endif // GENFUNCTIONS_H