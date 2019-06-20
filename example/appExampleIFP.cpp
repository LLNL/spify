
#include <cstdio>
#include <iostream>
#include "ExampleIFP.h"

int main(int argc, char **argv)
{
    //Parse file. (See ExampleIFP.cpp for details on changing parameters)
    ExampleIFP inputData(argv[1]);

    //TODO: Test use of all example parameters.

    //Use data for application logic.
    if( inputData.boolOption() )
    {
        std::cout << "Boolean option is on." << std::endl;
    }

    double vectorSum = 0;
    for(int j = 0; j< inputData.floatVectOption().size(); ++j)
    {
        vectorSum += inputData.floatVectOption()[j];
    }
    std::cout << "Sum of floatVectOption values: " << vectorSum << std::endl;

    std::cout << "Value of intOption : "  << inputData.intOption() << std::endl;

    return 0;
}

