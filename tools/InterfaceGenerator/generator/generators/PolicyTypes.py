"""SmartFactory code generator base.

Base of code generator for SmartFactory that provides SmartSchema object in
accordance with given internal model.

"""
# pylint: disable=W0402
# pylint: disable=C0302
import codecs
import collections
import os
import string
import uuid

from generator import Model


class GenerateError(Exception):

    """Generate error.

    This exception is raised when generator is unable to create
    output from given model.

    """

    pass


class CodeGenerator(object):

    """Base PolicyTypes generator.

    This class provides service which allows to generate *.h files with types required for Policy

    """

    def __init__(self):
        """Construct new object."""

        self._generated_structs = []
        self._structs_add_code = u""


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

        required_enums_for_policy = [
            # "HMILevel",
            # "FunctionID",
            # "VehicleDataType",
            "AppHMIType",
            # "RequestType",
            # "ModuleType",
            # "Common_AppPriority"
        ]
        
        get_first_enum_value_name = lambda enum : enum.elements.values()[0].name
        enum_required_for_policy = lambda enum : enum.name in required_enums_for_policy and "." not in get_first_enum_value_name(enum)
        
        # In case if "." is in FunctionID name this is HMI_API function ID and should not be included in Policy  enums
        
        required_enum_values = [val for val in interface.enums.values()
                               if enum_required_for_policy(val)]
        
        self._write_header_with_enums(filename, namespace, destination_dir, required_enum_values)
        self._write_cc_with_enums(filename, namespace, destination_dir, required_enum_values)
        


    def _write_header_with_enums(self,filename, namespace, destination_dir, required_enum_values):
        class_name = u"generated_{0}_policy_types".format(filename)
        header_file_name = u"".join("{0}.h".format(class_name))
        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)
        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            guard = u"_{0}_{1}_H__".format( class_name.upper(),
                    unicode(uuid.uuid1().hex.capitalize()))
            namespace_open, namespace_close = self._namespaces_strings(namespace)            
            f_h.write(self._h_file_tempalte.substitute(
                class_name=class_name,
                guard=guard,
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
                    unicode(uuid.uuid1().hex.capitalize()))
            namespace_open, namespace_close = self._namespaces_strings(namespace)            
            f_cc.write(self._cc_file_tempalte.substitute(
                class_name=class_name,
                header_file=header_file_name,
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

        namespace = unicode(namespace)
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
        enum_elements.insert(0, Model.EnumElement(
            name=u"INVALID_ENUM",
            description=None,
            design_description=None,
            issues=None,
            todos=None,
            platform=None,
            internal_name=None,
            value=u"-1"))
        return self._enum_template.substitute(
            comment=self._gen_comment(enum),
            name=enum.name,
            enum_items=self._indent_code(self._gen_enum_elements(
                enum_elements), 1))

    def _gen_enum_elements(self, enum_elements):
        """Generate enum elements for header file.

        Generates declaration of enumeration elements for the header file.

        Keyword arguments:
        enum_elements -- list of enum elements to generate.

        Returns:
        String with enum elements declaration source code.

        """

        return u",\n\n".join([self._gen_enum_element(x)
                              for x in enum_elements])

    def _gen_enum_element(self, enum_element):
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
                name=enum_element.primary_name,
                value=enum_element.value)
        else:
            return self._enum_element_with_no_value_template.substitute(
                comment=self._gen_comment(enum_element),
                name=enum_element.primary_name)
    
    def gen_enums_processing(self, enums):
        validation = "\n".join([self._gen_enum_validation(enum) for enum in enums])
        to_json = "\n".join([self._gen_enum_to_json(enum) for enum in enums])
        from_json = "\n".join([self._gen_enum_from_json(enum) for enum in enums])
        return "\n".join([validation, to_json, from_json])
        
    def _gen_enum_validation(self, enum):
        return self._valiation_enum_template.substitute(
            name = enum.name,
            enum_items = "\n".join([self._gen_enum_item_validation(enum_item) for enum_item in enum.elements.values()])
        )

    def _gen_enum_item_validation(self, item):
        return self._valiation_enum_item_template.substitute(
            name = item.name)
    
    def _gen_enum_to_json(self, enum):
        return self._enum_to_json_template.substitute(
            name = enum.name,
            enum_items = "\n".join([self._gen_enum_item_to_json(enum_item) for enum_item in enum.elements.values()])
        )

    def _gen_enum_item_to_json(self, item):
        return self._enum_to_json_item_template.substitute(
            name = item.name)


    def _gen_enum_from_json(self, enum):
        return self._enum_from_json_template.substitute(
            name = enum.name,
            enum_items = "\n".join([self._gen_enum_item_from_json(enum_item) for enum_item in enum.elements.values()])
        )

    def _gen_enum_item_from_json(self, item):
        return self._enum_from_json_item_template.substitute(
            name = item.name)

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
            type(interface_item_base) is Model.EnumElement else \
            interface_item_base.name
        brief_description = (u" * @brief {0}{1}.\n" if use_doxygen is
                             True else u"// {0}{1}.\n").format(
                                 brief_type_title,
                                 name)

        description = u"".join([(u" * {0}\n" if use_doxygen
                                is True else u"// {0}\n").format(x)
                                for x in self._normalize_multiline_comments(
                                    interface_item_base.description)])
        if description is not u"":
            description = u"".join([u" *\n" if use_doxygen
                                    is True else u"//\n", description])

        design_description = u"".join([(u" * {0}\n" if use_doxygen is
                                       True else u"// {0}\n").format(x)
                                       for x in
                                       self._normalize_multiline_comments(
                                           interface_item_base.
                                           design_description)])
        if design_description is not u"":
            design_description = u"".join([u" *\n" if use_doxygen is
                                           True else "//\n",
                                           design_description])

        issues = u"".join([(u" * @note {0}\n" if use_doxygen is
                           True else u"// Note: {0}\n").format(x)
                           for x in self._normalize_multiline_comments(
                               [x.value for x in interface_item_base.issues])])
        if issues is not u"":
            issues = u"".join([u" *\n" if use_doxygen is
                              True else u"//\n", issues])

        todos = u"".join([(u" * @todo {0}\n" if use_doxygen is
                          True else u"// ToDo: {0}\n").format(x)
                          for x in self._normalize_multiline_comments(
                              interface_item_base.todos)])
        if todos is not u"":
            todos = u"".join([u" *\n" if use_doxygen is
                              True else u"//\n", todos])

        returns = u""
        if type(interface_item_base) is Model.Function:
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
                x) if x is not u"" else u"\n" for x in code_lines])

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

    _cc_file_tempalte = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.h\n'''
        u''' * @brief Generated class ${class_name} source file.\n'''
        u''' *\n'''
        u'''*/\n'''
        u'''#include policy/policy_table/"${header_file}"\n'''
        u'''\n\n'''
        u'''$namespace_open'''
        u'''\n'''
        u'''$enums_content'''
        u'''\n'''
        u'''$namespace_close'''
        u'''\n\n''')


    _h_file_tempalte = string.Template(
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
        u'''  switch(val) {\n'''
        u'''$enum_items\n'''
        u'''    default: return false;\n'''
        u'''  }\n'''
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
        u'''    case $name: return "$name";''')

    _enum_from_json_template = string.Template(
        u'''bool EnumFromJsonString(const std::string& literal, $name* result) {\n'''
        u'''$enum_items'''
        u'''  return false;\n'''
        u'''};\n''')

    _enum_from_json_item_template = string.Template(
        u'''  if ("$name" == literal) {\n'''
        u'''    *result = $name;\n'''
        u'''    return true;\n'''
        u'''  }\n''')

    _enum_element_with_value_template = string.Template(
        u'''$comment\n'''
        u'''$name = $value''')

    _enum_element_with_no_value_template = string.Template(
        u'''$comment\n'''
        u'''$name''')

    _indent_template = u"  "
