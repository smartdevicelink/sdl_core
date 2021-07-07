"""SmartFactory code generator base.

Base of code generator for SmartFactory that provides SmartSchema object in
accordance with given internal model.

"""
# pylint: disable=W0402
# pylint: disable=C0302
import codecs
import os
import string
import uuid
import re

from model.enum import Enum
from model.enum_element import EnumElement
from model.function import Function


class GenerateError(Exception):

    """Generate error.

    This exception is raised when generator is unable to create
    output from given model.

    """

    pass

def to_camel_case(snake_str):
    components = snake_str.split('_')
    return components[0].lower() + "".join(x.title() for x in components[1:])

def to_snake_case(camel_str):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', camel_str)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1)

class CodeGenerator(object):

    """Base PolicyTypes generator.

    This class provides service which allows to generate *.h files with types required for Policy

    """

    def __init__(self):
        """Construct new object."""

        self._generated_structs = []
        self._structs_add_code = u""

    def generate_vehicledatatype_names(self, enum):
        template = string.Template('''static std::set<std::string> ${enum_name}_enum_names = { \n${values}\n};\n''')
        enum_element_name = lambda enum_element: enum_element.internal_name if enum_element.internal_name else enum_element.name
        values = ", \n".join(["  \"{}\"".format(enum_element_name(enum_element)) for enum_element in enum.elements.values()])
        return template.substitute(enum_name=enum.name, values = values)

    def generate_all_enum_values(self, enum):
        template = string.Template('''static std::set<mobile_apis::${enum_name}::eType> ${enum_name}_all_enum_values = { \n${values}\n};\n''')
        enum_element_name = lambda enum_element: enum_element.internal_name if enum_element.internal_name else enum_element.name
        values = ", \n".join(["  mobile_apis::{}::{}".format(enum.name, enum_element_name(enum_element)) for enum_element in enum.elements.values()])
        return template.substitute(enum_name=enum.name, values = values)

    def generate_enums_schema_factory_content(self, enums):
        return "\n".join([self._enum_factory_element_template.substitute(enum_name = enum.name) for enum in enums])

    def generate_enums_schema_factory_impl(self, enums):
        enums_values = ""
        for enum in enums:
            enums_values += "\n".join([self.generate_all_enum_values(enum)])
            if enum.name == "VehicleDataType":
                enums_values += "\n".join([self.generate_vehicledatatype_names(enum)])
        return self._enum_factory_template.substitute(enums_values = enums_values,
                                                      enum_factory_content = self.generate_enums_schema_factory_content(enums))

    def generate_POD_types_schema_factory_impl(self, enums):
        pod_types = {
        "Integer" : "int32_t",
        "Float" : "double",
        }
        '''if (enum_name == "DriverDistractionState") {
                            return Get<mobile_apis::DriverDistractionState::eType>(DriverDistractionState_all_enum_values);
                          }'''
        enums_values = "\n".join([self.generate_all_enum_values(enum) for enum in enums])
        return self._enum_factory_template.substitute(enums_values = enums_values,
                                                      enum_factory_content = self.generate_enums_schema_factory_content(enums))

    def generate(self, interface, filename, namespace, destination_dir):
        """ Generate SmartFactory source files.

        Generates source code files at destination directory in
        accordance with given model in specified namespace.

        Keyword arguments:
        interface -- model of the interface to generate source code for.
        filename -- name of initial XML file.
        namespace -- name of destination namespace.
        destination_dir -- directory to create source files.

        """

        if interface is None:
            raise GenerateError("Given interface is None.")

        params_set = set()
        for func in interface.functions.values():
            for param in func.params:
                params_set.add(param)
        parameter_enum = Enum('Parameter')
        
        for item in params_set:
            parameter_enum.elements[item.upper()] = EnumElement(item)
                

        required_enums_for_policy = [
            "HMILevel",
            "FunctionID",
            "HybridAppPreference",
            "AppHMIType",
            "RequestType",
            "ModuleType",
            "Common_AppPriority",
            "Parameter"
        ]

        self.required_empty_value = [
            "RequestType",
            "ModuleType",
            "Parameter"
        ]

        self.enum_items_naming_conversion_ = {
            "HMILevel" :  lambda item_name : "HL_" + item_name.replace("HMI_", ""),
            "AppHMIType" : lambda item_name : "AHT_" + item_name,
            "FunctionID" : lambda item_name : item_name,
            "HybridAppPreference" : lambda item_name : item_name,
            "RequestType" : lambda item_name : "RT_" + item_name,
            "ModuleType" : lambda item_name : "MT_" + item_name,
            "Common_AppPriority" :  lambda item_name : "P_" + item_name if not item_name == "VOICE_COMMUNICATION" else "P_VOICECOM",
            "Parameter" :  lambda item_name : "P_" + to_snake_case(item_name).upper()
        }

        self.enum_items_string_naming_conversion_ = {
            "HMILevel" :  lambda item_name : item_name,
            "AppHMIType" : lambda item_name : item_name,
            "FunctionID" : lambda item_name : item_name[:item_name.rfind("ID")],
            "HybridAppPreference" : lambda item_name : item_name,
            "RequestType" : lambda item_name : item_name,
            "ModuleType" : lambda item_name : item_name,
            "Common_AppPriority" :  lambda item_name : item_name if not item_name == "VOICE_COMMUNICATION" else "VOICECOM",
            "Parameter" :  lambda item_name : item_name
        }

        self.enum_naming_conversion_ = {
            "HMILevel" : "HmiLevel",
            "AppHMIType" : "AppHMIType",
            "FunctionID" : "FunctionID",
            "HybridAppPreference" : "HybridAppPreference",
            "RequestType" : "RequestType",
            "ModuleType" : "ModuleType",
            "Common_AppPriority" : "Priority",
            "Parameter" : "Parameter"
        }


        # In case if "." is in FunctionID name this is HMI_API function ID and should not be included in Policy  enums

        required_enum_values = list(filter(lambda e: e.name in required_enums_for_policy
                                    and "." not in list(e.elements.values())[0].name, list(interface.enums.values())))

        if filename == "MOBILE_API":
            self._write_cc_with_enum_schema_factory(filename, namespace, destination_dir, interface.enums.values())

            # Params should be generated as enum for MOBILE_API to validate RPCSpec parameters
            required_enum_values.append(parameter_enum)

        self._write_header_with_enums(filename, namespace, destination_dir, required_enum_values)
        self._write_cc_with_enums(filename, namespace, destination_dir, required_enum_values)

    def _write_cc_with_enum_schema_factory(self,filename, namespace, destination_dir, required_enum_values):
        class_name = u"generated_{0}_enum_schema_factory".format(filename)
        header_file_name = '''policy/policy_table/policy_enum_schema_factory.h'''
        cc_file_name = u"".join("{0}.cc".format(class_name))
        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)
        print(os.path.join(destination_dir, cc_file_name))
        with codecs.open(os.path.join(destination_dir, cc_file_name),
                         encoding="utf-8",
                         mode="w") as f_cc:
            guard = u"_{0}_{1}_CC__".format( class_name.upper(),
                    uuid.uuid1().hex.capitalize())
            namespace_open, namespace_close = self._namespaces_strings(namespace)
            includes = '''#include <set>\n'''\
            '''#include "interfaces/MOBILE_API.h"\n'''\
            '''#include "smart_objects/enum_schema_item.h"\n'''
            f_cc.write(self._cc_file_template.substitute(
                class_name=class_name,
                header_file=header_file_name,
                includes = includes,
                namespace_open=namespace_open,
                enums_content=self.generate_enums_schema_factory_impl(required_enum_values),
                namespace_close=namespace_close))

    def _write_header_with_enums(self,filename, namespace, destination_dir, required_enum_values):
        class_name = u"generated_{0}_policy_types".format(filename)
        header_file_name = u"".join("{0}.h".format(class_name))
        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)
        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            guard = u"_{0}_{1}_H__".format( class_name.upper(),
                    uuid.uuid1().hex.capitalize())
            namespace_open, namespace_close = self._namespaces_strings(namespace)
            f_h.write(self._h_file_template.substitute(
                class_name=class_name,
                guard=guard,
                includes="",
                namespace_open=namespace_open,
                enums_content=self._gen_enums(required_enum_values
                    ),
                namespace_close=namespace_close))

    def _write_cc_with_enums(self,filename, namespace, destination_dir, required_enum_values):
        class_name = u"generated_{0}_policy_types".format(filename)
        header_file_name = u"".join("{0}.h".format(class_name))
        cc_file_name = u"".join("{0}.cc".format(class_name))
        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)
        with codecs.open(os.path.join(destination_dir, cc_file_name),
                         encoding="utf-8",
                         mode="w") as f_cc:
            guard = u"_{0}_{1}_CC__".format( class_name.upper(),
                    uuid.uuid1().hex.capitalize())
            namespace_open, namespace_close = self._namespaces_strings(namespace)
            f_cc.write(self._cc_file_template.substitute(
                class_name=class_name,
                header_file=header_file_name,
                includes="",
                namespace_open=namespace_open,
                enums_content=self.gen_enums_processing(required_enum_values
                    ),
                namespace_close=namespace_close))

    def _namespaces_strings(self, namespace):

        """ Generate open and close namespaces strings

        Generates source code for opening and close namespaces

        Keyword arguments:
        namespace -- name of destination namespace.

        Returns:
        Tuple with namespace open string and namespace close string
        """

        namespace_open = u""
        namespace_close = u""
        if namespace:
            parts = namespace.split(u"::")
            for part in parts:
                namespace_open = u"".join(
                    [namespace_open,
                     self._namespace_open_template.substitute(name=part)])
                namespace_close = u"".join(
                    [namespace_close,
                     "}} // {0}\n".format(part)])

        return namespace_open, namespace_close


    def _gen_enums(self, enums):
        """Generate enums for header file.

        Generates declaration of enumerations for the header file.

        Keyword arguments:
        enums -- list of enums to generate.

        Returns:
        String with enums declaration source code.

        """

        if enums is None:
            raise GenerateError("Enums is None")

        return u"\n".join([self._gen_enum(x) for x in enums])

    def _gen_enum(self, enum):
        """Generate enum for header file.

        Generates declaration of enumeration for the header file.

        Keyword arguments:
        enum -- enum to generate.

        Returns:
        String with enum declaration source code.

        """

        enum_elements = enum.elements.values()
        return self._enum_template.substitute(
            comment=self._gen_comment(enum),
            name=self.enum_naming_conversion_[enum.name],
            enum_items=self._indent_code(self._gen_enum_elements(
                enum_elements, enum.name), 1))

    def _gen_enum_elements(self, enum_elements, enum_name):
        """Generate enum elements for header file.

        Generates declaration of enumeration elements for the header file.

        Keyword arguments:
        enum_elements -- list of enum elements to generate.

        Returns:
        String with enum elements declaration source code.

        """
        enum_elements = [self._gen_enum_element(x, enum_name)
                              for x in enum_elements]
        if (enum_name in self.required_empty_value):
            enum_elements.append(self._gen_empty_enum_element(enum_name))
        return u",\n\n".join(enum_elements)

    def _gen_empty_enum_element(self, enum_name) :
        return self._enum_element_with_value_template.substitute(
                comment="",
                name=self.enum_items_naming_conversion_[enum_name]("EMPTY"),
                value=-1)

    def _gen_enum_element(self, enum_element, enum_name):
        """Generate enum element for header file.

        Generates declaration of enumeration element for the header file.

        Keyword arguments:
        enum_element -- enum element to generate.

        Returns:
        String with enum element declaration source code.

        """

        if enum_element.value is not None:
            return self._enum_element_with_value_template.substitute(
                comment=self._gen_comment(enum_element),
                name=self.enum_items_naming_conversion_[enum_name](enum_element.primary_name),
                value=enum_element.value)
        else:
            return self._enum_element_with_no_value_template.substitute(
                comment=self._gen_comment(enum_element),
                name=self.enum_items_naming_conversion_[enum_name](enum_element.primary_name))

    def gen_enums_processing(self, enums):
        validation = "\n".join([self._gen_enum_validation(enum) for enum in enums])
        to_json = "\n".join([self._gen_enum_to_json(enum) for enum in enums])
        from_json = "\n".join([self._gen_enum_from_json(enum) for enum in enums])
        return "\n".join([validation, to_json, from_json])

    def _gen_enum_validation(self, enum):
        return self._valiation_enum_template.substitute(
            name = self.enum_naming_conversion_[enum.name],
            enum_items = "\n".join([self._gen_enum_item_validation(enum_item.name, enum.name) for enum_item in enum.elements.values()])
        )

    def _gen_enum_item_validation(self, item_name, enum_name):
        return self._valiation_enum_item_template.substitute(
            name = self.enum_items_naming_conversion_[enum_name](item_name))

    def _gen_enum_to_json(self, enum):
        name = self.enum_naming_conversion_[enum.name]
        enum_items = "\n".join([self._gen_enum_item_to_json(enum_item.name, enum.name) for enum_item in enum.elements.values()])
        if (enum.name in self.required_empty_value):
            enum_items+= "\n" + self._gen_enum_item_to_json("EMPTY", enum.name)
        return self._enum_to_json_template.substitute(
            name = name,
            enum_items = enum_items
        )

    def _gen_enum_item_to_json(self, item_name, enum_name):
        return self._enum_to_json_item_template.substitute(
            name =  self.enum_items_naming_conversion_[enum_name](item_name),
            string_name = self.enum_items_string_naming_conversion_[enum_name](item_name)
            )


    def _gen_enum_from_json(self, enum):
        name = self.enum_naming_conversion_[enum.name]
        enum_items = "\n".join([self._gen_enum_item_from_json(enum_item.name, enum.name) for enum_item in enum.elements.values()])
        if (enum.name in self.required_empty_value):
            enum_items += "\n" + self._gen_enum_item_from_json("EMPTY", enum.name)
        return self._enum_from_json_template.substitute(
            name = name,
            enum_items = enum_items
        )

    def _gen_enum_item_from_json(self, item_name, enum_name):
        return self._enum_from_json_item_template.substitute(
            name = self.enum_items_naming_conversion_[enum_name](item_name),
            string_name = self.enum_items_string_naming_conversion_[enum_name](item_name)
            )

    def _gen_comment(self, interface_item_base, use_doxygen=True):
        """Generate doxygen comment for iterface_item_base for header file.

        Generates doxygen comment for any iterface_item_base for the header
        file.

        Keyword arguments:
        interface_item_base -- object to generate doxygen comment for.
        use_doxygen -- Flag that indicates does function uses doxygen or not.

        Returns:
        String with generated doxygen comment.

        """

        brief_type_title = None
        interface_item_base_classname = interface_item_base.__class__.__name__
        if interface_item_base_classname in self._model_types_briefs:
            brief_type_title = \
                self._model_types_briefs[interface_item_base_classname]
        else:
            raise GenerateError("Unable to create comment for unknown type " +
                                interface_item_base_classname)

        name = interface_item_base.primary_name if \
            type(interface_item_base) is EnumElement else \
            interface_item_base.name
        brief_description = (u" * @brief {0}{1}.\n" if use_doxygen is
                             True else u"// {0}{1}.\n").format(
                                 brief_type_title,
                                 name)

        description = u"".join([(u" * {0}\n" if use_doxygen
                                is True else u"// {0}\n").format(x)
                                for x in self._normalize_multiline_comments(
                                    interface_item_base.description)])
        if description != u"":
            description = u"".join([u" *\n" if use_doxygen
                                    is True else u"//\n", description])

        design_description = u"".join([(u" * {0}\n" if use_doxygen is
                                       True else u"// {0}\n").format(x)
                                       for x in
                                       self._normalize_multiline_comments(
                                           interface_item_base.
                                           design_description)])
        if design_description != u"":
            design_description = u"".join([u" *\n" if use_doxygen is
                                           True else "//\n",
                                           design_description])

        issues = u"".join([(u" * @note {0}\n" if use_doxygen is
                           True else u"// Note: {0}\n").format(x)
                           for x in self._normalize_multiline_comments(
                               [x.value for x in interface_item_base.issues])])
        if issues != u"":
            issues = u"".join([u" *\n" if use_doxygen is
                              True else u"//\n", issues])

        todos = u"".join([(u" * @todo {0}\n" if use_doxygen is
                          True else u"// ToDo: {0}\n").format(x)
                          for x in self._normalize_multiline_comments(
                              interface_item_base.todos)])
        if todos != u"":
            todos = u"".join([u" *\n" if use_doxygen is
                              True else u"//\n", todos])

        returns = u""
        if type(interface_item_base) is Function:
            returns = u"".join([u" *\n", self._function_return_comment])

        template = self._comment_doxygen_template if use_doxygen is \
            True else self._comment_cc_template

        return template.substitute(
            brief_description=brief_description,
            description=description,
            design_description=design_description,
            issues=issues,
            todos=todos,
            returns=returns)

    def _indent_code(self, code, indent_level):
        """Indent given source code.

        Indents given source code right by given indentation level.

        Keyword arguments:
        code -- given source code.
        indent_level -- desired indentation level.

        Returns:
        String with processed code.

        """

        code_lines = code.split("\n")
        return u"".join(
            [u"{0}{1}\n".format(
                self._indent_template * indent_level,
                x) if x != u"" else u"\n" for x in code_lines])

    @staticmethod
    def _normalize_multiline_comments(initial_strings):
        """Normalize multiline comments.

        Makes multiline comment clean of any line breaks creating additional
        strings for the comment.

        Keyword arguments:
        initial_strings -- initial list of strings to process.

        Returns:
        New list of the strings (with contains no strings with line breaks).

        """

        result = []
        for initial_string in initial_strings:
            result = result + initial_string.splitlines()
        return result

    _model_types_briefs = dict(
        {u"EnumElement": u"",
         u"Enum": u"Enumeration ",
         u"Function": u"Method that generates schema for function ",
         u"Struct": u"Method that generates schema item for structure ",
         u"Param": u"Struct member ",
         u"FunctionParam": u"Function parameter "})

    _cc_file_template = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.h\n'''
        u''' * @brief Generated class ${class_name} source file.\n'''
        u''' *\n'''
        u'''*/\n'''
        u'''#include "${header_file}"\n'''
        u'''${includes}'''
        u'''\n\n'''
        u'''$namespace_open'''
        u'''\n'''
        u'''$enums_content'''
        u'''\n'''
        u'''$namespace_close'''
        u'''\n\n''')


    _h_file_template = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.h\n'''
        u''' * @brief Generated class ${class_name} header file.\n'''
        u''' *\n'''
        u'''* Copyright (c) 2017, Ford Motor Company\n'''
        u'''* All rights reserved.\n'''
        u'''*\n'''
        u'''* Redistribution and use in source and binary forms, with or without\n'''
        u'''* modification, are permitted provided that the following conditions are met:\n'''
        u'''*\n'''
        u'''* Redistributions of source code must retain the above copyright notice, this\n'''
        u'''* list of conditions and the following disclaimer.\n'''
        u'''*\n'''
        u'''* Redistributions in binary form must reproduce the above copyright notice,\n'''
        u'''* this list of conditions and the following\n'''
        u'''* disclaimer in the documentation and/or other materials provided with the\n'''
        u'''* distribution.\n'''
        u'''*\n'''
        u'''* Neither the name of the Ford Motor Company nor the names of its contributors\n'''
        u'''* may be used to endorse or promote products derived from this software\n'''
        u'''* without specific prior written permission.\n'''
        u'''*\n'''
        u'''* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"\n'''
        u'''* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n'''
        u'''* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n'''
        u'''* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n'''
        u'''* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n'''
        u'''* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n'''
        u'''* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n'''
        u'''* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n'''
        u'''* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n'''
        u'''* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n'''
        u'''* POSSIBILITY OF SUCH DAMAGE.\n'''
        u'''*/\n'''
        u'''\n'''
        u'''#ifndef $guard\n'''
        u'''#define $guard\n'''
        u'''#include <string>\n'''
        u'''${includes}'''
        u'''\n'''
        u'''$namespace_open'''
        u'''$enums_content'''
        u'''$namespace_close'''
        u'''#endif //$guard\n'''
        u'''\n\n''')


    _namespace_open_template = string.Template(
        u'''namespace $name {\n''')

    _comment_doxygen_template = string.Template(
        u'''/**\n'''
        u'''$brief_description'''
        u'''$description'''
        u'''$design_description'''
        u'''$issues'''
        u'''$todos'''
        u'''$returns */''')

    _enum_template = string.Template(
        u'''$comment\n'''
        u'''enum $name {\n'''
        u'''$enum_items};\n'''
        u'''bool IsValidEnum($name val);\n'''
        u'''const char* EnumToJsonString($name val);\n'''
        u'''bool EnumFromJsonString(const std::string& literal, $name* result);\n'''
        )
    _valiation_enum_template = string.Template(
        u'''bool IsValidEnum($name val) {\n'''
        u'''  return !(std::string(EnumToJsonString(val)).empty());\n'''
        u'''};\n''')

    _valiation_enum_item_template = string.Template(
        u'''    case $name: return true;''')

    _enum_to_json_template = string.Template(
        u'''const char* EnumToJsonString($name val) {\n'''
        u'''  switch (val) {\n'''
        u'''$enum_items\n'''
        u'''    default: return "";\n'''
        u'''  }\n'''
        u'''};\n''')

    _enum_to_json_item_template = string.Template(
        u'''    case $name: return "$string_name";''')

    _enum_from_json_template = string.Template(
        u'''bool EnumFromJsonString(const std::string& literal, $name* result) {\n'''
        u'''$enum_items'''
        u'''  return false;\n'''
        u'''};\n''')

    _enum_from_json_item_template = string.Template(
        u'''  if ("$string_name" == literal) {\n'''
        u'''    *result = $name;\n'''
        u'''    return true;\n'''
        u'''  }\n''')

    _enum_element_with_value_template = string.Template(
        u'''$comment\n'''
        u'''$name = $value''')

    _enum_element_with_no_value_template = string.Template(
        u'''$comment\n'''
        u'''$name''')
    _enum_factory_template = string.Template(
        '''${enums_values}\n'''
        '''template<typename EnumType>\n'''
        '''std::shared_ptr<smart_objects::ISchemaItem> Create(const std::set<EnumType>& all_enum_values) {\n'''
        '''using namespace smart_objects;\n'''
        '''  static auto schemaItem = TEnumSchemaItem<EnumType>::create(all_enum_values, TSchemaItemParameter<EnumType>());\n'''
        '''  return schemaItem;\n'''
        '''}\n\n'''
        '''std::shared_ptr<smart_objects::ISchemaItem> EnumSchemaItemFactory::Get(const std::string & enum_name) {\n'''
        '''${enum_factory_content}\n'''
        '''  return std::shared_ptr<smart_objects::ISchemaItem>();\n'''
        '''}\n\n'''
        '''bool EnumSchemaItemFactory::IsRPCSpecVehicleDataType(const std::string& vd_name) {\n'''
        '''  for(auto& item_name : VehicleDataType_enum_names) {\n'''
        '''    if (vd_name == item_name) {\n'''
        '''      return true;\n'''
        '''   }\n'''
        '''  }\n'''
        '''  return false;\n'''
        '''}\n''')
    _enum_factory_element_template = string.Template(
        '''  if (enum_name == "${enum_name}") {\n'''
        '''    return Create<mobile_apis::${enum_name}::eType>(${enum_name}_all_enum_values);\n'''
        '''  }''')
    _POD_type_factory_element_template = string.Template(
        '''  if (typename == "${type_name}") {\n'''
        '''    return TNumberSchemaItem<${cpp_type}>::create(TSchemaItemParameter<double>(${min_val}), TSchemaItemParameter<double>(${${max_val}}), TSchemaItemParameter<${cpp_type}>());'''
        '''  }''')
    _indent_template = u"  "
