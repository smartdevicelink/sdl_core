from argparse import ArgumentParser
from ford_xml_parser import FordXmlParser
from xml.etree import ElementTree
from os import popen


node_name = '/com/ford/hmi'
interface_name_prefix = 'com.ford.hmi.'


class Impl(FordXmlParser):
    def find_notifications(self, interface_el):
        notifications = list()
        for function_el in interface_el.findall('function[@messagetype="notification"]'):
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
        for param_el in response_el.findall('param'):
            arg_el = self.create_arg_element(result, param_el, interface)
            arg_el.set('direction', 'out')
        return result


    def create_arg_element(self, parent, param_el, interface):
        arg_el = ElementTree.SubElement(parent, 'arg')
        arg_el.set('name', param_el.get('name'))
        arg_el.set('type', self.convert_to_dbus_type(self.make_param_desc(param_el), interface))
        return arg_el


    def convert_to_introspection(self, out_el_tree):
        for interface_el in self.el_tree.findall('interface'):
            interface = interface_el.get('name')
            interface_name = interface_name_prefix + interface

            notifications = self.find_notifications(interface_el)
            signals = [self.convert_to_signal(n, interface) for n in notifications]

            request_responses = self.find_request_response_pairs(interface_el)
            methods = [self.convert_to_method(r, interface) for r in request_responses]

            if signals or methods:
                el = ElementTree.SubElement(out_el_tree, 'interface', attrib={'name':interface_name})
                for m in methods: el.append(m)
                for s in signals: el.append(s)


arg_parser = ArgumentParser()
arg_parser.add_argument('--infile', required=True)
arg_parser.add_argument('--outfile', required=True)
args = arg_parser.parse_args()

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()
out_tree_root = ElementTree.Element('node', attrib={'name':node_name})
out_tree = ElementTree.ElementTree(out_tree_root)

impl = Impl(in_tree_root)
impl.convert_to_introspection(out_tree_root)

out = popen("xmllint --format --output " + args.outfile + " -", "w")
out.write('<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">')
out_tree.write(out)

