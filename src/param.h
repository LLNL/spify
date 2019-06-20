#ifndef SPIFY_PARAM_H_93F5F26EF36C4E9995768306F1C06121
#define SPIFY_PARAM_H_93F5F26EF36C4E9995768306F1C06121

#include "yaml-cpp/yaml.h"
#include <ostream>
#include <typeinfo>

namespace spify
{

class parser;

//Future Consideration: Add soft bounded min/max constraints.
//Future Consideration: Create IFPParamConstraint class to clean up constraint
//                      handling and printing.

class param
{
    public:
        explicit param(const std::string& key);
        virtual ~param(){};

        friend class parser;

    private:
        param(){}; //disallow default constructor

    protected:
        std::string m_key;
        std::string m_shortDesc;
        std::string m_longDesc;

        //Accessor functions
        const std::string& key(){ return m_key; };
        const std::string& shortDesc(){ return m_shortDesc; };
        const std::string& longDesc(){ return m_longDesc; };

        //String functions used for descriptions in create master input files.
        template<class T> static const std::string& dummyString();
        template<class T> static const std::string& typeString();

     //Polymorphic funcs.
        //check for existence and validity of parameter in input file 
        //throw exception if invalid 
        virtual void validate(YAML::Node &inputFile) = 0;
        virtual void print(std::ostream& os);
        virtual const std::string& fullTypeString() = 0;
        virtual const std::type_info& typeInfo() = 0;
};

} //namespace spify

#endif
