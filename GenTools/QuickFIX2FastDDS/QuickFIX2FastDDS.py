"""
   Copyright (C) 2021 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over OpenDDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

"""

import sys
import getopt
import RequiredFieldsComponents
import ComponentParser
import MessageParser

import xml.etree.ElementTree as xml_parser

DONT_MODIFY_TEXT = "/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/\n"

def print_help():
    print('QuickFIX2FastDDS.py converts specified in requirements file messages, groups and fields from FIX specs to '
          'OpenDDS IDL libraries and conversion helper classes')
    print('QuickFIX2FastDDS.py -r <requirements> -s <fix_specs>')
    print('try this : python QuickFIX2FastDDS.py -r ../spec/dats_requirements.xml -s ../../FIXGateway/spec/FIX44.xml')

def main(argv):
    requirements = ''
    fix_specs = ''

    try:
        opts, args = getopt.getopt(argv, "hr:s:", ["rfile=", "sfile="])
    except getopt.GetoptError:
        print_help()
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print_help()
            sys.exit()
        elif opt in ("-r", "--rfile"):
            requirements = arg
        elif opt in ("-s", "--sfile"):
            fix_specs = arg

    if requirements == "" or fix_specs == "":
        print_help()
        exit(1)

    print('Requirements file is :', requirements)
    print('FIX Specs file is :', fix_specs)

    generate( requirements, fix_specs)

def generate( requirements, fix_spec):
    requiredFieldsComponents = RequiredFieldsComponents.RequiredFieldsComponents(requirements)
    fix_specs = xml_parser.parse(fix_spec)

    component_parser = ComponentParser.ComponentParser(requiredFieldsComponents)
    message_parser = MessageParser.MessageParser(requiredFieldsComponents, component_parser)

    header_list_comp = []
    header_messages = dict()
    required_header_fields = []

    print("----- Processing Required Fields for QuickFIX Components -----")
    component_parser.parse_components(fix_specs)
    print("--------------------------------------------------------------\n")

    print("----- Processing Required Fields for QuickFIX Header -----")
    header_out = component_parser.parse_components_rec('Header', fix_specs.find('header'),header_messages, header_list_comp, required_header_fields, False)
    component_dependency_dictionary = component_parser.component_dep_dict # this list is required to populate dependecies for MPC to work
    print("--------------------------------------------------------------\n")

    print("----- Processing Required Fields for QuickFIX Message -----")
    message_parser.parse_messages(fix_specs, component_dependency_dictionary)
    print("--------------------------------------------------------------\n")

    component_parser.compenent_dict["Header"] = header_out
    component_parser.complete_dependency_list.append("Header")
    component_parser.component_dep_dict["Header"] = []

    message_parser.store_message()
    component_parser.store_components()
#/Users/mkipnis/Fast-DDS/Fast-DDS-Gen/scripts/fastddsgen *.idl -typeobject
if __name__ == "__main__":
   main(sys.argv[1:])
