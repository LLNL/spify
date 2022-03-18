#ifndef SPIFY_PARSER_H_E048CA79EAD94B3E9743BDE02485A493
#define SPIFY_PARSER_H_E048CA79EAD94B3E9743BDE02485A493

#include "yaml-cpp/yaml.h"
#include "spify/param.h"

namespace spify
{

class parser
{
    public:
        //Defined T for bool, int, double, std:string, std::vector<>s of same
        //and std::map<T,U>s of most pairs
        template<class T> T get(const std::string& key);
        template<class T> void get(const std::string& key, T& val);
        void printMasterFile(const std::string& os);

    protected:
        parser(const std::string& fname); //create YAML::Node from input file
        virtual ~parser();

        void addParameter(param* p); //error on duplicate key or parameters
                                     //  already validated.
        void validateParameters();   //only allowed once,
                                     //  called by first call to get()
        bool m_parametersValidated;

    private:
        parser();  //disallow default constructor
        parser(const parser& sp); //disallow copy construcotr
        parser& operator=(const parser& sp); //disallow assignment

        YAML::Node m_inputFile;
        //Future Consideration: Use smart-pointer (C++11 or boost)
        //Future Consideration: Use unordered_map (C++11)
        typedef std::map<std::string,param*> pMap; //Make iters look cleaner.
        pMap m_params;
        std::vector<std::string> m_paramOrder; //Keep track of insertion order
                                               //  so we can print params in
                                               //  order they were added.
};

} //namespace spify

#endif
