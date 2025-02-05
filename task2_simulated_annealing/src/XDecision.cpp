#include <iostream>
#include <cstdio>
#include <ctime>
#include <unordered_map>
#include "tinyxml2.h"
#include "XDecision.h"


// type for representation of one proceccor

Processor::Processor(uint64_t exTime) : _executionTime(exTime), 
                                       _idMaxExecTime(0), _idMinExecTime(0) {}


void Processor::push(int id, uint64_t exTime)
{
    if (empty()) { _idMaxExecTime = id; _idMinExecTime = id; }
    this->emplace(id, exTime);
    _executionTime += exTime;
    if ((*this)[_idMaxExecTime] < exTime)      _idMaxExecTime = id;
    else if ((*this)[_idMinExecTime] > exTime) _idMinExecTime = id;
}


void Processor::eraiseJob(int id)
{
    if (empty()) return;
    _executionTime -= (*this)[id];
    erase(id);
    using Tmp = const std::pair<int, int>;
    auto cmp = [&](Tmp& p1, Tmp& p2) { return p1.second < p2.second; };
    if (not empty() and id == _idMaxExecTime)
        _idMaxExecTime = (*std::max_element(begin(), end(), cmp)).first;
    if (not empty() and id == _idMinExecTime)
        _idMinExecTime = (*std::min_element(begin(), end(), cmp)).first;
}


Processor::operator uint64_t() const { return _executionTime; }


void Processor::print() const
{
    std::cout << "\tEXECUTION TIME = " << _executionTime << std::endl;
    std::cout << "\tID MAX EXEC TIME JOB = " << _idMaxExecTime << std::endl;
    std::cout << "\tID MIN EXEC TIME JOB = " << _idMinExecTime << std::endl;
    std::cout << "\tIS EMPTY? = " << (empty() ? "YES" : "NO") << std::endl;
    std::cout << "\tJOBS:" << std::endl;
    for (const auto& job : *this)
    {
        std::cout << "\t\tJOB: [ID, EXECUTION TIME] = [" << job.first << ", "  
                  << job.second << "]" << std::endl;
    }
    std::cout << std::endl;
}

// end type for representation of one proceccor


// type of decision

TypeDecision::TypeDecision(uint64_t v, 
                           uint64_t sum) : _targetValue(v), _executionTime(sum) {} 


TypeDecision::TypeDecision(char* fileName)
{
    this->parseInputData(fileName);
}


AbstractTypeDecision<MyDataType>&
TypeDecision::operator=(AbstractTypeDecision<MyDataType>& decision)
{
    if (this == &decision) return *this;
    _data = decision.getData();
    _targetValue = decision.targetFunc();
    _executionTime = decision.getExecTime();
    _idMaxProc = decision.getIdMaxProc();
    _idMinProc = decision.getIdMinProc();
    return *this;
}


AbstractTypeDecision<MyDataType>& 
TypeDecision::operator=(AbstractTypeDecision<MyDataType>&& decision)
{
    if (this == &decision) return *this;
    _data = std::move(decision.getData());
    _targetValue = decision.targetFunc();
    _executionTime = decision.getExecTime();
    _idMaxProc = decision.getIdMaxProc();
    _idMinProc = decision.getIdMinProc();
    decision.setToZero();
    return *this;
}

// XML parser for input data

void TypeDecision::parseInputData(char* fileName)
{
    using namespace tinyxml2;

    XMLError eResult;
    XMLDocument xmlDocument;
    eResult = xmlDocument.LoadFile(fileName); // load XML file
    XMLCheckResult(eResult);

    XMLNode * xmlNode = xmlDocument.FirstChildElement(); // root tag
    if (xmlNode == nullptr) exit(XML_ERROR_FILE_READ_ERROR);

    XMLElement * pListElement = xmlNode->FirstChildElement("processors");
    if (pListElement == nullptr) exit(XML_ERROR_PARSING_ELEMENT);

    int numOfProcessors;
    eResult = pListElement->QueryIntAttribute("count", &numOfProcessors);
    XMLCheckResult(eResult);

    for (int i = 0; i < numOfProcessors; i++) _data.emplace(i, Processor(0));
    uint64_t sum = 0;
    
    // int randp = arc4random_uniform(numOfProcessors);
    pListElement = xmlNode->FirstChildElement("job");
    while (pListElement != nullptr)
    {
        int id, executionTime;
        eResult = pListElement->QueryIntAttribute("id", &id);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryIntAttribute("execution_time", 
                                                  &executionTime);
        XMLCheckResult(eResult);
        sum += executionTime;
        int randp = arc4random_uniform(numOfProcessors);
        _data[randp].push(id, executionTime);
        pListElement = pListElement->NextSiblingElement("job");
    }
    updateTargetValue();
    _executionTime = sum;
}


void TypeDecision::moveJob(int id, int from, int to)
{
    if (from == to) return;
    _data[to].push(id, _data[from][id]);
    _data[from].eraiseJob(id);
    updateTargetValue();
}


void TypeDecision::updateTargetValue()
{
    using Tmp = const std::pair<int, Processor>;
    auto cmp = [&](Tmp& p1, Tmp& p2) {
                    return p1.second._executionTime < p2.second._executionTime;
                };

    uint64_t max = 0; 
    int  id = 0;

    for (const auto& proc : _data)
    {
        if (max < proc.second._executionTime)
        {
            max = proc.second._executionTime;
            id = proc.first;
        }
    }
    auto min = std::min_element(_data.begin(), _data.end(), cmp);
    _idMaxProc = id;
    _idMinProc = (*min).first;
    _targetValue = max - (*min).second._executionTime;
}


void TypeDecision::setToZero()
{
    _targetValue = 0;
    _executionTime = 0;
    _idMaxProc = 0;
    _idMinProc = 0;
}


uint64_t TypeDecision::targetFunc() const { return _targetValue; }


int TypeDecision::getIdMaxProc() const { return _idMaxProc; }


int TypeDecision::getIdMinProc() const { return _idMinProc; }


uint64_t TypeDecision::getExecTime() const { return _executionTime; }


MyDataType& TypeDecision::getData() { return _data; }


void TypeDecision::print() const
{
    std::cout << "PROCESSORS:" << std::endl;
    for (const auto& proc : _data)
    {
        std::cout << "\tID OF PROCESSOR = " << proc.first << std::endl; 
        proc.second.print();
    }
    std::cout << "ID MAX PROC = " << _idMaxProc << std::endl;
    std::cout << "ID MIN PROC = " << _idMinProc << std::endl;
    std::cout << "EXECUTION TIME (SUM) = " << _executionTime << std::endl;
    std::cout << "TARGET VALUE = " << _targetValue << std::endl;
    std::cout << "///////////////////////////" << std::endl;
    std::cout << std::endl;
}

// end type of decision