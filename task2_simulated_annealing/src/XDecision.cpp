#include <iostream>
#include <unordered_map>
#include "tinyxml2.h"
#include "XDecision.h"


// type for representation of one proceccor

Processor::Processor(int exTime) : executionTime(exTime) {}


void Processor::push(int id, int exTime) 
{
    this->emplace(id, exTime);
    executionTime += exTime;
}


void Processor::eraiseJob(int id)
{
    executionTime -= (*this)[id];
    this->erase(id);
}


void Processor::print() const
{
    std::cout << "\tEXECUTION TIME = " << executionTime << std::endl;
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

    for (int i = 0; i < numOfProcessors; i++) 
    { 
        data.emplace(i, Processor(0));
    }

    pListElement = xmlNode->FirstChildElement("job");
    while (pListElement != nullptr)
    {
        int id, executionTime;
        eResult = pListElement->QueryIntAttribute("id", &id);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryIntAttribute("execution_time", 
                                                  &executionTime);
        XMLCheckResult(eResult);
        data[0].push(id, executionTime);
        pListElement = pListElement->NextSiblingElement("job");
    }
    targetValue = data[0].executionTime;
    sum = targetValue;
}


void TypeDecision::moveJob(int id, int from, int to)
{
    if (from == to) return;
    data[to].push(id, data[from][id]);
    data[from].eraiseJob(id);
    updateTargetValue();
}


void TypeDecision::updateTargetValue()
{
    using Tmp = const std::pair<int, Processor>;
    auto cmp = [&](Tmp& p1, Tmp& p2) {
                    return p1.second.executionTime < p2.second.executionTime;
                };
    auto max = (*std::max_element(data.begin(), 
                                  data.end(), 
                                  cmp)).second.executionTime;
    auto min = (*std::min_element(data.begin(), 
                                  data.end(), 
                                  cmp)).second.executionTime;
    targetValue = max - min;
}


int TypeDecision::targetFunc() const { return targetValue; }


int TypeDecision::getSum() const { return sum; }


std::unordered_map<int, Processor>& TypeDecision::getData() { return data; }


void TypeDecision::print() const
{
    std::cout << "TARGET VALUE = " << targetValue << std::endl;
    std::cout << "PROCESSORS:" << std::endl;
    for (const auto& proc : data)
    {
        std::cout << "\tID OF PROCESSOR = " << proc.first << std::endl; 
        proc.second.print();
    }
    std::cout << "///////////////////////////" << std::endl;
    std::cout << std::endl;
}

// end type of decision