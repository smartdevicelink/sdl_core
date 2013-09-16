from argparse import ArgumentParser
from xml.etree import ElementTree


namespace_name = 'ford_message_descriptions'
namespace = namespace_name + '::'

class ParamDesc:
    pass


def find_enums(el_tree):
    enums = dict()
    for interface_el in el_tree.findall('interface'):
        interface_name = interface_el.get('name')
        for enum_el in interface_el.findall('enum'):
            enum_name = enum_el.get('name')
            enums[(interface_name,enum_name)] = enum_el
    return enums


def make_param_desc(param_el):
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


def find_structs(el_tree):
    structs = dict()
    for interface_el in el_tree.findall('interface'):
        interface_name = interface_el.get('name')
        for struct_el in interface_el.findall('struct'):
            struct_name = struct_el.get('name')
            struct = list()
            for param_el in struct_el.findall('param'):
                param_desc = make_param_desc(param_el)
                struct.append(param_desc)
            structs[(interface_name, struct_name)] = struct
    return structs


def write_param_definition(param_var_name, param, out, structs, enums, interface):
    if param.type in ['Integer', 'String', 'Boolean', 'Float']:
        param_type = param.type
    else:
        param_type = param.type.split('.')
        if len(param_type) > 1:
            param_type = (param_type[0], param_type[1])
        else:
            param_type = (interface, param_type[0])

    if param_type in structs: out.write('const ' + namespace + 'StructDescription ')
    elif param_type in enums: out.write('const ' + namespace + 'EnumDescription ')
    else: out.write('const ' + namespace + 'ParameterDescription ')

    out.write(param_var_name + " = {\n")

    if param_type in structs or param_type in enums:
        out.write("  {\n")
        shift = ' ' * 4
    else:
        shift = ' ' * 2

    out.write(shift + "\"" + param.name + "\",\n")

    if param_type in structs:
        out.write(shift + namespace + "Struct,\n")
    elif param_type in enums:
        out.write(shift + namespace + "Enum,\n")
    else:
        out.write(shift + namespace + param_type + ",\n")

    if param.array:
        out.write(shift + "true,\n")
    else:
        out.write(shift + "false,\n")

    if param.mandatory:
        out.write(shift + "true\n")
    else:
        out.write(shift + "false\n")

    if param_type in structs or param_type in enums:
        out.write("  },\n")
        if param_type in structs:
            out.write("  Structs::" + param_type[0] + "__" + param_type[1] + "__parameters\n");
        elif param_type in enums:
            out.write("  Enums::" + param_type[0] + "__" + param_type[1] + "__entries\n");

    out.write("};\n")


def write_struct_params_declarations(structs, out):
    out.write("struct Structs {\n");
    for (interface, name), params in structs.iteritems():
        params_var_name = interface + '__' + name + '__parameters'
        out.write("  static const " + namespace + "ParameterDescription* " + params_var_name + "[];\n")
    out.write("};\n\n")


def write_enum_entries_declarations(enums, out):
    out.write("struct Enums {\n");
    for interface, name in enums:
        entries_var_name = interface + '__' + name + '__entries'
        out.write("  static const " + namespace + "EnumDescription::Entry* " + entries_var_name + "[];\n")
    out.write("};\n\n")


def write_parameters(params, out, name, structs, enums, interface):
    n = 1
    for param in params:
        param_var_name = name + str(n)
        n = n + 1
        write_param_definition(param_var_name, param, out, structs, enums, interface)


def write_struct_params_definitions(structs, enums, out):
    for (interface, name), params in structs.iteritems():
        param_var_name = interface + '__' + name + '__parameter'
        write_parameters(params, out, param_var_name, structs, enums, interface)
        params_var_name = 'Structs::' + interface + '__' + name + '__parameters'
        out.write("const " + namespace + "ParameterDescription* " + params_var_name + "[] = {\n")
        for n in range(1, len(params) + 1):
            name = param_var_name + str(n)
            out.write("  (const " + namespace + "ParameterDescription*)&" + name + ",\n")
        out.write(  "  NULL };\n\n")


