#include <iostream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "../src/functions.h"


const double ABS_ERROR = 0.001;


// testing createFunction(...)
TEST(MyTest, TestFuncFactory) {
    FuncFactory factory;
    std::string functions[] = {"const", "exp", "ident", "polynomial", "power"};
    int i = 0;
    for (const auto& funcName : factory.getAvailableFunctions()) {
        ASSERT_EQ(funcName, functions[i]);
        ++i;
    }
    // test Const creating
    auto fconst1 = factory.createFunction("const", 1.33);
    auto fconst2 = factory.createFunction("const");

    ASSERT_TRUE(std::dynamic_pointer_cast<Const>(fconst1));
    ASSERT_TRUE(std::dynamic_pointer_cast<Const>(fconst2));

    ASSERT_NEAR((*fconst1)(22), 1.33, ABS_ERROR);
    ASSERT_NEAR((*fconst2)(-30.321), 0., ABS_ERROR);
    // end test Const creating

    // test Polynomial creating
    auto fpolynomial1 = factory.createFunction("polynomial");
    auto fpolynomial2 = factory.createFunction("polynomial", {1});
    auto fpolynomial3 = factory.createFunction("polynomial", {1.22, 21, 2.213, 322});

    ASSERT_THROW(factory.createFunction("polynomial", {}), std::logic_error);
    ASSERT_TRUE(std::dynamic_pointer_cast<Polynomial>(fpolynomial1));
    ASSERT_TRUE(std::dynamic_pointer_cast<Polynomial>(fpolynomial2));
    ASSERT_TRUE(std::dynamic_pointer_cast<Polynomial>(fpolynomial3));

    ASSERT_NEAR((*fpolynomial1)(22), 0., ABS_ERROR);
    ASSERT_NEAR((*fpolynomial2)(15.33), 1., ABS_ERROR);
    ASSERT_NEAR((*fpolynomial3)(1.33), 790.61369, ABS_ERROR);
    // end test Polynomial creating

    // test Ident creating
    auto fident = factory.createFunction("ident");
    ASSERT_TRUE(std::dynamic_pointer_cast<Ident>(fident));
    ASSERT_NEAR((*fident)(22.3), 22.3, ABS_ERROR);
    // end test Ident creating

     // test Power creating
    auto fpower1 = factory.createFunction("power");
    auto fpower2 = factory.createFunction("power", 3);
    auto fpower3 = factory.createFunction("power", -3);

    ASSERT_TRUE(std::dynamic_pointer_cast<Power>(fpower1));
    ASSERT_TRUE(std::dynamic_pointer_cast<Power>(fpower2));
    ASSERT_TRUE(std::dynamic_pointer_cast<Power>(fpower3));

    ASSERT_NEAR((*fpower1)(22.3), 1., ABS_ERROR);
    ASSERT_NEAR((*fpower2)(2), 8., ABS_ERROR);
    ASSERT_NEAR((*fpower3)(2), 0.125, ABS_ERROR);
    // end test Power creating

    // test Exp creating
    auto fexp1 = factory.createFunction("exp");
    ASSERT_TRUE(std::dynamic_pointer_cast<Exp>(fexp1));
    ASSERT_NEAR((*fexp1)(3), 20.0855, ABS_ERROR);
    // end test Exp creating
}


