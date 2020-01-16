"""SmartFactory code generator base.

Base of code generator for SmartFactory that provides SmartSchema object in
accordance with given internal model.

"""
import codecs
import collections
import logging
import os
import string
import uuid
from abc import abstractmethod

from generators.GenerateError import GenerateError
from model.array import Array
from model.boolean import Boolean
from model.double import Double
from model.enum import Enum
from model.enum_element import EnumElement
from model.enum_subset import EnumSubset
from model.function import Function
from model.integer import Integer
from model.param import Param
from model.string import String
from model.struct import Struct


class SmartFactoryBase(object):
    """Base SmartFactory generator.

    This class provides service which allows to generate pair of *.h and
    *.cc files by given interface model.

    """

    def __init__(self):
        """Construct new object."""

        self._generated_structs = []
        self._structs_add_code = ""
        self.logger = logging.getLogger('SmartFactoryBase')

    @abstractmethod
    def get_version(self):
        pass

    def generate(self, interface, filename, namespace, destination_dir):
        """Generate SmartFactory source files.

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

        self._generated_structs = []
        self._structs_add_code = ""

        if "messageType" in interface.enums:
            interface.enums["messageType"] = self._preprocess_message_type(
                interface.enums["messageType"])

        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)

        namespace_open = ""
        namespace_close = ""

        if namespace:
            parts = namespace.split("::")
            for part in parts:
                namespace_open = "".join(
                    [namespace_open,
                     self._namespace_open_template.substitute(name=part)])
                namespace_close = "".join(
                    [namespace_close,
                     "}} // {0}\n".format(part)])

        class_name = os.path.splitext(filename)[0]
        guard = "__CSMARTFACTORY_{0}_{1}_H__".format(
            class_name.upper(),
            uuid.uuid1().hex.capitalize())
        header_file_name = "".join("{0}.h".format(class_name))

        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            f_h.write(self._h_file_template.substitute(
                class_name=class_name,
                guard=guard,
                namespace_open=namespace_open,
                enums_content=self._gen_enums(
                    interface.enums.values(),
                    interface.structs.values()),
                namespace_close=namespace_close))

        self._gen_struct_schema_items(interface.structs.values())

        function_id_items = ""
        if "FunctionID" in interface.enums:
            function_id = interface.enums["FunctionID"]
            function_id_items = "\n".join(
                [self._impl_code_loc_decl_enum_insert_template.substitute(
                    var_name="function_id_items",
                    enum=function_id.name,
                    value=x.primary_name)
                    for x in function_id.elements.values()])

        message_type_items = ""
        if "messageType" in interface.enums:
            message_type = interface.enums["messageType"]
            message_type_items = "\n".join(
                [self._impl_code_loc_decl_enum_insert_template.substitute(
                    var_name="message_type_items",
                    enum=message_type.name,
                    value=x.primary_name)
                    for x in message_type.elements.values()])

        header_file_name = "".join("{0}_schema.h".format(class_name))
        guard = "__CSMARTFACTORY_{0}_{1}_HPP__".format(
            class_name.upper(),
            uuid.uuid1().hex.capitalize())
        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            f_h.write(self._hpp_schema_file_template.substitute(
                class_name=class_name,
                guard=guard,
                header_file_name="".join("{0}.h".format(class_name)),
                namespace_open=namespace_open,
                class_content=self._gen_h_class(
                    class_name,
                    interface.params,
                    interface.functions.values(),
                    interface.structs.values()),
                namespace_close=namespace_close))

        with codecs.open(os.path.join(destination_dir,
                                      "".join("{0}_schema.cc".format(class_name))),
                         encoding="utf-8", mode="w") as f_s:
            f_s.write(self._cc_file_template.substitute(
                header_file_name=header_file_name,
                namespace=namespace,
                class_name=class_name,
                function_id_items=self._indent_code(function_id_items, 1),
                message_type_items=self._indent_code(message_type_items, 1),
                struct_schema_items=self._structs_add_code,
                pre_function_schemas=self._gen_pre_function_schemas(
                    interface.functions.values()),
                function_schemas=self._gen_function_schemas(
                    interface.functions.values()),
                function_schemas_switch=self._gen_function_schema_switch(
                    interface.functions.values()),
                init_function_impls=self._gen_function_impls(
                    interface.functions.values(),
                    namespace,
                    class_name),
                init_structs_impls=self._gen_sturct_impls(
                    interface.structs.values(),
                    namespace,
                    class_name),
                enum_string_coversions=self._gen_enum_to_str_converters(
                    interface.enums.values(),
                    namespace)))

    def _preprocess_message_type(self, message_type):
        """Preprocess message_type enum.

        In base class this method is unimplemented and will cause runtime
        exception. This method must be overridden by the subclasses to
        return message_type enum after preprocessing.

        Keyword arguments:
        message_type -- message_type enum to preprocess.

        """

        raise GenerateError("Unexpected call to the unimplemented function.")

    def _gen_enum_to_str_converters(self, enums, namespace):
        """Generate enum to string converters.

        Generates part of source code with specific enum to string value
        converting functions.

        Keyword arguments:
        enums -- list of enums to generate string converting functions.
        namespace -- namespace to address enums.

        Returns:
        String value with enum to string converting functions.

        """

        if enums is None:
            raise GenerateError("Enums is None")

        return "\n".join([self._enum_to_str_converter_template.substitute(
            namespace=namespace,
            enum=x.name,
            cstringvalues=self._indent_code(self._gen_enum_cstring_values(x), 2),
            enumvalues=self._indent_code(self._gen_enum_enum_values(x, namespace), 2))
            for x in enums])

    def _gen_enum_cstring_values(self, enum):
        """Generate list of c-string representing enum values.
        Keyword arguments:
        enum -- enum to generate string mapping.
        Returns:
        String value with c-string values.
        """
        return ",\n".join(['"' + x.name + '"' for x in enum.elements.values()])

    def _gen_enum_enum_values(self, enum, namespace):
        """Generate list of all enum values.
        Keyword arguments:
        enum -- enum to generate the list.
        namespace -- namespace to address enum.
        Returns:
        String value with enum values.
        """
        return ",\n".join([namespace + "::" + enum.name + "::" + x.primary_name for x in enum.elements.values()])

    def _gen_h_class(self, class_name, params, functions, structs):
        """Generate source code of class for header file.

        Generates source code of class that should be used in the
        header file.

        Keyword arguments:
        class_name -- name of the class to generate.
        params -- class parameters.
        functions -- list of functions to generate methods for.
        structs -- structures to generate methods for.

        Returns:
        String with complete *.h file source code.

        """

        return self._class_h_template.substitute(
            comment=self._gen_class_comment(class_name, params),
            class_name=class_name,
            init_function_decls=self._gen_function_decls(functions),
            init_struct_decls=self._gen_structs_decls(structs))

    def _gen_structs_decls(self, structs):
        """Generate method prototypes for structs for header file.

        Generates method prototypes for structs that should be used in the
        header file.

        Keyword arguments:
        structs -- list of structs to generate methods for.

        Returns:
        String with structs init methods declarations source code.

        """

        if structs is None:
            raise GenerateError("Structs is None")

        return "\n".join([self._indent_code(
            self._gen_struct_decl(x), 1) for x in structs])

    def _gen_struct_decl(self, struct):
        """Generate method prototype for struct for header file.

        Generates method prototype for struct that should be used in the
        header file.

        Keyword arguments:
        struct -- struct to generate method for.

        Returns:
        String with struct inin method declaration source code.

        """

        return self._struct_decl_template.substitute(
            comment=self._gen_comment(struct),
            struct_name=struct.name)

    def _gen_function_decls(self, functions):
        """Generate method prototypes for functions for header file.

        Generates method prototypes for functions that should be used in the
        header file.

        Keyword arguments:
        functions -- list of functions to generate methods for.

        Returns:
        String with function init methods declarations source code.

        """

        if functions is None:
            raise GenerateError("Functions is None")

        return "\n".join([self._indent_code(
            self._gen_function_decl(x), 1) for x in functions])

    def _gen_function_decl(self, function):
        """Generate method prototype for function for header file.

        Generates method prototype for function that should be used in the
        header file.

        Keyword arguments:
        function -- function to generate method for.

        Returns:
        String with function declaration source code.

        """

        return self._function_decl_template.substitute(
            comment=self._gen_comment(function),
            function_id=function.function_id.primary_name,
            message_type=function.message_type.primary_name)

    def _gen_struct_schema_items(self, structs):
        """Generate struct schema items initialization code for source file.

        Generates struct schema items initialization code that should be used
        in the source file.

        Keyword arguments:
        structs -- list of structs to generate code for.

        Returns:
        String with struct schema items initialization source code.

        """

        if structs is None:
            raise GenerateError("Structs is None")

        for struct in structs:
            self._process_struct(struct)

    def _process_struct(self, struct):
        """Process struct recursively to provide correct initialization.

        This method create source code for cc file that guarantees that
        structures are initialized in right order (to resolve all
        dependencies).

        Keyword arguments:
        struct -- struct to process.

        """

        if struct.name in self._generated_structs:
            return

        for member in struct.members.values():
            self._process_struct_member(member)

        self._structs_add_code = "\n".join(
            [self._structs_add_code, self._indent_code(
                self._gen_struct_schema_item(struct), 1)])
        self._generated_structs.append(struct.name)

    def _process_struct_member(self, member):
        """Process struct member recursively to provide correct initialization.

        This method ensures that nested structs (if any) are generated.

        Keyword arguments:
        member -- struct member to process.

        """

        if type(member.param_type) is Struct:
            self._ensure_struct_generated(member.param_type)

    def _ensure_struct_generated(self, struct):
        """Ensure that struct is already generated.

        If struct is already created this method returns otherwise it
        runs generation of struct to resolve dependency.

        Keyword arguments:
        struct -- struct to ensure existence.

        """

        if struct.name in self._generated_structs:
            return

        self._process_struct(struct)

    def _gen_struct_schema_item(self, struct):
        """Generate struct schema item initialization code for source file.

        Generates struct schema item initialization code that should be used
        in the source file.

        Keyword arguments:
        struct -- struct to generate code for.

        Returns:
        String with struct schema item initialization source code.

        """

        return self._struct_schema_item_template.substitute(name=struct.name)

    def _gen_pre_function_schemas(self, functions):
        """Generate specific code that goes before schema initialization.

        In base class this function is unimplemented and it will raise an
        runtime exception. Subclasses must implement this function in order
        to provide format-specific code that goes before schema initialization.

        Keyword arguments:
        functions -- list of functions to generate code for.

        """

        raise GenerateError("Unexpected call to the unimplemented function.")

    def _gen_function_schemas(self, functions):
        """Generate functions initialization code for source file.

        Generates functions schema initialization code that should be used
        in the source file.

        Keyword arguments:
        functions -- list of functions to generate code for.

        Returns:
        String with functions schema initialization source code.

        """

        if functions is None:
            raise GenerateError("Functions is None")

        return "".join([self._indent_code(
            self._gen_function_schema(x), 1)
            for x in functions])

    def _gen_function_schema_switch(self, functions):
        """Generate initialization code of each function for source file.

        Generates function schema initialization code that should be used
        in the source file.

        Keyword arguments:
        function -- function to generate method for.

        Returns:
        String with switch for functions initialization source code.

        """

        if functions is None:
            raise GenerateError("Functions is None")

        def function_id_switch(self, message_type, functions):
            case_list = [self._function_id_case_template.substitute(
                function_id=x.function_id.primary_name,
                message_type=x.message_type.primary_name)
                for x in functions if x.message_type.primary_name == message_type]
            case_list.append("default:\n  break;\n")
            switch_function_id_cases = self._indent_code("".join(case_list), 1)[:-1]

            return self._indent_code(self._function_switch_template.substitute(
                switchable="function_id",
                cases=switch_function_id_cases
            ), 1)[:-1]

        message_type_case_list = [self._message_type_case_template.substitute(
            message_type=message_type,
            case_body=function_id_switch(self, message_type, functions)
        ) for message_type in set([x.message_type.primary_name for x in functions])]
        message_type_case_list.append("default:\n  break;\n")
        message_type_cases = self._indent_code("".join(message_type_case_list), 1)[:-1]

        return self._indent_code(self._function_switch_template.substitute(
            switchable="message_type",
            cases=message_type_cases
        ), 1)[:-1]

    def _gen_function_schema(self, function):
        """Generate function initialization code for source file.

        Generates function schema initialization code that should be used
        in the source file.

        Keyword arguments:
        function -- function to generate method for.

        Returns:
        String with function schema initialization source code.

        """

        return self._function_schema_template.substitute(
            function_id=function.function_id.primary_name,
            message_type=function.message_type.primary_name)

    def _gen_sturct_impls(self, structs, namespace, class_name):
        """Generate structs implementation for source file.

        Generates implementation code of methods that provide schema items for
        structs. This code should be used in the source file.

        Keyword arguments:
        structs -- list of structs to generate methods for.
        namespace -- name of destination namespace.
        class_name -- name of the parent class.

        Returns:
        String with structs implementation source code.

        """

        if structs is None:
            raise GenerateError("Structs is None")

        return "\n".join([self._gen_struct_impl(
            x, namespace, class_name) for x in structs])

    def _gen_struct_impl(self, struct, namespace, class_name):
        """Generate struct implementation for source file.

        Generates implementation code of method that provide schema item for
        struct. This code should be used in the source file.

        Keyword arguments:
        struct -- struct to generate method for.
        namespace -- name of destination namespace.
        class_name -- name of the parent class.

        Returns:
        String with structs implementation source code.

        """
        processed_enums = []
        return self._struct_impl_template.substitute(
            namespace=namespace,
            class_name=class_name,
            struct_name=struct.name,
            code=self._indent_code(
                self._struct_impl_code_tempate.substitute(
                    schema_loc_decl=self._gen_schema_loc_decls(
                        struct.members.values(), processed_enums),
                    schema_items_decl=self._gen_schema_items_decls(
                        struct.members.values()),
                    schema_item_fill=self._gen_schema_items_fill(
                        struct.members.values(), struct.since, struct.until, struct.deprecated, struct.removed)),
                1))

    def _enum_has_history_present(self, enum):
        """
        Check if any elements in an enum has history signature
        """
        for element in enum.param_type.elements.values():
            if (element.history is not None or
                        element.since is not None or
                        element.until is not None or
                        element.removed is not None):
                return True
        return False

    def _element_has_history_present(self, element):
        """
        Check if a specific element has a history signature
        """
        if (element.history is not None or
                    element.since is not None or
                    element.until is not None or
                    element.removed is not None):
            return True
        return False

    def _enum_param_type_has_history_present(self, param_type):
        """
        Check if any elements in an enum has history signature
        """
        for element in param_type.elements.values():
            if (element.history is not None or
                        element.since is not None or
                        element.until is not None or
                        element.removed is not None):
                return True
        return False

    def _gen_schema_loc_decls(self, members, processed_enums):
        """Generate local declarations of variables for schema.

        This method generates full set of required variables for
        enums and enum subsets.

        Keyword arguments:
        members -- struct/function members/params.
        processed_enums -- list of already processed enums.

        Returns:
        String with local declarations source code.

        """

        result = ""
        for member in members:
            if type(member.param_type) is Enum and \
                            member.param_type.name not in processed_enums:
                has_history = self._enum_has_history_present(member)
                local_var = self._gen_schema_loc_emum_var_name(
                    member.param_type)
                result = "\n".join(
                    ["".join(
                        [result, self._impl_code_loc_decl_enum_template.
                            substitute(
                            type=member.param_type.name,
                            var_name=local_var)]),
                        "\n".join(
                            [self._impl_code_loc_decl_enum_insert_template.
                                substitute(
                                var_name=local_var,
                                enum=member.param_type.name,
                                value=x.primary_name)
                                for x in member.param_type.elements.values()])])

                if has_history:
                    history_result = "\n"
                    history_result += self._impl_code_loc_decl_enum_history_set_template.substitute(
                        type=member.param_type.name)
                    history_result += "\n"
                    history_result += "\n".join(
                        [self._impl_code_loc_decl_enum_history_set_value_init_template.substitute(
                            enum=member.param_type.name,
                            value=x.primary_name)
                            for x in member.param_type.elements.values() if self._element_has_history_present(x)])
                    history_result += "\n"
                    history_map_result = []

                    for x in member.param_type.elements.values():
                        if self._element_has_history_present(x):
                            history_map_result.append(
                                self._impl_code_loc_decl_enum_history_set_insert_template.
                                    substitute(
                                    enum=member.param_type.name,
                                    value=x.primary_name,
                                    since=x.since if x.since is not None else "",
                                    until=x.until if x.until is not None else "",
                                    removed=x.removed if x.removed is not None else "false"))
                            if x.history is not None:
                                history_list = x.history
                                for item in history_list:
                                    history_map_result.append(
                                        self._impl_code_loc_decl_enum_history_set_insert_template.
                                            substitute(
                                            enum=member.param_type.name,
                                            value=x.primary_name,
                                            since=item.since if item.since is not None else "",
                                            until=item.until if item.until is not None else "",
                                            removed=item.removed if item.removed is not None else "false"))

                    history_result += "\n".join(history_map_result)
                    result += "\n"
                    result += history_result

                processed_enums.append(member.param_type.name)
                result = "".join([result, "\n\n"]) if result else ""
            elif type(member.param_type) is EnumSubset:
                local_var = self._gen_schema_loc_emum_s_var_name(member.name)
                result = "\n".join(
                    ["".join(
                        [result, self._impl_code_loc_decl_enum_template.
                            substitute(
                            type=member.param_type.enum.name,
                            var_name=local_var)]),
                        "\n".join(
                            [self._impl_code_loc_decl_enum_insert_template.
                                substitute(
                                var_name=local_var,
                                enum=member.param_type.enum.name,
                                value=x.primary_name)
                                for x in member.param_type.
                                allowed_elements.values()])])
                result = "".join([result, "\n\n"]) if result else ""
            elif type(member.param_type) is Array:
                result = "".join(
                    [result, self._gen_schema_loc_decls(
                        [Param(name=member.param_type.element_type.name
                        if type(member.param_type.element_type) is
                           EnumSubset else "",
                               param_type=member.param_type.element_type)],
                        processed_enums)])

        return result

    def _gen_schema_items_decls(self, members):
        """Generate schema items declarations.

        Generates declaration and initialization of schema items
        which is uses as struct members/function parameters.

        Keyword arguments:
        members -- list of struct members/function parameters.

        Returns:
        String with schema items declaration source code.

        """

        result_array = []
        for x in members:
            result_array.append(self._gen_schema_item_decl(x))
            count = 0
            if x.history is not None:
                history_list = x.history
                for item in history_list:
                    item.name += "_history_v" + str(len(history_list) - count)
                    result_array.append(self._gen_schema_item_decl(item))
                    count += 1
                result_array.append(self._gen_history_vector_decl(history_list, x.name))

        result = "\n\n".join(result_array)
        return result

    def _gen_history_vector_decl(self, history_list, name):
        """Generate History Vector Declaration.

            Generates the declaration and initialization
            of a vector of schema items

            Arguments:
            history_list -> list of history items
            name -> name of parent parameter name

            Returns:
            String with history array code.
        """
        result_array = []
        result_array.append(self._impl_code_shared_ptr_vector_template.substitute(var_name=name))
        result = "\n".join(result_array)
        return result

    def _gen_schema_item_decl(self, member):
        """Generate schema item declaration.

        Generates declaration and initialization of schema item
        which is uses as struct member/function parameter.

        Keyword arguments:
        member -- struct member/function parameter.

        Returns:
        String with schema item declaration source code.

        """

        return self._impl_code_item_decl_temlate.substitute(
            comment=self._gen_comment(member, False),
            var_name=self._gen_schema_item_var_name(member),
            item_decl=self._gen_schema_item_decl_code(
                member.param_type,
                member.name,
                member.default_value))

    def _gen_schema_item_decl_code(self, param, member_name, default_value):

        """Generate schema item initialization code.

        Generates type-specific code that initializes schema item

        Keyword arguments:
        param -- value of parameter type.
        mamber_name -- name of struct member/function parameter.
        default_value -- default value (used only for function parameters).

        Returns:
        String with schema item initialization source code.

        """
        code = ""
        if type(param) is Boolean:
            code = self._impl_code_bool_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [["bool", None if param.default_value is None
                    else "true" if param.default_value is True else "false"]]))
        elif type(param) is Integer:
            if not param.max_value or param.max_value and param.max_value < 2 ** 31:
                code = self._impl_code_integer_item_template.substitute(
                    type="int32_t",
                    params=self._gen_schema_item_param_values(
                        [["int32_t", param.min_value],
                         ["int32_t", param.max_value],
                         ["int32_t", param.default_value]]))
            elif param.max_value and param.max_value < 2 ** 63:
                code = self._impl_code_integer_item_template.substitute(
                    type="int64_t",
                    params=self._gen_schema_item_param_values(
                        [["int64_t", param.min_value],
                         ["int64_t", str(param.max_value) + "LL"],
                         ["int64_t", param.default_value]]))
            else:
                raise GenerateError("Parameter '{}' value too large: {}".format(member_name, vars(param)))
        elif type(param) is Double:
            code = self._impl_code_integer_item_template.substitute(
                type="double",
                params=self._gen_schema_item_param_values(
                    [["double", param.min_value],
                     ["double", param.max_value],
                     ["double", param.default_value]]))
        elif type(param) is String:
            code = self._impl_code_string_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [["size_t", param.min_length],
                     ["size_t", param.max_length],
                     ["std::string", "".join(
                         ['"', param.default_value, '"']) if param.default_value
                                                             is not None else ""]]))
        elif type(param) is Array:
            code = self._impl_code_array_item_template.substitute(
                params="".join(
                    ["".join(
                        [self._gen_schema_item_decl_code(
                            param.element_type,
                            param.element_type.name if type(param.element_type)
                                                       is EnumSubset else "",
                            None),
                            ", "]),
                        self._gen_schema_item_param_values(
                            [["size_t", param.min_size],
                             ["size_t", param.max_size]])]))
        elif type(param) is Struct:
            code = self._impl_code_struct_item_template.substitute(
                name=param.name)
        elif type(param) is Enum:
            if self._enum_param_type_has_history_present(param):
                code = self._impl_code_enum_item_with_history_template.substitute(
                    type=param.name,
                    params="".join(
                        [self._gen_schema_loc_emum_var_name(param),
                         ", ",
                         self._impl_gen_schema_enum_history_map_template.substitute(name=param.name),
                         ", ",
                         self._gen_schema_item_param_values(
                             [["".join([param.name, "::eType"]),
                               "".join([param.name, "::",
                                        default_value.primary_name]) if
                               default_value is not None else None]])]))
            else:
                code = self._impl_code_enum_item_template.substitute(
                    type=param.name,
                    params="".join(
                        [self._gen_schema_loc_emum_var_name(param),
                         ", ",
                         self._gen_schema_item_param_values(
                             [["".join([param.name, "::eType"]),
                               "".join([param.name, "::",
                                        default_value.primary_name]) if
                               default_value is not None else None]])]))
        elif type(param) is EnumSubset:
            code = self._impl_code_enum_item_template.substitute(
                type=param.enum.name,
                params="".join(
                    [self._gen_schema_loc_emum_s_var_name(member_name),
                     ", ",
                     self._gen_schema_item_param_values(
                         [["".join([param.enum.name, "::eType"]),
                           default_value.primary_name if default_value
                                                         is not None else None]])]))
        else:
            raise GenerateError("Unexpected type of parameter: " +
                                str(type(param)))
        return code

    def _gen_schema_item_param_values(self, params):
        """Generate values of schema item initialization parameters.

        Generates part of code which is used as schema item initialization
        parameters.

        Keyword arguments:
        params -- list of tuples which maps parameter type to parameter value.

        Returns:
        String with schema item initialization parameters source code.

        """

        result = ""
        for param in params:
            value = self._impl_code_item_param_value_template.substitute(
                type=param[0],
                value=str(param[1] if param[1] is not None else ""))
            result = "".join([result, "".join(
                [", ", value])
            if result else value])

        return result

    def _gen_schema_items_fill(self, members=None, since=None, until=None, deprecated=None, removed=None):
        """Generate schema items fill code.

        Generates source code that fills new schema with items.

        Keyword arguments:
        members -- list of struct members/function parameters to process.

        Returns:
        String with function schema items fill code.

        """
        result_array = []
        for x in members:
            # If history, create Smember History vector first
            if x.history is not None:
                history_list = x.history
                for item in history_list:
                    result_array.append(self._gen_history_vector_item_fill(item, x.name))
            result_array.append(self._gen_schema_item_fill(x, since, until, deprecated, removed))

        result = "\n".join(result_array)
        return "".join([result, "\n\n"]) if result else ""

    def _gen_schema_params_fill(self, message_type_name):
        """Generate schema params fill code.

        In the base class this method is not implemented (raises exception).
        This method must be implemented by the subclasses to specify format
        specific PARAMS for the function.

        Keyword arguments:
        message_type_name -- Name of the messageType enum element.

        Returns:
        String with function schema params fill code.

        """

        raise GenerateError("Unexpected call to the unimplemented function.")

    def _check_member_history(self, member):
        """
            Checks set of rules that history items are valid
            Raises error if rules are violated
        """
        if (member.since is None and
                    member.until is None and
                    member.deprecated is None and
                    member.removed is None and
                    member.history is None):
            return
        if member.history is not None and member.since is None:
            raise GenerateError("Error: Missing since version parameter for " + member.name)
        if member.until is not None:
            raise GenerateError("Error: Until should only exist in history tag for " + member.name)
        if member.history is None:
            if (member.until is not None or
                        member.deprecated is not None or
                        member.removed is not None):
                raise GenerateError("Error: No history present for " + member.name)
        if member.deprecated is not None and member.removed is not None:
            raise GenerateError("Error: Deprecated and removed should not be present together for " + member.name)
        if member.history is not None:
            for item in member.history:
                if item.since is None or item.until is None:
                    raise GenerateError("Error: History items require since and until parameters for " + member.name)

    def _gen_schema_item_fill(self, member, since, until, deprecated, removed):
        """Generate schema item fill code.

        Generates source code that fills new schema with item.

        Keyword arguments:
        member -- struct member/function parameter to process.

        Returns:
        String with schema item fill code.

        """
        self._check_member_history(member)

        if (since is not None or
                    member.since is not None):
            if member.history is not None:
                return self._impl_code_item_fill_template_with_version_and_history_vector.substitute(
                    name=member.name,
                    var_name=self._gen_schema_item_var_name(member),
                    is_mandatory="true" if member.is_mandatory is True else "false",
                    since=member.since if member.since is not None else since if since is not None else "",
                    until=member.until if member.until is not None else until if until is not None else "",
                    deprecated=member.deprecated if member.deprecated is not None else deprecated if deprecated is
                                                                                                     not None else
                    "false",
                    removed=member.removed if member.removed is not None else removed if removed is not None else
                    "false",
                    vector_name=member.name)
            else:
                return self._impl_code_item_fill_template_with_version.substitute(
                    name=member.name,
                    var_name=self._gen_schema_item_var_name(member),
                    is_mandatory="true" if member.is_mandatory is True else "false",
                    since=member.since if member.since is not None else since if since is not None else "",
                    until=member.until if member.until is not None else until if until is not None else "",
                    deprecated=member.deprecated if member.deprecated is not None else deprecated if deprecated is
                                                                                                     not None else
                    "false",
                    removed=member.removed if member.removed is not None else removed if removed is not None else
                    "false")
        else:
            return self._impl_code_item_fill_template.substitute(
                name=member.name,
                var_name=self._gen_schema_item_var_name(member),
                is_mandatory="true" if member.is_mandatory is True else "false")

    def _gen_history_vector_item_fill(self, member, vector_name):
        """Generate schema item fill code.

        Generates source code that fills history vector with item.

        Keyword arguments:
        member -- struct member/function parameter to process.

        Returns:
        String with schema item fill code.

        """

        if (member.since is not None or
                    member.until is not None or
                    member.deprecated is not None or
                    member.removed is not None):
            return self._impl_code_append_history_vector_template.substitute(
                vector_name=vector_name,
                name=member.name,
                mandatory="true" if member.is_mandatory is True else "false",
                since=member.since if member.since is not None else "",
                until=member.until if member.until is not None else "",
                deprecated=member.deprecated if member.deprecated is not None else "false",
                removed=member.removed if member.removed is not None else "false")
        else:
            self.logger.warning("Warning! History item does not have any version history. Omitting {}"
                                .format(member.name))

    @staticmethod
    def _gen_schema_item_var_name(member):
        """Generate schema item variable name.

        Generates variable name for local declarations.

        Keyword arguments:
        member -- struct member/function parameter to process.

        Returns:
        String with schema item variable name.

        """

        return "".join([member.name, "_SchemaItem"])

    @staticmethod
    def _gen_schema_loc_emum_var_name(param_type):
        """Generate name of enum local variable.

        Generates name of local variable that defines allowed enum elements.

        Keyword arguments:
        param_type -- parameter type object.

        Returns:
        String with name of enum local variable.

        """

        return "".join([param_type.name, "_all_enum_values"])

    @staticmethod
    def _gen_schema_loc_emum_s_var_name(member_name):
        """Generate name of enum subset local variable.

        Generates name of local variable that defines allowed enum
        subset elements.

        Keyword arguments:
        param_type -- parameter type object.

        Returns:
        String with name of enum subset local variable.

        """

        return "".join([member_name, "_allowed_enum_subset_values"])

    def _gen_function_impls(self, functions, namespace, class_name):
        """Generate functions implementation for source file.

        Generates implementation code of methods that provide schema for
        functions. This code should be used in the source file.

        Keyword arguments:
        functions -- list of functions to generate methods for.
        namespace -- name of destination namespace.
        class_name -- name of the parent class.

        Returns:
        String with functions implementation source code.

        """

        if functions is None:
            raise GenerateError("Functions is None")

        return "\n".join([self._gen_function_impl(
            x, namespace, class_name) for x in functions])

    def _gen_function_impl(self, function, namespace, class_name):
        """Generate function implementation for source file.

        Generates implementation code of method that provides schema for
        function. This code should be used in the source file.

        Keyword arguments:
        function -- function to generate method for.
        namespace -- name of destination namespace.
        class_name -- name of the parent class.

        Returns:
        String with function implementation source code.

        """

        processed_enums = []
        return self._function_impl_template.substitute(
            namespace=namespace,
            class_name=class_name,
            function_id=function.function_id.primary_name,
            message_type=function.message_type.primary_name,
            code=self._indent_code(
                self._function_impl_code_tempate.substitute(
                    schema_loc_decl=self._gen_schema_loc_decls(
                        function.params.values(), processed_enums),
                    schema_items_decl=self._gen_schema_items_decls(
                        function.params.values()),
                    schema_item_fill=self._gen_schema_items_fill(
                        function.params.values(), function.since, function.until, function.deprecated,
                        function.removed),
                    schema_params_fill=self._gen_schema_params_fill(
                        function.message_type.name)),
                1))

    def _gen_enums(self, enums, structs):
        """Generate enums for header file.

        Generates declaration of enumerations for the header file.

        Keyword arguments:
        enums -- list of enums to generate.

        Returns:
        String with enums declaration source code.

        """

        if enums is None:
            raise GenerateError("Enums is None")

        if structs is None:
            raise GenerateError("Structs is None")

        if structs:
            struct_id_enum_elements = collections.OrderedDict()
            for struct in structs:
                struct_id_enum_elements[struct.name] = EnumElement(
                    name=struct.name)
            return "\n".join(
                [self._gen_enum(
                    Enum(name="StructIdentifiers",
                         elements=struct_id_enum_elements)),
                    "\n".join([self._gen_enum(x) for x in enums])])

        return "\n".join([self._gen_enum(x) for x in enums])

    def _gen_enum(self, enum):
        """Generate enum for header file.

        Generates declaration of enumeration for the header file.

        Keyword arguments:
        enum -- enum to generate.

        Returns:
        String with enum declaration source code.

        """

        enum_elements = list(enum.elements.values())
        enum_elements.insert(0, EnumElement(
            name="INVALID_ENUM",
            description=None,
            design_description=None,
            issues=None,
            todos=None,
            platform=None,
            internal_name=None,
            value="-1"))
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

        return ",\n\n".join([self._gen_enum_element(x)
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

    def _gen_class_comment(self, class_name, params):
        """Generate doxygen comment to the class for header file.

        Generates doxygen comment for the class that should be used in
        the header file.

        Keyword arguments:
        class_name -- name of the class.
        params -- class parameters.

        Returns:
        String with generated doxygen class comment.

        """

        return self._class_comment_template.substitute(
            class_name=class_name,
            class_params="".join(
                [" *     {0} - {1}\n".format(x[0],
                                             x[1]) for x in params.items()])
            if params else " *    none\n")

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
        brief_description = (" * @brief {0}{1}.\n" if use_doxygen is
                                                      True else "// {0}{1}.\n").format(
            brief_type_title,
            name)

        description = "".join([(" * {0}\n" if use_doxygen
                                              is True else "// {0}\n").format(x)
                               for x in self._normalize_multiline_comments(
                interface_item_base.description)])
        if description is not "":
            description = "".join([" *\n" if use_doxygen
                                             is True else "//\n", description])

        design_description = "".join([(" * {0}\n" if use_doxygen is
                                                     True else "// {0}\n").format(x)
                                      for x in
                                      self._normalize_multiline_comments(
                                          interface_item_base.
                                              design_description)])
        if design_description is not "":
            design_description = "".join([" *\n" if use_doxygen is
                                                    True else "//\n",
                                          design_description])

        issues = "".join([(" * @note {0}\n" if use_doxygen is
                                               True else "// Note: {0}\n").format(x)
                          for x in self._normalize_multiline_comments(
                [x.value for x in interface_item_base.issues])])
        if issues is not "":
            issues = "".join([" *\n" if use_doxygen is
                                        True else "//\n", issues])

        todos = "".join([(" * @todo {0}\n" if use_doxygen is
                                              True else "// ToDo: {0}\n").format(x)
                         for x in self._normalize_multiline_comments(
                interface_item_base.todos)])
        if todos is not "":
            todos = "".join([" *\n" if use_doxygen is
                                       True else "//\n", todos])

        returns = ""
        if type(interface_item_base) is Function:
            returns = "".join([" *\n", self._function_return_comment])

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
        return "".join(
            ["{0}{1}\n".format(
                self._indent_template * indent_level,
                x) if x is not "" else "\n" for x in code_lines])

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
        {"EnumElement": "",
         "Enum": "Enumeration ",
         "Function": "Method that generates schema for function ",
         "Struct": "Method that generates schema item for structure ",
         "Param": "Struct member ",
         "FunctionParam": "Function parameter "})

    _h_file_template = string.Template(
        '''/**\n'''
        ''' * @file ${class_name}.h\n'''
        ''' * @brief Generated class ${class_name} header file.\n'''
        ''' *\n'''
        ''' * This class is a part of SmartObjects solution. It provides\n'''
        ''' * factory functionallity which allows client to use '''
        '''SmartSchemas\n'''
        ''' * in accordance with definitions from ${class_name}.xml file\n'''
        ''' */\n'''
        '''// Copyright (c) 2013, Ford Motor Company\n'''
        '''// All rights reserved.\n'''
        '''//\n'''
        '''// Redistribution and use in source and binary forms, '''
        '''with or without\n'''
        '''// modification, are permitted provided that the following '''
        '''conditions are met:\n'''
        '''//\n'''
        '''// Redistributions of source code must retain the above '''
        '''copyright notice, this\n'''
        '''// list of conditions and the following disclaimer.\n'''
        '''//\n'''
        '''// Redistributions in binary form must reproduce '''
        '''the above copyright notice,\n'''
        '''// this list of conditions and the following\n'''
        '''// disclaimer in the documentation and/or other materials '''
        '''provided with the\n'''
        '''// distribution.\n'''
        '''//\n'''
        '''// Neither the name of the Ford Motor Company nor the names '''
        '''of its contributors\n'''
        '''// may be used to endorse or promote products derived '''
        '''from this software\n'''
        '''// without specific prior written permission.\n'''
        '''//\n'''
        '''// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND '''
        '''CONTRIBUTORS "AS IS"\n'''
        '''// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT '''
        '''LIMITED TO, THE\n'''
        '''// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR '''
        '''A PARTICULAR PURPOSE\n'''
        '''// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER '''
        '''OR CONTRIBUTORS BE\n'''
        '''// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, '''
        '''EXEMPLARY, OR\n'''
        '''// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, '''
        '''PROCUREMENT OF\n'''
        '''// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; '''
        '''OR BUSINESS\n'''
        '''// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, '''
        '''WHETHER IN\n'''
        '''// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE '''
        '''OR OTHERWISE)\n'''
        '''// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF '''
        '''ADVISED OF THE\n'''
        '''// POSSIBILITY OF SUCH DAMAGE.\n\n'''
        '''#ifndef $guard\n'''
        '''#define $guard\n'''
        '''\n'''
        '''$namespace_open'''
        '''$enums_content'''
        '''$namespace_close'''
        '''#endif //$guard\n'''
        '''\n\n''')

    _hpp_schema_file_template = string.Template(
        '''/**\n'''
        ''' * @file ${class_name}.h\n'''
        ''' * @brief Generated class ${class_name} header file.\n'''
        ''' *\n'''
        ''' * This class is a part of SmartObjects solution. It provides\n'''
        ''' * factory functionallity which allows client to use '''
        '''SmartSchemas\n'''
        ''' * in accordance with definitions from ${class_name}.xml file\n'''
        ''' */\n'''
        '''// Copyright (c) 2013, Ford Motor Company\n'''
        '''// All rights reserved.\n'''
        '''//\n'''
        '''// Redistribution and use in source and binary forms, '''
        '''with or without\n'''
        '''// modification, are permitted provided that the following '''
        '''conditions are met:\n'''
        '''//\n'''
        '''// Redistributions of source code must retain the above '''
        '''copyright notice, this\n'''
        '''// list of conditions and the following disclaimer.\n'''
        '''//\n'''
        '''// Redistributions in binary form must reproduce '''
        '''the above copyright notice,\n'''
        '''// this list of conditions and the following\n'''
        '''// disclaimer in the documentation and/or other materials '''
        '''provided with the\n'''
        '''// distribution.\n'''
        '''//\n'''
        '''// Neither the name of the Ford Motor Company nor the names '''
        '''of its contributors\n'''
        '''// may be used to endorse or promote products derived '''
        '''from this software\n'''
        '''// without specific prior written permission.\n'''
        '''//\n'''
        '''// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND '''
        '''CONTRIBUTORS "AS IS"\n'''
        '''// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT '''
        '''LIMITED TO, THE\n'''
        '''// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR '''
        '''A PARTICULAR PURPOSE\n'''
        '''// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER '''
        '''OR CONTRIBUTORS BE\n'''
        '''// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, '''
        '''EXEMPLARY, OR\n'''
        '''// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, '''
        '''PROCUREMENT OF\n'''
        '''// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; '''
        '''OR BUSINESS\n'''
        '''// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, '''
        '''WHETHER IN\n'''
        '''// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE '''
        '''OR OTHERWISE)\n'''
        '''// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF '''
        '''ADVISED OF THE\n'''
        '''// POSSIBILITY OF SUCH DAMAGE.\n\n'''
        '''#ifndef $guard\n'''
        '''#define $guard\n'''
        '''\n'''
        '''#include "formatters/CSmartFactory.h"\n'''
        '''#include "smart_objects/smart_schema.h"\n'''
        '''#include "smart_objects/schema_item.h"\n'''
        '''#include "smart_objects/object_schema_item.h"\n'''
        '''#include "$header_file_name"\n'''
        '''\n'''
        '''$namespace_open'''
        '''\n\n'''
        '''$class_content'''
        '''\n\n'''
        '''$namespace_close'''
        '''\n'''
        '''#endif //$guard\n'''
        '''\n''')

    _namespace_open_template = string.Template(
        '''namespace $name {\n''')

    _cc_file_template = string.Template(
        '''/**\n'''
        ''' * @file ${class_name}.cc\n'''
        ''' * @brief Generated class ${class_name} source file.\n'''
        ''' *\n'''
        ''' * This class is a part of SmartObjects solution. It provides\n'''
        ''' * factory functionallity which allows client to use '''
        '''SmartSchemas\n'''
        ''' * in accordance with definitions from ${class_name}.xml file\n'''
        ''' */\n'''
        '''// Copyright (c) 2019, SmartDeviceLink Consortium, Inc.\n'''
        '''// All rights reserved.\n'''
        '''//\n'''
        '''// Redistribution and use in source and binary forms, '''
        '''with or without\n'''
        '''// modification, are permitted provided that the following '''
        '''conditions are met:\n'''
        '''//\n'''
        '''// Redistributions of source code must retain the above '''
        '''copyright notice, this\n'''
        '''// list of conditions and the following disclaimer.\n'''
        '''//\n'''
        '''// Redistributions in binary form must reproduce '''
        '''the above copyright notice,\n'''
        '''// this list of conditions and the following\n'''
        '''// disclaimer in the documentation and/or other materials '''
        '''provided with the\n'''
        '''// distribution.\n'''
        '''//\n'''
        '''// Neither the name of the SmartDeviceLink Consortium, Inc. nor the names '''
        '''of its\n'''
        '''// contributors may be used to endorse or promote products derived '''
        '''from this\n'''
        '''// software without specific prior written permission.\n'''
        '''//\n'''
        '''// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND '''
        '''CONTRIBUTORS "AS IS"\n'''
        '''// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT '''
        '''LIMITED TO, THE\n'''
        '''// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR '''
        ''''A PARTICULAR PURPOSE\n'''
        '''// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER '''
        '''OR CONTRIBUTORS BE\n'''
        '''// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, '''
        '''EXEMPLARY, OR\n'''
        '''// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, '''
        '''PROCUREMENT OF\n'''
        '''// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; '''
        '''OR BUSINESS\n'''
        '''// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, '''
        '''WHETHER IN\n'''
        '''// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE '''
        '''OR OTHERWISE)\n'''
        '''// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF '''
        '''ADVISED OF THE\n'''
        '''// POSSIBILITY OF SUCH DAMAGE.\n\n'''
        '''#include <map>\n'''
        '''#include <set>\n'''
        '''\n'''
        '''#include "$header_file_name"\n'''
        '''#include "smart_objects/always_true_schema_item.h"\n'''
        '''#include "smart_objects/always_false_schema_item.h"\n'''
        '''#include "smart_objects/array_schema_item.h"\n'''
        '''#include "smart_objects/bool_schema_item.h"\n'''
        '''#include "smart_objects/object_schema_item.h"\n'''
        '''#include "smart_objects/string_schema_item.h"\n'''
        '''#include "smart_objects/enum_schema_item.h"\n'''
        '''#include "smart_objects/number_schema_item.h"\n'''
        '''#include "smart_objects/schema_item_parameter.h"\n'''
        '''\n'''
        '''using namespace ns_smart_device_link::ns_smart_objects;\n'''
        '''\n'''
        '''$namespace::$class_name::$class_name()\n'''
        ''' : ns_smart_device_link::ns_json_handler::CSmartFactory<FunctionID::eType, '''
        '''messageType::eType, StructIdentifiers::eType>() {\n'''
        '''  TStructsSchemaItems struct_schema_items;\n'''
        '''  InitStructSchemes(struct_schema_items);\n'''
        '''\n'''
        '''  std::set<FunctionID::eType> function_id_items;\n'''
        '''${function_id_items}'''
        '''\n'''
        '''  std::set<messageType::eType> message_type_items;\n'''
        '''${message_type_items}'''
        '''\n'''
        '''  InitFunctionSchemes(struct_schema_items, function_id_items, '''
        '''message_type_items);\n'''
        '''}\n'''
        '''\n'''
        '''std::shared_ptr<ISchemaItem> $namespace::$class_name::'''
        '''ProvideObjectSchemaItemForStruct(\n'''
        '''    const TStructsSchemaItems &struct_schema_items,\n'''
        '''    const StructIdentifiers::eType struct_id) {\n'''
        '''  const TStructsSchemaItems::const_iterator it = '''
        '''struct_schema_items.find(struct_id);\n'''
        '''  if (it != struct_schema_items.end()) {\n'''
        '''    return it->second;\n'''
        '''  }\n'''
        '''\n'''
        '''  return ns_smart_device_link::ns_smart_objects::'''
        '''CAlwaysFalseSchemaItem::create();\n'''
        '''}\n'''
        '''\n'''
        '''bool $namespace::$class_name::AddCustomMember(FunctionID::eType function_id,\n'''
        '''                                              messageType::eType message_type,\n'''
        '''                                              std::string member_key, 
        ns_smart_device_link::ns_smart_objects::SMember member) {\n'''
        '''  using namespace ns_smart_device_link::ns_json_handler;\n'''
        '''  using namespace ns_smart_device_link::ns_smart_objects;\n'''
        '''  SmartSchemaKey<FunctionID::eType, messageType::eType> shema_key(function_id, message_type);\n'''
        '''  auto function_schema = functions_schemes_.find(shema_key);\n'''
        '''  if (functions_schemes_.end() == function_schema){\n'''
        '''    return false;\n'''
        '''  }\n'''
        '''\n'''
        '''  auto schema = function_schema->second.getSchemaItem();\n'''
        '''  auto msg_params_schema_item = schema->GetMemberSchemaItem(
        ns_smart_device_link::ns_json_handler::strings::S_MSG_PARAMS);\n'''
        '''  if (!msg_params_schema_item.is_initialized()){\n'''
        '''    return false;\n'''
        '''  }\n'''
        '''\n'''
        '''  msg_params_schema_item->mSchemaItem->AddMemberSchemaItem(member_key, member);\n'''
        '''  return true;\n'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::ResetFunctionSchema(FunctionID::eType function_id,\n'''
        '''                         messageType::eType message_type) {\n'''
        '''  InitFunctionSchema(function_id, message_type);\n'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::InitStructSchemes(\n'''
        '''    TStructsSchemaItems &struct_schema_items) {'''
        '''$struct_schema_items'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::InitFunctionSchemes(\n'''
        '''    const TStructsSchemaItems &struct_schema_items,\n'''
        '''    const std::set<FunctionID::eType> &function_id_items,\n'''
        '''    const std::set<messageType::eType> &message_type_items) {\n'''
        '''$pre_function_schemas'''
        '''$function_schemas'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::InitFunctionSchema(\n'''
        '''    const FunctionID::eType &function_id,\n'''
        '''    const messageType::eType &message_type) {\n'''
        '''\n'''
        '''  TStructsSchemaItems struct_schema_items;\n'''
        '''  InitStructSchemes(struct_schema_items);\n'''
        '''\n'''
        '''  std::set<FunctionID::eType> function_id_items { function_id };\n'''
        '''  std::set<messageType::eType> message_type_items { message_type };\n'''
        '''\n'''
        '''$function_schemas_switch'''
        '''}\n'''
        '''\n'''
        '''//------------- Functions schemes initialization -------------\n'''
        '''\n'''
        '''$init_function_impls'''
        '''\n'''
        '''//----------- Structs schema items initialization ------------\n'''
        '''\n'''
        '''$init_structs_impls'''
        '''\n'''
        '''//-------------- String to value enum mapping ----------------\n'''
        '''\n'''
        '''namespace ns_smart_device_link {\n'''
        '''namespace ns_smart_objects {\n'''
        '''\n'''
        '''$enum_string_coversions'''
        '''\n'''
        '''} // ns_smart_objects\n'''
        '''} // ns_smart_device_link\n'''
        '''\n''')

    _enum_to_str_converter_template = string.Template(
        '''template<>\n'''
        '''const EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''EnumToCStringMap\n'''
        '''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''enum_to_cstring_map_ =\n'''
        '''  EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''InitEnumToCStringMap();\n'''
        '''\n'''
        '''template<>\n'''
        '''const EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''CStringToEnumMap\n'''
        '''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''cstring_to_enum_map_ =\n'''
        '''  EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''InitCStringToEnumMap();\n'''
        '''\n'''
        '''template<>\n'''
        '''const char* const\n'''
        '''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''cstring_values_[] = {\n'''
        '''${cstringvalues}'''
        '''};\n'''
        '''\n'''
        '''template<>\n'''
        '''const ${namespace}::${enum}::eType\n'''
        '''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        '''enum_values_[] = {\n'''
        '''${enumvalues}'''
        '''};\n'''
        '''\n''')

    _function_switch_template = string.Template(
        '''switch(${switchable}) {\n'''
        '''${cases}'''
        '''}\n''')

    _message_type_case_template = string.Template(
        '''case messageType::${message_type}: {\n'''
        '''${case_body}'''
        '''  break;\n'''
        '''}\n''')

    _function_id_case_template = string.Template(
        '''case FunctionID::${function_id}: {\n'''
        '''  ns_smart_device_link::ns_json_handler::SmartSchemaKey<FunctionID::eType, messageType::eType> shema_key(
        function_id, message_type);\n'''
        '''  functions_schemes_[shema_key] = '''
        '''InitFunction_${function_id}_${message_type}('''
        '''struct_schema_items, function_id_items, message_type_items);\n'''
        '''  break;\n'''
        '''}\n''')

    _struct_schema_item_template = string.Template(
        '''std::shared_ptr<ISchemaItem> struct_schema_item_${name} = '''
        '''InitStructSchemaItem_${name}(struct_schema_items);\n'''
        '''struct_schema_items.insert(std::make_pair('''
        '''StructIdentifiers::${name}, struct_schema_item_${name}));\n'''
        '''structs_schemes_.insert(std::make_pair('''
        '''StructIdentifiers::${name}, CSmartSchema('''
        '''struct_schema_item_${name})));''')

    _function_schema_template = string.Template(
        '''functions_schemes_.insert(std::make_pair(ns_smart_device_link::'''
        '''ns_json_handler::'''
        '''SmartSchemaKey<FunctionID::eType, messageType::eType>'''
        '''(FunctionID::$function_id, messageType::$message_type), '''
        '''InitFunction_${function_id}_${message_type}('''
        '''struct_schema_items, function_id_items, message_type_items)));''')

    _struct_impl_template = string.Template(
        '''std::shared_ptr<ISchemaItem> $namespace::$class_name::'''
        '''InitStructSchemaItem_${struct_name}(\n'''
        '''    const TStructsSchemaItems &struct_schema_items) {\n'''
        '''$code'''
        '''}\n''')

    _struct_impl_code_tempate = string.Template(
        '''${schema_loc_decl}'''
        '''${schema_items_decl}'''
        '''Members '''
        '''schema_members;\n\n'''
        '''${schema_item_fill}'''
        '''return CObjectSchemaItem::create(schema_members);''')

    _impl_code_loc_decl_enum_template = string.Template(
        '''std::set<${type}::eType> ${var_name};''')

    _impl_code_loc_decl_enum_history_set_template = string.Template(
        '''std::map<${type}::eType, std::vector<ElementSignature>> ${type}_element_signatures;''')

    _impl_code_loc_decl_enum_insert_template = string.Template(
        '''${var_name}.insert(${enum}::${value});''')

    _impl_code_loc_decl_enum_history_set_value_init_template = string.Template(
        '''${enum}_element_signatures[${enum}::${value}] = std::vector<ElementSignature>();'''
    )

    _impl_code_loc_decl_enum_history_set_insert_template = string.Template(
        '''${enum}_element_signatures[${enum}::${value}].push_back(ElementSignature("${since}", "${until}", 
        ${removed}));''')

    _impl_gen_schema_enum_history_map_template = string.Template(
        '''${name}_element_signatures''')

    _impl_code_item_decl_temlate = string.Template(
        '''${comment}'''
        '''std::shared_ptr<ISchemaItem> ${var_name} = ${item_decl};''')

    _impl_code_shared_ptr_vector_template = string.Template(
        '''std::vector<SMember> ${var_name}_history_vector;''')

    _impl_code_append_history_vector_template = string.Template(
        '''${vector_name}_history_vector.push_back(SMember(${name}_SchemaItem, ${mandatory}, "${since}", "${until}", 
        ${deprecated}, ${removed}));''')

    _impl_code_integer_item_template = string.Template(
        '''TNumberSchemaItem<${type}>::create(${params})''')

    _impl_code_bool_item_template = string.Template(
        '''CBoolSchemaItem::create(${params})''')

    _impl_code_string_item_template = string.Template(
        '''CStringSchemaItem::create(${params})''')

    _impl_code_array_item_template = string.Template(
        '''CArraySchemaItem::create(${params})''')

    _impl_code_struct_item_template = string.Template(
        '''ProvideObjectSchemaItemForStruct(struct_schema_items, '''
        '''StructIdentifiers::${name})''')

    _impl_code_enum_item_template = string.Template(
        '''TEnumSchemaItem<${type}::eType>::create(${params})''')

    _impl_code_enum_item_with_history_template = string.Template(
        '''TEnumSchemaItem<${type}::eType>::createWithSignatures(${params})''')

    _impl_code_item_param_value_template = string.Template(
        '''TSchemaItemParameter<$type>($value)''')

    _impl_code_item_fill_template = string.Template(
        '''schema_members["${name}"] = SMember(${var_name}, ${is_mandatory});''')

    _impl_code_item_fill_template_with_version = string.Template(
        '''schema_members["${name}"] = SMember(${var_name}, ${is_mandatory}, "${since}", "${until}", ${deprecated}, 
        ${removed});''')

    _impl_code_item_fill_template_with_version_and_history_vector = string.Template(
        '''schema_members["${name}"] = SMember(${var_name}, ${is_mandatory}, "${since}", "${until}", ${deprecated}, 
        ${removed}, ${vector_name}_history_vector);''')

    _function_impl_template = string.Template(
        '''CSmartSchema $namespace::$class_name::'''
        '''InitFunction_${function_id}_${message_type}(\n'''
        '''    const TStructsSchemaItems &struct_schema_items,\n'''
        '''    const std::set<FunctionID::eType> &function_id_items,\n'''
        '''    const std::set<messageType::eType> &message_type_items) {\n'''
        '''$code'''
        '''}\n''')

    _function_impl_code_tempate = string.Template(
        '''${schema_loc_decl}'''
        '''${schema_items_decl}'''
        '''Members '''
        '''schema_members;\n\n'''
        '''${schema_item_fill}'''
        '''Members '''
        '''params_members;\n'''
        '''${schema_params_fill}'''
        '''\n'''
        '''Members '''
        '''root_members_map;\n'''
        '''root_members_map[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_MSG_PARAMS] = '''
        '''SMember(CObjectSchemaItem::'''
        '''create(schema_members), true);\n'''
        '''root_members_map[ns_smart_device_link::ns_json_handler::'''
        '''strings::S_PARAMS] = '''
        '''SMember(CObjectSchemaItem::'''
        '''create(params_members), true);\n\n'''
        '''return CSmartSchema(CObjectSchemaItem::'''
        '''create(root_members_map));''')

    _class_h_template = string.Template(
        '''$comment\n'''
        '''class $class_name : public ns_smart_device_link::ns_json_handler::'''
        '''CSmartFactory<FunctionID::eType, messageType::eType, '''
        '''StructIdentifiers::eType> {\n'''
        ''' public:\n'''
        '''  /**\n'''
        '''   * @brief Constructor.\n'''
        '''   */\n'''
        '''  $class_name();\n'''
        '''\n'''
        '''  /**\n'''
        '''   * @brief Adds custom members to existing list of params.\n'''
        '''   */\n'''
        '''  bool AddCustomMember(FunctionID::eType function_id,\n'''
        '''                       messageType::eType message_type,\n'''
        '''                       std::string member_key, ns_smart_device_link::ns_smart_objects::SMember member);\n'''
        '''\n'''
        '''  /**\n'''
        '''   * @brief Reset function schema to state defined in API.\n'''
        '''   */\n'''
        '''  void ResetFunctionSchema(FunctionID::eType function_id,\n'''
        '''                           messageType::eType message_type);\n'''
        '''\n'''
        '''  /**\n'''
        '''   * @brief Type that maps of struct IDs to schema items.\n'''
        '''   */\n'''
        '''  typedef std::map<const StructIdentifiers::eType, '''
        '''std::shared_ptr<ns_smart_device_link::ns_smart_objects::'''
        '''ISchemaItem> > TStructsSchemaItems;\n'''
        '''\n'''
        ''' protected:\n'''
        '''  /**\n'''
        '''   * @brief Helper that allows to make reference to struct\n'''
        '''   *\n'''
        '''   * @param struct_schema_items Struct schema items.\n'''
        '''   * @param struct_id ID of structure to provide.\n'''
        '''   *\n'''
        '''   * @return std::shared_ptr of strucute\n'''
        '''   */\n'''
        '''  static '''
        '''std::shared_ptr<ns_smart_device_link::ns_smart_objects::ISchemaItem> '''
        '''ProvideObjectSchemaItemForStruct(\n'''
        '''        const TStructsSchemaItems &struct_schema_items,\n'''
        '''        const StructIdentifiers::eType struct_id);\n'''
        '''\n'''
        '''  /**\n'''
        '''   * @brief Initializes all struct schemes.\n'''
        '''   */\n'''
        '''  void InitStructSchemes('''
        '''TStructsSchemaItems &struct_schema_items);\n'''
        '''\n'''
        '''  /**\n'''
        '''   * @brief Initializes all function schemes.\n'''
        '''   *\n'''
        '''   * @param struct_schema_items Struct schema items.\n'''
        '''   * @param function_id_items Set of all elements '''
        '''of FunctionID enum.\n'''
        '''   * @param message_type_items Set of all elements '''
        '''of messageType enum.\n'''
        '''   */\n'''
        '''  void InitFunctionSchemes(\n'''
        '''      const TStructsSchemaItems &struct_schema_items,\n'''
        '''      const std::set<FunctionID::eType> &function_id_items,\n'''
        '''      const std::set<messageType::eType> '''
        '''&message_type_items);\n'''
        '''\n'''
        '''  /**\n'''
        '''   * @brief Initializes single function schema.\n'''
        '''   *\n'''
        '''   * @param function_id Function ID of schema to be initialized.\n'''
        '''   * @param message_type Message type of schema to be initialized.\n'''
        '''   */\n'''
        '''  void InitFunctionSchema(\n'''
        '''      const FunctionID::eType &function_id,\n'''
        '''      const messageType::eType &message_type);\n'''
        '''\n'''
        '''$init_function_decls'''
        '''\n'''
        ''' public:\n'''
        '''$init_struct_decls'''
        '''};''')

    _function_return_comment = ''' * @return ns_smart_device_link::''' \
                               '''ns_smart_objects::CSmartSchema\n'''

    _function_decl_template = string.Template(
        '''$comment\n'''
        '''static ns_smart_device_link::ns_smart_objects::CSmartSchema '''
        '''InitFunction_${function_id}_${message_type}(\n'''
        '''    const TStructsSchemaItems &struct_schema_items,\n'''
        '''    const std::set<FunctionID::eType> &function_id_items,\n'''
        '''    const std::set<messageType::eType> &message_type_items);''')

    _struct_decl_template = string.Template(
        '''$comment\n'''
        '''static '''
        '''std::shared_ptr<ns_smart_device_link::ns_smart_objects::ISchemaItem> '''
        '''InitStructSchemaItem_${struct_name}(\n'''
        '''    const TStructsSchemaItems &struct_schema_items);''')

    _class_comment_template = string.Template(
        '''/**\n'''
        ''' * @brief Class $class_name.\n'''
        ''' *\n'''
        ''' * Params:\n'''
        '''$class_params'''
        ''' */''')

    _comment_doxygen_template = string.Template(
        '''/**\n'''
        '''$brief_description'''
        '''$description'''
        '''$design_description'''
        '''$issues'''
        '''$todos'''
        '''$returns */''')

    _comment_cc_template = string.Template(
        '''$brief_description'''
        '''$description'''
        '''$design_description'''
        '''$issues'''
        '''$todos'''
        '''$returns''')

    _enum_template = string.Template(
        '''namespace $name {\n'''
        '''$comment\n'''
        '''enum eType {\n'''
        '''$enum_items};\n'''
        '''} // $name\n''')

    _enum_element_with_value_template = string.Template(
        '''$comment\n'''
        '''$name = $value''')

    _enum_element_with_no_value_template = string.Template(
        '''$comment\n'''
        '''$name''')

    _indent_template = "  "
