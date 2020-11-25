#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "functions.h"


int main(int argc, char **argv)
{   
    FuncFactory factory;
    auto functions = factory.getAvailableFunctions();
    for (const auto& func : functions)
    {
        std::cout << func << std::endl;
    }

    for (const auto& funcName : {"const", "ident"})
    {
        auto func = factory.createFunction(funcName);
        std::cout << (*func)(22) << std::endl;
    }


    return 0;
}