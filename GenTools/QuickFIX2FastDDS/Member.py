"""
   Copyright (C) 2021 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
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

class Member:

    def __init__(self, type, name, fixObj, ddsObj, gen_requirements ):

        self.type = type
        self.name = name
        self.fixObj = fixObj
        self.ddsObj = ddsObj
        #if self.ddsObj == "ddsMsg.c_NoMDEntries[NoMDEntries_group_cnt_index]":
        #    print("->>>" + self.ddsObj)
        self.gen_requirements = gen_requirements

    def get_idl_statement(self):
        return "\t\t" + self.type.get("dds") + " " + self.name + ";\n"

    def get_logger_statement(self, entity_name):
        return " << ""\"ddsMsg." + self.name + " : \" << ddsMsg." + self.name + "() << std::endl"

    def get_cpp_member_statement(self, member_name):
        fix2dds_cpp_def = ""
        dds2fix_cpp_def = ""

        var_name = "fix" + member_name

        to_dds = self.type.get("to_dds")
        fix2dds_cpp_def += "\tif (" + self.fixObj + ".isSetField(FIX::FIELD::" + member_name + ") )\n"
        if to_dds is not None:
            formatted_statement = (to_dds % (member_name, self.fixObj, member_name))
            fix2dds_cpp_def += "\t\t"+self.ddsObj+"." + member_name + " ( " + formatted_statement + ");\n"
        else:
            fix2dds_cpp_def += "\t\t"+self.ddsObj+"." + member_name + " ( FIELD_GET_REF( " + self.fixObj + "," + member_name + "));\n"

        if self.type.attrib['dds'] == 'float' or self.type.attrib['dds'] == 'long' or self.type.attrib['dds'] == 'unsigned long long':
            fix2dds_cpp_def += '\telse \n'
            fix2dds_cpp_def += "\t\t"+self.ddsObj+"." + member_name + " ( 0 );\n\n"
        else:
            fix2dds_cpp_def += "\n"

        to_fix = self.type.get("to_fix")
        if to_fix is not None:
          #to_fix_type=self.type.get("to_fix_type");
          formatted_statement = (to_fix % ( self.ddsObj, member_name,  member_name,  self.fixObj ) );
          dds2fix_cpp_def += "\t" + formatted_statement+"\n\n";
        else:
            dds2fix_cpp_def += "\tFIX::" + member_name + " " + var_name + "("+self.ddsObj+"."+ member_name+"());\n";
            dds2fix_cpp_def += "\t"+self.fixObj+".setField("+var_name+");\n\n";

        return fix2dds_cpp_def, dds2fix_cpp_def;

    def get_cpp_declare_statement(self):

        fix2dds_cpp_def = ""
        dds2fix_cpp_def = ""

        if self.gen_requirements is None or self.gen_requirements.get("gen_cpp") == 'Y':
            fix2dds_member_statement_cpp, dds2fix_member_statement_cpp = self.get_cpp_member_statement(self.name)
            fix2dds_cpp_def += fix2dds_member_statement_cpp
            dds2fix_cpp_def += dds2fix_member_statement_cpp

        return fix2dds_cpp_def, dds2fix_cpp_def



class GroupMember(Member):

    def __init__(self, type, name, parent ):
        self.type = type
        self.name = name
        self.parent = parent

    def add_group_components(self, component_item, component_parser, field_types):
        #print("Group Component :" + component_item.get("name"));

        component_name = component_item.get("name");

        self.group_components = component_item
        self.component_parser = component_parser
        self.field_types = field_types

    def get_idl_statement(self):
        idl_def = "\t\t" + self.name + "Seq c_" + self.name + ";\n"
        return idl_def

    def get_logger_statement(self, entity_name):

        parent_component = self.component_parser.compenent_dict.get(self.parent)

        logger_def = "\n\t\t;out << \"ddsMsg.c_" + self.name + "\" << std::endl; \n\t \tout << \"{\" << std::endl;\n"
        for group_component in parent_component.groups:
            logger_def += "\n\t\tfor ( int tt = 0; tt < ddsMsg.c_" + self.name + "().size(); tt++)\n\t\t{\n"
            for member in group_component.members:
                logger_def += "\n\t\t\t ;out << \"" + "ddsMsg.c_" + self.name + "()[\" << tt << \"]." + \
                            member.name + " : " + "\" << ddsMsg.c_" + self.name + "()[tt]." + member.name + "() << std::endl;";
            logger_def += "\n\t\t};\n"

        logger_def += "\n\t\t;out << \"}\" << std::endl;"

        return logger_def


    def get_cpp_declare_statement(self):
        fix2dds_cpp_def = ""
        dds2fix_cpp_def = ""

        group_count_index_var =  self.type +"_group_cnt"+"_index"
        fix2dds_cpp_statement, dds2fix_cpp_statement = self.get_cpp_dds_to_fix(group_count_index_var)

        group_count_var = self.type +"_group_cnt"


        fix2dds_cpp_def += "// There is a group\n\n" + "\tFIX::" + self.type + "  " + group_count_var + " = " + "FIELD_GET_REF( fixMsg," + self.type + ");\n\n"
        fix2dds_cpp_def += "\tddsMsg.c_"+self.name+"().resize("+ group_count_var+ ".getValue());\n\n"
        fix2dds_cpp_def += "\tint "+ group_count_index_var +" = 0;\n\twhile ( "+ group_count_index_var +" < "+ group_count_var+".getValue())\n\t{\n"
        fix2dds_cpp_def += "\t\t" + self.get_fix_group_definition(self.group_components) + ";\n"
        fix2dds_cpp_def += "\t\tfixMsg.getGroup("+group_count_index_var+"+1, grp);\n"
        #fix2dds_cpp_def += "\t\tfixMsg.getGroup(ii, FIX::FIELD::" + self.type + ", grp);\n"
        fix2dds_cpp_def += fix2dds_cpp_statement
        fix2dds_cpp_def += "\t\t"+group_count_index_var+"++;\n"
        fix2dds_cpp_def += "\t};\n;"

        dds2fix_cpp_def += "\tfor ( long "+ group_count_index_var +" = 0; "+ group_count_index_var +"<ddsMsg.c_" + self.name + "().size();"+group_count_index_var+"++)\n\t{\n";
        dds2fix_cpp_def += "\t\t" + self.get_fix_group_definition(self.group_components) + ";\n"
        dds2fix_cpp_def += dds2fix_cpp_statement;
        #dds2fix_cpp_def += "\t\nfixMsg.addGroup(FIX::FIELD::"+self.type+",grp);\n";
        dds2fix_cpp_def += "\t\n\t};"

        return fix2dds_cpp_def, dds2fix_cpp_def

    def get_cpp_dds_to_fix(self, index_var):

        fix2dds_cpp_def = ""
        dds2fix_cpp_def = ""

        for group_component in self.group_components:
            if group_component.tag == "field":
                if not self.component_parser.is_required(group_component):

                    if self.name not in self.component_parser.requiredFieldsComponents.required_fields:
                        continue

                    required_fields = self.component_parser.requiredFieldsComponents.components_required_fields_components[self.name];
                    if not required_fields:
                        print("Warning : No fields in [" + self.name + "]")
                    else:
                        field_name = group_component.get("name")
                        try:
                            if field_name not in required_fields.get("field"):
                                continue
                        except:
                            print("Exception no fields " + self.name)

                field_name = group_component.get("name")
                field_type = self.field_types.get(field_name)

                gen_requiements = None

                if field_type in self.component_parser.requiredFieldsComponents.types.keys(): #and self.name in  self.component_parser.requiredFieldsComponents.required_components.keys():
                    type = self.component_parser.requiredFieldsComponents.types[field_type]
                    #gen_requiements = self.component_parser.requiredFieldsComponents.required_components[self.name];
                    member = Member(type, field_name, "grp", "ddsMsg.c_"+self.name+"()["+index_var+"]", gen_requiements)
                    fix2dds_cpp_statement, dds2fix_cpp_statement = member.get_cpp_declare_statement()
                    fix2dds_cpp_def += fix2dds_cpp_statement
                    dds2fix_cpp_def += dds2fix_cpp_statement

            elif group_component.tag == "component":

                sub_component_name = group_component.get("name")

                gen_requiements = None
                if sub_component_name in self.component_parser.compenent_dict.keys() and sub_component_name in self.component_parser.requiredFieldsComponents.required_components.keys():
                    sub_components = self.component_parser.compenent_dict[sub_component_name]

                    for component_member in sub_components.members:
                        field_name = component_member.name
                        field_type = component_member.type
                        #if component_member.name in self.component_parser.requiredFieldsComponents.required_components.keys():
                            #gen_requiements = self.component_parser.requiredFieldsComponents.required_components[component_member.name];
                        member = Member(field_type, field_name, "grp", "ddsMsg.c_" + self.name + "()[" + index_var + "]", gen_requiements);
                        fix2dds_cpp_statement, dds2fix_cpp_statement = member.get_cpp_declare_statement()
                        fix2dds_cpp_def += fix2dds_cpp_statement
                        dds2fix_cpp_def += dds2fix_cpp_statement

            else:
                print("TODO: Add nested groups handler - [" + group_component.tag + "-" + group_component.get("name") + "] - " + self.name);

        return fix2dds_cpp_def, dds2fix_cpp_def

    def get_fix_field(self, group_component):

        if group_component.tag == "component":
            #print("TODO: Deal with nested repeating groups");

            # I will hard-code for now
            if group_component.get("name") == "Instrument":
                return "FIX::FIELD::Symbol"
            else:
                return "GenSrc - Investigate"; # To Fix, identify required fields in a given component, select tag of the first field

        elif group_component.tag == "field":
            return "FIX::FIELD::" + group_component.get("name")
        else:
            print("Undefine group element : " + group_component.get("name") + " : " + group_component.tag)
        return ""

    def get_fix_group_definition(self, group_components ):

        group_definition = "FIX::Group grp(FIX::FIELD::"+self.type+"," + self.get_fix_field(group_components[0])+",FIX::message_order("
        for group_component in group_components:
            if group_component.tag == "field":
                group_definition += self.get_fix_field(group_component) + ",";

        group_definition += "0));\n"

        return group_definition;

class ComponentMember(Member):

    def __init__(self, type, name):
        self.type = type
        self.name = name

    def get_idl_statement(self):
        return "\t\tDistributedATS::" + self.name + " c_" + self.name + ";\n"

    def get_logger_statement(self, entity_name):
        return "\t\t" + self.name + "::log(out," + self.name + ");\n"

    def get_cpp_declare_statement(self):
        fix2dds_cpp_def = "\t" + self.name + "Adapter::FIX2DDS(fixMsg, ddsMsg.c_" + self.name + ");\n\n"
        dds2fix_cpp_def = "\t" + self.name + "Adapter::DDS2FIX(ddsMsg.c_" + self.name + ", fixMsg);\n\n"

        return fix2dds_cpp_def, dds2fix_cpp_def
