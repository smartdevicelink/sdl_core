from xml.etree import ElementTree


class ParamDesc:
    pass


class FordXmlParser:
    def __init__(self, in_el_tree):
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


    def make_param_desc(self, param_el):
        param_desc = ParamDesc()
        param_desc.name = param_el.get('name')
        param_desc.type = param_el.get('type')
        if param_el.get('mandatory') == 'false':
            param_desc.mandatory = False
        else:
            param_desc.mandatory = True
        if param_el.get('array') == 'true':
            param_desc.array = True
        else:
            param_desc.array = False
        return param_desc


    def find_structs(self):
        self.structs = dict()
        for interface_el in self.el_tree.findall('interface'):
            interface_name = interface_el.get('name')
            for struct_el in interface_el.findall('struct'):
                struct_name = struct_el.get('name')
                struct = list()
                for param_el in struct_el.findall('param'):
                    param_desc = self.make_param_desc(param_el)
                    struct.append(param_desc)
                self.structs[(interface_name, struct_name)] = struct


    def convert_struct_to_dbus(self, param_type):
        ret = '('
        struct = self.structs[param_type]
        for param in struct:
            ret = ret + self.convert_to_dbus_type(param, param_type[0])
        ret = ret + ')'
        return ret


    def convert_to_dbus_type(self, param, interface):
        if param.type == 'Integer': restype = 'i'
        elif param.type == 'String': restype = 's'
        elif param.type == 'Boolean': restype = 'b'
        elif param.type == 'Float': restype = 'd' # D-Bus double
        else:
            param_type = param.type.split('.')
            if len(param_type) > 1:
                param_type = (param_type[0], param_type[1])
            else:
                param_type = (interface, param_type[0])

            if param_type in self.enums: restype = 'i' # D-Bus 32-bit signed int
            elif param_type in self.structs: restype = self.convert_struct_to_dbus(param_type)
            else: raise RuntimeError('Unknown type: ' + param.type)

        if param.array: restype = 'a' + restype
        if not param.mandatory: restype = '(b' + restype + ')'
        return restype

