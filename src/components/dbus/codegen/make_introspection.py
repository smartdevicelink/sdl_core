from argparse import ArgumentParser
from ford_xml_parser import FordXmlParser
from ford_xml_parser import node_name
from xml.etree import ElementTree
from os import popen


class Impl(FordXmlParser):
    def convert_to_introspection(self, out_el_tree):
        for interface_el in self.el_tree.findall('interface'):
            el = self.create_introspection_iface_el(interface_el)
            if el is not None:
                out_el_tree.append(el)


arg_parser = ArgumentParser()
arg_parser.add_argument('--infile', required=True)
arg_parser.add_argument('--outfile', required=True)
arg_parser.add_argument('--interfacepath', required=True)
args = arg_parser.parse_args()

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()
out_tree_root = ElementTree.Element('node', attrib={'name':node_name})
out_tree = ElementTree.ElementTree(out_tree_root)

#D-bus functions
el = ElementTree.SubElement(out_tree_root, 'interface', attrib={'name':'org.freedesktop.DBus.Introspectable'})
el = ElementTree.SubElement(el, 'method', attrib={'name':'Introspect'})
el = ElementTree.SubElement(el, 'arg', attrib={'type':'s','name':'xml_data','direction':'out'})

impl = Impl(in_tree_root, args.interfacepath)
impl.convert_to_introspection(out_tree_root)

out = popen("xmllint --format --output " + args.outfile + " -", "w")
out.write('<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">')
out_tree.write(out)

