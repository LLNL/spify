
#include <iostream>
#include "spify/vector_param.h"

namespace spify
{

template <class T>
vector_param<T>::vector_param
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
    m_boundedMin = false;
    m_boundedMax = false;
    m_discreteValues = std::vector<T>();
}

template <class T>
inline vector_param<T>* vector_param<T>::shortDesc(const std::string& shortDesc)
{
    m_shortDesc = shortDesc;
    return this;
}

template <class T>
inline vector_param<T>* vector_param<T>::longDesc(const std::string& longDesc)
{
    m_longDesc = longDesc;
    return this;
}

template <class T>
inline vector_param<T>* vector_param<T>::defaultValue(const std::vector<T>& value)
{
    m_defaultValue = value;
    m_defaultDefined = true;
    return this;
}

template <class T>
inline vector_param<T>* vector_param<T>::boundMin(const T& value)
{
    m_minBound = value;
    m_boundedMin = true;
    return this;
}

template <class T>
inline vector_param<T>* vector_param<T>::boundMax(const T& value)
{
    m_maxBound = value;
    m_boundedMax = true;
    return this;
}

template <class T>
inline vector_param<T>* vector_param<T>::discreteValues(
    const std::vector<T>& values)
{
    if(!values.size())
    {
        throw(std::logic_error("vector_param: Attempted to constrain discrete"
                               " values with empty array."));
    }
    m_discreteValues = values;
    return this;
}


template <class T>
void vector_param<T>::validate(YAML::Node &inputFile)
{
    std::vector<T> values;
    bool foundVal=false;
    if(inputFile[m_key])
    {
        try
        { 
            values = inputFile[m_key].template as<std::vector<T> >();
            foundVal = true;
        }
        catch (const YAML::Exception &e)
        {
            if(m_defaultDefined)
            {
                std::cout << "Warning: failed to parse optional input value \""
                          << m_key << "\"" << std::endl
                          << "Using default value of: " <<  std::endl
                          << "[";
                if(m_defaultValue.size())
                {
                    for(int j = 0; j < m_defaultValue.size()-1; ++j)
                    {
                        std::cout << m_defaultValue[j] << ", ";
                    }
                    std::cout << m_defaultValue[m_defaultValue.size()-1] << "]"
                              << std::endl;
                }
                else
                {
                    std::cout << "]" << std::endl;
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
        for(int j = 0; j < values.size(); ++j)
        {
            checkConstraints(values[j]);
        }
    }
    else
    {
        std::cerr << "Error: didn't find required input value \"" << m_key
                  << "\"" << " (" << fullTypeString() << ")." << std::endl;
        throw(std::runtime_error("value not found."));
    } 
}


template<class T>
void vector_param<T>::checkConstraints(const T &value)
{
    if (m_boundedMin && value < m_minBound)
    {
        std::cerr << "Value provided for parameter \"" << m_key << "\" ("
                  << value << ") "
                  << " is below minimum bound (" << m_minBound << ")."
                  << std::endl;
        throw(std::runtime_error("Invalid value."));
    }
    if (m_boundedMax && value > m_maxBound)
    {
        std::cerr << "Value provided for parameter \"" << m_key << "\" ("
                  << value << ") "
                  << " is above maximum bound (" << m_maxBound << ")."
                  << std::endl;
        throw(std::runtime_error("Invalid value."));
    }
    if(m_discreteValues.size())
    {
        bool match = false;
        for(int j = 0; j < m_discreteValues.size(); ++j)
        {
            if(m_discreteValues[j] == value)
            {
                match = true;
                break;
            }
        }
        if(!match)
        {
            std::cerr << "Value provided for parameter \"" << m_key << "\" ("
                      << value << ") "
                      << "is not one of acceptable values [";
            for(int j = 0; j < m_discreteValues.size()-1; ++j)
            {
                std:: cerr << m_discreteValues[j] << ",";
            }
            std::cerr << m_discreteValues[m_discreteValues.size()-1] << "]."
                      << std::endl;
            throw(std::runtime_error("Invalid value."));
        }
    }
}


template<class T>
void vector_param<T>::print(std::ostream& os)
{
    param::print(os);
    if(m_boundedMin)
    {
        os << "#Must be greater than " << m_minBound << std::endl;
    }
    if(m_boundedMax)
    {
        os << "#Must be less than " << m_maxBound << std::endl;
    }
    if(m_discreteValues.size())
    {
        os << "#Must be one of [";
        for(int j = 0; j < m_discreteValues.size()-1; ++j)
        {
            os << m_discreteValues[j] << ", ";
        }
        os << m_discreteValues[m_discreteValues.size()-1] << "]" << std::endl;
    }
    if(m_defaultDefined)
    {
        os << "#Optional with default value of [";
        if(m_defaultValue.size())
        {
            for(int j = 0; j < m_defaultValue.size()-1; ++j)
            {
                os << m_defaultValue[j] << ", ";
            }
            os << m_defaultValue[m_defaultValue.size()-1] << "]" << std::endl;
        }
        else
        {
            os << "]" << std::endl;
        }
    }
    os << m_key << ": [" << dummyString<T>() << "," << dummyString<T>() << ","
       << dummyString<T>() << "]" << std::endl;
}

//Explicit declaration of known types.
template class vector_param<bool>;
template class vector_param<int>;
template class vector_param<double>;
template class vector_param<std::string>;

} //namespace spify