// testing main functionality
TEST(MyTest, TestIFunction) {
    FuncFactory factory;

    Power fpower1;
    ASSERT_NEAR(fpower1(24.21), 1., ABS_ERROR);
    ASSERT_NEAR(fpower1(-24.21), 1., ABS_ERROR);
    ASSERT_NEAR(fpower1.getDerive(4.1), 0., ABS_ERROR);
    ASSERT_EQ(fpower1.toString(), "1");

    Power fpower2(2);
    ASSERT_NEAR(fpower2(24.21), 586.1241, ABS_ERROR);
    ASSERT_NEAR(fpower2(-24.21), 586.1241, ABS_ERROR);
    ASSERT_NEAR(fpower2.getDerive(4.1), 8.2, ABS_ERROR);
    ASSERT_EQ(fpower2.toString(), "x^2");


    auto fpower3 = factory.createFunction("power", 1);
    ASSERT_NEAR((*fpower3)(24.21), 24.21, ABS_ERROR);
    ASSERT_NEAR((*fpower3)(-24.21), -24.21, ABS_ERROR);
    ASSERT_NEAR(fpower3->getDerive(4.1), 1., ABS_ERROR);
    ASSERT_EQ(fpower3->toString(), "x");

    auto fpower4 = factory.createFunction("power", -2);
    ASSERT_NEAR((*fpower4)(5.2), 0.03698, ABS_ERROR);
    ASSERT_NEAR((*fpower4)(-5.2), 0.03698, ABS_ERROR);
    ASSERT_NEAR(fpower4->getDerive(-2.5), 0.128, ABS_ERROR);
    ASSERT_EQ(fpower4->toString(), "x^-2");

    auto fpower5 = factory.createFunction("power", 9);
    ASSERT_NEAR((*fpower5)(10), 1e+9, ABS_ERROR);
    ASSERT_NEAR((*fpower5)(-10), -1e+9, ABS_ERROR);
    ASSERT_NEAR(fpower5->getDerive(4.1), 718643.271, ABS_ERROR);
    ASSERT_EQ(fpower5->toString(), "x^9");


    auto fexp1 = factory.createFunction("exp");
    ASSERT_NEAR((*fexp1)(2), 7.3888, ABS_ERROR);
    ASSERT_NEAR(fexp1->getDerive(2), 7.3888, ABS_ERROR);
    ASSERT_EQ(fexp1->toString(), "e^x");

    Exp fexp2;
    ASSERT_NEAR(fexp2(2), 7.3888, ABS_ERROR);
    ASSERT_NEAR(fexp2.getDerive(2), 7.3888, ABS_ERROR);
    ASSERT_EQ(fexp2.toString(), "e^x");


    auto fident1 = factory.createFunction("ident");
    ASSERT_NEAR((*fident1)(-222.3), -222.3, ABS_ERROR);
    ASSERT_NEAR(fident1->getDerive(222.3), 1., ABS_ERROR);
    ASSERT_EQ(fident1->toString(), "x");

    Ident fident2;
    ASSERT_NEAR(fident2(-222.3), -222.3, ABS_ERROR);
    ASSERT_NEAR(fident2.getDerive(222.3), 1., ABS_ERROR);
    ASSERT_EQ(fident2.toString(), "x");


    auto fconst1 = factory.createFunction("const", 2.);
    ASSERT_NEAR((*fconst1)(24.21), 2., ABS_ERROR);
    ASSERT_NEAR((*fconst1)(-24.21), 2., ABS_ERROR);
    ASSERT_NEAR(fconst1->getDerive(4.1), 0., ABS_ERROR);
    ASSERT_EQ(fconst1->toString(), "2");

    Const fconst2(-322.123);
    ASSERT_NEAR(fconst2(24.21), -322.123, ABS_ERROR);
    ASSERT_NEAR(fconst2(-24.21), -322.123, ABS_ERROR);
    ASSERT_NEAR(fconst2.getDerive(4.1), 0., ABS_ERROR);
    ASSERT_EQ(fconst2.toString(), "-322.123");


    auto fpolynomial1 = factory.createFunction("polynomial", {7, 0, 3, 15});
    ASSERT_NEAR((*fpolynomial1)(10), 15307., ABS_ERROR);
    ASSERT_NEAR(fpolynomial1->getDerive(10), 4560., ABS_ERROR);
    ASSERT_EQ(fpolynomial1->toString(), "7 + 3*x^2 + 15*x^3");

    auto fpolynomial2 = factory.createFunction("polynomial", {1, 0, 3});
    ASSERT_NEAR((*fpolynomial2)(27.12), 2207.4832, ABS_ERROR);
    ASSERT_NEAR(fpolynomial2->getDerive(-10.2), -61.2, ABS_ERROR);
    ASSERT_EQ(fpolynomial2->toString(), "1 + 3*x^2");

    auto fpolynomial3 = factory.createFunction("polynomial", {0, 0, 0, 1., 0, 2.33, 0, 0});
    ASSERT_NEAR((*fpolynomial3)(2.2), 130.728, ABS_ERROR);
    ASSERT_NEAR(fpolynomial3->getDerive(2.2), 287.428, ABS_ERROR);
    ASSERT_EQ(fpolynomial3->toString(), "x^3 + 2.33*x^5");


    auto fpolynomial4 = factory.createFunction("polynomial", {1, 2, 1});
    ASSERT_NEAR((*fpolynomial4)(1), 4., ABS_ERROR);
    ASSERT_NEAR((*fpolynomial4)(10), 121., ABS_ERROR);
    ASSERT_NEAR((*fpolynomial4)(-10), 81., ABS_ERROR);
    ASSERT_NEAR(fpolynomial4->getDerive(1), 4., ABS_ERROR);
    ASSERT_EQ(fpolynomial4->toString(), "1 + 2*x + x^2");


    Polynomial pl({1, 2, 3 , 4});
    Polynomial fpolynomial5 = pl;
    ASSERT_NEAR(fpolynomial5(2.2), 62.512, ABS_ERROR);
    ASSERT_NEAR(fpolynomial5.getDerive(2.2), 73.28, ABS_ERROR);
    ASSERT_EQ(fpolynomial5.toString(), "1 + 2*x + 3*x^2 + 4*x^3");
}


