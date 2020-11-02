#include <memory>
#include <unordered_map>
#include <vector>
#include "XDecision.h"
#include "mutation.h"


// mutation

int MyOperation::getIdProcessorFrom(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) const
{
    // std::vector<int> positiveExecTimes;
    // positiveExecTimes.reserve(decision->getData().size());
    // for (const auto& proc : decision->getData())
    // {
    //     if (proc.second.getExecTime() > 0)
    //         positiveExecTimes.push_back(proc.first);
    // }
    // int randomFrom = arc4random_uniform(positiveExecTimes.size());
    // return positiveExecTimes[randomFrom];

    int sum = decision->getExecTime();
    int random = arc4random_uniform(sum);
    auto it = decision->getData().begin();

    int right = (*it).second.getExecTime();
    while (random >= right)
    {
        it++;
        right += (*it).second.getExecTime();
    }
    return (*it).first;
}


int MyOperation::getIdProcessorTo(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) const
{
    // int numOfProcessors = decision->getData().size();
    // int randomTo = arc4random_uniform(numOfProcessors);
    // while (randomTo == from) randomTo = arc4random_uniform(numOfProcessors);
    // return randomTo;

    int size = decision->getData().size();
    int sum = decision->getExecTime();
    int random = arc4random_uniform((size - 1) * sum);
    auto it = decision->getData().begin();

    if (size <= 1) return (*it).first;

    int right = sum - (*it).second.getExecTime();
    while (random >= right)
    {
        it++;
        right += sum - (*it).second.getExecTime();
    }
    return (*it).first;
}


// template<typename TCont>
// int MyOperation::getRandomIdNeg(const TCont& cont, int execTime) const
// {
//     // int sizeOfProcessor = decision->getData()[from].size();
//     // auto itJob = decision->getData()[from].begin();
//     // if (sizeOfProcessor <= 1) return (*itJob).first;
//     // int randid = arc4random_uniform(sizeOfProcessor);
//     // auto it = std::next(itJob, randid);
//     // return (*it).first;

//     int size = cont.size();
//     // int sum = decision->getData()[from].getExecTime();
//     int random = arc4random_uniform((size - 1) * execTime);
//     auto it = cont.begin();

//     if (size <= 1) return (*it).first;

//     int right = execTime - (*it).second.getExecTime();
//     while(random >= right)
//     {
//         it++;
//         right += execTime - (*it).second.getExecTime();
//     }
//     return (*it).first;
// }

int MyOperation::getRandomJobId(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision, 
                                int from) const
{
    // int sizeOfProcessor = decision->getData()[from].size();
    // auto itJob = decision->getData()[from].begin();
    // if (sizeOfProcessor <= 1) return (*itJob).first;
    // int randid = arc4random_uniform(sizeOfProcessor);
    // auto it = std::next(itJob, randid);
    // return (*it).first;

    int size = decision->getData()[from].size();
    int sum = decision->getData()[from].getExecTime();
    int random = arc4random_uniform((size - 1) * sum);
    auto it = decision->getData()[from].begin();

    if (size <= 1) return (*it).first;

    int right = sum - (*it).second;
    while(random >= right)
    {
        it++;
        right += sum - (*it).second;
    }
    return (*it).first;
}


std::shared_ptr<AbstractTypeDecision<MyDataType>>
MyOperation::mutate(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision)
{
    int size = decision->getData().size();

    if (size <= 1) return decision;

    int from = this->getIdProcessorFrom(decision);
    int to = this->getIdProcessorTo(decision);
    while (to == from) to = this->getIdProcessorTo(decision);
    int id = this->getRandomJobId(decision, from);

    std::shared_ptr<AbstractTypeDecision<MyDataType>> newDecision(new TypeDecision);
    *newDecision = *decision;
    newDecision->moveJob(id, from, to);
    return newDecision;
}

// end mutation