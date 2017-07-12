#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  @file make_qml_dbus_cpp.py
#  @brief Generator of QML to QDbus C++ part
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
    def make_dbus_type_declarations(self, out):
        for struct in self.structs.items():
            self.write_struct_declaration(struct, out)

    def make_dbus_metatype_declarations(self, out):
        for struct in self.structs.items():
            self.write_struct_metatype_declaration(struct, out)


    def write_struct_declaration(self, ((iface, name), params), out):
        struct_name = "{0}_{1}".format(iface, name)
        out.write("struct {0} {{\n".format(struct_name))
        with CodeBlock(out) as out:
            for param in params:
                out.write("{0} {1};\n".format(self.qt_param_type(param), param.name))
            out.write("{0}()\n".format(struct_name))
            initializersList = map(lambda x: "{0}({1})".format(x.name, defaultValue(x)), [p for p in params if p.type != "String" and not p.struct and not p.array])
            if initializersList:
                out.write(": " + ",\n    ".join(initializersList))

            out.write("{ }\n")

        out.write("};\n")
        out.write('QDBusArgument& operator << (QDBusArgument&, const ' + struct_name + "&);\n")
        out.write('const QDBusArgument& operator >> (const QDBusArgument&, ' + struct_name + "&);\n")
        out.write('void PutArgToMap(QVariantMap& map, const char* name, const ' + struct_name + "& v);\n")
        out.write('QVariant ValueToVariant(const ' + struct_name + "& v);\n")
        out.write('QVariant ValueToVariant(const QList<' + struct_name + " >& v);\n")
        out.write('bool GetArgFromMap(const QVariantMap& map, const char* name, ' + struct_name + "& v);\n")
        out.write('bool VariantToValue(const QVariant& variant, ' + struct_name + "& v);\n")
        out.write('bool VariantToValue(const QVariant& variant, QList<' + struct_name + " >& v);\n")


    def write_struct_metatype_declaration(self, ((iface, name), params), out):        
        struct_name = iface + '_' + name
        out.write('Q_DECLARE_METATYPE(' + struct_name + ")\n")
        out.write('Q_DECLARE_METATYPE(OptionalArgument<' + struct_name + ">)\n")
        out.write('Q_DECLARE_METATYPE(QList<' + struct_name + ">)\n")
        out.write('Q_DECLARE_METATYPE(OptionalArgument<QList<' + struct_name + "> >)\n\n")
            

    def make_dbus_type_definitions(self, out):
        for struct in self.structs.items():
            self.write_struct_definition(struct, out)

    def write_struct_definition(self, ((iface, name), params), out):
        struct_name = iface + '_' + name
        
        out.write('QDBusArgument& operator << (QDBusArgument& arg, const ' + struct_name + "& v) {\n")
        with CodeBlock(out) as out:
            out.write("arg.beginStructure();\n")
            for param in params:
                out.write("arg << v.{0};\n".format(param.name))
            out.write("arg.endStructure();\n")
            out.write("return arg;\n")
        out.write("}\n\n")

        out.write('const QDBusArgument& operator >> (const QDBusArgument& arg, ' + struct_name + "& v) {\n")
        with CodeBlock(out) as out:
            out.write("arg.beginStructure();\n")
            for param in params:
                out.write("arg >> v.{0};\n".format(param.name))
            out.write("arg.endStructure();\n")
            out.write("return arg;\n")
        out.write("}\n\n")

        out.write('QVariant ValueToVariant(const ' + struct_name + "& v) {\n")
        with CodeBlock(out) as out:
            out.write("QVariantMap map;\n")
            for param in params:
                out.write("""PutArgToMap(map, "{0}", v.{0});\n""".format(param.name))
            out.write("return QVariant::fromValue(map);\n")
        out.write("}\n\n")

        out.write('QVariant ValueToVariant(const QList<' + struct_name + ">& v) {\n")
        with CodeBlock(out) as out:
            out.write("QList<QVariant> ret;\n")
            out.write("for (QList<{0}>::const_iterator i = v.begin(); i != v.end(); ++i)\n".format(struct_name))
            with CodeBlock(out) as out:
                out.write("ret.append(ValueToVariant(*i));\n")
            out.write("return QVariant::fromValue(ret);\n")
        out.write("}\n\n")

        out.write('void PutArgToMap(QVariantMap& map, const char* name, const ' + struct_name + "& v) {\n")
        with CodeBlock(out) as out:
            out.write("map.insert(name, ValueToVariant(v));\n")
        out.write("}\n\n")

        out.write('bool VariantToValue(const QVariant& variant, ' + struct_name + "& v) {\n")
        with CodeBlock(out) as out:
            out.write("if (variant.type() != QVariant::Map) {\n")
            with CodeBlock(out) as out:
                out.write("""LOG4CXX_ERROR(logger_, "Input argument isn't a map");\n""")
                out.write("return false;\n")
            out.write("}\n")
            out.write("QVariantMap map = variant.toMap();\n")
            for param in params:
                out.write("if (!GetArgFromMap(map, \"" + param.name + "\", v." + param.name + ")) return false;\n")
            out.write("return true;\n")
        out.write("}\n\n")

        out.write('bool VariantToValue(const QVariant& variant, QList<' + struct_name + ">& v) {\n")
        with CodeBlock(out) as out:
            out.write("if (variant.type() != QVariant::List) {\n")
            with CodeBlock(out) as out:
                out.write("""LOG4CXX_ERROR(logger_, "Input argument isn't a list");\n""")
                out.write("return false;\n")
            out.write("}\n")
            out.write("QList<QVariant> list = variant.toList();\n")
            out.write("for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {\n");
            with CodeBlock(out) as out:
                out.write(struct_name + " s;\n");
                out.write("if (!VariantToValue(*i, s)) return false;\n");
                out.write("v.append(s);\n");
            out.write("}\n")
            out.write("return true;\n")
        out.write("}\n\n")

        out.write('bool GetArgFromMap(const QVariantMap& map, const char* name, ' + struct_name + "& v) {\n")
        with CodeBlock(out) as out:
            out.write("QVariantMap::const_iterator it = map.find(name);\n")
            out.write("if (map.end() == it) {\n")
            with CodeBlock(out) as out:
                out.write("""LOG4CXX_WARN(logger_, "Argument '" << name << "' not found");\n""")
                out.write("return false;\n")
            out.write("}\n")
            out.write("if (it->type() != QVariant::Map) {\n")
            with CodeBlock(out) as out:
                out.write("""LOG4CXX_ERROR(logger_, "Argument '" << name << "' isn't a map");\n""")
                out.write("return false;\n")
            out.write("}\n")
            out.write("const QVariantMap& inmap = it->toMap();\n")
            for param in params:
                out.write("if (!GetArgFromMap(inmap, \"" + param.name + "\", v." + param.name + ")) return false;\n")
            out.write("return true;\n")
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
                out.write("if ({0}.count() < {1}) {{ {2}; }}\n".format(param_name, param.minSize, fail_statement))
            if param.maxSize != None:
                out.write("if ({0}.count() > {1}) {{ {2}; }}\n".format(param_name, param.maxSize, fail_statement))
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
                out.write('if (%s) { %s; }\n' % (' || '.join(conditions), fail_statement))
        elif param.type == "String":
            conditions = []
            if (param.minLength > 0):
                conditions.append("(%s.size() < %s)" % (param_name, param.minLength))
            if (param.maxLength > 0):
                conditions.append("(%s.size() > %s)" % (param_name, param.maxLength))
            if conditions:
                with CodeBlock(out) as out:
                    out.write('if (%s) { %s;\n }\n' % (' || '.join(conditions), fail_statement))

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


    def from_variant_func_name(self, param, interface):
        prefix = ''
        if not param.mandatory:
            param_copy = copy(param)
            param_copy.mandatory = True
            return 'opt_' + self.from_variant_func_name(param_copy, interface)
        if param.array:
            param_copy = copy(param)
            param_copy.array = False
            return 'arr_' + self.from_variant_func_name(param_copy, interface)

        if param.type in ['Integer', 'String', 'Boolean', 'Float']:
            param_type = param.type
        else:
            param_type = param.type.split('.')
            if len(param_type) > 1:
                param_type = (param_type[0], param_type[1])
            else:
                param_type = (interface, param_type[0])
            if param_type in self.structs:
                param_type = param_type[0] + '_' + param_type[1]
            elif param_type in self.enums:
                param_type = 'Integer'
        return param_type + '_from_variant'


    def make_method_signature(self, request, response, interface, add_classname):
        in_params = [self.make_param_desc(x, interface) for x in request.findall('param')]
        out_params = [self.make_param_desc(x, interface) for x in response.findall('param')]

        signature_len = len(''.join(map(lambda x: self.convert_to_dbus_type(x), in_params + out_params)))
        if signature_len > 255:
            raise RuntimeError("Too long signature of {0} method. Maximum valid length is 255, actual is {1}", request.get('name'), signature_len)

        return "int {0}{1} ({2}{3}const QDBusMessage& message, QString& userMessage_out{4}{5})".format(
                                        interface + "Adaptor::" if add_classname else "",
                                        request.get('name'),
                                        ", ".join(map(lambda x: "const {0}& {1}_in".format(self.qt_param_type(x), x.name), in_params)),
                                        ", " if in_params else "",
                                        ", " if out_params else "",
                                        ", ".join(map(lambda x: "{0}& {1}_out".format(self.qt_param_type(x), x.name), out_params)))


    def make_signal_signature(self, signal, interface, add_void):
        params = signal.findall('param')
        if add_void:
            retstr = 'void '
        else:
            retstr = ''
        retstr = retstr + signal.get('name') + '('
        params_num = len(params)
        for i in range(0, params_num):
            param_desc = self.make_param_desc(params[i], interface)
            param_type = self.qt_param_type(param_desc)
            retstr = retstr + param_type + ' ' + param_desc.name
            if i <> params_num - 1: retstr = retstr + ", "
        retstr = retstr + ')'
        return retstr


    def make_qml_signal_signature(self, signal, interface, name, short=False, add_classname=False):
        params = signal.findall('param')
        if short:
            retstr = ''
        else:
            retstr = 'void '
        if add_classname:
            retstr = retstr + interface + 'Adaptor::'
        retstr = retstr + name + '('
        params_num = len(params)
        for i in range(0, params_num):
            param_desc = self.make_param_desc(params[i], interface)
            if param_desc.struct or param_desc.array or not param_desc.mandatory: typ = 'QVariant'
            elif param_desc.type == 'Integer' or param_desc.enum: typ = 'int'
            elif param_desc.type == 'Boolean': typ = 'bool'
            elif param_desc.type == 'Float': typ = 'double'
            elif param_desc.type == 'String': typ = 'QString'
            else: typ = 'QVariant'
            retstr = retstr + typ
            if not short: retstr = retstr + ' ' + param_desc.name
            if i <> params_num - 1: retstr = retstr + ", "
        retstr = retstr + ')'
        return retstr


    def write_adaptor_declaration(self, interface_el, notifications, request_responses, out):
        global prefix_class_item
        def glue_strings(strings):
            ret = list()
            curstr = ''
            for str in strings:
                curstr = curstr + str
                if(str[-1] == '>'):
                    ret.append(curstr)
                    curstr = ''
            return ret
        ifacename = interface_el.get('name')
        out.write("class " + ifacename + "Adaptor : public QDBusAbstractAdaptor {\n");
        out.write("  Q_OBJECT\n");
        out.write("  Q_CLASSINFO(\"D-Bus Interface\", \"" + self.interface_path + '.' + ifacename + "\")\n");
        out.write("  Q_CLASSINFO(\"D-Bus Introspection\",\n");
        introspection_el = self.create_introspection_iface_el(interface_el, 'hmi')
        introspection = glue_strings(ElementTree.tostringlist(introspection_el))
        for str in introspection: 
            str = str.replace('"', '\\"')
            out.write('"' + str + '"' + "\n")
        out.write("  )\n")
        out.write(" public:\n")
        out.write("  explicit " + ifacename + "Adaptor(QObject *parent = 0);\n")
        out.write("  void SetApi(Q%sItem*);\n" % prefix_class_item)
        out.write("  DBusController *dbusController;\n")
        out.write(" public slots:\n")
        for (request, response) in request_responses:
            signature = self.make_method_signature(request, response, ifacename, False)
            out.write("  " + signature + ";\n")
        out.write(" signals:\n")
        for n in notifications:
            signature = self.make_signal_signature(n, ifacename, True)
            out.write("  " + signature + ";\n")
        out.write(" private slots:\n")
        for n in notifications:
            signature = self.make_qml_signal_signature(n, ifacename, n.get('name') + '_qml', False)
            out.write("  " + signature + ";\n")
        out.write(" private:\n")
        out.write("  Q%sItem* api_;\n" % prefix_class_item)
        out.write("};\n\n");

    def write_adaptor_definition(self, interface_el, notifications, request_responses, out):
        global prefix_class_item
        iface_name = interface_el.get('name')
        classname = iface_name + 'Adaptor'
        out.write("{0}::{0}(QObject* parent) : QDBusAbstractAdaptor(parent) {{}}\n".format(classname))

        out.write("void {0}::SetApi(Q{1}Item* api) {{\n".format(classname, prefix_class_item))
        with CodeBlock(out) as out:
            out.write("api_ = api;\n")
            for n in notifications:
                signame = n.get('name')
                signame = signame[:1].lower() + signame[1:]
                slotname = n.get('name') + '_qml'
                sig_signature = self.make_qml_signal_signature(n, iface_name, signame, True)
                slot_signature = self.make_qml_signal_signature(n, iface_name, slotname, True)
                out.write("connect(api_, SIGNAL(" + sig_signature + "), this, SLOT(" + slot_signature + "));\n")
            out.write("LOG4CXX_TRACE(logger_, \"CONNECT SIGNALS: \" << __PRETTY_FUNCTION__ );\n")
        out.write("}\n\n")

        for (request,response) in request_responses:
            in_params = [ self.make_param_desc(x, iface_name) for x in request.findall('param') ]
            out_params = [ self.make_param_desc(x, iface_name) for x in response.findall('param') ]

            out.write("bool fill{0}{1}Reply(QDBusMessage& message, const QVariantMap& map) {{\n".format(classname, request.get('name')))
            with CodeBlock(out) as out:
                out.write("int retCode_out = 0;\n")
                out.write("GetArgFromMap(map, \"__retCode\", retCode_out);\n")
                out.write("QVariant retCode_arg = QVariant::fromValue(retCode_out);\n")
                out.write("message << retCode_arg;\n")
                out.write("QString userMessage_out;\n")
                out.write("GetArgFromMap(map, \"__message\", userMessage_out);\n")
                out.write("QVariant userMessage_arg = QVariant::fromValue(userMessage_out);\n")
                out.write("message << userMessage_arg;\n")
                for p in out_params:
                    param_name = p.name
                    param_type = self.qt_param_type(p)
                    out.write("%s %s_out;\n" % (param_type, p.name))
                    out.write("if (!GetArgFromMap(map, \"{0}\", {0}_out)) {{ return false; }}\n".format(p.name))
                    self.write_param_validation(p, p.name + "_out", "return false", out)
                    out.write("QVariant {0}_arg;\n".format(p.name))
                    out.write("{0}_arg.setValue({0}_out);\n".format(p.name))
                    out.write("message << {0}_arg;\n".format(p.name))
                    out.write("LOG4CXX_DEBUG(logger_, \"Output arguments:\\n\" << QVariant(map));\n")
                out.write("LOG4CXX_TRACE(logger_, \"REPLY ASYNC: \" << __PRETTY_FUNCTION__ );\n")
                out.write("return true;\n")
            out.write("}\n\n")
            
            out.write("{0} {{\n".format(self.make_method_signature(request, response, iface_name, True)))
            with CodeBlock(out) as out:
                out.write("LOG4CXX_TRACE(logger_, \"ENTER: \" << __PRETTY_FUNCTION__ );\n")
                out.write("int ret = 0;\n")
                return_statement = "return ret;\n"
                out.write("QVariantMap in_arg;\n");
                out.write("QVariant out_arg_v;\n");
                for param in in_params:
                    self.write_param_validation(param, param.name + "_in", "RaiseDbusError(this, InvalidData); return ret", out)
                    out.write("PutArgToMap(in_arg, \"" + param.name + "\", " + param.name + "_in);\n")
                out.write("LOG4CXX_DEBUG(logger_, \"Input arguments:\\n\" << in_arg);\n")
                method_name = request.get('name')[:1].lower() + request.get('name')[1:]

                out.write("dbusController->message = &message;\n")
                out.write("dbusController->fill = &fill{0}{1}Reply;\n".format(classname, request.get("name")))

                out.write("""if (!QMetaObject::invokeMethod(api_, "{0}", Qt::{1}Connection, Q_RETURN_ARG(QVariant, out_arg_v), Q_ARG(QVariant, QVariant(in_arg)))) {{\n""".format(method_name, invoke_type_connection))
                with CodeBlock(out) as out:
                    out.write("RaiseDbusError(this, InvalidData);\n")
                    out.write("LOG4CXX_ERROR(logger_, \"Can't invoke method " + method_name +"\");\n    ")
                    out.write("return ret;\n")
                out.write("}\n")

                out.write("dbusController->message = NULL;\n")
                out.write("dbusController->fill = NULL;\n")
                out.write("if (message.isDelayedReply()) {\n")
                with CodeBlock(out) as out:
                    out.write("return ret;\n")
                out.write("}\n")

                out.write("QVariantMap out_arg;\n")
                out.write("if (out_arg_v.type() == QVariant::Map) {\n")
                with CodeBlock(out) as out:
                    out.write("out_arg = out_arg_v.toMap();\n")
                out.write("};\n")

                out.write("int err;\n")
                out.write("""if (GetArgFromMap(out_arg, "__errno", err)) { RaiseDbusError(this, err); return ret; }\n""")

                out.write("int async_uid;\n")
                out.write("if (GetArgFromMap(out_arg, \"__async_uid\", async_uid)) {\n")
                with CodeBlock(out) as out:
                    out.write("message.setDelayedReply(true);\n")
                    out.write("dbusController->addMessage(message, &fill%s%sReply, async_uid);\n" % (classname, request.get('name')))
                    out.write("LOG4CXX_TRACE(logger_, \"EXIT ASYNC: \" << __PRETTY_FUNCTION__ );\n")
                    out.write("return ret;\n");
                out.write("}\n\n")

                for param in out_params:
                    out.write("if (!GetArgFromMap(out_arg, \"{0}\", {0}_out)) {{ RaiseDbusError(this, InvalidData); return ret; }}\n".format(param.name))
                    self.write_param_validation(param, param.name + "_out", "RaiseDbusError(this, InvalidData); return ret", out)

                out.write("GetArgFromMap(out_arg, \"__retCode\", ret);\n")
                out.write("GetArgFromMap(out_arg, \"__message\", userMessage_out);\n")
                out.write("LOG4CXX_DEBUG(logger_, \"Output arguments:\\n\" << QVariant(out_arg));\n")
                out.write("LOG4CXX_TRACE(logger_, \"EXIT: \" << __PRETTY_FUNCTION__ );\n")
                out.write("return ret;\n")
            out.write("}\n\n")

        for n in notifications:
            slotname = n.get('name') + '_qml'
            slot_signature = self.make_qml_signal_signature(n, iface_name, slotname, False, True)
            out.write(slot_signature + " {\n")
            params = n.findall('param')
            out.write("  LOG4CXX_TRACE(logger_, \"EMIT SIGNAL: \" << __PRETTY_FUNCTION__ );\n")
            out.write("  LOG4CXX_DEBUG(logger_, \"Arguments:\\n{\"")
            for p in params[0:-1]:
                param = self.make_param_desc(p, iface_name)
                out.write(" << \" " + param.name + ":\" << " + param.name + " << \",\"")
            for p in params[-1:]: # last param without comma in end line
                p = params[-1]
                param = self.make_param_desc(p, iface_name)
                out.write(" << \" " + param.name + ":\" << " + param.name)
            out.write(" << \" }\");\n")
            for p in params:
                param = self.make_param_desc(p, iface_name)
                param_type = self.qt_param_type(param)
                param_name = 'p_' + param.name
                if param.mandatory:
                    if param.array or (param.type not in ['Integer', 'String', 'Float', 'Boolean'] and not param.enum):
                        out.write('  ' + param_type + ' ' + param_name + ";\n")
                        out.write('  if (!VariantToValue(' + param.name + ', ' + param_name + ")) {\n")
                        out.write("    LOG4CXX_ERROR(logger_, \"Can't convert variant to value\");\n")
                        out.write("  return;}\n")
                else:
                    out.write('  ' + param_type + ' ' + param_name + ";\n")
                    out.write('  ' + param_name + '.presence = !' + param.name + ".isNull();\n")
                    out.write('  if (' + param_name + ".presence) {\n")
                    out.write('    if (!VariantToValue(' + param.name + ', ' + param_name + ".val)) {\n")
                    out.write("      LOG4CXX_ERROR(logger_, \"Can't convert variant to value\");\n")
                    out.write("      return;\n    }\n")
                    out.write("  }\n")
            out.write('  emit ' + n.get('name') + '(')
            for i in range(len(params)):
                param = self.make_param_desc(params[i], iface_name)
                basic_type = (param.type in ['Integer', 'String', 'Float', 'Boolean']) or param.enum
                if param.array or (not param.mandatory) or (not basic_type):
                    param_name = 'p_' + param.name
                else:
                    param_name = param.name
                out.write(param_name)
                if i != len(params) - 1: out.write(', ')
            out.write(");\n")
            out.write("}\n\n")


    def make_dbus_adaptor_declarations(self, out):
        for interface_el in self.el_tree.findall('interface'):
            notifications = self.find_notifications_by_provider(interface_el, "hmi")
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "hmi")
            if len(notifications) > 0 or len(request_responses) > 0:
                self.write_adaptor_declaration(interface_el, notifications, request_responses, out)


    def make_dbus_adaptor_definitions(self, out):
        for interface_el in self.el_tree.findall('interface'):
            notifications = self.find_notifications_by_provider(interface_el, "hmi")
            request_responses = self.find_request_response_pairs_by_provider(interface_el, "hmi")
            if len(notifications) > 0 or len(request_responses) > 0:
                self.write_adaptor_definition(interface_el, notifications, request_responses, out)


    def make_dbus_register_metatypes_declaraion(self, out):
        out.write("void RegisterDbusMetatypes();\n")


    def make_dbus_register_metatypes_definition(self, out):
        out.write("void RegisterDbusMetatypes() {\n")
        for (iface, name) in self.structs:
            struct_name = iface + '_' + name
            out.write('qDBusRegisterMetaType<' + struct_name + ">();\n")
            out.write('qDBusRegisterMetaType<OptionalArgument<' + struct_name + "> >();\n")
            out.write('qDBusRegisterMetaType<QList<' + struct_name + "> >();\n")
            out.write('qDBusRegisterMetaType<OptionalArgument<QList<' + struct_name + "> > >();\n")
        out.write("}\n")


    def make_api_adaptors_class(self, out):
        global prefix_class_item
        out.write("struct ApiAdaptors {\n")
        interfaces = self.el_tree.findall('interface')
        def filt(iface):
            return self.find_notifications_by_provider(iface, "hmi") or self.find_request_response_pairs_by_provider(iface, "hmi")
        interfaces = filter(filt, interfaces) 
        for interface_el in interfaces:
            name = interface_el.get('name') + 'Adaptor'
            out.write("  " + name + "* " + name + "_;\n")
        out.write("  ApiAdaptors() :\n")
        for i in range(len(interfaces)):
            name = interfaces[i].get('name') + 'Adaptor'
            out.write("    " + name + "_(NULL)")
            if i <> len(interfaces) - 1: out.write(',')
            out.write("\n")
        out.write("  {}\n")
        out.write("  void Init(QObject* p) {\n")
        for interface_el in interfaces:
            name = interface_el.get('name') + 'Adaptor'
            out.write("    " + name + "_ = new " + name + "(p);\n")
        out.write("  }\n")
        out.write("  void SetApi(QObject* p) {\n")
        for interface_el in interfaces:
            name = interface_el.get('name') + 'Adaptor'
            chname = interface_el.get('name')
            out.write("    " + name + ("_->SetApi(p->findChild<Q%sItem*>(\"" % prefix_class_item) + chname + "\"));\n")
        out.write("  }\n")
        out.write("  void SetDBusController(DBusController* dc) {\n")
        for interface_el in interfaces:
            name = interface_el.get('name') + 'Adaptor'
            chname = interface_el.get('name')
            out.write("    " + name + "_->dbusController = dc;\n")
        out.write("  }\n")
        out.write("};\n\n")



