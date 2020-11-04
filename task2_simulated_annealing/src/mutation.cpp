#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/math.h>
#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/__cxx_version>

#include <memory>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>

#include "XDecision.h"
#include "mutation.h"


// mutation


MyOperation::MyOperation(bool isMin) : _isMin(isMin) {}


AbstractMutation<MyDataType>& 
MyOperation::operator=(AbstractMutation<MyDataType>& mut)
{
    if (this == &mut) return *this;
    _isMin = mut.isMin();
    return *this;
}


bool MyOperation::isMin() const { return _isMin; }


template<typename TCont>
int MyOperation::getRandomIdPositive(TCont& cont, int execTime)
{
    int random = arc4random_uniform(execTime);
    auto it = cont.begin();

    int right = (*it).second;
    while (random >= right)
    {
        it++;
        right += (*it).second;
    }
    return (*it).first;
}


template<typename TCont>
int MyOperation::getRandomIdNegative(TCont& cont, int execTime, bool shouldNotBeEmpty)
{
    int size = cont.size();
    auto it = cont.begin();
    std::unordered_map<int, int> positive;
    auto it2 = positive.begin();

    if (shouldNotBeEmpty)
    {
        for (const auto& el : cont)
        {
            int ex = el.second;
            if (ex) positive[el.first] = ex;
        }
        size = positive.size();
        it2 = positive.begin();
    }
    int random = arc4random_uniform((size - 1) * execTime);

    if (size <= 1) return shouldNotBeEmpty ? (*it2).first : (*it).first;

    int right = shouldNotBeEmpty ? (execTime - (*it2).second) : (execTime - (*it).second);
    while(random >= right)
    {
        if (shouldNotBeEmpty) it2++; 
        else          it++;
        right += shouldNotBeEmpty ? (execTime - (*it2).second) : (execTime - (*it).second);
    }
    return shouldNotBeEmpty ? (*it2).first : (*it).first;
}


std::shared_ptr<AbstractTypeDecision<MyDataType>>
MyOperation::minCriterion(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision)
{
    int from, to, id;
    int idMaxProc = decision->getIdMaxProc();
    int idMaxExecTime = decision->getData()[idMaxProc]._idMaxExecTime;
    int mExTJob = decision->getData()[idMaxProc][idMaxExecTime];
    int idMinProc = decision->getIdMinProc();
    int idMaxExecTimeOnMin, mExTJobM;
    bool isEmpty = decision->getData()[idMinProc].empty();
    std::shared_ptr<AbstractTypeDecision<MyDataType>> newDecision(new TypeDecision);
    if (not isEmpty)
    {
        idMaxExecTimeOnMin = decision->getData()[idMinProc]._idMaxExecTime;
        mExTJobM = decision->getData()[idMinProc][idMaxExecTimeOnMin];
    }
    int target = decision->targetFunc();
    if (mExTJob < target)
    {
        from = idMaxProc;
        to = decision->getIdMinProc();
        id = idMaxExecTime;
        *newDecision = *decision;
        newDecision->moveJob(id, from, to);
        return newDecision;

    }
    else if (not isEmpty and (mExTJobM < mExTJob) and (mExTJob < target + mExTJobM))
    {
        *newDecision = *decision;
        newDecision->moveJob(idMaxExecTime, idMaxProc, decision->getIdMinProc());
        newDecision->moveJob(idMaxExecTimeOnMin, decision->getIdMinProc(), idMaxProc);
        return newDecision;
    }
    else
    {
        int execTime = decision->getExecTime();
        from = getRandomIdPositive(decision->getData(), execTime);
        to = getRandomIdNegative(decision->getData(), execTime);
        if (to == from) return decision;
        execTime = decision->getData()[from]._executionTime;
        id = getRandomIdNegative(decision->getData()[from], execTime);
    }
    *newDecision = *decision;
    newDecision->moveJob(id, from, to);
    return newDecision;
}


std::shared_ptr<AbstractTypeDecision<MyDataType>>
MyOperation::maxCriterion(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision)
{
    int execTime = decision->getExecTime();
    int from = getRandomIdNegative(decision->getData(), execTime, true);
    int to = getRandomIdPositive(decision->getData(), execTime);
    if (to == from) return decision;
    execTime = decision->getData()[from]._executionTime;
    int id = getRandomIdPositive(decision->getData()[from], execTime);

    std::shared_ptr<AbstractTypeDecision<MyDataType>> newDecision(new TypeDecision);
    *newDecision = *decision;
    newDecision->moveJob(id, from, to);
    return newDecision;
}


std::shared_ptr<AbstractTypeDecision<MyDataType>>
MyOperation::mutate(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision)
{
    int size = decision->getData().size();
    if (size <= 1) return decision;

    return (_isMin) ? minCriterion(decision) : maxCriterion(decision);
}

// end mutation