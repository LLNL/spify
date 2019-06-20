
#include <cstdio>
#include <iostream>
#include "ComplexIFP.h"

int main(int argc, char **argv)
{
    ComplexIFP inputData(argv[1]);
    double myDoubleVal;
    std::cout << inputData.get<bool>("boolOption") << std::endl;
    std::cout << inputData.get<int>("intOption") << std::endl;
   
    inputData.get("floatOption",myDoubleVal);
    std::cout << myDoubleVal << std::endl;
    return 0;
}

