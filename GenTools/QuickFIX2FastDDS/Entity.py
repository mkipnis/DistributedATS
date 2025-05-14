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


class Entity:

    def __init__(self, components_parser, entity_name, item, is_message):
        self.entity_name = entity_name
        self.item = item
        self.members = []
        self.groups = []
        self.is_message = is_message
        self.component_parser = components_parser

    def add_member(self, member):
        self.members.append(member)

    def add_group(self, entity):
        self.groups.append(entity)

    def get_entity_idl(self):

        comp_str = ""

        for group in self.groups:
            group_idl = group.get_entity_idl()
            comp_str += group_idl

        if self.item.tag == "message":
            # comp_str += "\t@topic\n\tstruct " + self.entity_name + "\n\t{\n\tDistributedATS::Header m_Header;\n";
            comp_str += "\t\n\tstruct " + self.entity_name + \
                        "\n\t{\n\t\tstring DATS_Source;\n " \
                        "    \t\tstring DATS_Destination;\n" \
                        "    \t\tstring DATS_SourceUser;\n" \
                        "    \t\tstring DATS_DestinationUser;\n\n\n\t\tDistributedATS::Header fix_header;\n\n"
        elif self.item.tag == 'group':
            comp_str += "\tstruct " + self.entity_name + "\n\t{\n"
        else:
            comp_str += "module DistributedATS\n{\n\tstruct " + self.entity_name + "\n\t{\n"

        for member in self.members:
            comp_str += member.get_idl_statement()

        if self.item.tag == "message":
            comp_str += "\t};\n"
        elif self.item.tag == 'group':
            comp_str += "\t};\n\ttypedef sequence<" + self.entity_name + "> " + self.entity_name + "Seq;\n\n"
        else:
            comp_str += "\t};\n};"

        return comp_str

    # class prefix DistributedATS for messages
    def get_entity_logger(self, class_prefix):

        comp_str = "\n\nclass " + self.entity_name + "Logger\n{\n\tpublic:\n"
        if self.is_message:
            comp_str += "\t\tstatic void log(std::ostream & out, " + class_prefix + self.entity_name + "::" + self.entity_name + "& ddsMsg )  __attribute__ ((visibility (\"default\")))\n"
            comp_str += "\t\t{\n\t\t\t;out<< \"Message : " + self.entity_name + " { \" << std::endl;\n"

            comp_str += "\n\t\t\t;out \n\t\t\t << \"ddsMsg.DATS_Source : \" << ddsMsg.DATS_Source() << " \
                        "std::endl "
            comp_str += "\n\t\t\t << \"ddsMsg.DATS_Destination : \" << ddsMsg.DATS_Destination() << std::endl"
            comp_str += "\n\t\t\t << \"ddsMsg.DATS_SourceUser : \" << ddsMsg.DATS_SourceUser() << std::endl"
            comp_str += "\n\t\t\t << \"ddsMsg.DATS_DestinationUser : \" << ddsMsg.DATS_DestinationUser() << std::endl;"
            comp_str += "\n\n\t\t\tHeaderLogger::log(out, ddsMsg.fix_header());\n"
        else:
            comp_str += "\t\tstatic void log(std::ostream & out, DistributedATS::" + self.entity_name + "& ddsMsg )  __attribute__ ((visibility (\"default\")))\n\t\t{\n"
            comp_str += "\n\t\t\t;out<< \"" + self.entity_name + " { \"  << std::endl\n"

        comp_str += "\n\t\t\t;out\n"

        for member in self.members:
            comp_str += "\t\t\t"
            comp_str += member.get_logger_statement(self.entity_name)
            comp_str += "\n"

        # comp_str += " out <<std::endl";
        comp_str += ";\t\t\tout << \"}\";\n\t\tout << std::endl;};\n"

        return comp_str

    # class prefix DistributedATS for messages
    def get_entity_hpp(self, class_prefix):
        comp_str = "\n\nclass " + self.entity_name + "Adapter\n{\n\tpublic:\n"
        if self.is_message == True:
            comp_str += "\t\tstatic void FIX2DDS(const FIX::Message&, " + class_prefix + self.entity_name + "::" + self.entity_name + "& ddsMsg )  __attribute__ ((visibility (\"default\")));\n";
            comp_str += "\t\tstatic void DDS2FIX(const " + class_prefix + self.entity_name + "::" + self.entity_name + "& ddsMsg, FIX::Message&)  __attribute__ ((visibility (\"default\")));\n";
        else:
            comp_str += "\t\tstatic void FIX2DDS(const FIX::FieldMap&, DistributedATS::" + self.entity_name + "& ddsMsg )  __attribute__ ((visibility (\"default\")));\n";
            comp_str += "\t\tstatic void DDS2FIX(const DistributedATS::" + self.entity_name + "& ddsMsg, FIX::FieldMap&)  __attribute__ ((visibility (\"default\")));\n";

        comp_str += "\n};"
        return comp_str

    def get_entity_cpp(self, class_prefix):

        if self.is_message == True:
            comp_str = "\n\nvoid " + self.entity_name + "Adapter::FIX2DDS(const FIX::Message& fixMsg, " + class_prefix + self.entity_name + "::" + self.entity_name + "& ddsMsg )\n{\n"
            comp_str += "\tHeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());\n\n"
        else:
            comp_str = "\n\n"  # + self.group_templates + "\n";
            comp_str += "void " + self.entity_name + "Adapter::FIX2DDS(const FIX::FieldMap& fixMsg, DistributedATS::" + self.entity_name + "& ddsMsg )\n{\n"

        fix2dds = ""
        dds2fix = ""
        for member in self.members:
            fix2dds_decl, dds2fix_decl = member.get_cpp_declare_statement()
            fix2dds += fix2dds_decl
            dds2fix += dds2fix_decl

        comp_str += fix2dds
        comp_str += "\n};\n\n"

        # comp_str += "\n\nvoid " + self.entity_name+"Adapter::DDS2FIX(const "+class_prefix + self.entity_name + "::"+ self.entity_name+"& ddsMsg, FIX::FieldMap& fixMsg)\n{\n";
        if self.is_message == True:
            comp_str += "\n\nvoid " + self.entity_name + "Adapter::DDS2FIX(const " + class_prefix + self.entity_name + "::" + self.entity_name + "& ddsMsg, FIX::Message& fixMsg)\n{\n";
            comp_str += "\tHeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());\n\n"
        else:
            comp_str += "\n\nvoid " + self.entity_name + "Adapter::DDS2FIX( const DistributedATS::" + self.entity_name + "& ddsMsg, FIX::FieldMap& fixMsg)\n{\n";
        comp_str += dds2fix
        comp_str += "\n};\n\n"

        return comp_str
