#include <iostream>
#include <unordered_map>
#include "tinyxml2.h"
#include "XDecision.h"


// type for representation of one proceccor

Processor::Processor(int exTime) : _executionTime(exTime) {}


void Processor::push(int id, int exTime) 
{
    this->emplace(id, exTime);
    _executionTime += exTime;
}


void Processor::eraiseJob(int id)
{
    _executionTime -= (*this)[id];
    this->erase(id);
}

int Processor::getExecTime() const { return _executionTime; }


void Processor::print() const
{
    std::cout << "\tEXECUTION TIME = " << _executionTime << std::endl;
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

TypeDecision::TypeDecision(int v, 
                           int sum) : AbstractTypeDecision<MyDataType>(v, sum) 
{
}


TypeDecision::TypeDecision(char* fileName)
{
    this->parseInputData(fileName);
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
    int sum = 0;

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
                    return p1.second.getExecTime() < p2.second.getExecTime();
                };
    auto max = (*std::max_element(_data.begin(), 
                                  _data.end(), 
                                  cmp)).second.getExecTime();
    auto min = (*std::min_element(_data.begin(), 
                                  _data.end(), 
                                  cmp)).second.getExecTime();
    _targetValue = max - min;
}


int TypeDecision::targetFunc() const { return _targetValue; }


int TypeDecision::getExecTime() const { return _executionTime; }


std::unordered_map<int, Processor>& TypeDecision::getData() { return _data; }


void TypeDecision::print() const
{
    std::cout << "PROCESSORS:" << std::endl;
    for (const auto& proc : _data)
    {
        std::cout << "\tID OF PROCESSOR = " << proc.first << std::endl; 
        proc.second.print();
    }
    std::cout << "EXECUTION TIME (SUM) = " << _executionTime << std::endl;
    std::cout << "TARGET VALUE = " << _targetValue << std::endl;
    std::cout << "///////////////////////////" << std::endl;
    std::cout << std::endl;
}

// end type of decision