#include "spify/param.h"

namespace spify
{

param::param
(
    const std::string& key
)
{
    m_key = key;
}

void param::print(std::ostream& os)
{
    if(m_shortDesc.size())
    {
        os << "#" << m_shortDesc << std::endl;
    }
    if(m_longDesc.size())
    {
        //Modified Stackoverflow implementation
        //void wrap(const std::string& input, size_t width, std::ostream &os,
        //          size_t indent = 0, const std::string& prefix = "")
        size_t width = 60;
        size_t indent = 0;
        std::string prefix = "#";
        size_t prefix_len = prefix.size();

        std::istringstream in(m_longDesc);

        os << prefix;
        os << std::string(indent, ' '); 
        size_t current = indent;
        std::string word;

        while (in >> word) {
            if (current + word.size() > width) {
                os << std::endl << prefix << std::string(indent, ' ');
                current = indent + prefix_len;
            }
            os << word << ' ';
            current += word.size() + 1;
        }
        os << std::endl;
    }
    os << "#Type: " << fullTypeString() << std::endl;
}

//Default versions
template<class T>
const std::string& param::typeString()
{
    const static std::string s = "unknown type";
    return s;
}

template<class T>
const std::string& param::dummyString()
{
    const static std::string s = "foo";
    return s;
}

//Macro code generator
#define DEFINE_IFPPARAMETER_STRINGS(T,typeStr,dummyStr) \
    template<> const std::string& param::typeString<T>()\
    { \
        const static std::string s = typeStr; \
        return s; \
    } \
    template<> const std::string& param::dummyString<T>()\
    { \
        const static std::string s = dummyStr; \
        return s; \
    }

//Scalar Types
DEFINE_IFPPARAMETER_STRINGS(bool,"boolean","y|n");
DEFINE_IFPPARAMETER_STRINGS(int,"integer","I");
DEFINE_IFPPARAMETER_STRINGS(double,"floating-point","x.y");
DEFINE_IFPPARAMETER_STRINGS(std::string,"string","<string>");

//Vector Types
DEFINE_IFPPARAMETER_STRINGS(std::vector<bool>,"boolean vector","[y|n,y|n,y|n]");
DEFINE_IFPPARAMETER_STRINGS(std::vector<int>,"integer vector","[I,I,I]");
DEFINE_IFPPARAMETER_STRINGS(std::vector<double>,"floating-point vector",
                            "[x.y,x.y,x.y]");
DEFINE_IFPPARAMETER_STRINGS(std::vector<std::string>,"string vector",
                            "[<string>,<string>,<string>]");

//UGLY fix for macro-template combination.
#define COMMA() ,

//Map Types
DEFINE_IFPPARAMETER_STRINGS(std::map<int COMMA() bool>,"integer:boolean map",
                            "{I:y|n,I:y|n,I:y|n}");
DEFINE_IFPPARAMETER_STRINGS(std::map<int COMMA() int>,"integer:integer map",
                            "{I:I,I:I,I:I}");
DEFINE_IFPPARAMETER_STRINGS(std::map<int COMMA() double>,
                            "integer:floating-point map","{I:x.y,I:x.y,I:x.y}");
DEFINE_IFPPARAMETER_STRINGS(std::map<int COMMA() std::string>,
                            "integer:string map",
                            "{I:<string>,I:<string>,I:<string>}");

DEFINE_IFPPARAMETER_STRINGS(std::map<std::string COMMA() bool>,
                            "string:boolean map",
                            "{<string>:y|n,<string>:y|n,<string>:y|n}");
DEFINE_IFPPARAMETER_STRINGS(std::map<std::string COMMA() int>,
                            "string:integer map",
                            "{<string>:I,<string>:I,<string>:I}");
DEFINE_IFPPARAMETER_STRINGS(std::map<std::string COMMA() double>,
                            "string:floating-point map",
                            "{<string>:x.y,<string>:x.y,<string>:x.y}");
DEFINE_IFPPARAMETER_STRINGS(std::map<std::string COMMA() std::string>,
                            "string:string map",
                            "{<string>:<string>,<string>:<string>,...");

DEFINE_IFPPARAMETER_STRINGS(std::map<double COMMA() double>,
                            "floating-point:floating-point map",
                            "{x.y:x.y,x.y:x.y,x.y:x.y}");

//Cleanup macro defs
#undef COMMA
#undef DEFINE_IFPPARAMATER_STRINGS

} //namespace spify

