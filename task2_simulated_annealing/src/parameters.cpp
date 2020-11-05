#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>

#include "parameters.h"


void setParameters(std::unordered_map<std::string, int>& params)
{
    std::ifstream paramFile;
    paramFile.open("parameters.txt");
    if (not paramFile.is_open())
    {
        std::cerr << "Can't open the param file with name parameters.txt\n";
        exit(1);
    }
    std::string paramName, equal, value;
    while (paramFile >> paramName >> equal >> value)
    {
        params[paramName] = std::stoi(value);
    }
    paramFile.close();
}