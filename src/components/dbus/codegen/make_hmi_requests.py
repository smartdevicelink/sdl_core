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
    def make_requests_classes_header(self, out):
        for interface_el in self.el_tree.findall('interface'):
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "sdl")
            for (request, response) in request_responses:
                request_full_name = interface_el.get('name') + '_' +request.get('name')
                out.write('class ' + request_full_name + ': public HMIRequest {\n')
                with CodeBlock(out) as output:
                    output.write('Q_OBJECT\n')
                out.write(' public:\n')
                with CodeBlock(out) as output:
                    output.write(request_full_name + '(QJSValue hmi_callback, QDBusInterface *interface, QList<QVariant> args, QString name):\n')
                    output.write('  HMIRequest(hmi_callback, interface, args, name) {}\n')
                out.write(' private:\n')
                with CodeBlock(out) as output:
                    output.write('QJSValueList fillArgsList();\n};\n\n')
                         

    def make_header_file(self, out):
        out.write("namespace requests {\n")
        out.write("class HMIRequest: public QObject {\n")
        with CodeBlock(out) as output:
            output.write('Q_OBJECT\n')
        out.write('public:\n')
        with CodeBlock(out) as output:
            output.write('HMIRequest(QJSValue hmi_callback, QDBusInterface *interface, QList<QVariant> args, QString name );\n')
        out.write('protected:\n')
        with CodeBlock(out) as output:
            output.write('virtual QJSValueList fillArgsList() = 0;\n')
            output.write('QDBusPendingCallWatcher *watcher_;\n')
            output.write('QJSValue hmi_callback_;\n\n')
            output.write('template<typename T>\n')
            output.write('QJSValue CreateQJSValue(T value) {\n')
            output.write('  return QJSValue(value);\n')
            output.write('}\n\n')
            output.write('template<typename T>\n')
            output.write('QJSValue CreateQJSValue(QList<T> value) {\n')
            with CodeBlock(output) as output:
                output.write('QJSValue array = hmi_callback_.engine()->newArray();\n')
                output.write('int i = 0;\n')
                output.write('foreach (T item, value) {\n')
                output.write('  QJSValue value = CreateQJSValue(item);\n')
                output.write('  array.setProperty(i, value);\n')
                output.write('  ++i;\n')
                output.write('}\n')
                output.write('return array;\n')
            output.write('}\n')
        out.write('private:\n')
        with CodeBlock(out) as output:
            output.write('QDBusInterface *interface_;\n')
            output.write('QList<QVariant> args_;\n')
        out.write('public slots:\n')
        with CodeBlock(out) as output:
            output.write('void invokeCallback();\n')
        out.write('};\n\n')
        output.write('template<>\n')
        output.write('QJSValue HMIRequest::CreateQJSValue(QStringList value);\n')
        for (interface_name, struct_name) in self.structs:
            out.write('template<>\n')
            out.write('QJSValue HMIRequest::CreateQJSValue(Common_' + struct_name + ' value);\n\n')
        impl.make_requests_classes_header(out)
        out.write("}  // namespace requests\n")


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
        return "xxx"


    def make_requests_methods_source(self, out):
        for interface_el in self.el_tree.findall('interface'):
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "sdl")
            for (request, response) in request_responses:
                iface_name = interface_el.get('name')
                request_full_name = iface_name + '_' +request.get('name')
                out.write('QJSValueList ' + request_full_name + '::fillArgsList() {\n')
                out.write('  QDBusPendingReply< ')
                count = 0
                for param_el in response.findall('param'):
                    param = self.make_param_desc(param_el, iface_name)
                    out.write(self.qt_param_type(param))
                    count += 1
                    if count < len(response.findall('param')):
                        out.write(',')
                out.write(' > reply = *watcher_;\n')

                with CodeBlock(out) as out:
                    out.write('QJSValueList qjsValueList;\n\n')
                    out.write('QJSValue param;\n\n')
                    count = 0
                    for param_el in response.findall('param'):
                        if param_el.get('mandatory') == 'false':
                            out.write('if (reply.argumentAt<' + str(count) + '>().presence) {\n')
                            out.write('  param = CreateQJSValue(reply.argumentAt<' + str(count) + '>().val);\n')
                            out.write('} else {\n')
                            out.write('  param = QJSValue();\n')
                            out.write('}\n')
                            out.write('qjsValueList.append(param);\n')
                        else:
                            out.write('param = CreateQJSValue(reply.argumentAt<' + str(count) + '>());\n')
                            out.write('qjsValueList.append(param);\n')
                        count += 1
                    out.write('return qjsValueList;\n')
                out.write('}\n\n')


    def make_source_file(self, out):
        out.write("namespace requests {\n")
        for interface_el in self.el_tree.findall('interface'):
            for struct_el in interface_el.findall('struct'):
                out.write('template<>\n')
                out.write('QJSValue HMIRequest::CreateQJSValue(Common_' + struct_el.get('name') + ' value){\n') 
                with CodeBlock(out) as output:
                    output.write('QJSValue object = hmi_callback_.engine()->newObject();\n')
                for param_el in struct_el.findall('param'):
                    param_name = param_el.get('name')
                    if param_el.get('mandatory') == 'true':
                        with CodeBlock(out) as output:
                            output.write('object.setProperty("' + param_name + '", CreateQJSValue(value.' + param_name + '));\n')
                    elif param_el.get('mandatory') == 'false':
                        with CodeBlock(out) as output:
                            output.write('object.setProperty("' + param_name + '", value.' + param_name + '.presence ? CreateQJSValue(value.' + param_name + '.val) : QJSValue());\n')
                out.write('return object;\n')
                out.write('}\n\n')
             
        
        out.write('HMIRequest::HMIRequest(QJSValue hmi_callback, QDBusInterface *interface, QList<QVariant> args, QString name) :\n')
        out.write('      hmi_callback_(hmi_callback), interface_(interface), args_(args) {\n')
        with CodeBlock(out) as output:
            output.write('QDBusPendingCall pcall = interface->asyncCallWithArgumentList(name, args);\n')
            output.write('watcher_ = new QDBusPendingCallWatcher(pcall);\n')
            output.write('QObject::connect(watcher_, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(invokeCallback()));\n}\n\n') 
        out.write('void HMIRequest::invokeCallback() {\n')
        with CodeBlock(out) as output:
            output.write('if (!hmi_callback_.isUndefined()) {\n')
            with CodeBlock(output) as output:
                output.write('QJSValueList qjsValueList;\n')
                output.write('qjsValueList = this->fillArgsList();\n')
                output.write('hmi_callback_.call(qjsValueList);\n')
            output.write('}\n')
            output.write('watcher_->deleteLater();\n')
            output.write('this->deleteLater();\n}\n\n')
        impl.make_requests_methods_source(out)
        out.write("}  // namespace requests\n")