def write_enum_entries_definitions(enums, out):
    for (interface, name), enum_el in enums.iteritems():
        n = 1
        for element_el in enum_el.findall('element'):
            entry_var_name = interface + '__' + name + '__entry' + str(n)
            value = element_el.get("value")
            if value is None:
                value = str(n)
            out.write(namespace + "EnumDescription::Entry " + entry_var_name + " = {\"" + element_el.get("name") + "\", " + value + "};\n")
            n = n + 1

        entries_var_name = "Enums::" + interface + '__' + name + '__entries'
        out.write("const " + namespace + "EnumDescription::Entry* " + entries_var_name + "[] = {\n")
        for n in range(1, len(enum_el.findall('element')) + 1):
            entry_var_name = interface + '__' + name + '__entry' + str(n)
            out.write("  &" + entry_var_name + ",\n")
        out.write("  NULL };\n\n")


def write_message_definition(message_el, interface, out, structs, enums):
    name = message_el.get('name')
    messagetype = message_el.get('messagetype')
    params = list()
    for param_el in message_el.findall('param'):
        param_desc = make_param_desc(param_el)
        params.append(param_desc)
    param_var_name = interface + '__' + name + '__' + messagetype + '__parameter'
    write_parameters(params, out, param_var_name, structs, enums, interface)

    params_var_name = param_var_name + 's'
    out.write("const " + namespace + "ParameterDescription* " + params_var_name + "[] = {\n")
    for n in range(1, len(params) + 1):
        param_name = param_var_name + str(n)
        out.write("  (const " + namespace + "ParameterDescription*)&" + param_name + ",\n")
    out.write("  NULL };\n")

    message_desc_name = interface + '__' + name + '__' + messagetype
    out.write("const " + namespace + "MessageDescription " + message_desc_name + " = {\n")
    out.write("  \"" + interface + "\",\n")
    out.write("  \"" + name + "\",\n")
    out.write("  hmi_apis::messageType::" + messagetype + ",\n")
    out.write("  hmi_apis::FunctionID::" + interface + "_" + name + ",\n")
    out.write("  " + params_var_name + "\n")
    out.write("};\n\n")


def write_message_definitions(el_tree, out, structs, enums):
    for interface_el in el_tree.findall('interface'):
        interface = interface_el.get('name')
        message_els = interface_el.findall('function')
        for message_el in message_els:
            write_message_definition(message_el, interface, out, structs, enums)


def make_message_descriptions(in_el_tree, out):
    structs = find_structs(in_el_tree)
    enums = find_enums(in_el_tree)
    write_struct_params_declarations(structs, out)
    write_enum_entries_declarations(enums, out)
    write_struct_params_definitions(structs, enums, out)
    write_enum_entries_definitions(enums, out)
    write_message_definitions(in_el_tree, out, structs, enums)


def make_message_array(el_tree, out):
    out.write("const MessageDescription* message_descriptions[] = {\n")
    for interface_el in el_tree.findall('interface'):
        interface = interface_el.get('name')
        message_els = interface_el.findall('function')
        for message_el in message_els:
            name = message_el.get('name')
            messagetype = message_el.get('messagetype')
            message_desc_name = interface + '__' + name + '__' + messagetype
            out.write("  &" + message_desc_name + ",\n")
    out.write("  NULL\n")
    out.write("};\n")


arg_parser = ArgumentParser()
arg_parser.add_argument('--infile', required=True)
arg_parser.add_argument('--outfile', required=True)
args = arg_parser.parse_args()

in_tree = ElementTree.parse(args.infile)
in_tree_root = in_tree.getroot()

out = open(args.outfile, "w")

out.write("#include \"dbus/message_descriptions.h\"\n\n");

out.write("namespace {\n\n");
make_message_descriptions(in_tree_root, out)
out.write("}\n\n");

out.write("namespace " + namespace_name + " {\n\n");
make_message_array(in_tree_root, out)
out.write("}\n\n");

