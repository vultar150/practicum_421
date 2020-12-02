#include <iostream>
#include <vector>

#include "functions.h"


int main(int argc, char **argv)
{
    try {
        FuncFactory factory;
        auto functions = factory.getAvailableFunctions();
        for (const auto& func : functions) {
            std::cout << func << std::endl;
        }
    }
    catch (std::logic_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << typeid(err).name() << std::endl;
    }
    return 0;
}