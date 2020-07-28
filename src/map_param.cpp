
#include <iostream>
#include "spify/map_param.h"

namespace spify
{

template <class T, class U>
map_param<T,U>::map_param
(
    const std::string& key
)
:
param(key)
{
//    m_defaultValue = UNDEFINED;
    m_defaultDefined = false;
//    m_minBound = UNDEFINED;
//    m_maxBound = UNDEFINED;
    m_boundedMinFirst = false;
    m_boundedMaxFirst = false;
    m_boundedMinSecond = false;
    m_boundedMaxSecond = false;
    m_discreteValuesFirst = std::vector<T>();
    m_discreteValuesSecond = std::vector<U>();
}


template <class T, class U>
inline map_param<T,U>* map_param<T,U>::shortDesc(const std::string& shortDesc)
{
    m_shortDesc = shortDesc;
    return this;
}

template <class T, class U>
inline map_param<T,U>* map_param<T,U>::longDesc(const std::string& longDesc)
{
    m_longDesc = longDesc;
    return this;
}

template <class T, class U>
inline map_param<T,U>* map_param<T,U>::defaultValue(const std::map<T,U>& value)
{
    m_defaultValue = value;
    m_defaultDefined = true;
    return this;
}

//First Constraint Funcs
template <class T, class U>
inline map_param<T,U>* map_param<T,U>::boundMinFirst(const T& value)
{
    m_minBoundFirst = value;
    m_boundedMinFirst = true;
    return this;
}

template <class T, class U>
inline map_param<T,U>* map_param<T,U>::boundMaxFirst(const T& value)
{
    m_maxBoundFirst = value;
    m_boundedMaxFirst = true;
    return this;
}

template <class T, class U>
inline map_param<T,U>* map_param<T,U>::discreteValuesFirst(
   const std::vector<T>& values)
{
    if(!values.size())
    {
        throw(std::logic_error("map_param: Attempted to constrain discrete"
                               " values with empty array."));
    }
    m_discreteValuesFirst = values;
    return this;
}

//Second Constraint Funcs
template <class T, class U>
inline map_param<T,U>* map_param<T,U>::boundMinSecond(const U& value)
{
    m_minBoundSecond = value;
    m_boundedMinSecond = true;
    return this;
}

template <class T, class U>
inline map_param<T,U>* map_param<T,U>::boundMaxSecond(const U& value)
{
    m_maxBoundSecond = value;
    m_boundedMaxSecond = true;
    return this;
}

template <class T, class U>
inline map_param<T,U>* map_param<T,U>::discreteValuesSecond(
    const std::vector<U>& values)
{
    if(!values.size())
    {
        throw(std::logic_error("map_param: Attempted to constrain discrete"
                               " values with empty array."));
    }
    m_discreteValuesSecond = values;
    return this;
}


template <class T, class U>
void map_param<T,U>::validate(YAML::Node &inputFile)
{
    std::map<T,U> values;
    bool foundVal=false;
    if(inputFile[m_key])
    {
        try
        { 
            values = inputFile[m_key].template as<std::map<T,U> >();
            foundVal = true;
        }
        catch (const YAML::Exception &e)
        {
            if(m_defaultDefined)
            {
                std::cout << "Warning: failed to parse optional input value \""
                          << m_key << "\"" << std::endl
                          << "Using default value of: " <<  std::endl
                          << "{" << std::endl;
                if(m_defaultValue.begin() != m_defaultValue.end())
                {
                    typename std::map<T,U>::const_iterator end_iter
                       = (--m_defaultValue.end());
                    for(typename std::map<T,U>::const_iterator 
                            mapiter=m_defaultValue.begin(); 
                            mapiter != end_iter; ++mapiter)
                    {
                        std::cout << "" << mapiter->first << ": "
                                  << mapiter->second << "," << std::endl;
                    }
                    std::cout << "" << end_iter->first << ": "
                              << end_iter->second << std::endl
                              << "}" << std::endl;
                }
                else 
                {
                    std::cout << "}" << std::endl;
                }
            }
            else 
            {
                std::cerr << "Error: failed to parse required input value \""
                          << m_key <<"\""
                          << " (" << fullTypeString() << ")." << std::endl;
                throw(std::runtime_error("value parse error."));
            }
        }
    } 
    if(m_defaultDefined || foundVal)
    {
        if(!foundVal)
        {
            values = m_defaultValue;
            inputFile[m_key] = values; //add value to inputFile
        }
        for(typename std::map<T,U>::const_iterator 
                mapiter=values.begin();
                mapiter != values.end(); ++mapiter)
        {
            checkConstraints(*mapiter);
        }
    }
    else
    {
        std::cerr << "Error: didn't find required input value \"" << m_key << "\""
                  << " (" << fullTypeString() << ")." << std::endl;
        throw(std::runtime_error("value not found."));
    } 
}


template<class T, class U>
void map_param<T,U>::checkConstraints(const std::pair<T,U> &value)
{
    if (m_boundedMinFirst && value.first < m_minBoundFirst)
    {
        std::cerr << "Value provided for first map value of parameter \""
                  << m_key << "\" (" << value.first << ") "
                  << " is below minimum bound (" << m_minBoundFirst << ")."
                  << std::endl;
        throw(std::runtime_error("Invalid value."));
    }
    if (m_boundedMinSecond && value.second < m_minBoundSecond)
    {
        std::cerr << "Value provided for second map value of parameter \""
                  << m_key << "\" (" << value.second << ") "
                  << " is below minimum bound (" << m_minBoundSecond << ")."
                  << std::endl;
        throw(std::runtime_error("Invalid value."));
    }

    if (m_boundedMaxFirst && value.first > m_maxBoundFirst)
    {
        std::cerr << "Value provided for first map value parameter \""
                  << m_key << "\" (" << value.first << ") "
                  << " is above maximum bound (" << m_maxBoundFirst << ")."
                  << std::endl;
        throw(std::runtime_error("Invalid value."));
    }
    if (m_boundedMaxSecond && value.second > m_maxBoundSecond)
    {
        std::cerr << "Value provided for second map value parameter \""
                  << m_key << "\" (" << value.second << ") "
                  << " is above maximum bound (" << m_maxBoundSecond << ")."
                  << std::endl;
        throw(std::runtime_error("Invalid value."));
    }

    if(m_discreteValuesFirst.size())
    {
        bool match = false;
        for(int j = 0; j < m_discreteValuesFirst.size(); ++j)
        {
            if(m_discreteValuesFirst[j] == value.first)
            {
                match = true;
                break;
            }
        }
        if(!match)
        {
            std::cerr << "Value provided for first map value of parameter \""
                      << m_key << "\" (" << value.first << ") "
                      << "is not one of acceptable values [";
            for(int j = 0; j < m_discreteValuesFirst.size()-1; ++j)
            {
                std:: cerr << m_discreteValuesFirst[j] << ",";
            }
            std:: cerr << m_discreteValuesFirst[m_discreteValuesFirst.size()-1]
                       << "]." << std::endl;
            throw(std::runtime_error("Invalid value."));
        }
    }
    if(m_discreteValuesSecond.size())
    {
        bool match = false;
        for(int j = 0; j < m_discreteValuesSecond.size(); ++j)
        {
            if(m_discreteValuesSecond[j] == value.second)
            {
                match = true;
                break;
            }
        }
        if(!match)
        {
            std::cerr << "Value provided for second map value of parameter \""
                      << m_key << "\" (" << value.second << ") "
                      << "is not one of acceptable values [";
            for(int j = 0; j < m_discreteValuesSecond.size()-1; ++j)
            {
                std:: cerr << m_discreteValuesSecond[j] << ",";
            }
            std::cerr << m_discreteValuesSecond[m_discreteValuesSecond.size()-1]
                      << "]." << std::endl;
            throw(std::runtime_error("Invalid value."));
        }
    }
}


template<class T, class U>
void map_param<T,U>::print(std::ostream& os)
{
    param::print(os);
    if(m_boundedMinFirst)
    {
        os << "#First values must be greater than "
           << m_minBoundFirst << std::endl;
    }
    if(m_boundedMaxFirst)
    {
        os << "#First values must be less than "
           << m_maxBoundFirst << std::endl;
    }
    if(m_boundedMinSecond)
    {
        os << "#Second values must be greater than "
           << m_minBoundSecond << std::endl;
    }
    if(m_boundedMaxSecond)
    {
        os << "#Second values must be less than "
           << m_maxBoundSecond << std::endl;
    }
    if(m_discreteValuesFirst.size())
    {
        os << "#First values must be one of [";
        for(int j = 0; j < m_discreteValuesFirst.size()-1; ++j)
        {
            os << m_discreteValuesFirst[j] << ", ";
        }
        os << m_discreteValuesFirst[m_discreteValuesFirst.size()-1] << "]"
           << std::endl;
    }
    if(m_discreteValuesSecond.size())
    {
        os << "#Second values must be one of [";
        for(int j = 0; j < m_discreteValuesSecond.size()-1; ++j)
        {
            os << m_discreteValuesSecond[j] << ", ";
        }
        os << m_discreteValuesSecond[m_discreteValuesSecond.size()-1] << "]"
           << std::endl;
    }
    if(m_defaultDefined)
    {
        os << "#Optional with default value of" << std::endl << "#{"
           << std::endl;
        if(m_defaultValue.begin() != m_defaultValue.end())
        {
            typename std::map<T,U>::const_iterator end_iter
                = (--m_defaultValue.end());
            for(typename std::map<T,U>::const_iterator
                    mapiter=m_defaultValue.begin();
                    mapiter != end_iter; ++mapiter)
            {
                os << "#" << mapiter->first << ": " << mapiter->second << ","
                   << std::endl;
            }
            os << "#" << end_iter->first << ": " << end_iter->second
               << std::endl << "#}" << std::endl;
        }
        else
        {
            os << "#}" << std::endl;
        }
    }
    os << m_key << ": {" << std::endl
       << dummyString<T>() << ": " << dummyString<U>() << "," << std::endl
       << dummyString<T>() << ": " << dummyString<U>() << "," << std::endl
       << dummyString<T>() << ": " << dummyString<U>() << std::endl
       << "}" << std::endl;
}


//Explicit declaration of known types.
template class map_param<int,bool>;
template class map_param<int,int>;
template class map_param<int,double>;
template class map_param<int,std::string>;
template class map_param<std::string,bool>;
template class map_param<std::string,int>;
template class map_param<std::string,double>;
template class map_param<std::string,std::string>;
template class map_param<double,double>;

} //namespace spify

