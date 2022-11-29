#include "spify/parser.h"
#include <fstream>
#include <iostream>
#include <cassert>


namespace spify
{

parser::parser(const std::string& fileName)
{
    try
    {
        m_inputFile = YAML::LoadFile(fileName);
    }
    catch(...)
    {
        std::cout << "Couldn't parse inputfile: " + fileName << std::endl;
        exit(-1);
    }
    m_parametersValidated = false;
}


parser::~parser()
{
    //Until smart pointers we have to delete pointers manually
    for(pMap::const_iterator p=m_params.begin(); p != m_params.end(); ++p)
    {
        delete p->second;
    }
}


void parser::addParameter(param* p)
{
    const std::string& newKey = p->key();
    if(m_parametersValidated)
    {
       throw(std::logic_error("parser: attempted to add parameter ("+ newKey +
                              ") after validation."));
    }
    //Don't allow overwriting old params
    pMap::const_iterator oldP =  m_params.find(newKey);
    if(oldP != m_params.end())
    {
        throw(std::logic_error("parser: attempted to overwrite exisiting"
                               " parameter: " + newKey));
    }
    m_params[newKey] = p;
    m_paramOrder.push_back(newKey); //Keep insertion order.
}


void parser::validateParameters()
{
    //Secret phrase prints master file.
    //  Now that all parameters are added we can print full master.
    if(m_inputFile["printMasterFileTo"]) 
    {
        printMasterFile(m_inputFile["printMasterFileTo"].as<std::string>());
    }
    for(pMap::const_iterator p=m_params.begin(); p != m_params.end(); ++p)
    {
        try
        {
            p->second->validate(m_inputFile);
        }
        //Future consideration: Create specific exception for invalid parameter
        catch( const std::runtime_error& re )
        {
//            exit(-1);
            throw;
        }
    }
    m_parametersValidated = true;
}


template<class T>
T parser::get(const std::string& key)
{
    //Validate, if not already.
    if(!m_parametersValidated)
    {
        validateParameters();
    }

    //Don't allow getting parameters that are not parameters.
    pMap::const_iterator p =  m_params.find(key);
    if(p == m_params.end())
    {
        throw(std::logic_error("parser: attempted to get parameter that's not"
                               " in database: " + key));
    }

    //Make sure we access parameter as the type it was defined.
    if(typeid(T) == p->second->typeInfo())
    {
        try
        {
            return m_inputFile[key].as<T>();
        }
        catch(const YAML::Exception &e)
        {
            if(!m_inputFile[key])
            {
                //If key doesn't exist we don't know this parameter.
                //  NB: This shouldn't happen as we already checked to make sure
                //      a parameter has this key and the parameter should have
                //      made sure that the key exists in the inputFile.
                assert(0 &&
                       "Parameter not found after validation: coding error?");
            }
            else
            {
                assert(0 &&
                       "Parameter type invalid after validation: coding error?");
            }
        }
    }
    else
    {
        //Asked for parameter by wrong type.
        const std::string& queriedType = param::typeString<T>();
        const std::string& paramType = p->second->fullTypeString();
        throw(std::logic_error("parser: attempt to get parameter " + key +
                               " as " + queriedType + ", but parameter is of"
                               " type " + paramType + ".  Check parameter"
                               " logic for consistency."));
   }
}

template<class T>
void parser::get(const std::string& key, T& val)
{
    val = get<T>(key);
}


void parser::printMasterFile(const std::string& masterFileName)
{
    std::cout << "Printing out master file to " << masterFileName << " ... ";
    std::ofstream ofs;
    //Over-writing existing file
    ofs.open(masterFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    //Future Consideration: Add a header.  Needs some descriptors in parser
    for(int j = 0; j < m_paramOrder.size(); ++j)
    {
        const std::string& key = m_paramOrder[j];
        m_params[key]->print(ofs);
        ofs << std::endl;
    }
    ofs.close();
    std::cout << "Done." << std::endl;
}


//Declare all the get functions for data types we support.
template bool parser::get<bool>(const std::string& key);
template int parser::get<int>(const std::string& key);
template double parser::get<double>(const std::string& key);
template std::string parser::get<std::string>(const std::string& key);
template std::vector<int>  parser::get<std::vector<int> >(const std::string& key);
template std::vector<double>  parser::get<std::vector<double> >(const std::string& key);
template std::vector<std::string>  parser::get<std::vector<std::string> >(const std::string& key);
template std::map<int,bool>  parser::get<std::map<int,bool> >(const std::string& key);
template std::map<int,int>  parser::get<std::map<int,int> >(const std::string& key);
template std::map<int,double>  parser::get<std::map<int,double> >(const std::string& key);
template std::map<int,std::string>  parser::get<std::map<int,std::string> >(const std::string& key);
template std::map<std::string,bool>  parser::get<std::map<std::string,bool> >(const std::string& key);
template std::map<std::string,int>  parser::get<std::map<std::string,int> >(const std::string& key);
template std::map<std::string,double>  parser::get<std::map<std::string,double> >(const std::string& key);
template std::map<std::string,std::string>  parser::get<std::map<std::string,std::string> >(const std::string& key);
template std::map<double,double>  parser::get<std::map<double,double> >(const std::string& key);

template void parser::get<bool>(const std::string& key, bool& val);
template void parser::get<int>(const std::string& key, int& val);
template void parser::get<double>(const std::string& key, double& val);
template void parser::get<std::string>(const std::string& key, std::string& val);
template void parser::get<std::vector<int> >(const std::string& key, std::vector<int> & val);
template void parser::get<std::vector<double> >(const std::string& key, std::vector<double> & val);
template void parser::get<std::vector<std::string> >(const std::string& key, std::vector<std::string> & val);
template void parser::get<std::map<int,bool> >(const std::string& key, std::map<int,bool> & val);
template void parser::get<std::map<int,int> >(const std::string& key, std::map<int,int> & val);
template void parser::get<std::map<int,double> >(const std::string& key, std::map<int,double> & val);
template void parser::get<std::map<int,std::string> >(const std::string& key, std::map<int,std::string> & val);
template void parser::get<std::map<std::string,bool> >(const std::string& key, std::map<std::string,bool> & val);
template void parser::get<std::map<std::string,int> >(const std::string& key, std::map<std::string,int> & val);
template void parser::get<std::map<std::string,double> >(const std::string& key, std::map<std::string,double> & val);
template void parser::get<std::map<std::string,std::string> >(const std::string& key, std::map<std::string,std::string> & val);
template void parser::get<std::map<double,double> >(const std::string& key, std::map<double,double> & val);

} //namespace spify
