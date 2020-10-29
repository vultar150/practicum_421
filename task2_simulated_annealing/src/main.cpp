#include <iostream>
#include <memory>
#include "classes.h"


using Law = std::unique_ptr<AbstractTDecreaseLaw>;
using DataRepresentation = std::vector<Processor>;
using DecisionType = std::unique_ptr<AbstractTypeDecision<DataRepresentation>>;

int main(int argc, char** argv)
{
    Law tempLaw1 = std::make_unique<BoltzmannLaw>(10);
    Law tempLaw2 = std::make_unique<CauchyLaw>(10);
    Law tempLaw3 = std::make_unique<ThirdLaw>(10);

    // std::cout << tempLaw1->getDecreaseTemperature(3) << std::endl;
    // std::cout << tempLaw2->getDecreaseTemperature(3) << std::endl;
    // std::cout << tempLaw3->getDecreaseTemperature(3) << std::endl;

    DecisionType decision = std::make_unique<TypeDecision>(argv[1]);

    decision->print();
    decision->moveJob(3, 0, 1);
    decision->print();
    return 0;
}