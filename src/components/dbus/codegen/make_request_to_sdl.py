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

class Impl(FordXmlParser):

    def args_for_function_definition(self, params, iface_name, out):
        for param_el in params:
            param = self.make_param_desc(param_el, iface_name)
            out.write('QVariant %s,' % (param.name))
        out.write('Q%sValue hmi_callback' % prefix_class_item)      

    def make_requests_for_header(self, out):
        for interface_el in self.el_tree.findall('interface'):
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "sdl")            
            for (request, response) in request_responses:
                all_params = list()
                for param_el in request.findall('param'):
                    all_params.append(param_el)                    
                with CodeBlock(out) as output:
                    output.write("Q_INVOKABLE bool %s_%s(" % (interface_el.get('name'), request.get('name')))
                impl.args_for_function_definition(all_params, interface_el.get('name'), out)
                output.write(");\n")
                 

    def make_header_file(self, out):
        out.write("class QDBusInterface;\n")
        out.write("class RequestToSDL : public QObject\n")
        out.write("{\n") 
        out.write('  Q_OBJECT\n')
        out.write(" public:\n")     
        with CodeBlock(out) as output:  
            output.write("explicit RequestToSDL(QObject *parent = 0);\n")
            output.write("~RequestToSDL();\n")
        impl.make_requests_for_header(out)
        out.write(" private:\n")
        with CodeBlock(out) as output:
            for interface_el in self.el_tree.findall('interface'):
                output.write('QDBusInterface *%s;\n' % interface_el.get('name'))
        out.write("};\n")


    def qt_param_type(self, param):
        if not param.mandatory:
            param_copy = copy(param)
            param_copy.mandatory = True
            return "OptionalArgument< " + self.qt_param_type(param_copy) + " >"
        if param.array:
            param_copy = copy(param)
            param_copy.array = False
            if param.type == 'String':
                return "QStringList"
            return "QList< " + self.qt_param_type(param_copy) + " >"
        if param.type == 'Integer' or param.enum:
            return 'int'
        elif param.type == 'String':
            return 'QString'
        elif param.type == 'Boolean':
            return 'bool'
        elif param.type == 'Float':
            return 'double'
        elif param.struct:
            return "_".join(param.fulltype)
        else:
            raise RuntimeError('Unknown type: ' + param.type)


    def make_requests_for_source(self, out):
        for interface_el in self.el_tree.findall('interface'):
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "sdl")
            for (request, response) in request_responses:
                request_name = request.get('name')
                iface_name = interface_el.get('name')
                request_full_name = iface_name + '_' + request_name
                out.write('bool RequestToSDL::' + request_full_name + '(')
                for param_el in request.findall('param'):                    
                    out.write('QVariant %s, ' % (param_el.get('name')))
                out.write('Q%sValue hmi_callback) {\n' % prefix_class_item)
                with CodeBlock(out) as output:
                    output.write('LOG4CXX_TRACE(logger_, "ENTER");\n')
                    output.write('QList<QVariant> args;\n')           
                    for param_el in request.findall('param'):
                        param = self.make_param_desc(param_el, iface_name)
                        output.write('%s %s;\n' % (impl.qt_param_type(param), param.name + "_tmp"))
                        output.write('if (VariantToValue(%s, %s)) {\n' % (param.name, param.name + '_tmp'))
                        with CodeBlock(output) as out:
                            self.write_param_validation(param, param.name + "_tmp",
                                "\nLOG4CXX_ERROR(logger_, \"%s in %s out of bounds\");\nreturn false" % (param.name, request_full_name),
                                out)
                            out.write('args << QVariant::fromValue(%s);\n' % (param.name + '_tmp'))
                        output.write('} else {\n')
                        with CodeBlock(output) as out:
                            out.write('LOG4CXX_ERROR(logger_, "%s in %s is NOT valid");\n' % (param.name, request_full_name))
                            out.write('return false;\n')
                        out.write('}\n')
                    output.write('new requests::' + request_full_name + '(hmi_callback, ' + interface_el.get('name') + ' , args, '
                                  + '"' + request_name + '");\n')
                    output.write('LOG4CXX_TRACE(logger_, "EXIT");\n')
                    output.write('return true;\n')
                out.write('}\n\n')


    def write_param_validation(self, param, param_name, fail_statement, out, level=0):
        if not param.mandatory and (param.restricted or param.restrictedArray or (param.struct and any(map(lambda x: x.restricted, self.structs[param.fulltype])))):
            out.write("if (%s.presence) {\n" % param_name)
            param_copy = copy(param)
            param_copy.mandatory = True
            with CodeBlock(out) as out:
                self.write_param_validation(param_copy, param_name + ".val", fail_statement, out, level+1)
            out.write("}\n")
        elif param.array:
            if param.minSize > 0:
                out.write("if ({0}.count() < {1}) {{".format(param_name, param.minSize))
                with CodeBlock(out) as out:
                    out.write("{0};\n".format(fail_statement))
                out.write("}\n")
            if param.maxSize != None:
                out.write("if ({0}.count() > {1}) {{".format(param_name, param.maxSize))
                with CodeBlock(out) as out:
                    out.write("{0};\n".format(fail_statement))
                out.write("}\n")
            if param.restricted:
                out.write('for ({0}::const_iterator it_{2} = {1}.begin(); it_{2} != {1}.end(); ++it_{2}) {{\n'.format(self.qt_param_type(param), param_name, level))
                with CodeBlock(out) as out:
                    param_copy = copy(param)
                    param_copy.array = False
                    self.write_param_validation(param_copy, "(*it_{0})".format(level), fail_statement, out, level+1)
                out.write("}\n")
        elif param.struct:
            for p in self.structs[param.fulltype]:
                self.write_param_validation(p, "{0}.{1}".format(param_name, p.name), fail_statement, out, level+1)
        elif param.type == "Integer" or param.type == "Float":
            conditions = []
            if (param.minValue != None):
                conditions.append("(%s < %s)" % (param_name, param.minValue))
            if (param.maxValue != None):
                conditions.append("(%s > %s)" % (param_name, param.maxValue))
            if conditions:
                out.write('if (%s) {' % ' || '.join(conditions))
                with CodeBlock(out) as out:
                    out.write('%s;\n' % fail_statement)
                out.write("}\n")
        elif param.type == "String":
            conditions = []
            if (param.minLength > 0):
                conditions.append("(%s.size() < %s)" % (param_name, param.minLength))
            if (param.maxLength > 0):
                conditions.append("(%s.size() > %s)" % (param_name, param.maxLength))
            if conditions:
                out.write('if (%s) {' % ' || '.join(conditions))
                with CodeBlock(out) as out:
                    out.write('%s;\n' % (fail_statement))
                out.write("}\n")

    def make_source_file(self, out):
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
arg_parser.add_argument('--version', required=False, help="Qt version 4.8.5 (default) or 5.5.1")
arg_parser.add_argument('--outdir', required=True, help="path to directory where output files request_to_sdl.h, request_to_sdl.cc will be saved")
args = arg_parser.parse_args()


