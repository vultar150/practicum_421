#include <iostream>
#include <vector>

#include "functions.h"


int main(int argc, char **argv)
{
    try {
        FuncFactory factory;
        // std::vector<IFunctionPtr> cont;
        auto functions = factory.getAvailableFunctions();
        for (const auto& func : functions)
        {
            std::cout << func << std::endl;
        }
        std::cout << std::endl;
        auto ff = factory.createFunction("power", 2);
        std::cout << ff->toString() << "  for x = 2 is " << (*ff)(2) << std::endl;
        std::cout << ff->toString() << "  for x = 4 is " << (*ff)(4) << std::endl;
        std::cout << "derive for x = 4 is " << (*ff).getDerive(4) << std::endl;
        std::cout << std::endl;

        auto gg = factory.createFunction("polynomial", {7, 0, 3, 15});

        std::cout << ff->toString() << "  for x = 10 is " << (*ff)(10) << std::endl;
        std::cout << gg->toString() << "  for x = 10 is " << (*gg)(10) << std::endl;

        std::cout << std::endl;
        auto fConst = factory.createFunction("const", 2.);
        std::cout << fConst->toString() << "  for x = 7 is " << (*fConst)(7) << std::endl;
        std::cout << "derive for x = 64 is " << fConst->getDerive(64) << std::endl;

        auto fPolynomial = factory.createFunction("polynomial", {1, 0, 3});
        std::cout << fPolynomial->toString() << "  for x = 2 is " << (*fPolynomial)(2) << std::endl;
        std::cout << "derive for x = 2 is " << fPolynomial->getDerive(2) << std::endl;

        auto fPolynomial2 = factory.createFunction("polynomial", {1, 1, 2});
        std::cout << fPolynomial2->toString() << "  for x = 2.2 is " << (*fPolynomial2)(2.2) << std::endl;
        std::cout << "derive for x = 2.2 is " << fPolynomial2->getDerive(2.2) << std::endl;


        auto fExp = factory.createFunction("exp");
        std::cout << fExp->toString() << "  for x = 2.5 is " << (*fExp)(2.5) << std::endl;
        std::cout << "derive for x = 2.5 is " << fExp->getDerive(2.5) << std::endl;

        auto fIdent = factory.createFunction("ident");
        std::cout << fIdent->toString() << "  for x = 5.5 is " << (*fIdent)(5.5) << std::endl;
        std::cout << fIdent->toString() << "  for x = 5 is " << (*fIdent)(5) << std::endl;
        std::cout << "derive for x = 5.5 is " << fIdent->getDerive(5.5) << std::endl;

        auto fPolynomial3 = factory.createFunction("polynomial", {0, 0, 0, 1., 0, 2.33, 0, 0});
        std::cout << fPolynomial3->toString() << "  for x = 2.2 is " << (*fPolynomial3)(2.2) << std::endl;
        std::cout << "derive for x = 2.2 is " << fPolynomial3->getDerive(2.2) << std::endl;
        std::cout << std::endl;

        Polynomial* pl = new Polynomial({1, 2, 3 , 4});
        Polynomial* fPolynomial4 = new Polynomial(*pl);
        std::cout << fPolynomial4->toString() << "  for x = 2.2 is " << (*fPolynomial4)(2.2) << std::endl;
        std::cout << "derive for x = 2.2 is " << fPolynomial4->getDerive(2.2) << std::endl;
        std::cout << std::endl;

        auto sum = *fPolynomial3 + *fExp;
        std::cout << fPolynomial3->toString() << "  for x = 2 is " << (*fPolynomial3)(2) << std::endl;
        std::cout << fExp->toString() << "  for x = 2 is " << (*fExp)(2) << std::endl;
        std::cout << sum.toString() << "  for x = 2 is " << sum(2) << std::endl;
        std::cout << "derive for x = 2 is " << sum.getDerive(2) << std::endl;

        auto sum2 = *fExp + *fIdent;
        std::cout << fExp->toString() << "  for x = 2 is " << (*fExp)(2) << std::endl;
        std::cout << sum2.toString() << "  for x = 2 is " << sum2(2) << std::endl;
        std::cout << "derive for x = 2 is " << sum2.getDerive(2) << std::endl;
        std::cout << "///////// TESTING MULTYPLY ////////" << std::endl;
        auto mul = (*fExp) * (*fIdent);
        std::cout << fExp->toString() << "  for x = 2 is " << (*fExp)(2) << std::endl;
        std::cout << mul.toString() << "  for x = 2 is " << mul(2) << std::endl;
        std::cout << "derive for x = 2 is " << mul.getDerive(2) << std::endl;

        // std::string str = std::string("sdsd");
        auto mul2 = *fConst + mul * (*fPolynomial);
        std::cout << fPolynomial->toString() << "  for x = 2 is " << (*fPolynomial)(2) << std::endl;
        std::cout << mul2.toString() << "  for x = 2 is " << mul2(2) << std::endl;
        std::cout << "derive for x = 2 is " << mul2.getDerive(2) << std::endl;

        std::cout << "///////// TESTING DIVIDE ////////" << std::endl;

        auto div1 = mul2 / (*fPolynomial3);
        std::cout << mul2.toString() << "  for x = 2 is " << mul2(2) << std::endl;
        std::cout << "derive for x = 2 is " << mul2.getDerive(2) << std::endl << std::endl;

        std::cout << fPolynomial3->toString() << "  for x = 2 is " << (*fPolynomial3)(2) << std::endl;
        std::cout << "derive for x = 2 is " << fPolynomial3->getDerive(2) << std::endl;
    
        std::cout << "///////// RESULT ////////" << std::endl;
        std::cout << div1.toString() << "  for x = 2 is " << div1(2) << std::endl;
        std::cout << "derive for x = 2 is " << div1.getDerive(2) << std::endl;

        std::cout << "///////// TEST OF ROOT ///////////" << std::endl;
        auto f = factory.createFunction("polynomial", {1, 2, 1});
        std::cout << f->toString() << "  for x = 1 is " << (*f)(1) << std::endl;
        std::cout << "derive for x = 1 is " << f->getDerive(1) << std::endl;

        // std::cout << "root of " << f->toString() << " = 0 is x = " << getRoot(*f) << std::endl;
        auto g = mul2 - *fExp;
        double root = getRoot(g, 1000, -2);
        std::cout << "root of " << g.toString() << " = 0 is x = " << root << std::endl;
        std::cout << g.toString() << "  for x = " << root << " is " << g(root) << std::endl;

        auto f2 = *f * *f;
        std::cout << f2.toString() << "  for x = 1 is " << f2(1) << std::endl;
        std::cout << "derive for x = 1 is " << f2.getDerive(1) << std::endl;
        root = getRoot(f2, 1000, -0.5);
        std::cout << "root of " << f2.toString() << " = 0 is x = " << root << std::endl;

        auto f3 = Ident() + Const(1.);
        std::cout << f3.toString() << "  for x = 1 is " << f3(1) << std::endl;
        std::cout << "derive for x = 1 is " << f3.getDerive(1) << std::endl;
        root = getRoot(f3);
        std::cout << "root of " << f3.toString() << " = 0 is x = " << root << std::endl;
        std::cout << f3.toString() << "  for x = " << root << " is " << f3(root) << std::endl;
    }
    catch (std::logic_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << typeid(err).name() << std::endl;
    }
    // *fPolynomial4 = *fPolynomial3;
    // std::cout << fPolynomial4->toString() << "  for x = 2.2 is " << (*fPolynomial4)(2.2) << std::endl;
    // std::cout << "derive for x = 2.2 is " << fPolynomial4->getDerive(2.2) << std::endl;
    // std::cout << std::endl;
    // std::cout << fPolynomial3->toString() << "  for x = 2.2 is " << (*fPolynomial3)(2.2) << std::endl;
    // std::cout << "derive for x = 2.2 is " << fPolynomial3->getDerive(2.2) << std::endl;
    return 0;
}