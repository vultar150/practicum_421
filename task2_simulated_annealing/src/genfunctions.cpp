#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "genfunctions.h"


using namespace tinyxml2;


void fillDataMap (char* fileName, generatorParamType& generatorParams)
{
    std::ifstream file;
    file.open(fileName);
    if (not file.is_open())
    {
        std::cerr << "Can't open the file with name "
                  << fileName << std::endl;
        exit(1);
    }
    file >> generatorParams["NUMBER_OF_PROC"];
    file >> generatorParams["NUMBER_OF_JOBS"];
    file >> generatorParams["LEFT_BOUND"];
    file >> generatorParams["RIGHT_BOUND"];
    file.close();
}


void generateData (generatorParamType& generatorParams, char* fileName) 
{
    XMLDocument xmlDocument; // creating XML document
    XMLDeclaration* xmlDeclaration = xmlDocument.NewDeclaration();
    xmlDocument.InsertEndChild(xmlDeclaration);
    XMLNode* xmlNode = xmlDocument.NewElement("Data"); // creating root tag
    xmlDocument.InsertEndChild(xmlNode); // bind root tag

    XMLElement* pElement = xmlDocument.NewElement("processors");
    pElement->SetAttribute("count", 
                            std::stoi(generatorParams["NUMBER_OF_PROC"]));
    xmlNode->InsertEndChild(pElement);
    int numberOfJobs = std::stoi(generatorParams["NUMBER_OF_JOBS"]);
    for (int i = 0; i < numberOfJobs; i++)
    {
        pElement = xmlDocument.NewElement("job");
        pElement->SetAttribute("id", i);
        int leftBound = std::stoi(generatorParams["LEFT_BOUND"]);
        int rightBound = std::stoi(generatorParams["RIGHT_BOUND"]);
        int delta = arc4random_uniform(rightBound - leftBound + 1); 
        int executionTime = leftBound + delta;
        pElement->SetAttribute("execution_time", executionTime);
        xmlNode->InsertEndChild(pElement);
    }

    std::string nameXmlFile = fileName; // set name for XML file
    nameXmlFile.replace(nameXmlFile.find(".txt"), 4, ".xml");

    xmlDocument.SaveFile(nameXmlFile.c_str()); // save XML file
}