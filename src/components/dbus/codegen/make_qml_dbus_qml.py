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
from sys import argv
from argparse import ArgumentParser
from xml.etree import ElementTree
from copy import copy
from ford_xml_parser import FordXmlParser

namespace = 'dbus_qml'
qt_quick_version = '1.1'
type_variant_name = 'variant'

class Impl(FordXmlParser):
    def write_function(self, ifacename, request, response, out):
        name = request.get('name')
        name = name[:1].lower() + name[1:]
        out.write("  function " + name + "(params) {\n")
        out.write("""    console.debug("{0}Proxy::{1}")\n""".format(ifacename, name))
        out.write("    try {\n")
        out.write("      if(\"{0}\" in sdl{1})\n".format(name, ifacename))
        out.write("        return sdl{0}.{1}(" .format(ifacename, name))
        params = request.findall('param')
        for i in range(len(params)):
            out.write('params.' + params[i].get('name'))
            if i <> len(params) - 1:
                out.write(', ')
        out.write(")\n")
        out.write("      else\n")
        out.write("""        return { "__errno": Common.Result.UNSUPPORTED_REQUEST }\n""")
        out.write("    } catch(err) {\n")
        out.write("""        return { "__errno": err }\n""")
        out.write("    }\n")
        out.write("  }\n\n")


    def write_signal_param(self, param, out):
        if param.array or param.struct or not param.mandatory: typ = type_variant_name
        elif param.type == 'Integer' or param.enum: typ = 'int'
        elif param.type == 'String': typ = 'string'
        elif param.type == 'Boolean': typ = 'bool'
        elif param.type == 'Float': typ = 'double'
        else: typ = type_variant_name
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
        name = notification.get('name')
        out.write("""  on{1}: console.debug("emitted {0}:{1}")\n""".format(ifacename, name))


    def write_qml(self, iface, out):
        global qt_quick_version
        name = iface.get('name')
        out.write("import QtQuick %s\n" % qt_quick_version)
        out.write("""import "Common.js" as Common\n""")
        out.write("import \"..\"\n\n")
        out.write("Item {\n")
        out.write("  " + name + " {\n")
        out.write("    id: sdl" + name + "\n")
        out.write("  }\n")

        notifications = impl.find_notifications_by_provider(interface_el, "hmi")
        request_responses = impl.find_request_response_pairs_by_provider(interface_el, "hmi")
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
    out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
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

arg_parser = ArgumentParser(description="Generator of Qt to QDbus QML part")
arg_parser.add_argument('--infile', required=True, help="full name of input file, e.g. applink/src/components/interfaces/QT_HMI_API.xml")
arg_parser.add_argument('--version', required=False, help="Qt version 4.8.5 (default) or 5.5.1")
arg_parser.add_argument('--outdir', required=True, help="path to directory where output files with pattern name <intarface name>Proxy.qml will be saved, e.g. BasicCommunicationProxy.qml, ButtonsProxy.qml VRProxy.qml")
args = arg_parser.parse_args()

if args.version == "4.8.5":
    qt_quick_version = "1.1"
    type_variant_name = "variant"
elif args.version == "5.5.1":
    qt_quick_version = "2.0"
    type_variant_name = "var"

if not path.isdir(args.outdir):
    makedirs(args.outdir)

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root)

for interface_el in in_tree_root.findall('interface'):
    notifications = impl.find_notifications_by_provider(interface_el, "hmi")
    request_responses = impl.find_request_response_pairs_by_provider(interface_el, "hmi")
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
        outfile.write(".pragma library\n")
        enum_files[iface] = outfile
    impl.write_js_enum(enum, outfile)


