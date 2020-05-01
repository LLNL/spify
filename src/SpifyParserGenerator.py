#!/bin/env python

import re
from textwrap import wrap

class SpifyParserGenerator:
    "Make a spify parser from a list of parameters"

    def __init__(self):
        pass

    H_STRING = """
//spify Example:
//
//  See README and appExampleIFP.cpp for
//  details.
//
//  The structure of this file should not require
//  modification.  Only the name of the class.

#ifndef {IFPName}_H
#define {IFPName}_H

#include "spify/spify.h"

class {IFPName} : public spify::parser
{{
    public:
        {IFPName}(const std::string& fileName);
        ~{IFPName}(){{}};
    private:
        {IFPName}(); //disallow default constructor
    public:
{PARAM_ACCESS_FUNCS}
    private:
{PARSER_MEMBER_DATA}
}};

#endif
"""

    CPP_STRING = """
#include "{IFPName}.h"

//See README for details on implementing new parser.

{IFPName}::{IFPName}(const std::string& fileName)
: spify::parser(fileName)
{{
{CPP_CONSTRUCTOR}
}}

{CPP_MEMBER_ACCESS}
"""

#Convert type strings to cpp types
    def map_strtype_to_type(self,type):
        type_var_map_str = {
            'bool' : 'bool', 'int': 'int',
            'double': 'double', 'string': 'std::string',
            'v_bool' : 'std::vector<bool>',
            'v_int': 'std::vector<int>',
            'v_double': 'std::vector<double>',
            'v_string': 'std::vector<std::string>',
            'm_int_bool' : 'std::map<int,bool>',
            'm_int_int': 'std::map<int,int>',
            'm_int_double': 'std::map<int,double>',
            'm_int_string': 'std::map<int,std::string>',
            'm_string_bool' : 'std::map<std::string,bool>',
            'm_string_int': 'std::map<std::string,int>',
            'm_string_double': 'std::map<std::string,double>',
            'm_string_string': 'std::map<std::string,std::string>',
            'm_double_double': 'std::map<double,double>'
        }
        return type_var_map_str[type]


#Convert type strings to master file type descriptions
    def map_strtype_to_typestr(self,type):
        type_var_map_str = {
            'bool' : 'boolean', 'int': 'integer',
            'double': 'floating-point', 'string': 'string',
            'v_bool' : 'boolean vector',
            'v_int': 'integer vector',
            'v_double': 'floating-point vector',
            'v_string': 'string vector',
            'm_int_bool' : 'integer:boolean map',
            'm_int_int': 'integer:integer map',
            'm_int_double': 'integer:floating-point map',
            'm_int_string': 'integer:string map',
            'm_string_bool' : 'string:boolean map',
            'm_string_int': 'string:integer map',
            'm_string_double': 'string:floating-point map',
            'm_string_string': 'string:string map',
            'm_double_double': 'floating-point:floating-point map'
        }
        return type_var_map_str[type]

#Convert type strings to master file dummy examples
    def map_strtype_to_dummystr(self,type):
        type_var_map_str = {
            'bool' : 'y|n', 'int': 'I',
            'double': 'x.y', 'string': '<string>',
            'v_bool' : '[y|n,y|n,y|n]',
            'v_int': '[I,I,I]',
            'v_double': '[x.y,x.y,x.y]',
            'v_string': '[<string>,<string>,<string>]',
            'm_int_bool' : '{\nI:y|n,\nI:y|n,\nI:y|n\n}',
            'm_int_int': '{\nI:I,\nI:I,\nI:I\n}',
            'm_int_double': '{\nI:x.y,\nI:x.y,\nI:x.y\n}',
            'm_int_string': '{\nI:<string>,\nI:<string>,\nI:<string>\n}',
            'm_string_bool' : '{\n<string>:y|n,\n<string>:y|n,\n<string>:y|n\n}',
            'm_string_int': '{\n<string>:I,\n<string>:I,\n<string>:I\n}',
            'm_string_double': '{\n<string>:x.y,\n<string>:x.y,\n<string>:x.y\n}',
            'm_string_string': '{\n<string>:<string>,\n<string>:<string>,\n<string>:<string>\n}',
            'm_double_double': '{\nx.y:x.y,\nx.y:x.y,\nx.y:x.y\n}'
        }
        return type_var_map_str[type]

#Accessor functions for parameters in header.
    def make_p_access_funcs(self,params):
        p_access_funcs_str = ""
        for p in params:
            var_str = self.map_strtype_to_type(p['type'])        
            p_access_funcs_str += \
             "        const {type}& {name}() const;\n".format(type=var_str,name=p['name'])
        return p_access_funcs_str