arg_parser = ArgumentParser(description="Generator of Qt to QDbus C++ part")
arg_parser.add_argument('--infile', required=True, help="full name of input file, e.g. applink/src/components/interfaces/QT_HMI_API.xml")
arg_parser.add_argument('--version', required=False, help="Qt version 4.8.5 (default) or 5.5.1")
arg_parser.add_argument('--outdir', required=True, help="path to directory where output files qml_dbus.cc, qml_dbus.h will be saved")
args = arg_parser.parse_args()

if args.version == "4.8.5":
    prefix_class_item = 'Declarative'
    invoke_type_connection = 'Direct'
elif args.version == "5.5.1":
    prefix_class_item = 'Quick'
    invoke_type_connection = 'BlockingQueued'

header_name = 'qml_dbus.h'
source_name = 'qml_dbus.cc'

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

impl = Impl(in_tree_root, 'com.ford.sdl.hmi')

header_out = open(args.outdir + '/' + header_name, "w")
source_out = open(args.outdir + '/' + source_name, "w")

header_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
header_out.write("""/**
 * @file qml_dbus.h
 * @brief Generated QDbus adaptors header file
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
header_out.write("#ifndef SRC_COMPONENTS_DBUS_QML_DBUS_H_\n");
header_out.write("#define SRC_COMPONENTS_DBUS_QML_DBUS_H_\n\n");
header_out.write("#include <QtDBus/QDBusArgument>\n");
header_out.write("#include <QtDBus/QDBusMessage>\n");
header_out.write("#include <QtDBus/QDBusConnection>\n");
header_out.write("#include <QtDBus/QDBusAbstractAdaptor>\n");
header_out.write("#include <QtDBus/QDBusMetaType>\n");
header_out.write("#include <Qt%s/Q%sItem>\n" % (prefix_class_item, prefix_class_item));
header_out.write("#include \"qml_dbus_common.h\"\n\n");
header_out.write("#include \"dbus_controller.h\"\n\n");
impl.make_dbus_type_declarations(header_out)
impl.make_dbus_adaptor_declarations(header_out)
impl.make_dbus_register_metatypes_declaraion(header_out)
impl.make_api_adaptors_class(header_out)
impl.make_dbus_metatype_declarations(header_out)
header_out.write("#endif // #ifndef SRC_COMPONENTS_DBUS_QML_DBUS_H_\n");

source_out.write("// Warning! This file is generated by '%s'. Edit at your own risk.\n" % argv[0])
source_out.write("""/**
 * @file qml_dbus.cc
 * @brief Generated QDbus adaptors source file
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
source_out.write("#include \"" + header_name + "\"\n\n");
source_out.write("#include \"utils/logger.h\"\n")
source_out.write("#ifdef ENABLE_LOG\n")
source_out.write("extern log4cxx::LoggerPtr logger_;\n")
source_out.write("#endif // ENABLE_LOG\n\n")
impl.make_dbus_type_definitions(source_out)
impl.make_dbus_adaptor_definitions(source_out)
impl.make_dbus_register_metatypes_definition(source_out)