TEST(MyTest, AddTest) {
    FuncFactory factory;
    auto fpolynomial1 = factory.createFunction("polynomial", {0, 0, 0, 1., 0, 2.33});
    auto fexp = factory.createFunction("exp");

    auto sum = *fpolynomial1 + *fexp;
    ASSERT_NEAR(sum(2), 89.9491, ABS_ERROR);
    ASSERT_NEAR(sum.getDerive(2), 205.789, ABS_ERROR);
    ASSERT_EQ(sum.toString(), "x^3 + 2.33*x^5 + e^x");

    auto sum2 = *fexp + *fpolynomial1;
    ASSERT_NEAR(sum2(2), 89.9491, ABS_ERROR);
    ASSERT_NEAR(sum2.getDerive(2), 205.789, ABS_ERROR);
    ASSERT_EQ(sum2.toString(), "e^x + x^3 + 2.33*x^5");

    auto sum4 = *fpolynomial1 + Exp();
    ASSERT_NEAR(sum4(2), 89.9491, ABS_ERROR);
    ASSERT_NEAR(sum4.getDerive(2), 205.789, ABS_ERROR);
    ASSERT_EQ(sum4.toString(), "x^3 + 2.33*x^5 + e^x");

    auto sum5 = Exp() + *fpolynomial1;
    ASSERT_NEAR(sum5(2), 89.9491, ABS_ERROR);
    ASSERT_NEAR(sum5.getDerive(2), 205.789, ABS_ERROR);
    ASSERT_EQ(sum5.toString(), "e^x + x^3 + 2.33*x^5");

    auto sum6 = Exp() + *fpolynomial1 + Power(2);
    ASSERT_NEAR(sum6(2), 93.9491, ABS_ERROR);
    ASSERT_NEAR(sum6.getDerive(2), 209.789, ABS_ERROR);
    ASSERT_EQ(sum6.toString(), "e^x + x^3 + 2.33*x^5 + x^2");

    Ident fident;
    auto sum7 = Exp() + *fpolynomial1 + fident + Power(2);
    ASSERT_NEAR(sum7(2), 95.9491, ABS_ERROR);
    ASSERT_NEAR(sum7.getDerive(2), 210.789, ABS_ERROR);
    ASSERT_EQ(sum7.toString(), "e^x + x^3 + 2.33*x^5 + x + x^2");

    auto sum8 = sum7 + Const(5.);
    ASSERT_NEAR(sum8(2), 100.9491, ABS_ERROR);
    ASSERT_NEAR(sum8.getDerive(2), 210.789, ABS_ERROR);
    ASSERT_EQ(sum8.toString(), "e^x + x^3 + 2.33*x^5 + x + x^2 + 5");

    // test exception
    ASSERT_THROW("abc" + Ident() + fident, std::logic_error);
    ASSERT_THROW(Ident() + "abc" + fident, std::logic_error);
    ASSERT_THROW(Ident() + fident + "abc", std::logic_error);

    ASSERT_THROW(5 + Ident() + fident, std::logic_error);
    ASSERT_THROW(Ident() + 5 + fident, std::logic_error);
    ASSERT_THROW(Ident() + fident + 5, std::logic_error);
}


