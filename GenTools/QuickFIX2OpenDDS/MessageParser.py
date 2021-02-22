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

import QuickFIX2OpenDDS;

class MessageParser:

    def __init__(self, requiredFieldsComponents, component_parser ):
        self.requiredFieldsComponents = requiredFieldsComponents;
        self.component_parser = component_parser;
        self.message_list = [];
        self.message_dep_dict = {};
        self.message_dict = {};
        self.messages = dict();
        self.message_dep_dict = dict();
        self.internal_dependences = [];

    def parse_messages(self, quick_fix_specs, component_dependencies):

        for item in quick_fix_specs.find('messages'):
            message_name = item.get("name");
            if message_name not in self.requiredFieldsComponents.required_messages:
                continue;

            print("Processing Message : " + message_name );

            message_list_comp = [];

            required_fields = self.requiredFieldsComponents.required_fields;

            message_entity = self.component_parser.parse_components_rec(message_name, item, self.messages, message_list_comp, required_fields, True);

            current_dependency_list = message_list_comp;

            # in order to generate correct MPC file, we need all depenecies used by all components
            for component_dependency in message_list_comp:
                self.populate_component_dependency_list(component_dependency, current_dependency_list, component_dependencies);

            message_list_comp.append('Header');
            self.message_list.append(current_dependency_list);

            self.message_dep_dict[message_name] = message_list_comp;
            self.message_dict[message_name] = message_entity;

    # Triverse all dependencies
    def populate_component_dependency_list(self, component_dependency_name, current_depency_list, component_dependencies):

        print("Checking dependency ["+ component_dependency_name+ "]" )
        if component_dependency_name in component_dependencies.keys():

            if component_dependency_name not in current_depency_list:
                current_depency_list.append(component_dependency_name);

            component_depenecy_list = component_dependencies.get(component_dependency_name);

            for component_depenecy_list_component_name in component_depenecy_list:
                self.populate_component_dependency_list(component_depenecy_list_component_name, current_depency_list, component_dependencies);


    def populate_all_dependencies(self, name ):

        if name not in self.internal_dependences:
            self.internal_dependences.append(name);

        if name in self.message_dep_dict.keys():
            msg_deps = self.message_dep_dict[name];
            for msg_dep in msg_deps:
                self.populate_all_dependences(self, msg_dep );

    def get_dds_export_statement(self):
        dds_exports = str("\tdynamicflags   = DISTRIBUTEDATS_BUILD_DLL \n" +
                        "\tlibout         = $(DATS_HOME)/lib \n" +
                        "\trequires += tao_orbsvcs \n" +
                        "\tafter    += Svc_Utils \n" +
                        "\tincludes      += $(TAO_ROOT)/orbsvcs \n" +
                        "\tidlflags      += -I$(TAO_ROOT)/orbsvcs \ \n" +
                        "\t\t-Wb,export_macro=DistributedATS_Export \ \n" +
                        "\t\t-Wb,export_include=DistributedATS_Export.h \n" +
                        "\t\tdcps_ts_flags += -Wb,export_macro=DistributedATS_Export \n\n");

        return dds_exports;

    def store_mpc(self, message_name, dependency_list ):

        mpc_file = open("../idl/QuickFixIDL" + message_name + ".mpc", "w")
        mpc_file.write(QuickFIX2OpenDDS.DONT_MODIFY_TEXT);
        mpc_file.write("project(" + message_name + ") : dcps {\n\n");
        mpc_file.write(self.get_dds_export_statement());
        #mpc_file.write("\n\tincludes +=       $(QUICKFIX_HOME)/include \n\tlibpaths +=       $(QUICKFIX_HOME)/lib\n\tlibs += quickfix\n");

        for dependency_name in dependency_list:
            self.populate_all_dependencies(dependency_name);
            if dependency_name in self.message_dep_dict.keys():
                component_list_comp = self.message_dep_dict[dependency_name];
                for component_name in component_list_comp:
                    self.populate_all_dependencies(component_name);

        mpc_file.write("\tTypeSupport_Files{\n\t" + message_name + ".idl\n");
        for in_dep in self.internal_dependences:
            mpc_file.write(in_dep + ".idl\n");
        mpc_file.write("\n}\n\n");

        mpc_file.write("\tIDL_Files{\n\t" + message_name + "TypeSupport.idl\n\n");
        mpc_file.write(message_name + ".idl\n");
        for in_dep in self.internal_dependences:
            mpc_file.write(in_dep + ".idl\n");
            mpc_file.write(in_dep + "TypeSupport.idl\n");
        mpc_file.write("\n}\n\n");

        mpc_file.write("\tHeader_Files {\n\n\t}\n");
        mpc_file.write("\tSource_Files {\n\n\t}\n");

        mpc_file.write("}\n\n");

    def store_adapter_mpc(self, message_list, complete_dependency_list):

        mpc_file = open("../idl/Adapters.mpc", "w")
        mpc_file.write("project( Adapters ) : dcps {\n\n");
        after_list_str = "\tafter +=";

        for message_name in message_list:
            after_list_str += " ";
            after_list_str += message_name;

        after_list_str += "\n";

        mpc_file.write(after_list_str);

        mpc_file.write(self.get_dds_export_statement());

        mpc_file.write("\n\tincludes += $(QUICKFIX_HOME)/include \n\tlibpaths += $(QUICKFIX_HOME)/lib\n\tlibs += quickfix\n");

        headers_str = "\tHeader_Files {\n\n\t";
        source_str = "\tSource_Files {\n\n\t";
        for dependency in complete_dependency_list:
            headers_str += "\t"+dependency+"Adapter.hpp\n";
            source_str += "\t" + dependency + "Adapter.cpp\n";
        headers_str += "\t}\n";
        source_str += "\t}\n";

        mpc_file.write(headers_str);
        mpc_file.write(source_str);

        mpc_file.write("}\n\n");

    def store_idls(self, message_name, message_contexts, dependency_list):

        idl_file = open("../idl/" + message_name + ".idl", "w")

        idl_file.write(QuickFIX2OpenDDS.DONT_MODIFY_TEXT);
        #idl_file.write("#include \"DATSTypes.idl\"\n");
        for dependency_name in dependency_list:
            idl_file.write("#include \"" + dependency_name + ".idl\"\n");

            if dependency_name not in self.component_parser.complete_dependency_list:
                self.component_parser.complete_dependency_list.append(dependency_name)

        idl_file.write("\n");

        idl_file.write("module DistributedATS_" + message_name + "\n");
        idl_file.write("{\n#pragma DCPS_DATA_TYPE \"DistributedATS_" + message_name + "::" + message_name + "\"\n\n");

        idl_file.write(message_contexts.get_entity_idl());
        idl_file.write("};\n");

        idl_file.close();

    def store_logger(self, message_name, message_contexts, dependency_list):

        logger_file = open("../idl/" + message_name + "Logger.hpp", "w")

        logger_file.write(QuickFIX2OpenDDS.DONT_MODIFY_TEXT);
        logger_file.write("#ifndef __" + message_name + "Logger_h__\n");
        logger_file.write("#define __" + message_name + "Logger_h__\n\n");

        logger_file.write("#include \"" + message_name + "TypeSupportImpl.h\"\n");

        for dependency_name in dependency_list:
            logger_file.write("#include \"" + dependency_name + "Logger.hpp\"\n");

            if dependency_name not in self.component_parser.complete_dependency_list:
                self.component_parser.complete_dependency_list.append(dependency_name)

        logger_file.write(";\n");

        logger_file.write(message_contexts.get_entity_logger("DistributedATS_"));
        logger_file.write("\n};\n");

        logger_file.write("\n#endif\n\n");
        logger_file.close();

    def store_hpp_cpp(self, message_name, message_contexts, dependency_list):

        hpp_file = open("../idl/" + message_name + "Adapter.hpp", "w")
        cpp_file = open("../idl/" + message_name + "Adapter.cpp", "w")
        cpp_file.write(QuickFIX2OpenDDS.DONT_MODIFY_TEXT);
        cpp_file.write("#include \"" + message_name+"Adapter.hpp\"\n");
        cpp_file.write("#include \"ConvertUtils.h\"\n");

        self.component_parser.complete_dependency_list.append(message_name);
        hpp_file.write(QuickFIX2OpenDDS.DONT_MODIFY_TEXT);
        hpp_file.write("#ifndef __" + message_name + "_h__\n");
        hpp_file.write("#define __" + message_name + "_h__\n\n");
        hpp_file.write("#include \"" + message_name + "TypeSupportImpl.h\"\n");
        hpp_file.write("#include <quickfix/Message.h>\n\n using namespace DistributedATS;\n\n");

        for dependency_name in dependency_list:
            hpp_file.write("#include \"" + dependency_name + "Adapter.hpp\"\n");

            if dependency_name not in self.component_parser.complete_dependency_list:
                self.component_parser.complete_dependency_list.append(dependency_name)

        hpp_file.write(message_contexts.get_entity_hpp("DistributedATS_"));
        cpp_file.write(message_contexts.get_entity_cpp("DistributedATS_"));


        hpp_file.write("\n#endif\n\n");
        hpp_file.close();

    def store_message(self):

        for message_name, message_contexts in self.message_dict.items():

            if message_name not in self.requiredFieldsComponents.required_messages:
                continue;

            dependency_list = self.message_dep_dict[message_name];

            self.store_mpc(message_name, dependency_list);
            self.store_idls(message_name, message_contexts, dependency_list);
            self.store_hpp_cpp(message_name, message_contexts, dependency_list);
            self.store_logger(message_name, message_contexts, dependency_list);

        self.store_adapter_mpc(self.message_dict.keys(), self.component_parser.complete_dependency_list);
