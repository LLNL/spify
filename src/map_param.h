#ifndef SPIFY_MAP_PARAM_H_0443192E8C7D4D7D8DAB2346CFC4003A
#define SPIFY_MAP_PARAM_H_0443192E8C7D4D7D8DAB2346CFC4003A

#include "spify/param.h"

namespace spify
{

template <class T, class U>
class map_param : public param
{
    public:
        explicit map_param(const std::string& key);
        ~map_param(){};

        //Named Parameter Idiom
        map_param<T,U>* shortDesc(const std::string& shortDesc);
        map_param<T,U>* longDesc(const std::string& longDesc);

        map_param<T,U>* defaultValue(const std::map<T,U>& value);
        map_param<T,U>* boundMinFirst(const T& value);
        map_param<T,U>* boundMinSecond(const U& value);
        map_param<T,U>* boundMaxFirst(const T& value);
        map_param<T,U>* boundMaxSecond(const U& value);
        map_param<T,U>* discreteValuesFirst(const std::vector<T>& value);
        map_param<T,U>* discreteValuesSecond(const std::vector<U>& value);

    private:
        bool m_defaultDefined;
        bool m_boundedMinFirst;
        bool m_boundedMinSecond;
        bool m_boundedMaxFirst;
        bool m_boundedMaxSecond;
        std::map<T,U> m_defaultValue;
        T m_minBoundFirst;
        T m_maxBoundFirst;
        U m_minBoundSecond;
        U m_maxBoundSecond;
        std::vector<T> m_discreteValuesFirst;
        std::vector<U> m_discreteValuesSecond;

        void checkConstraints(const std::pair<T,U>& values);

    protected:
        //check for existence and validity of parameter in input file 
        //throw exception if invalid 
        void validate(YAML::Node &inputFile);
        void print(std::ostream& os);
        const std::type_info& typeInfo(){return typeid(std::map<T,U>);};
        const std::string& fullTypeString()
            {return typeString<std::map<T,U> >();};
};

} //namespace spify

#endif
