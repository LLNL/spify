#ifndef SPIFY_VECTOR_PARAM_H_CDE29AE8B5544BBA8BA9386D6FF61D1C
#define SPIFY_VECTOR_PARAM_H_CDE29AE8B5544BBA8BA9386D6FF61D1C

#include "spify/param.h"

namespace spify
{

template <class T>
class vector_param : public param
{
    public:
        explicit vector_param(const std::string& key);
        ~vector_param(){};

        //Named Parameter Idiom
        vector_param<T>* shortDesc(const std::string& shortDesc);
        vector_param<T>* longDesc(const std::string& longDesc);

        vector_param<T>* defaultValue(const std::vector<T>& value);
        vector_param<T>* boundMin(const T& value);
        vector_param<T>* boundMax(const T& value);
        vector_param<T>* discreteValues(const std::vector<T>& value);

    private:
        bool m_defaultDefined;
        bool m_boundedMin;
        bool m_boundedMax;
        std::vector<T> m_defaultValue;
        T m_minBound;
        T m_maxBound;
        std::vector<T> m_discreteValues;

        void checkConstraints(const T& values);

    protected:
        //check for existence and validity of parameter in input file 
        //throw exception if invalid 
        void validate(YAML::Node &inputFile);
        void print(std::ostream& os);
        const std::string& fullTypeString()
            {return typeString<std::vector<T> >();};
        const std::type_info& typeInfo(){return typeid(std::vector<T>);};
};

} //namespace spify

#endif
