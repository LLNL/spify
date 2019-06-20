#ifndef ComplexIFP_H
#define ComplexIFP_H

#include "spify/spify.h"

class ComplexIFP : public spify::parser
{
    public:
        ComplexIFP(const std::string& fileName);
};

#endif
