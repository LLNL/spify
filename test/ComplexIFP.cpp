
#include "ComplexIFP.h"

ComplexIFP::ComplexIFP(const std::string& fileName)
: spify::parser(fileName)
{
    addParameter
    (
        (new spify::scalar_param<bool>("boolOption"))
        ->shortDesc("My Boolean Option")
        ->defaultValue(false)
    );

    std::vector<int> ioDiscrete;
    ioDiscrete.push_back(0);
    ioDiscrete.push_back(1);
    ioDiscrete.push_back(3);
    ioDiscrete.push_back(5);
    ioDiscrete.push_back(7);
    addParameter
    (
        (new spify::scalar_param<int>("intOption"))
        ->longDesc(
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
                  )
        ->discreteValues(ioDiscrete)
    );

    std::vector<int> ivoDefault;
    ivoDefault.push_back(-3);
    ivoDefault.push_back(-2);
    ivoDefault.push_back(-1);
    ivoDefault.push_back(0);
    addParameter
    (
        (new spify::vector_param<int>("intVectOption"))
        ->shortDesc("My Integer Vector Option")
        ->defaultValue(ivoDefault)
        ->boundMin(-10)
        ->boundMax(10)
    );

    addParameter
    (
        (new spify::scalar_param<double>("floatOption"))
        ->defaultValue(300.0)
        ->boundMax(3000.0)
    );

    addParameter
    (
        (new spify::vector_param<double>("floatVectOption"))
        ->boundMin(0.0)
        ->boundMax(100.0)
    );

    std::vector<std::string> soDiscrete;
    soDiscrete.push_back(std::string("foo"));
    soDiscrete.push_back(std::string("bar"));
    soDiscrete.push_back(std::string("baz"));
    addParameter
    (
        (new spify::scalar_param<std::string>("stringOption"))
        ->defaultValue("foo")
        ->discreteValues(soDiscrete)
    );

    addParameter
    (
        (new spify::vector_param<std::string>("stringVectOption"))
        ->shortDesc("My String Vector Option")
    );

    std::vector<int> iiMODiscrete;
    iiMODiscrete.push_back(1);
    iiMODiscrete.push_back(2);
    iiMODiscrete.push_back(4);
    iiMODiscrete.push_back(6);
    addParameter
    (
        (new spify::map_param<int,int>("intintMapOption"))
        ->shortDesc("My Integer-Integer Map Option")
        ->discreteValuesFirst(iiMODiscrete)
    );

    std::map<std::string,int> siMODefault;
    siMODefault["foo"] = 1;
    siMODefault["bar"] = 2;
    addParameter
    (
        (new spify::map_param<std::string,int>("stringintMapOption"))
        ->defaultValue(siMODefault)
    );

    std::map<double,double> ffMODefault;
    ffMODefault[1.0] = 10.0;
    ffMODefault[2.0] = 20.0;
    ffMODefault[3.0] = 30.0;
    addParameter
    (
        (new spify::map_param<double,double>("floatfloatMapOption"))
        ->defaultValue(ffMODefault)
    );
}

