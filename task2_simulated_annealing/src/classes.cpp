#include <iostream>
#include <cmath>
#include "tinyxml2.h"
#include "classes.h"

using namespace tinyxml2;


// types for representation of one job and one proceccor

Job::Job(int id, int t) : id(id), executionTime(t) {}


Job::Job(const Job& job)
{
    id = job.id;
    executionTime = job.executionTime;
}


Job& Job::operator=(const Job& job)
{
    if (this == &job) return *this;
    id = job.id;
    executionTime = job.executionTime;
    return *this;
}


void Job::print() const
{
    std::cout << "\tJob: [" << id << ", "  
              << executionTime << "]" << std::endl;
}


Processor::Processor(int id, int t) : id(id), executionTime(t) {}


void Processor::push(const Job& job) 
{
    this->emplace_back(job);
    executionTime += job.executionTime;
}


void Processor::eraise_job(std::list<Job>::iterator it)
{
    executionTime -= (*it).executionTime;
    this->erase(it);
}


void Processor::print() const
{
    std::cout << "id = " << id << std::endl;
    std::cout << "Execution time = " << executionTime << std::endl;
    for (const auto& job : *this) job.print();
    std::cout << std::endl;
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

TypeDecision::TypeDecision() {}


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

    for (int i = 0; i < numOfProcessors; i++) { data.emplace_back(Processor(i)); }

    pListElement = xmlNode->FirstChildElement("job");
    while (pListElement != nullptr)
    {
        int id, executionTime;
        eResult = pListElement->QueryIntAttribute("id", &id);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryIntAttribute("execution_time", &executionTime);
        XMLCheckResult(eResult);
        data[0].push(Job(id, executionTime));
        pListElement = pListElement->NextSiblingElement("job");
    }
    targetValue = data[0].executionTime;
}


void TypeDecision::moveJob(int id, int from, int to)
{
    auto lm = [&] (Job& job) { return job.id == id; };
    auto it = std::find_if(data[from].begin(), data[from].end(), lm);
    if (it == data[from].end()) 
    {
        std::cerr << "Wrong ID" << std::endl;
        return;
    }
    Job job(*it);
    data[from].eraise_job(it);
    data[to].push(job);
    updateTargetValue();
}


void TypeDecision::updateTargetValue()
{
    auto cmp = [&] (Processor& proc1, Processor& proc2) { 
                        return proc1.executionTime < proc2.executionTime; };
    auto max = (*std::max_element(data.begin(), data.end(), cmp)).executionTime;
    auto min = (*std::min_element(data.begin(), data.end(), cmp)).executionTime;
    targetValue = max - min;
}


void TypeDecision::print() const
{
    std::cout << "Target value = " << targetValue << std::endl;
    for (const auto& proc : data) proc.print();
}

// template<typename ProcType>
// TypeDecision<ProcType>::TypeDecision(char* fileName)
// {
//     std::cout << "SSS" << std::endl;
//     this->parseInputData(fileName);
// }

// template<typename ProcType>
// void TypeDecision<ProcType>::parseInputData(char* fileName)
// {
//     std::cout << fileName << std::endl;
// }

// TypeDecision::TypeDecision(char* fileName)
// {
//     parseInputData(fileName);
// }

// void TypeDecision::parseInputData(char* fileName)
// {
//     XMLError eResult;
//     XMLDocument xmlDocument;

//     eResult = xmlDocument.LoadFile(fileName); // load XML file
//     XMLCheckResult(eResult);

// //// XML parsing//////////////////////////////////////////
//     XMLNode * xmlNode = xmlDocument.FirstChildElement(); // root tag
//     if (xmlNode == nullptr) return XML_ERROR_FILE_READ_ERROR;
//     // XMLError eResult;

//     XMLElement * pListElement = xmlNode->FirstChildElement("processors");
//     if (pListElement == nullptr) exit(XML_ERROR_PARSING_ELEMENT);


//     while (pListElement != nullptr)
//     {
//         vector<TaskPosition> taskPos;
//         int id, major_frame, priority, period, processorNum, BCET, WCET;
//         eResult = pListElement->QueryIntAttribute("index", &id);
//         XMLCheckResult(eResult);
//         eResult = pListElement->QueryIntAttribute("maj_fr", &major_frame);
//         XMLCheckResult(eResult);
//         eResult = pListElement->QueryIntAttribute("prio", &priority);
//         XMLCheckResult(eResult);
//         eResult = pListElement->QueryIntAttribute("period", &period);
//         XMLCheckResult(eResult); 
//         eResult = pListElement->QueryIntAttribute("proc", &processorNum);
//         XMLCheckResult(eResult);
//         eResult = pListElement->QueryIntAttribute("bcet", &BCET);
//         XMLCheckResult(eResult);
//         eResult = pListElement->QueryIntAttribute("wcet", &WCET);
//         XMLCheckResult(eResult);
//         usd[id] = false;
//         tasks[id] = new Task(id, major_frame, priority, period, processorNum, BCET, WCET);
//         processors[processorNum] = taskPos;
//         pListElement = pListElement->NextSiblingElement("task");
//     }
// }

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