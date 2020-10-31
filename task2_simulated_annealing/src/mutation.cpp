#include <memory>
#include <unordered_map>
#include "XDecision.h"
#include "mutation.h"


// mutation

int MyOperation::getIdProcessorFrom(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) const
{
    int sum = decision->getSum();
    int randomFrom = arc4random_uniform(sum);
    auto it = decision->getData().begin();
    int right = (*it).second.executionTime;
    while (randomFrom >= right)
    {
        it++;
        right += (*it).second.executionTime;
    }
    return (*it).first;
}


int MyOperation::getIdProcessorTo(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) const
{
    int numOfProcessors = decision->getData().size();
    int sum = decision->getSum();
    int randomTo = arc4random_uniform((numOfProcessors - 1) * sum);
    auto it = decision->getData().begin();
    int right = sum - (*it).second.executionTime;
    while (randomTo >= right)
    {
        it++;
        right += sum - (*it).second.executionTime;
    }
    return (*it).first;
}


int MyOperation::getRandomJobId(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision, 
                                int from) const
{
    int sizeOfProcessor = decision->getData()[from].size();
    int execTimeOfProc = decision->getData()[from].executionTime;
    auto itJob = decision->getData()[from].begin();

    if (sizeOfProcessor <= 1) return (*itJob).first;

    int randJob = arc4random_uniform((sizeOfProcessor - 1) * execTimeOfProc);
    int right = execTimeOfProc - (*itJob).second;
    while(randJob >= right)
    {
        itJob++;
        right += execTimeOfProc - (*itJob).second;
    }
    return (*itJob).first;
}


std::shared_ptr<AbstractTypeDecision<MyDataType>>
MyOperation::mutate(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision)
{
    int numOfProcessors = decision->getData().size();

    if (numOfProcessors <= 1) return decision;

    int from = this->getIdProcessorFrom(decision);
    int to = this->getIdProcessorTo(decision);
    if (from == to) return decision;
    int id = this->getRandomJobId(decision, from);

    std::shared_ptr<AbstractTypeDecision<MyDataType>> newDecision(new TypeDecision);
    *newDecision = *decision;
    newDecision->moveJob(id, from, to);
    return newDecision;
}

// end mutation