#Member data in header
    def make_p_member_data(self,params):
        p_member_data_str = ""
        for p in params:
            var_str = self.map_strtype_to_type(p['type'])        
            p_member_data_str += \
             "        {type} m_{name};\n".format(type=var_str,name=p['name'])
        return p_member_data_str


    def write_h(self,name,params):
        p_access_funcs = self.make_p_access_funcs(params)
        p_member_data = self.make_p_member_data(params)
        with open('{class_name}.h'.format(class_name=name),'w') as hfile:
            hfile.write(self.H_STRING.format(IFPName=name,
                              PARAM_ACCESS_FUNCS=p_access_funcs,
                              PARSER_MEMBER_DATA=p_member_data))

#Need to add quotes around string values in parameter definitions
    def wrap_string(self,val,type):
        if type == "std::string":
            return "\""+val+"\""
        else:
            return val

    def make_scalar_param_cpp(self,param):
        base_str = """    addParameter
    (
        (new spify::scalar_param<{type}>("{name}")){options}  
    ); 
    """
        options_str = ""
        type_str = self.map_strtype_to_type(param['type'])
        for o in ['defaultValue','boundMax', 'boundMin']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}({ovalue})'.format(oname=o,ovalue=self.wrap_string(param[o],type_str))
        for o in ['shortDesc','longDesc']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}(\"{ovalue}\")'.format(oname=o,ovalue=re.sub("\n","\"\n\"",param[o]))
        if 'discreteValues' in param:
            dv = param['discreteValues']
            if dv != None:
                dv_str = "    std::vector<{type}> {name}_disv;\n".format(
                    type=type_str,name=param['name'])
                for v in dv:
                    dv_str += \
                    '    {name}_disv.push_back({dval});\n'.format(name=param['name'],dval=self.wrap_string(v,type_str))
                base_str = dv_str + base_str
                options_str +=  '\n        ->discreteValues({name}_disv)'.format(name=param['name'])
        return base_str.format(name=param['name'],type=type_str,options=options_str)


    def make_vector_param_cpp(self,param):
        base_str = """    addParameter
    (
        (new spify::vector_param<{type}>("{name}")){options}  
    ); 
    """
        type_str = self.map_strtype_to_type(param['type'].split('_')[1])
        options_str = ""
        for o in ['boundMax', 'boundMin']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}({ovalue})'.format(oname=o,ovalue=self.wrap_string(param[o],type_str))
        for o in ['shortDesc','longDesc']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}(\"{ovalue}\")'.format(oname=o,ovalue=re.sub("\n","\"\n\"",param[o]))
        if 'defaultValue' in param:
            dv = param['defaultValue']
            if dv != None:
                dv_str = "    {type} {name}_defv;\n".format(
                    type=self.map_strtype_to_type(param['type']),name=param['name'])
                for v in dv:
                    dv_str += \
                    '    {name}_defv.push_back({dval});\n'.format(name=param['name'],dval=self.wrap_string(v,type_str))
                base_str = dv_str + base_str
                options_str +=  '\n        ->defaultValue({name}_defv)'.format(name=param['name'])
        if 'discreteValues' in param:
            dv = param['discreteValues']
            if dv != None:
                dv_str = "    {type} {name}_disv;\n".format(
                    type=self.map_strtype_to_type(param['type']),name=param['name'])
                for v in dv:
                    dv_str += \
                    '    {name}_disv.push_back({dval});\n'.format(name=param['name'],dval=self.wrap_string(v,type_str))
                base_str = dv_str + base_str
                options_str +=  '\n        ->discreteValues({name}_disv)'.format(name=param['name'])
        return base_str.format(name=param['name'],type=type_str,options=options_str)


    def make_map_param_cpp(self,param):
        base_str = """    addParameter
    (
        (new spify::map_param<{type1},{type2}>("{name}")){options}  
    ); 
"""
        [type1_str,type2_str] = [self.map_strtype_to_type(x) for x in param['type'].split('_')[1:3]]
        options_str = ""
        for o in ['boundMaxFirst', 'boundMinFirst']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}({ovalue})'.format(oname=o,ovalue=self.wrap_string(param[o],type1_str))
        for o in ['boundMaxSecond','boundMinSecond']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}({ovalue})'.format(oname=o,ovalue=self.wrap_string(param[o],type2_str))
        for o in ['shortDesc','longDesc']:
            if o in param and param[o] != None:
                options_str += '\n        ->{oname}(\"{ovalue}\")'.format(oname=o,ovalue=re.sub("\n","\"\n\"",param[o]))
        if 'defaultValue' in param:
            dv = param['defaultValue']
            if dv != None:
                dv_str = "    std::map<{type1},{type2}> {name}_defv;\n".format(
                    type1=type1_str,type2=type2_str,name=param['name'])
                for k,v in dv.items():
                    dv_str += \
                    '    {name}_defv[{key}] = {val};\n'.format(name=param['name'],key=self.wrap_string(k,type1_str),val=self.wrap_string(v,type2_str))
                base_str = dv_str + base_str
                options_str +=  '\n        ->defaultValue({name}_defv)'.format(name=param['name'])
        if 'discreteValuesFirst' in param:
            dv = param['discreteValuesFirst']
            if dv != None:
                dv_str = "    std::vector<{type}> {name}_disv1;\n".format(
                    type=type1_str,name=param['name'])
                for v in dv:
                    dv_str += \
                    '    {name}_disv1.push_back({dval});\n'.format(name=param['name'],dval=self.wrap_string(v,type1_str))
                base_str = dv_str + base_str
                options_str +=  '\n        ->discreteValuesFirst({name}_disv1)'.format(name=param['name'])
        if 'discreteValuesSecond' in param:
            dv = param['discreteValuesSecond']
            if dv != None:
                dv_str = "    std::vector<{type2}> {name}_disv2;\n".format(
                    type2=type2_str,name=param['name'])
                for v in dv:
                    dv_str += \
                    '    {name}_disv2.push_back({dval});\n'.format(name=param['name'],dval=self.wrap_string(v,type2_str))
                base_str = dv_str + base_str
                options_str +=  '\n        ->discreteValuesSecond({name}_disv2)'.format(name=param['name'])
        return base_str.format(name=param['name'],type1=type1_str,type2=type2_str,options=options_str)


    def make_param_cpp(self,param):
        if param['type'].startswith('v_'):
            return self.make_vector_param_cpp(param)
        elif param['type'].startswith('m_'):
            return self.make_map_param_cpp(param)
        else:
            return self.make_scalar_param_cpp(param)

    def make_constructor_assignment(self,param):
        base_str = "    get(\"{name}\",m_{name});\n"
        return base_str.format(name=param["name"])

    def make_param_cpp_access(self,name,param):
        base_str = """const {type}& {class_name}::{var_name}() const
{{
    if(!m_parametersValidated)
    {{
       throw(std::logic_error("{class_name}: attempted to access parameter "
                             "\\"{var_name}\\" before validation."));
    }}
    return m_{var_name};
}}
"""
        return base_str.format(type=self.map_strtype_to_type(param['type']),class_name=name,
                               var_name=param['name'])


    def make_cpp_constructor(self,params):
        cpp_constructor_str = ""
        for p in params:
            cpp_constructor_str += self.make_param_cpp(p) + "\n"
        cpp_constructor_str += "    validateParameters();\n\n"
        cpp_constructor_str += "    //Assign values to member data.\n"
        for p in params:
            cpp_constructor_str += self.make_constructor_assignment(p)
        return cpp_constructor_str


    def make_cpp_member_access(self,name,params):
        cpp_member_access_str = ""
        for p in params:
            cpp_member_access_str += self.make_param_cpp_access(name,p) + "\n"
        return cpp_member_access_str


    def write_cpp(self,name,params):
        with open('{class_name}.cpp'.format(class_name=name),'w') as cppfile:
            cppfile.write( self.CPP_STRING.format(IFPName=name,
                           CPP_CONSTRUCTOR=self.make_cpp_constructor(params),
                           CPP_MEMBER_ACCESS=self.make_cpp_member_access(name,params)) )


    def validate_param(self,param):
        valid_param_types = [
        'bool', 'int', 'double', 'string',
        'v_bool', 'v_int', 'v_double', 'v_string',
        'm_int_bool', 'm_int_int', 'm_int_double', 'm_int_string',
        'm_string_bool', 'm_string_int', 'm_string_double', 'm_string_string',
        'm_double_double' ]
        if not param['name']:
            return False
        if param['type'].lower() not in valid_param_types:
            return False
        return True


    def validate_params(self,params):
        for p in params:
            success = self.validate_param(p)
            if not success:
                print("Parameter: "+ str(p) + " not valid.")
                return False
        return True