arg_parser = ArgumentParser(description="Generator of Qt to QDbus C++ part")
arg_parser.add_argument('--infile', required=True, help="full name of input file, e.g. applink/src/components/interfaces/QT_HMI_API.xml")
arg_parser.add_argument('--version', required=False, help="Qt version 4.8.5 (default) or 5.5.1")
arg_parser.add_argument('--outdir', required=True, help="path to directory where output files request_to_sdl.h, request_to_sdl.cc will be saved")
args = arg_parser.parse_args()

if args.version == "4.8.5":
    prefix_class_item = 'Declarative'
    invoke_type_connection = 'Direct'
elif args.version == "5.5.1":
    prefix_class_item = 'Quick'
    invoke_type_connection = 'BlockingQueued'

header_name = 'hmi_requests.h'
source_name = 'hmi_requests.cc'

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root, 'com.ford.sdl.hmi')

header_out = open(args.outdir + '/' + header_name, "w")
source_out = open(args.outdir + '/' + source_name, "w")

header_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
header_out.write("""/**
 * @file hmi_requests.h
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
header_out.write("#ifndef SRC_COMPONENTS_QTHMI_QMLMODELQT5_HMIREQUESTS_\n");
header_out.write("#define SRC_COMPONENTS_QTHMI_QMLMODELQT5_HMIREQUESTS_\n\n");
header_out.write("#include <QObject>\n");
header_out.write("#include <QJSValue>\n");
header_out.write("#include <QDBusPendingCall>\n");
header_out.write("#include <QDBusPendingCallWatcher>\n");
header_out.write('#include <QDBusPendingReply>\n');
header_out.write("#include <QDBusAbstractInterface>\n");
header_out.write("#include <QDBusInterface>\n");
header_out.write("#include <QJSEngine>\n");

header_out.write('#include "qml_dbus.h"\n\n');

impl.make_header_file(header_out)

header_out.write("#endif  // SRC_COMPONENTS_QTHMI_QMLMODELQT5_HMIREQUESTS_\n");

source_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
source_out.write("""/**
 * @file hmi_requests.cc
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
source_out.write('#include "hmi_requests.h"\n\n')
impl.make_source_file(source_out)
