#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  @file ford_xml_parser.py
#  @brief Parser for HMI_API.xml
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

from xml.etree import ElementTree
from collections import OrderedDict

node_name = '/com/ford/hmi'

class ParamDesc:
    pass

class FordXmlParser:
    def __init__(self, in_el_tree, interface_path=None):
        self.interface_path = interface_path
        self.el_tree = in_el_tree
        self.find_enums()
        self.find_structs()

    def find_enums(self):
        self.enums = dict()
        for interface_el in self.el_tree.findall('interface'):
            interface_name = interface_el.get('name')
            for enum_el in interface_el.findall('enum'):
                enum_name = enum_el.get('name')
                self.enums[(interface_name,enum_name)] = enum_el


    def make_param_desc(self, param_el, iface=None):
        param_desc = ParamDesc()
        param_desc.name = param_el.get('name')
        param_desc.type = param_el.get('type')
        param_desc.enum = False
        param_desc.struct = False
        param_desc.fulltype = param_desc.type
        if param_el.get('mandatory') == 'false':
            param_desc.mandatory = False
        else:
            param_desc.mandatory = True
        if param_el.get('array') == 'true':
            param_desc.array = True
        else:
            param_desc.array = False
        param_desc.minValue = param_el.get('minvalue') if param_el.get('minvalue') else 0
        param_desc.maxValue = param_el.get('maxvalue')
        param_desc.minLength = param_el.get('minlength') if param_el.get('minlength') else 0
        param_desc.maxLength = param_el.get('maxlength')
        param_desc.minSize = param_el.get('minsize')
        param_desc.maxSize = param_el.get('maxsize')

	param_desc.restricted = param_desc.minValue != None or \
	                        param_desc.maxValue != None or \
			                param_desc.minLength > 0 or \
			                param_desc.maxLength > 0
	param_desc.restrictedArray = param_desc.array and (param_desc.minSize > 0 or param_desc.maxSize > 0)
        if iface is None:
            return param_desc
        if param_desc.type not in ['Integer', 'String', 'Boolean', 'Float']:
            param_type = param_desc.type.split('.')
            if len(param_type) > 1:
                param_type = (param_type[0], param_type[1])
            else:
                param_type = (iface, param_type[0])
            param_desc.fulltype = param_type
            if param_type in self.enums: param_desc.enum = True
            elif param_type in self.structs: param_desc.struct = True
        return param_desc


    def find_structs(self):
        self.structs = OrderedDict()
        for interface_el in self.el_tree.findall('interface'):
            interface_name = interface_el.get('name')
            for struct_el in interface_el.findall('struct'):
                struct_name = struct_el.get('name')
                self.structs[(interface_name, struct_name)] = []

        for interface_el in self.el_tree.findall('interface'):
            interface_name = interface_el.get('name')
            for struct_el in interface_el.findall('struct'):
                struct_name = struct_el.get('name')
                for param_el in struct_el.findall('param'):
                    param_desc = self.make_param_desc(param_el, interface_name)
                    self.structs[(interface_name, struct_name)].append(param_desc)


    def convert_struct_to_dbus(self, param_type):
        ret = '('
        struct = self.structs[param_type]
        for param in struct:
            ret = ret + self.convert_to_dbus_type(param)
        ret = ret + ')'
        return ret


    def convert_to_dbus_type(self, param):
        if param.type == 'Integer': restype = 'i'
        elif param.type == 'String': restype = 's'
        elif param.type == 'Boolean': restype = 'b'
        elif param.type == 'Float': restype = 'd' # D-Bus double
        elif param.enum: restype = 'i' # D-Bus 32-bit signed int
        elif param.struct: restype = self.convert_struct_to_dbus(param.fulltype)
        else: raise RuntimeError('Unknown type: ' + param.type)
        if param.array: restype = 'a' + restype
        if not param.mandatory: restype = '(b' + restype + ')'
        return restype


    def find_notifications(self, interface_el):
        notifications = list()
        for function_el in interface_el.findall('function[@messagetype="notification"]'):
            notifications.append(function_el)
        return notifications


    def find_notifications_by_provider(self, interface_el, provider):
        notifications = list()
        condition = 'function[@messagetype="notification"][@provider="%s"]' % provider
        for function_el in interface_el.findall(condition):
            notifications.append(function_el)
        return notifications


    def find_request_response_pairs(self, interface_el):
        result = list()
        request_els = interface_el.findall('function[@messagetype="request"]')
        response_els = interface_el.findall('function[@messagetype="response"]')
        for request_el in request_els:
            name = request_el.get('name')
            response_el = next(r for r in response_els if r.get('name') == name)
            result.append((request_el, response_el))
        return result


    def find_request_response_pairs_by_provider(self, interface_el, provider):
        result = list()
        condition = 'function[@messagetype="request"][@provider="%s"]' % provider
        request_els = interface_el.findall(condition)
        response_els = interface_el.findall('function[@messagetype="response"]')
        for request_el in request_els:
            name = request_el.get('name')
            response_el = next(r for r in response_els if r.get('name') == name)
            result.append((request_el, response_el))
        return result


    def convert_to_signal(self, notification_el, interface):
        result = ElementTree.Element('signal')
        result.set('name', notification_el.get('name'))
        for param_el in notification_el.findall('param'):
            self.create_arg_element(result, param_el, interface)
        return result


    def convert_to_method(self, (request_el, response_el), interface):
        result = ElementTree.Element('method')
        result.set('name', request_el.get('name'))
        for param_el in request_el.findall('param'):
            arg_el = self.create_arg_element(result, param_el, interface)
            arg_el.set('direction', 'in')

        arg_el = ElementTree.SubElement(result, 'arg')
        arg_el.set('name', 'retCode')
        arg_el.set('type', 'i')
        arg_el.set('direction', 'out')
        
        for param_el in response_el.findall('param'):
            arg_el = self.create_arg_element(result, param_el, interface)
            arg_el.set('direction', 'out')
        return result


    def create_arg_element(self, parent, param_el, interface):
        arg_el = ElementTree.SubElement(parent, 'arg')
        arg_el.set('name', param_el.get('name'))
        arg_el.set('type', self.convert_to_dbus_type(self.make_param_desc(param_el, interface)))
        return arg_el


    def create_introspection_iface_el(self, interface_el, provider):
        interface = interface_el.get('name')
        interface_name = self.interface_path + '.' + interface

        notifications = self.find_notifications_by_provider(interface_el, provider)
        signals = [self.convert_to_signal(n, interface) for n in notifications]

        request_responses = self.find_request_response_pairs_by_provider(interface_el, provider)
        methods = [self.convert_to_method(r, interface) for r in request_responses]

        if signals or methods:
            el = ElementTree.Element('interface', attrib={'name':interface_name})
            for m in methods: el.append(m)
            for s in signals: el.append(s)
            return el



