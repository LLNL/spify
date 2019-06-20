
#include "Simple01IFP.h"

Simple01IFP::Simple01IFP(const std::string& fileName)
: spify::parser(fileName)
{
    addParameter
    (
        (new spify::scalar_param<bool>("boolOption"))
        ->shortDesc("My Boolean Option")
        ->defaultValue(false)
    );

    std::vector<int> intOptionVals;
    intOptionVals.push_back(0);
    intOptionVals.push_back(1);
    intOptionVals.push_back(2);
    intOptionVals.push_back(3);
    intOptionVals.push_back(5);
    intOptionVals.push_back(7);

    addParameter
    (
        (new spify::scalar_param<int>("intOption"))
        ->shortDesc("My Integer Option")
        ->discreteValues(intOptionVals)
    );
}