#Make header and cpp
    def generate(self,name,params):
        valid = self.validate_params(params)
        if not valid:
            exit(1)
        print("Parameters OK")
        self.write_h(name,params)
        self.write_cpp(name,params)

#Generate parameter string for master file
    def pretty_print(self,param):
        out_str = ""
        o = "shortDesc"
        if o in param and param[o] != None:
             out_str += "#{ovalue}\n".format(ovalue=re.sub("\n","\n#",param[o]))
        o = "longDesc"
        if o in param and param[o] != None:
             long_desc = "#{ovalue}".format(ovalue=re.sub("\n"," ",param[o])) 
             out_str += "\n#".join(wrap(long_desc,59))
             out_str += "\n"
        out_str += "#Type: {type}\n".format(type=self.map_strtype_to_typestr(param["type"]))
        if param["type"].startswith("m_"):
            o = "boundMinFirst"
            if o in param and param[o] != None:
                 out_str += "#First values must be greater than or equal to {0}\n".format(param[o])
            o = "boundMaxFirst"
            if o in param and param[o] != None:
                 out_str += "#First values must be less than or equal to {0}\n".format(param[o])
            o = "discreteValuesFirst"
            if o in param and param[o] != None:
                 out_str += "#First values must be one of "
                 out_str += re.sub("'","",str(param[o]))
                 out_str += "\n"
            o = "boundMinSecond"
            if o in param and param[o] != None:
                 out_str += "#Second values must be greater than or equal to {0}\n".format(param[o])
            o = "boundMaxSecond"
            if o in param and param[o] != None:
                 out_str += "#Second values must be less than or equal to {0}\n".format(param[o])
            o = "discreteValuesSecond"
            if o in param and param[o] != None:
                 out_str += "#Second values must be one of "
                 out_str += re.sub("'","",str(param[o]))
                 out_str += "\n"
            o = "defaultValue"
            if o in param and param[o] != None:
                 def_val_str = re.sub("'","",str(param[o]))
                 def_val_str = re.sub("{","\n#{\n#",str(def_val_str))
                 def_val_str = re.sub(",\ *",",\n#",str(def_val_str))
                 def_val_str = re.sub("}","\n#}",str(def_val_str))
                 out_str += "#Optional with default value of: "
                 out_str+= "{0}\n".format(def_val_str)
        elif param["type"].startswith("v_"):
            o = "boundMin"
            if o in param and param[o] != None:
                 out_str += "#Values must be greater than or equal to {0}\n".format(param[o])
            o = "boundMax"
            if o in param and param[o] != None:
                 out_str += "#Values must be less than or equal to {0}\n".format(param[o])
            o = "discreteValues"
            if o in param and param[o] != None:
                 out_str += "#Values must be one of "
                 out_str += re.sub("'","",str(param[o]))
                 out_str += "\n"
            o = "defaultValue"
            if o in param and param[o] != None:
                 def_val_str = re.sub("'","",str(param[o]))
                 out_str += "#Optional with default value of: "
                 out_str+= "{0}\n".format(def_val_str)
        else:
            o = "boundMin"
            if o in param and param[o] != None:
                 out_str += "#Must be greater than or equal to {0}\n".format(param[o])
            o = "boundMax"
            if o in param and param[o] != None:
                 out_str += "#Must be less than or equal to {0}\n".format(param[o])
            o = "discreteValues"
            if o in param and param[o] != None:
                 out_str += "#Must be one of "
                 out_str += re.sub("'","",str(param[o]))
                 out_str += "\n"
            o = "defaultValue"
            if o in param and param[o] != None:
                 def_val_str = re.sub("'","",str(param[o]))
                 out_str += "#Optional with default value of: "
                 out_str+= "{0}\n".format(def_val_str)
        out_str += "{name}: {dummyvalue}\n".format(name=param["name"],dummyvalue=self.map_strtype_to_dummystr(param["type"]))
        out_str += "\n"
        return out_str


    def make_master_file(self,name,params):
        master_file_name="{name}Master.yml".format(name=name)
        print("Generating master file as {0} ...".format(master_file_name))
        with open(master_file_name,'w') as master_file:
            for p in params:
                master_file.write(self.pretty_print(p))



if __name__ == "__main__":
    from spify_parser_gen import spify_parser_name,spify_parser_params
    spg = SpifyParserGenerator()
    spg.generate(spify_parser_name,spify_parser_params)
    spg.make_master_file(spify_parser_name,spify_parser_params)

