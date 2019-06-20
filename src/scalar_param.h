#ifndef SPIFY_SCALAR_PARAM_H_F2C553320BA540FEA6BC1082EA69128A
#define SPIFY_SCALAR_PARAM_H_F2C553320BA540FEA6BC1082EA69128A

#include "spify/param.h"

namespace spify
{

template <class T>
class scalar_param : public param
{
    public:
        explicit scalar_param(const std::string& key);
        ~scalar_param(){};

        //Named Parameter Idiom
        scalar_param<T>* shortDesc(const std::string& shortDesc);
        scalar_param<T>* longDesc(const std::string& longDesc);

        scalar_param<T>* defaultValue(const T& value);
        scalar_param<T>* boundMin(const T& value);
        scalar_param<T>* boundMax(const T& value);
        scalar_param<T>* discreteValues(const std::vector<T>& value);

    private:
        bool m_defaultDefined;
        bool m_boundedMin;
        bool m_boundedMax;
        T m_defaultValue;
        T m_minBound;
        T m_maxBound;
        std::vector<T> m_discreteValues;

        void checkConstraints(const T& value);

    protected:
        //check for existence and validity of parameter in input file 
        //throw exception if invalid 
        void validate(YAML::Node &inputFile);
        void print(std::ostream& os);
        const std::string& fullTypeString(){return typeString<T>();};
        const std::type_info& typeInfo(){return typeid(T);};
};

} // namespace spify

#endif
