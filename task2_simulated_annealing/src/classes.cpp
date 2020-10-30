#include <iostream>
#include <cmath>
#include "tinyxml2.h"
#include "classes.h"

using namespace tinyxml2;


// types for representation of one job and one proceccor

Processor::Processor(int exTime) : executionTime(exTime) {}


void Processor::push(int id, int exTime) 
{
    this->emplace(id, exTime);
    executionTime += exTime;
}


void Processor::eraise_job(int id)
{
    executionTime -= (*this)[id];
    this->erase(id);
}


void Processor::print() const
{
    std::cout << "EXECUTION TIME = " << executionTime << std::endl;
    for (const auto& job : *this)
    {
        std::cout << "\tJOB: [ID, EXECUTION TIME] = [" << job.first << ", "  
                  << job.second << "]" << std::endl;
    }
}

// end types for representation of one job and one proceccor



// Temperature decrease laws

BoltzmannLaw::BoltzmannLaw(double T0) : T0(T0) {}

CauchyLaw::CauchyLaw(double T0) : T0(T0) {}

ThirdLaw::ThirdLaw(double T0) : T0(T0) {}



double BoltzmannLaw::getDecreaseTemperature(int it) const
{
    return T0 / std::log1p(it);
}


double CauchyLaw::getDecreaseTemperature(int it) const
{
    return T0 / (1 + it);
}


double ThirdLaw::getDecreaseTemperature(int it) const
{
    return T0 * std::log1p(it) / (1 + it);
}

// end temperature decrease laws



// type of decision

TypeDecision::TypeDecision(int targetValue) : targetValue(targetValue) {}


TypeDecision::TypeDecision(char* fileName)
{
    this->parseInputData(fileName);
}


void TypeDecision::parseInputData(char* fileName)
{
    XMLError eResult;
    XMLDocument xmlDocument;
    eResult = xmlDocument.LoadFile(fileName); // load XML file
    XMLCheckResult(eResult);

//// XML parsing//////////////////////////////////////////
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
        eResult = pListElement->QueryIntAttribute("execution_time", &executionTime);
        XMLCheckResult(eResult);
        data[0].push(id, executionTime);
        pListElement = pListElement->NextSiblingElement("job");
    }
    targetValue = data[0].executionTime;
}


void TypeDecision::moveJob(int id, int from, int to)
{
    data[to].push(id, data[from][id]);
    data[from].eraise_job(id);
    updateTargetValue();
}


void TypeDecision::updateTargetValue()
{
    using Tmp = const std::pair<int, Processor>;
    auto cmp = [&](Tmp& p1, Tmp& p2) {
                        return p1.second.executionTime < p2.second.executionTime;
                    };
    auto max = (*std::max_element(data.begin(), data.end(), cmp)).second.executionTime;
    auto min = (*std::min_element(data.begin(), data.end(), cmp)).second.executionTime;
    targetValue = max - min;
}


void TypeDecision::print() const
{
    std::cout << "TARGET VALUE = " << targetValue << std::endl;
    for (const auto& proc : data)
    {
        std::cout << "ID OF PROCESSOR = " << proc.first << std::endl; 
        proc.second.print();
    }
    std::cout << "///////////////////////////" << std::endl;
    std::cout << std::endl;
}

// end type of decision


// // sequential algorithm

// void SequentialAlgorithm::mainCycle(TypeForDecision* input, 
//                                     Mutation* mutation, 
//                                     TemperatureDecreaseLaw* tempDecreaseLaw)
// {
//     int it = 0;

//     while ()
// }

// // end sequential algorithm