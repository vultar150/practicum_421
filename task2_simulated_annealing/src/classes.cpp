#include <iostream>
#include <cmath>
#include <iterator>
#include "tinyxml2.h"
#include "classes.h"

using namespace tinyxml2;


// type for representation of one proceccor

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
    data[from].eraise_job(id);
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




// mutation

int MyOperation::getIdProcessorFrom(AbstractTypeDecision<MyDataType>* decision) const
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


int MyOperation::getIdProcessorTo(AbstractTypeDecision<MyDataType>* decision) const
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


int MyOperation::getRandomJobId(AbstractTypeDecision<MyDataType>* decision, 
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


AbstractTypeDecision<MyDataType>* 
MyOperation::mutate(AbstractTypeDecision<MyDataType>* decision)
{
    int numOfProcessors = decision->getData().size();

    if (numOfProcessors <= 1) return decision;

    int from = this->getIdProcessorFrom(decision);
    int to = this->getIdProcessorTo(decision);
    if (from == to) return decision;
    int id = this->getRandomJobId(decision, from);

    AbstractTypeDecision<MyDataType>* newDecision = new TypeDecision;
    *newDecision = *decision;
    newDecision->moveJob(id, from, to);
    return newDecision;
}

// end mutation


// // sequential algorithm

// void SequentialAlgorithm::mainCycle(TypeForDecision* input, 
//                                     Mutation* mutation, 
//                                     TemperatureDecreaseLaw* tempDecreaseLaw)
// {
//     int it = 0;

//     while ()
// }

// // end sequential algorithm