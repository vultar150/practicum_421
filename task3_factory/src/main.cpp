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
        auto f1 = factory.createFunction("const", 1.);
        auto f2 = factory.createFunction("power", 2);
        auto f3 = factory.createFunction("polynomial", {-1, 0, 0, 1});
     
        auto f4 = *f1 + *f2 + *f3;

        std::cout << f4.toString() << std::endl;
        double root = getRoot(f4, 1000, 10.);
        std::cout << f4.toString() << " = 0 root is " << root << std::endl;
        std::cout << f4.toString() << " for x = " << root << " is " << f4(root) << std::endl;

        auto f5 = factory.createFunction("polynomial", {1, 0, -3, 0, 1});
        std::cout << f5->toString() << std::endl;
        std::cout << "derive for x = 2 is " << f5->getDerive(2) << std::endl;

        auto fexp = factory.createFunction("exp");
        auto fexp2 = factory.createFunction("exp");
        auto fconst = factory.createFunction("const", (*fexp2)(2));

        auto fsum = *fexp - *fconst;
        root = getRoot(fsum, 1000, 4);
        std::cout << fsum.toString() << " = 0 root is " << root << std::endl;
        std::cout << fsum.toString() << " for x = " << root << " is " << fsum(root) << std::endl;


    }
    catch (std::logic_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << typeid(err).name() << std::endl;
    }
    return 0;
}