#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  @file make_qml_dbus_cpp.py
#  @brief Generator of QML to QDbus C++ part
#
# This file is a part of HMI D-Bus layer.
# 
# Copyright (c) 2014, Ford Motor Company
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
import os.path
from sys import argv
from xml.etree import ElementTree
from copy import copy
from ford_xml_parser import FordXmlParser, ParamDesc
from code_formatter import CodeBlock

prefix_class_item = 'Declarative'
invoke_type_connection = 'Direct'

def defaultValue(param):
    if param.type == "Integer":
        return "0"
    elif param.type == "Float":
        return "0.0"
    elif param.type == "Boolean":
        return "false"
    elif param.enum:
        return "0"

class Impl(FordXmlParser):


    def args_for_function_definition(self, params, out):
        if len(params) == 0:
            out.write('QJSValue hmi_callback')
            return
        for param in params:
            out.write('QVariant ' + param.get('name') + ', ')
        out.write('QJSValue hmi_callback')
        

    def make_requests_for_header(self, out):
        for interface_el in self.el_tree.findall('interface'):
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "sdl")            
            for (request, response) in request_responses:
                all_params = list()
                for param_el in request.findall('param'):
                    all_params.append(param_el)                    
                with CodeBlock(out) as output:
                    output.write("Q_INVOLABKE void " + interface_el.get('name') + "_" +request.get('name') + "(")
                impl.args_for_function_definition(all_params, out)
                output.write(");\n")
                 

    def make_header_file(self, out):
        out.write("class RequestToSDL : public QObject\n")
        out.write("{\n") 
        out.write(" public:\n")     
        with CodeBlock(out) as output:  
            output.write("explicit RequestToSDL(QObject *parent = 0);\n")
            output.write("~RequestToSDL();\n")
        impl.make_requests_for_header(out)
        out.write(" private:\n")
        with CodeBlock(out) as output:
            for interface_el in self.el_tree.findall('interface'):
                output.write('QDBusInterface *' + interface_el.get('name') + ';\n')
        out.write("};\n")


    def make_requests_for_source(self, out):
        for interface_el in self.el_tree.findall('interface'):
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "sdl")            
            for (request, response) in request_responses:
                out.write('void RequestToSDL::' + request.get('name') + '(QString name, ')         
                for param_el in request.findall('param'):
                    out.write('QVariant ' + param_el.get('name') + ', ')                    
                out.write('QJSValue hmi_callback) {\n')
                with CodeBlock(out) as output:
                    output.write('QList<QVariant> args;\n')
                    for param_el in request.findall('param'):
                        output.write('args << ' + param_el.get('name') + ';\n')
                    output.write('new ' + interface_el.get('name') + '_' + request.get('name') + '(' + interface_el.get('name') + ', name, args, hmi_callback);\n}\n')
                
        

    def make_source_file(self, out):
        out.write('#include "request_to_sdl.h"\n')
        out.write('\n')
        out.write('RequestToSDL::RequestToSDL(QObject *parent) {\n')
        with CodeBlock(out) as output:
            output.write('QDBusConnection bus = QDBusConnection::sessionBus();\n')
            for interface_el in self.el_tree.findall('interface'):
                iface_name = interface_el.get('name') 
                output.write(iface_name + ' = new QDBusInterface("com.ford.sdl.core", "/", "com.ford.sdl.core.' + iface_name + '", bus, this);\n')
        out.write('}\n\n')      
        out.write('RequestToSDL::~RequestToSDL() {\n')
        with CodeBlock(out) as output:
            for interface_el in self.el_tree.findall('interface'):
                iface_name = interface_el.get('name')                    
                output.write(iface_name + '->deleteLater();\n')
            output.write('this->deleteLater();\n')
        out.write('}\n\n')
        impl.make_requests_for_source(out)
        
            



arg_parser = ArgumentParser(description="Generator of Qt to QDbus C++ part")
arg_parser.add_argument('--infile', required=True, help="full name of input file, e.g. applink/src/components/interfaces/QT_HMI_API.xml")
arg_parser.add_argument('--version', required=False, help="Qt version 4.8.5 (default) or 5.1.0")
arg_parser.add_argument('--outdir', required=True, help="path to directory where output files request_to_sdl.h, request_to_sdl.cc will be saved")
args = arg_parser.parse_args()

if args.version == "4.8.5":
    prefix_class_item = 'Declarative'
    invoke_type_connection = 'Direct'
elif args.version == "5.1.0":
    prefix_class_item = 'Quick'
    invoke_type_connection = 'BlockingQueued'

header_name = 'request_to_sdl.h'
source_name = 'request_to_sdl.cc'

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root, 'com.ford.sdl.hmi')

header_out = open(args.outdir + '/' + header_name, "w")
source_out = open(args.outdir + '/' + source_name, "w")

header_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
header_out.write("""/**
 * @file request_to_sdl.h
 * @brief Generated class that process requests from qtHMI
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
header_out.write("#ifndef SRC_COMPONENTS_QTHMI_QMLMODELQT5_REQUESTTOSDL_\n");
header_out.write("#define SRC_COMPONENTS_QTHMI_QMLMODELQT5_REQUESTTOSDL_\n\n");
header_out.write("#include <QDBusConnection>\n");
header_out.write("#include <QJSValue>\n");
header_out.write("#include <QObject>\n");
header_out.write("#include <QDBusInterface>\n");
header_out.write('#include "hmi_requests.h"\n');

impl.make_header_file(header_out)

header_out.write("#endif SRC_COMPONENTS_QTHMI_QMLMODELQT5_REQUESTTOSDL_\n");


source_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
source_out.write("""/**
 * @file request_to_sdl.cc
 * @brief Generated class that process requests from qtHMI
 *
 * This file is a part of HMI D-Bus layer.
 */
// Copyright (c) 2014, Ford Motor Company
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

impl.make_source_file(source_out)
