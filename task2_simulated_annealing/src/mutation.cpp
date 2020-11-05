#include <cmath>

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
int MyOperation::getRandomIdPositive(TCont& cont, uint64_t execTime)
{
    uint64_t random = arc4random_uniform(execTime);
    auto it = cont.begin();

    uint64_t right = (*it).second;
    while (random >= right)
    {
        it++;
        right += (*it).second;
    }
    return (*it).first;
}


template<typename TCont>
int MyOperation::getRandomIdNegative(TCont& cont, uint64_t execTime, bool shouldNotBeEmpty)
{
    uint64_t size = cont.size();
    auto it = cont.begin();
    std::unordered_map<int, uint64_t> positive;
    auto it2 = positive.begin();

    if (shouldNotBeEmpty)
    {
        for (const auto& el : cont)
        {
            uint64_t ex = el.second;
            if (ex) positive[el.first] = ex;
        }
        size = positive.size();
        it2 = positive.begin();
    }
    uint64_t random = arc4random_uniform((size - 1) * execTime);

    if (size <= 1) return shouldNotBeEmpty ? (*it2).first : (*it).first;

    uint64_t right = shouldNotBeEmpty ? (execTime - (*it2).second) : (execTime - (*it).second);
    while(random >= right)
    {
        if (shouldNotBeEmpty) it2++; 
        else                  it++;
        right += shouldNotBeEmpty ? (execTime - (*it2).second) : (execTime - (*it).second);
    }
    return shouldNotBeEmpty ? (*it2).first : (*it).first;
}


std::shared_ptr<AbstractTypeDecision<MyDataType>>
MyOperation::minCriterion(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision)
{
    int idMaxProc     = decision->getIdMaxProc();
    int idMinExecTime = decision->getData()[idMaxProc]._idMinExecTime;
    int idMaxExecTime = decision->getData()[idMaxProc]._idMaxExecTime;
    int mExTJob       = decision->getData()[idMaxProc][idMinExecTime];
    int maxExTJob     = decision->getData()[idMaxProc][idMaxExecTime];
    int idMinProc     = decision->getIdMinProc();

    int  idMaxExecTimeOnMin, mExTJobM;
    bool isEmpty = decision->getData()[idMinProc].empty();
    std::shared_ptr<AbstractTypeDecision<MyDataType>> newDecision(new TypeDecision);

    if (not isEmpty)
    {
        idMaxExecTimeOnMin = decision->getData()[idMinProc]._idMaxExecTime;
        mExTJobM = decision->getData()[idMinProc][idMaxExecTimeOnMin];
    }
    int target = decision->targetFunc();
    int from = idMaxProc, to = idMinProc, id;
    
    if      (maxExTJob < target) { id = idMaxExecTime; }
    else if (mExTJob   < target) { id = idMinExecTime; }
    else if (not isEmpty and 
             (mExTJobM < maxExTJob) and (maxExTJob < target + mExTJobM))
    {
        *newDecision = *decision;
        newDecision->moveJob(idMaxExecTime, from, to);
        newDecision->moveJob(idMaxExecTimeOnMin, to, from);
        return newDecision;
    }
    else
    {
        uint64_t execTime = decision->getExecTime();
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