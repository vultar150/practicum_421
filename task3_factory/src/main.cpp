#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

#include "functions.h"


int main(int argc, char **argv)
{
    FuncFactory factory;
    std::vector<IFunctionPtr> cont;
    auto functions = factory.getAvailableFunctions();
    for (const auto& func : functions)
    {
        std::cout << func << std::endl;
    }
    std::cout << std::endl;
    auto ff = factory.createFunction("power", 2);
    std::cout << "POWER^2 (2) = " << (*ff)(2) << std::endl;
    std::cout << "POWER^2 (4) = " << (*ff)(4) << std::endl;
    std::cout << "DERIVE OF POWER^2 (4) = " << (*ff).getDerive(4) << std::endl;
    std::cout << std::endl;
    cont.push_back(ff);
    auto gg = factory.createFunction("polynomial", {7, 0, 3, 15});
    cont.push_back(gg);

    for (const auto& ptr: cont)
    {
        std::cout << "for x = 10 is " << (*ptr)(10) << std::endl; 
    }
    std::cout << std::endl;
    auto fConst = factory.createFunction("const", 2.);
    std::cout << "CONST<2.> = " << (*fConst)(7) << std::endl;
    std::cout << "DERIVE OF CONST<2.> = " << (*fConst).getDerive(64) << std::endl;

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
    std::cout << "IDENT(5) = " << (*fIdent)(5) << std::endl;
    std::cout << "DERIVE OF IDENT(5.5) = " << (*fIdent).getDerive(5.5) << std::endl;

    return 0;
}