TEST(MyTest, SubTest) {
    FuncFactory factory;
    auto fpolynomial1 = factory.createFunction("polynomial", {0, 0, 0, 1., 0, 2.33});
    Ident fident;

    auto sub1 = *fpolynomial1 - Exp();
    ASSERT_NEAR(sub1(2), 75.17117, ABS_ERROR);
    ASSERT_NEAR(sub1.getDerive(2), 191.01117, ABS_ERROR);
    ASSERT_EQ(sub1.toString(), "x^3 + 2.33*x^5 - e^x");

    ASSERT_THROW("abc" - Ident() + fident, std::logic_error);
    ASSERT_THROW(Ident() - "abc" + fident, std::logic_error);
    ASSERT_THROW(Ident() - fident - "abc", std::logic_error);

    ASSERT_THROW(5. - Ident() + fident, std::logic_error);
    ASSERT_THROW(Ident() - 5. + fident, std::logic_error);
    ASSERT_THROW(Ident() - fident - 5., std::logic_error);
}


TEST(MyTest, MulTest) {
    FuncFactory factory;
    auto fexp = factory.createFunction("exp");
    Ident fident;

    auto mul1 = *fexp * fident;
    ASSERT_NEAR(mul1(2), 14.7781, ABS_ERROR);
    ASSERT_NEAR(mul1.getDerive(2), 22.1672, ABS_ERROR);
    ASSERT_EQ(mul1.toString(), "e^x * x");


    auto fpolynomial2 = factory.createFunction("polynomial", {1, 0, 3});
    Const* fconst1 = new Const(2.);
    auto mul2 = *fconst1 + mul1 * (*fpolynomial2);
    ASSERT_NEAR(mul2(2), 194.115, ABS_ERROR);
    ASSERT_NEAR(mul2.getDerive(2), 465.511, ABS_ERROR);
    ASSERT_EQ(mul2.toString(), "2 + e^x * x * (1 + 3*x^2)");

    ASSERT_THROW("abc" * Ident() * fident, std::logic_error);
    ASSERT_THROW(mul2 * "abc" * fident, std::logic_error);
    ASSERT_THROW(mul1 * fident * "abc", std::logic_error);

    ASSERT_THROW(5 * Ident() * fident, std::logic_error);
    ASSERT_THROW(mul2 * 5 * fident, std::logic_error);
    ASSERT_THROW(mul1 * fident * 5, std::logic_error);
}


TEST(MyTest, DivideTest) {
    FuncFactory factory;
    auto fpolynomial1 = factory.createFunction("polynomial", {0, 0, 0, 1., 0, 2.33});
    auto fpolynomial2 = factory.createFunction("polynomial", {1, 0, 3});
    auto mul1 = Exp() * Ident();
    auto mul2 = Const(2.) + mul1 * (*fpolynomial2);

    auto div1 = mul2 / (*fpolynomial1);
    ASSERT_NEAR(div1(2), 2.3512, ABS_ERROR);
    ASSERT_NEAR(div1.getDerive(2), -0.0117306, ABS_ERROR);
    ASSERT_EQ(div1.toString(), "(2 + e^x * x * (1 + 3*x^2)) / (x^3 + 2.33*x^5)");

    ASSERT_THROW("abc" / Ident(), std::logic_error);
    ASSERT_THROW(mul2 / "abc", std::logic_error);
    ASSERT_THROW(Ident() / 5, std::logic_error);
    ASSERT_THROW(5 / div1, std::logic_error);
}



// gradient descent testing
TEST(MyTest, GradientDescentTesting) {
    FuncFactory factory;

    auto f = factory.createFunction("polynomial", {1, 2, 1});

    // first equation
    ASSERT_EQ(f->toString(), "1 + 2*x + x^2");
    ASSERT_NEAR((*f)(getRoot(*f, 1000, 10.)), 0., ABS_ERROR);


    auto fpolynomial2 = factory.createFunction("polynomial", {1, 0, 3});
    auto mul2 = Const(2.) + Exp() * Ident() * (*fpolynomial2);
    auto g = mul2 - Exp();

    // second equation
    ASSERT_EQ(g.toString(), "2 + e^x * x * (1 + 3*x^2) - e^x");
    ASSERT_NEAR(g(getRoot(g, 10000, -10)), 0., ABS_ERROR);

    auto f2 = *f * *f;

    // third equation
    ASSERT_EQ(f2.toString(), "(1 + 2*x + x^2) * (1 + 2*x + x^2)");
    ASSERT_NEAR(f2(getRoot(f2, 1000, 3)), 0., ABS_ERROR);
    ASSERT_THROW(f2(getRoot(f2, 1000, 10)), std::logic_error);

    auto f3 = Ident() + Const(1.);
    ASSERT_EQ(f3.toString(), "x + 1");
    ASSERT_NEAR(f3(getRoot(f3, 1000, -10)), 0., ABS_ERROR);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}