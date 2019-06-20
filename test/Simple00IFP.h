#ifndef SimpleInputFileParser_H
#define SimpleInputFileParser_H

#include "spify/spify.h"

class Simple00IFP : public spify::parser
{
    public:
        Simple00IFP(const std::string& fileName);
};

#endif
