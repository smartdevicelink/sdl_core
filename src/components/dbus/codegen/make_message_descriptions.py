#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  @file make_message_descriptions.py
#  @brief Generates HMI API message descriptions for D-Bus
#
# This file is a part of HMI D-Bus layer.
# 
# Copyright (c) 2013, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

from argparse import ArgumentParser
from xml.etree import ElementTree
from copy import copy
from ford_xml_parser import FordXmlParser
from os import path
from sys import argv

namespace_name = 'ford_message_descriptions'
namespace = namespace_name + '::'


class Impl(FordXmlParser):
    def write_param_definition(self, param_var_name, param, out):
        if param.array:
            tmp_param = copy(param)
            tmp_param.array = False
            tmp_param.mandatory = True
            dbus_sig = self.convert_to_dbus_type(tmp_param)
            self.write_param_definition(param_var_name + '_array', tmp_param, out)

        if param.array: out.write('const ' + namespace + 'ArrayDescription ')
        elif param.struct: out.write('const ' + namespace + 'StructDescription ')
        else: out.write('const ' + namespace + 'ParameterDescription ')

        out.write(param_var_name + " = {\n")

        if param.struct or param.array:
            out.write("  {\n")
            shift = ' ' * 4
        else:
            shift = ' ' * 2

        out.write(shift + "\"" + param.name + "\",\n")

        if param.array:
            out.write(shift + namespace + "Array,\n")
        elif param.struct:
            out.write(shift + namespace + "Struct,\n")
        elif param.enum:
            out.write(shift + namespace + "Enum,\n")
        else:
            out.write(shift + namespace + param.type + ",\n")

        if param.mandatory:
            out.write(shift + "true\n")
        else:
            out.write(shift + "false\n")

        if param.struct or param.array:
            out.write("  },\n")
            if param.array:
                out.write("  (const " + namespace + "ParameterDescription*)&" + param_var_name + "_array,\n")
                out.write("  \"" + dbus_sig + "\"\n")
            elif param.struct:
                out.write("  Structs::" + param.fulltype[0] + "__" + param.fulltype[1] + "__parameters\n")
        out.write("};\n")


    def write_struct_params_declarations(self, out):
        out.write("struct Structs {\n");
        for (interface, name), params in self.structs.iteritems():
            params_var_name = interface + '__' + name + '__parameters'
            out.write("  static const " + namespace + "ParameterDescription* " + params_var_name + "[];\n")
        out.write("};\n\n")


    def write_enum_entries_declarations(self, out):
        out.write("struct Enums {\n");
        for interface, name in self.enums:
            entries_var_name = interface + '__' + name + '__entries'
            out.write("  static const " + namespace + "EnumDescription::Entry* " + entries_var_name + "[];\n")
        out.write("};\n\n")


    def write_parameters(self, params, out, name):
        n = 1
        for param in params:
            param_var_name = name + str(n)
            n = n + 1
            self.write_param_definition(param_var_name, param, out)


    def write_struct_params_definitions(self, out):
        for (interface, name), params in self.structs.iteritems():
            param_var_name = interface + '__' + name + '__parameter'
            self.write_parameters(params, out, param_var_name)
            params_var_name = 'Structs::' + interface + '__' + name + '__parameters'
            out.write("const " + namespace + "ParameterDescription* " + params_var_name + "[] = {\n")
            for n in range(1, len(params) + 1):
                name = param_var_name + str(n)
                out.write("  (const " + namespace + "ParameterDescription*)&" + name + ",\n")
            out.write(  "  NULL };\n\n")


    def write_enum_entries_definitions(self, out):
        for (interface, name), enum_el in self.enums.iteritems():
            n = 1
            for element_el in enum_el.findall('element'):
                entry_var_name = interface + '__' + name + '__entry' + str(n)
                value = element_el.get("value")
                if value is None:
                    value = str(n)
                out.write(namespace + "EnumDescription::Entry " + entry_var_name + " = {\"" + element_el.get("name") + "\", " + value + "};\n")
                n = n + 1

            entries_var_name = "Enums::" + interface + '__' + name + '__entries'
            out.write("const " + namespace + "EnumDescription::Entry* " + entries_var_name + "[] = {\n")
            for n in range(1, len(enum_el.findall('element')) + 1):
                entry_var_name = interface + '__' + name + '__entry' + str(n)
                out.write("  &" + entry_var_name + ",\n")
            out.write("  NULL };\n\n")


    def write_message_definition(self, message_el, interface, out):
        name = message_el.get('name')
        messagetype = message_el.get('messagetype')
        params = list()
        for param_el in message_el.findall('param'):
            param_desc = self.make_param_desc(param_el, interface)
            params.append(param_desc)
        param_var_name = interface + '__' + name + '__' + messagetype + '__parameter'
        self.write_parameters(params, out, param_var_name)

        params_var_name = param_var_name + 's'
        out.write("const " + namespace + "ParameterDescription* " + params_var_name + "[] = {\n")
        for n in range(1, len(params) + 1):
            param_name = param_var_name + str(n)
            out.write("  (const " + namespace + "ParameterDescription*)&" + param_name + ",\n")
        out.write("  NULL };\n")

        message_desc_name = interface + '__' + name + '__' + messagetype
        out.write("const " + namespace + "MessageDescription " + message_desc_name + " = {\n")
        out.write("  \"" + interface + "\",\n")
        out.write("  \"" + name + "\",\n")
        out.write("  hmi_apis::messageType::" + messagetype + ",\n")
        out.write("  hmi_apis::FunctionID::" + interface + "_" + name + ",\n")
        out.write("  " + params_var_name + "\n")
        out.write("};\n\n")


    def write_message_definitions(self, out):
        for interface_el in self.el_tree.findall('interface'):
            interface = interface_el.get('name')
            message_els = interface_el.findall('function')
            for message_el in message_els:
                self.write_message_definition(message_el, interface, out)


    def make_message_descriptions(self, out):
        self.write_struct_params_declarations(out)
        #self.write_enum_entries_declarations(out)
        self.write_struct_params_definitions(out)
        #self.write_enum_entries_definitions(out)
        self.write_message_definitions(out)


    def make_message_array(self, out):
        out.write("const MessageDescription* message_descriptions[] = {\n")
        for interface_el in self.el_tree.findall('interface'):
            interface = interface_el.get('name')
            message_els = interface_el.findall('function')
            for message_el in message_els:
                name = message_el.get('name')
                messagetype = message_el.get('messagetype')
                message_desc_name = interface + '__' + name + '__' + messagetype
                out.write("  &" + message_desc_name + ",\n")
        out.write("  NULL\n")
        out.write("};\n")


arg_parser = ArgumentParser(description="Generates HMI API message descriptions for D-Bus on SDL core side")
arg_parser.add_argument('--infile', required=True, help="full name of input file, e.g. applink/src/components/interfaces/QT_HMI_API.xml")
arg_parser.add_argument('--outdir', required=True, help="path to directory where output file message_descriptions.cc will be saved")
args = arg_parser.parse_args()

if not path.isdir(args.outdir):
    makedirs(args.outdir)

outfile = 'message_descriptions.cc'

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root)

out = open(args.outdir + "/" + outfile, "w")

out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
out.write("""/**
 * @file message_descriptions.cc
 * @brief Generated HMI API message descriptions for D-Bus
 *
 * This file is a part of HMI D-Bus layer.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

""")

out.write("#include \"dbus/message_descriptions.h\"\n\n");
out.write("namespace {\n\n");
impl.make_message_descriptions(out)
out.write("}\n\n");

out.write("namespace " + namespace_name + " {\n\n");
impl.make_message_array(out)
out.write("}\n\n");

