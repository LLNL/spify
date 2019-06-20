#ifndef Simple01IFP_H
#define Simple01IFP_H

#include "spify/spify.h"


class Simple01IFP : public spify::parser
{
    public:
        Simple01IFP(const std::string& fileName);
};

#endif