if args.version == "4.8.5":
    prefix_class_item = 'Script'
    invoke_type_connection = 'Direct'
elif args.version == "5.5.1":
    prefix_class_item = 'JS'
    invoke_type_connection = 'BlockingQueued'
else:
    prefix_class_item = 'JS'
    invoke_type_connection = 'BlockingQueued'

header_name = 'request_to_sdl.h'
source_name = 'request_to_sdl.cc'

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root, 'com.ford.sdl.hmi')

header_out = open(args.outdir + '/' + header_name, "w")
source_out = open(args.outdir + '/' + source_name, "w")

header_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
header_out.write("""
/*
 Copyright (c) 2014, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/
""")
header_out.write("#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_REQUEST_TO_SDL_H_\n")
header_out.write("#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_REQUEST_TO_SDL_H_\n\n")

header_out.write("#include <QtCore/QObject>\n")
header_out.write("#include <QtCore/QVariant>\n")
header_out.write("#include <QtCore/QStringList>\n\n")
header_out.write('#include "qml_dbus.h"\n\n')
if args.version == "4.8.5":
    header_out.write("#include <QtScript/QScriptValue>\n")
elif args.version == "5.5.1":
    header_out.write("#include <QtQml/QJSValue>\n")

impl.make_header_file(header_out)
header_out.write("#endif  // SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_REQUEST_TO_SDL_H_")


source_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
source_out.write("""
/*
 Copyright (c) 2014, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

""")

source_out.write('#include "request_to_sdl.h"\n')
source_out.write("#include <QtDBus/QDBusConnection>\n")
source_out.write("#include <QtDBus/QDBusInterface>\n")
source_out.write('#include "hmi_requests.h"\n')
source_out.write('#include "utils/logger.h"\n\n')
source_out.write('CREATE_LOGGERPTR_GLOBAL(logger_, "DBusPlugin")\n\n')

impl.make_source_file(source_out)
