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

class Notifications_qml(FordXmlParser):
    #Used for qt signal names, because qt signals cannot begin with a capital letter
    def first_letter_to_lower_case(self, s):
        if len(s) == 0:
            return s
        else:
            return s[0].lower() + s[1:]

    def make_header(self, out):
        out.write("class SdlProxy: public Item {\n")
        with CodeBlock(out) as out:
            out.write("Q_OBJECT\n")
            out.write("Q_DISABLE_COPY(SdlProxy)\n")
            out.write("public:\n")
            out.write("explicit SdlProxy(Item* parent = 0);\n")
            out.write("private:\n")
            out.write("QDBusInterface *sdlBasicCommunicationInterface;\n")
            out.write("signals:\n")
        for interface_el in self.el_tree.findall('interface'):
            iface_name = interface_el.get('name')
            notifications = self.find_notifications_by_provider(interface_el, "sdl")
            for notification_el in notifications:
                with CodeBlock(out) as out:
                    out.write("void %s(" % self.first_letter_to_lower_case( notification_el.get("name")) )
                param_el_count = 1
                list_of_params = notification_el.findall("param")
                list_of_params_len = len(list_of_params)
                for param_el in list_of_params:
                    param = self.make_param_desc(param_el, iface_name)
                    out.write("QVariant %s" % param_el.get("name"))
                    if param_el_count < list_of_params_len:
                        out.write(", ")
                    param_el_count += 1
                out.write(");\n")
        with CodeBlock(out) as out:
            out.write("private slots:\n")
        for interface_el in self.el_tree.findall('interface'):
            iface_name = interface_el.get('name')
            notifications = self.find_notifications_by_provider(interface_el, "sdl")
            for notification_el in notifications:
                with CodeBlock(out) as out:
                    out.write("void slot_%s(" % notification_el.get("name"))
                param_el_count = 1
                list_of_params = notification_el.findall("param")
                list_of_params_len = len(list_of_params)
                for param_el in list_of_params:
                    param = self.make_param_desc(param_el, iface_name)
                    out.write("%s %s" % (self.qt_param_type(param), param_el.get("name")))
                    if param_el_count < list_of_params_len:
                        out.write(", ")
                    param_el_count += 1
                out.write(");\n")
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


    def qml_param_type(self, param):
        if not param.mandatory:
            return "QVariant"
        if param.array:
            return "QVariant"
        if param.type == 'Integer' or param.enum:
            return 'int'
        elif param.type == 'String':
            return 'QString'
        elif param.type == 'Boolean':
            return 'bool'
        elif param.type == 'Float':
            return 'double'
        elif param.struct:
            return "QVariant"
        else:
            raise RuntimeError('Unknown type: ' + param.type)         


    def make_source(self, out):
        def qml_args(variable_name_needed):
            param_el_count = 1
            list_of_params = notification_el.findall("param")
            list_of_params_len = len(list_of_params)
            for param_el in list_of_params:
                param = self.make_param_desc(param_el, iface_name)
                if variable_name_needed:
                    out.write("%s %s" % (self.qt_param_type(param), param_el.get("name")))
                else:
                    out.write("%s" % self.qt_param_type(param))
                if param_el_count < list_of_params_len:
                    out.write(", ")
                param_el_count += 1
        out.write("SdlProxy::SdlProxy(Item *parent): Item(parent) {\n")
        for interface_el in self.el_tree.findall('interface'):
            iface_name = interface_el.get('name')
            notifications = self.find_notifications_by_provider(interface_el, "sdl")
            for notification_el in notifications:
                notification_name = notification_el.get('name')
                with CodeBlock(out) as out:
                    out.write("QDBusConnection::sessionBus().connect(\n")
                    with CodeBlock(out) as out:
                        out.write("\"com.ford.sdl.core\", \"/\", \"com.ford.sdl.core.%s\",\n" % iface_name)
                        out.write("\"%s\", this, SLOT(slot_%s(" % (notification_name, notification_el.get("name")))      
                qml_args(variable_name_needed = False)
                out.write(")));\n")
        out.write("}\n\n")
        for interface_el in self.el_tree.findall('interface'):
            iface_name = interface_el.get('name')
            notifications = self.find_notifications_by_provider(interface_el, "sdl")
            for notification_el in notifications:
                notific_full_name = interface_el.get("name") + "_" + notification_el.get("name")
                out.write("void SdlProxy::slot_%s(" % notification_el.get("name"))
                qml_args(variable_name_needed = True)
                out.write(") {\n")
                with CodeBlock(out) as out:
                    out.write("LOG4CXX_TRACE(logger_, \"ENTER\");\n\n")     
                    for param_el in notification_el.findall("param"):
                        param = self.make_param_desc(param_el, iface_name)
                        tmp_param_name = param.name + "_qvariant"
                        out.write("QVariant %s;\n" % tmp_param_name)
                        out.write("%s = ValueToVariant(%s);\n" % (tmp_param_name, param.name))
                        self.write_param_validation(param, param.name, "\nLOG4CXX_ERROR(logger_, \"%s in %s out of bounds\")" % (param.name, notific_full_name), out)
                        out.write("\n")                    
                    out.write("emit %s(" % self.first_letter_to_lower_case( notification_el.get("name")) )
                param_el_count = 1
                list_of_params = notification_el.findall("param")
                list_of_params_len = len(list_of_params)
                for param_el in list_of_params:
                    param = self.make_param_desc(param_el, iface_name)
                    out.write("%s" %  param.name + "_qvariant")
                    if param_el_count < list_of_params_len:
                        out.write(", ")
                    param_el_count += 1  
                out.write(");\n")
                with CodeBlock(out) as out:                    
                    out.write("LOG4CXX_TRACE(logger_, \"EXIT\");\n")                                                 
                out.write("}\n\n")



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
        

#QVarian name;
#if (ttsName.presence) {                    
                


arg_parser = ArgumentParser(description="Generator of classes which Qt to QDbus C++ part")
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

header_name = 'sdl_proxy.h'
source_name = 'sdl_proxy.cc'

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Notifications_qml(in_tree_root, 'com.ford.sdl.hmi')

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
header_out.write("#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_SDL_PROXY_H_\n")
header_out.write("#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_SDL_PROXY_H_\n\n")
header_out.write("#include \"qml_dbus.h\"\n")
header_out.write("#include \"qt_version.h\"\n\n")
header_out.write("#include <QtCore/QVariant>\n")
header_out.write("#include <QtDBus/QDBusInterface>\n")
header_out.write("#if QT_4\n")
header_out.write("#include <QtDeclarative/QDeclarativeItem>\n")
header_out.write("typedef QDeclarativeItem Item;\n")
header_out.write("#elif QT_5\n")
header_out.write("#include <QtQuick/QQuickItem>\n")
header_out.write("typedef QQuickItem Item;\n")
header_out.write("#endif  // QT_VERSION\n")

impl.make_header(header_out)

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

source_out.write("#include \"sdl_proxy.h\"\n")
source_out.write("#include \"utils/logger.h\"\n")
source_out.write("CREATE_LOGGERPTR_GLOBAL(logger_, \"DBusPlugin\")\n\n")

impl.make_source(source_out)
