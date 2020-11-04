#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/math.h>
#include</Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/__cxx_version>
#include <iostream>
#include <string>
#include <unordered_map>

#include "genfunctions.h"


// in the program is used additional library "tinyxml2" the header
// file of which included in "genfunctions.h

// output of the generated data to the file of XML 
// format, according to the specified generator parameters

// generatorParamType defined in genfunction.h


int main (int argc, char** argv) 
{
    // generator parametrs

    generatorParamType generatorParams = { {"NUMBER_OF_PROC", "0"},
                                           {"NUMBER_OF_JOBS", "0"},
                                           {"LEFT_BOUND", "0"},
                                           {"RIGHT_BOUND", "0"} };
    fillDataMap (argv[1], generatorParams);  // read file with parametrs
    generateData (generatorParams, argv[1]); // generate 
    return 0;
}