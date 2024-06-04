#!/bin/env python

import os,sys

#Root of spify src directory
SPIFY_SRC_DIR = os.path.join(os.getcwd(),'..')

#Name your parser
spify_parser_name = "ExampleIFP"

spify_parser_params = []

#Specify parameters
spify_parser_params.append(
{
    'name':'boolOption',
    'type':'bool',
    'shortDesc':'My Boolean Option',
    'defaultValue': 0
}
)

spify_parser_params.append(
{
    'name':'intOption',
    'type':'int',
    'longDesc':
"""Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor
incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis
nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu
fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in
culpa qui officia deserunt mollit anim id est laborum.""",
    'discreteValues': [0,1,3,5,7]
}
)

spify_parser_params.append(
{
    'name':'intVectOption',
    'type':'v_int',
    'shortDesc':'My Integer Vector Option.',
    'defaultValue': [-3,-2,-1,0],
    'boundMin': -10,
    'boundMax': 10
}
)

spify_parser_params.append(
{
    'name':'floatOption',
    'type':'double',
    'defaultValue': 300.0,
    'boundMax': 300.0
}
)

spify_parser_params.append(
{
    'name':'floatVectOption',
    'type':'v_double',
    'boundMin': 0.0,
    'boundMax': 100.0
}
)



spify_parser_params.append(
{
    'name':'stringOption',
    'type':'string',
    'defaultValue': "foo",
    'discreteValues': ["foo","bar","baz"]
}
)


spify_parser_params.append(
{
    'name':'stringVectOption',
    'type':'v_string',
    'shortDesc': "My String Vector Option"
}
)

spify_parser_params.append(
{
    'name':'intIntMapOption',
    'type':'m_int_int',
    'shortDesc': "My Integer-Integer Map Option",
    'discreteValuesFirst': [1,2,4,6]
}
)

spify_parser_params.append(
{
    'name':'stringIntMapOption',
    'type':'m_string_int',
    'defaultValue': {"foo":1,"bar":2}
}
)

spify_parser_params.append(
{
    'name':'stringStringMapOption',
    'type':'m_string_string',
    'discreteValuesFirst': ["foo","bar","baz"],
    'discreteValuesSecond': ["parrot","buzzard","eagle"]
}
)

spify_parser_params.append(
{
    'name':'floatFloatMapOption',
    'type':'m_double_double',
    'defaultValue': {1.0:10.0, 2.0:20.0, 3.0:30.0}
}
)

#Make sure we can import SpifyParserGenerator
sys.path.append(os.path.join(SPIFY_SRC_DIR,'src'))

#Import
from SpifyParserGenerator import SpifyParserGenerator as spg

#Generate parser code
spg().generate(spify_parser_name,spify_parser_params)
#Generate master file
spg().make_master_file(spify_parser_name,spify_parser_params)

#Done.
