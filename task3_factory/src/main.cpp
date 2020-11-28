#include <iostream>
#include <vector>

#include "functions.h"


int main(int argc, char **argv)
{
    FuncFactory factory;
    // std::vector<IFunctionPtr> cont;
    auto functions = factory.getAvailableFunctions();
    for (const auto& func : functions)
    {
        std::cout << func << std::endl;
    }
    std::cout << std::endl;
    auto ff = factory.createFunction("power", 2);
    std::cout << ff->toString() << " for x = 2 is " << (*ff)(2) << std::endl;
    std::cout << ff->toString() << " for x = 4 is " << (*ff)(4) << std::endl;
    std::cout << "derive for x = 4 is " << (*ff).getDerive(4) << std::endl;
    std::cout << std::endl;

    auto gg = factory.createFunction("polynomial", {7, 0, 3, 15});

    std::cout << ff->toString() << " for x = 10 is " << (*ff)(10) << std::endl;
    std::cout << gg->toString() << " for x = 10 is " << (*gg)(10) << std::endl;

    std::cout << std::endl;
    auto fConst = factory.createFunction("const", 2.);
    std::cout << fConst->toString() << " for x = 7 is " << (*fConst)(7) << std::endl;
    std::cout << "derive for x = 64 is " << fConst->getDerive(64) << std::endl;

    auto fPolynomial = factory.createFunction("polynomial", {1, 0, 3});
    std::cout << fPolynomial->toString() << " for x = 2 is " << (*fPolynomial)(2) << std::endl;
    std::cout << "derive for x = 2 is " << fPolynomial->getDerive(2) << std::endl;

    auto fPolynomial2 = factory.createFunction("polynomial", {1, 1, 2});
    std::cout << fPolynomial2->toString() << " for x = 2.2 is " << (*fPolynomial2)(2.2) << std::endl;
    std::cout << "derive for x = 2.2 is " << fPolynomial2->getDerive(2.2) << std::endl;


    auto fExp = factory.createFunction("exp");
    std::cout << fExp->toString() << " for x = 2.5 is " << (*fExp)(2.5) << std::endl;
    std::cout << "derive for x = 2.5 is " << fExp->getDerive(2.5) << std::endl;

    auto fIdent = factory.createFunction("ident");
    std::cout << fIdent->toString() << " for x = 5.5 is " << (*fIdent)(5.5) << std::endl;
    std::cout << fIdent->toString() << " for x = 5 is " << (*fIdent)(5) << std::endl;
    std::cout << "derive for x = 5.5 is " << fIdent->getDerive(5.5) << std::endl;

    auto fPolynomial3 = factory.createFunction("polynomial", {0, 0, 0, 1., 0, 2.33, 0, 0});
    std::cout << fPolynomial3->toString() << " for x = 2.2 is " << (*fPolynomial3)(2.2) << std::endl;
    std::cout << "derive for x = 2.2 is " << fPolynomial3->getDerive(2.2) << std::endl;
    std::cout << std::endl;

    // *fPolynomial4 = *fPolynomial3;
    // std::cout << fPolynomial4->toString() << " for x = 2.2 is " << (*fPolynomial4)(2.2) << std::endl;
    // std::cout << "derive for x = 2.2 is " << fPolynomial4->getDerive(2.2) << std::endl;
    // std::cout << std::endl;
    // std::cout << fPolynomial3->toString() << " for x = 2.2 is " << (*fPolynomial3)(2.2) << std::endl;
    // std::cout << "derive for x = 2.2 is " << fPolynomial3->getDerive(2.2) << std::endl;
    return 0;
}