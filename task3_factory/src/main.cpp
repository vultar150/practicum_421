#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

#include "functions.h"


int main(int argc, char **argv)
{   
    FuncFactory factory;
    // std::vector<TFunctionPtr> cont;
    auto functions = factory.getAvailableFunctions();
    for (const auto& func : functions)
    {
        std::cout << func << std::endl;
    }
    std::cout << std::endl;

    auto fConst = factory.createFunction("const", 2);
    std::cout << "CONST(2) = " << (*fConst)(7) << std::endl;
    auto fPolynomial = factory.createFunction("polynomial", {1, 2, 3});
    std::cout << "POLYNOMIAL{1, 2, 3}(2) = " << (*fPolynomial)(2) << std::endl;
    auto fPolynomial2 = factory.createFunction("polynomial", {1, 1, 2});
    std::cout << "POLYNOMIAL{1, 1, 2}(2.2) = " << (*fPolynomial2)(2.2) << std::endl;

    std::cout << "DERIBE OF POLYNOMIAL{1, 2, 3}(2) = " 
              << fPolynomial->getDerive(2) << std::endl;

    auto fExp = factory.createFunction("exp");
    std::cout << "EXP(2.5) = " << (*fExp)(2.5) << std::endl;
    std::cout << "DERIVE OF EXP(2.5) = " << fExp->getDerive(2.5) << std::endl;

    auto fIdent = factory.createFunction("ident");
    std::cout << "IDENT(5.5) = " << (*fIdent)(5.5) << std::endl;
    std::cout << "DERIVE OF IDENT(5.5) = " << (*fIdent).getDerive(5.5) << std::endl;



    return 0;
}