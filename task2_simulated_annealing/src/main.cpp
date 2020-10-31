#include <iostream>
#include <memory>
#include "classes.h"


using Law = std::unique_ptr<AbstractTDecreaseLaw>;
using PointerADecType = std::shared_ptr<AbstractTypeDecision<MyDataType>>;
using AbstrMutationType = std::unique_ptr<AbstractMutation<MyDataType>>;

int main(int argc, char** argv)
{
    Law tempLaw1 = std::make_unique<BoltzmannLaw>(10);
    Law tempLaw2 = std::make_unique<CauchyLaw>(10);
    Law tempLaw3 = std::make_unique<ThirdLaw>(10);

    // std::cout << tempLaw1->getDecreaseTemperature(3) << std::endl;
    // std::cout << tempLaw2->getDecreaseTemperature(3) << std::endl;
    // std::cout << tempLaw3->getDecreaseTemperature(3) << std::endl;
    // AbstractTypeDecision<MyDataType>* tmp = new TypeDecision(argv[1]);
    //PointerADecType decision = std::make_shared<TypeDecision>(argv[1]);
    // tmp = decision;
    // TypeDecision* dec = dynamic_cast<TypeDecision*>(decision);
    // AbstractTypeDecision<MyDataType>* dec = new TypeDecision(argv[1]);
    // TypeDecision* dec = new TypeDecision(4);
    // TypeDecision dec2(argv[1]);
    // AbstractTypeDecision<MyDataType>* dec2 = nullptr;
    // *dec2 = *tmp;
    AbstractTypeDecision<MyDataType>* tmp = new TypeDecision(argv[1]);
    AbstractTypeDecision<MyDataType>* tmp2 = new TypeDecision;
    // *tmp2 = *tmp;
    // tmp->print();
    //PointerADecType decision2 = std::make_shared<TypeDecision>(5);
    // *decision2 = std::move(*decision);
    //*decision2 = *decision;
    // std::shared_ptr<TypeDecision> decision2 = std::make_shared<TypeDecision>(TypeDecision());
    //*decision2 = *decision;

    AbstrMutationType mutation = std::make_unique<MyOperation>();
    tmp2 = mutation->mutate(tmp);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();

    tmp2 = mutation->mutate(tmp2);
    tmp2->print();
    // tmp2 = mutation->mutate(tmp2);
    // tmp2 = mutation->mutate(tmp2);
    // tmp2 = mutation->mutate(tmp2);
    // tmp2->print();
    //tmp->print();
    // decision->print();
    // decision->moveJob(3, 0, 1);

    // AbstractTypeDecision<MyDataType>* tmp2 = mutation->mutate(tmp);
    // std::cout << "!!!!!!!!!!!!!!!!!!!!!!! TMP:" << std::endl;
    // dec.print();


    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!! DEC2:" << std::endl;

    // tmp2->print();

    // dec2.print();
    // decision2->print();
    // decision->moveJob(3, 0, 1);
    // decision->print();
    // decision->getData()[0].print();
    return 0;
}