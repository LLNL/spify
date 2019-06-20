

#include "Simple00IFP.h"

//Constructor
Simple00IFP::Simple00IFP(const std::string& fileName)
: spify::parser(fileName)
{
    addParameter
    (
        (new spify::scalar_param<bool>("myOption"))
        ->shortDesc("My Boolean Option")
        ->defaultValue(false)
    );
    validateParameters();
};


