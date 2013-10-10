#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  @file make_qml_dbus_qml.py
#  @brief Generator of QML to QDbus QML part
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

from os import path
from os import makedirs
from argparse import ArgumentParser
from xml.etree import ElementTree
from copy import copy
from ford_xml_parser import FordXmlParser

namespace = 'dbus_qml'

class Impl(FordXmlParser):
    def write_function(self, ifacename, request, response, out):
        name = request.get('name')
        name = name[:1].lower() + name[1:]
        out.write("  function " + name + "(params) {\n")
	out.write("      try {\n")
        out.write("          return sdl" + ifacename + "." + name + "(")
        params = request.findall('param')
        for i in range(len(params)):
            out.write('params.' + params[i].get('name'))
            if i <> len(params) - 1:
                out.write(', ')
        out.write(")\n")
        out.write("      } catch(err) {\n")
        out.write("""          return { "__errno": err }\n""")
        out.write("      }\n")
        out.write("  }\n\n")


    def write_signal_param(self, param, out):
        if param.array or param.struct or not param.mandatory: typ = 'var'
        elif param.type == 'Integer' or param.enum: typ = 'int'
        elif param.type == 'String': typ = 'string'
        elif param.type == 'Boolean': typ = 'bool'
        elif param.type == 'Float': typ = 'real'
        else: typ = 'var'
        out.write(typ + ' ' + param.name)


    def write_signal(self, ifacename, notification, out):
        name = notification.get('name')
        name = name[:1].lower() + name[1:]
        out.write("  signal " + name + "(")
        params = notification.findall('param')
        for i in range(len(params)):
            self.write_signal_param(self.make_param_desc(params[i], ifacename), out)
            if i <> len(params) - 1:
                out.write(', ')
        out.write(")\n")


    def write_qml(self, iface, out):
        name = iface.get('name')
        out.write("import QtQuick 2.0\n")
        out.write("import \"..\"\n\n")
        out.write("Item {\n")
        out.write("  " + name + " {\n")
        out.write("    id: sdl" + name + "\n")
        out.write("  }\n")

        notifications = impl.find_notifications(interface_el)
        request_responses = impl.find_request_response_pairs(interface_el)
        for request, response in request_responses:
            self.write_function(name, request, response, out)
        for n in notifications:
            self.write_signal(name, n, out)
        out.write("}\n")


    def write_js_enum(self, enum, out):
        out.write("var " + enum.get('name') + " = {\n")
        lastval = -1
        elements = enum.findall('element')
        for i in range(len(elements)):
            element = elements[i]
            value = element.get('value')
            if value is None:
                value = lastval + 1
            else:
                value = int(value)
            lastval = value
            name = element.get('internal_name')
            if name is None:
                name = element.get('name')
            out.write("  " + name + ': ' + str(value))
            if len(elements) - 1 != i: out.write(',')
            out.write("\n")
        out.write("}\n\n")


def write_header(out):
    out.write("""/**
  * Copyright (c) 2013, Ford Motor Company
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *
  * Redistributions of source code must retain the above copyright notice, this
  * list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following
  * disclaimer in the documentation and/or other materials provided with the
  * distribution.
  *
  * Neither the name of the Ford Motor Company nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  * POSSIBILITY OF SUCH DAMAGE.
  */

""")

arg_parser = ArgumentParser()
arg_parser.add_argument('--infile', required=True)
arg_parser.add_argument('--outdir', required=True)
args = arg_parser.parse_args()

if not path.isdir(args.outdir):
    makedirs(args.outdir)

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root)

for interface_el in in_tree_root.findall('interface'):
    notifications = impl.find_notifications(interface_el)
    request_responses = impl.find_request_response_pairs(interface_el)
    if notifications or request_responses:
        filename = interface_el.get('name')+'Proxy.qml'
        outfile = open(args.outdir + '/' + filename, 'w')
        write_header(outfile)
        impl.write_qml(interface_el, outfile)

enum_files = dict()
for (iface, name), enum in impl.enums.items():
    if iface in enum_files:
        outfile = enum_files[iface]
    else:
        filename = iface+'.js'
        outfile = open(args.outdir + '/' + filename, 'w')
        write_header(outfile)
        enum_files[iface] = outfile
    impl.write_js_enum(enum, outfile)


