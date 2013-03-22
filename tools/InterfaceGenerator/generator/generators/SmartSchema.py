"""SmartSchema generator.

Generator for SmartObjects schema source code.

"""
# pylint: disable=W0402
# pylint: disable=C0302
import os
import string
import uuid

from generator import Model


class GenerateError(Exception):

    """Generate error.

    This exception is raised when SmartSchema generator is unable to create
    output from given model.

    """

    pass


class SmartSchema(object):

    """SmartSchema generator.

    This class provides service which allows to generate pair of *.hpp and
    *.cpp file by given interface model.

    """

    def __init__(self):
        """Construct new object."""

        self._generated_structs = []
        self._structs_add_code = ""

    def generate(self, interface, filename, namespace, destination_dir):
        """Generate SmartObject source files.

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

        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)

        indent_level = 0
        namespace_open = ""
        namespace_close = ""

        if namespace:
            parts = namespace.split("::")
            for part in parts:
                namespace_open = "".join([namespace_open, self._indent_code(
                    self._namespace_open_template.substitute(name=part),
                    indent_level)])
                namespace_close = "".join([namespace_close, self._indent_code(
                    "}", len(parts) - indent_level - 1)])
                indent_level = indent_level + 1

        class_name = os.path.splitext(filename)[0]
        guard = "__CSMARTFACTORY_{0}_{1}_HPP__".format(
            class_name.upper(),
            uuid.uuid1().hex.capitalize())
        header_file_name = "".join("{0}.hpp".format(class_name))

        with open(os.path.join(destination_dir, header_file_name), "w") as f_h:
            f_h.write(self._hpp_file_tempalte.substitute(
                guard=guard,
                namespace_open=namespace_open,
                enums_content=self._indent_code(self._gen_enums(
                    interface.enums.values()), indent_level),
                class_content=self._indent_code(self._gen_hpp_class(
                    class_name,
                    interface.params,
                    interface.functions.values(),
                    interface.structs.values()), indent_level),
                namespace_close=namespace_close))

        self._gen_struct_schema_items(interface.structs.values())

        with open(os.path.join(destination_dir,
                               "".join("{0}.cpp".format(class_name))),
                  "w") as f_s:
            f_s.write(self._cpp_file_template.substitute(
                header_file_name=header_file_name,
                namespace=namespace,
                class_name=class_name,
                struct_schema_items=self._structs_add_code,
                function_schemas=self._gen_function_schemas(
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
                    interface.enums.values(), namespace)))

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

        return "\n".join([self._indent_code(
            self._enum_to_str_converter_template.substitute(
                namespace=namespace,
                enum=x.name,
                mapping=self._indent_code(
                    self._gen_enum_to_str_mapping(
                        x,
                        namespace),
                    2)),
            2) for x in enums])

    def _gen_enum_to_str_mapping(self, enum, namespace):
        """Generate enum to string mapping code.

        Generates part of source code with specific enum to string value
        mapping.

        Keyword arguments:
        enums -- enum to generate string mapping.
        namespace -- namespace to address enum.

        Returns:
        String value with enum to string mapping source code.

        """

        return "\n".join([self._enum_to_str_mapping_template.substitute(
            namespace=namespace,
            enum_name=enum.name,
            enum_value=x.primary_name,
            string=x.name) for x in enum.elements.values()])

    def _gen_hpp_class(self, class_name, params, functions, structs):
        """Generate source code of class for header file.

        Generates source code of class that should be used in the
        header file.

        Keyword arguments:
        class_name -- name of the class to generate.
        params -- class parameters.
        functions -- list of functions to generate methods for.
        structs -- structures to generate methods for.

        Returns:
        String with complete *.hpp file source code.

        """

        return self._class_hpp_template.substitute(
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

        This method create source code for cpp file that guarantees that
        structures are initialized in right order (to resolve all
        dependencies).

        Keyword arguments:
        struct -- struct to process.

        """

        if struct.name in self._generated_structs:
            return

        for member in struct.members.values():
            self._process_struct_member(member)

        self._structs_add_code = "".join(
            [self._structs_add_code, self._indent_code(
                self._gen_struct_schema_item(struct), 1)])
        self._generated_structs.append(struct.name)

    def _process_struct_member(self, member):
        """Process struct member recursively to provide correct initialization.

        This method ensures that nested structs (if any) are generated.

        Keyword arguments:
        member -- struct member to process.

        """

        if type(member.param_type) is Model.Struct:
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
                        struct.members.values())),
                1))

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
            if type(member.param_type) is Model.Enum and \
               member.param_type.name not in processed_enums:
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
                processed_enums.append(member.param_type.name)
                result = "".join([result, "\n\n"]) if result else ""
            elif type(member.param_type) is Model.EnumSubset:
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
            elif type(member.param_type) is Model.Array:
                result = "".join(
                    [result, self._gen_schema_loc_decls(
                        [Model.Param(name=member.param_type.element_type.name
                         if type(member.param_type.element_type) is
                         Model.EnumSubset else "",
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

        result = "\n\n".join(
            [self._gen_schema_item_decl(x) for x in members])

        return "".join([result, "\n\n"]) if result else ""

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
                member.default_value if type(member)
                is Model.FunctionParam else None))

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
        if type(param) is Model.Boolean:
            code = self._impl_code_bool_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [["bool", None if default_value is None
                      else "true" if default_value is True else "false"]]))
        elif type(param) is Model.Integer:
            code = self._impl_code_integer_item_template.substitute(
                type="int",
                params=self._gen_schema_item_param_values(
                    [["int", param.min_value],
                     ["int", param.max_value],
                     ["int", default_value]]))
        elif type(param) is Model.Double:
            code = self._impl_code_integer_item_template.substitute(
                type="double",
                params=self._gen_schema_item_param_values(
                    [["double", param.min_value],
                     ["double", param.max_value],
                     ["double", default_value]]))
        elif type(param) is Model.String:
            code = self._impl_code_string_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [["size_t", param.max_length],
                     ["std::string", default_value]]))
        elif type(param) is Model.Array:
            code = self._impl_code_array_item_template.substitute(
                params="".join(
                    ["".join(
                        [self._gen_schema_item_decl_code(
                            param.element_type,
                            param.element_type.name if type(param.element_type)
                            is Model.EnumSubset else "",
                            None),
                            ", "]),
                        self._gen_schema_item_param_values(
                            [["size_t", param.min_size],
                             ["size_t", param.max_size]])]))
        elif type(param) is Model.Struct:
            code = self._impl_code_struct_item_template.substitute(
                name=param.name)
        elif type(param) is Model.Enum:
            code = self._impl_code_enum_item_template.substitute(
                type=param.name,
                params="".join(
                    [self._gen_schema_loc_emum_var_name(param),
                     ", ",
                     self._gen_schema_item_param_values(
                         [["".join([param.name, "::eType"]),
                          default_value.primary_name if default_value
                          is not None else None]])]))
        elif type(param) is Model.EnumSubset:
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

    def _gen_schema_items_fill(self, members):
        """Generate schema items fill code.

        Generates source code that fills new schema with items.

        Keyword arguments:
        members -- list of struct members/function parameters to process.

        Returns:
        String with schema items fill code.

        """

        result = "\n".join(
            [self._gen_schema_item_fill(x) for x in members])

        return "".join([result, "\n\n"]) if result else ""

    def _gen_schema_item_fill(self, member):
        """Generate schema item fill code.

        Generates source code that fills new schema with item.

        Keyword arguments:
        member -- struct member/function parameter to process.

        Returns:
        String with schema item fill code.

        """

        return self._impl_code_item_fill_template.substitute(
            name=member.name,
            var_name=self._gen_schema_item_var_name(member),
            is_mandatory="true" if member.is_mandatory is True else "false")

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

        return "".join([param_type.name, "_allEnumValues"])

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

        return "".join([member_name, "_allowedEnumSubsetValues"])

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
                        function.params.values())),
                1))

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

        return "\n".join([self._gen_enum(x) for x in enums])

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
            name="INVALID_ENUM",
            description=None,
            design_description=None,
            issues=None,
            todos=None,
            platform=None,
            internal_name=None,
            value="-1"))
        return self._enum_template.substitute(
            comment=self._indent_code(self._gen_comment(enum), 1),
            name=enum.name,
            enum_items=self._indent_code(self._gen_enum_elements(
                enum_elements), 2))

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
            type(interface_item_base) is Model.EnumElement else \
            interface_item_base.name
        brief_description = (" * @brief {0}{1}.\n" if use_doxygen is True else "// {0}{1}.\n").format(brief_type_title, name)

        description = "".join([(" * {0}\n" if use_doxygen
                                is True else "// {0}\n").format(x)
                              for x in interface_item_base.description])
        if description is not "":
            description = "".join([" *\n" if use_doxygen
                                    is True else "//\n", description])

        design_description = "".join([(" * {0}\n" if use_doxygen is
                                       True else "// {0}\n").format(x)
                                     for x in
                                     interface_item_base.design_description])
        if design_description is not "":
            design_description = "".join([" *\n" if use_doxygen is
                                          True else "//\n", design_description])

        issues = "".join([(" * @note {0}\n" if use_doxygen is
                           True else "// Note: {0}\n").format(x.value)
                         for x in interface_item_base.issues])
        if issues is not "":
            issues = "".join([" *\n" if use_doxygen is
                              True else "//\n", issues])

        todos = "".join([(" * @todo {0}\n" if use_doxygen is
                          True else "// TODO: {0}\n").format(x)
                        for x in interface_item_base.todos])
        if todos is not "":
            todos = "".join([" *\n" if use_doxygen is True else "//\n", todos])

        returns = ""
        if type(interface_item_base) is Model.Function:
            returns = "".join([" *\n", self._function_return_comment])

        template = self._comment_doxygen_template if use_doxygen is \
            True else self._comment_cpp_template

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

    _model_types_briefs = dict(
        {"EnumElement": "",
         "Enum": "Enumeration ",
         "Function": "Method that generates schema for function ",
         "Struct": "Method that generates schema item for structure ",
         "Param": "Struct member ",
         "FunctionParam" : "Function parameter "})

    _hpp_file_tempalte = string.Template(
        '''#ifndef $guard\n'''
        '''#define $guard\n'''
        '''\n'''
        '''#include "JSONHandler/CSmartFactory.hpp"\n'''
        '''#include "SmartObjects/CSmartSchema.hpp"\n'''
        '''#include "SmartObjects/ISchemaItem.hpp"\n'''
        '''#include "SmartObjects/TSharedPtr.hpp"\n'''
        '''\n'''
        '''$namespace_open'''
        '''$enums_content'''
        '''$class_content'''
        '''$namespace_close'''
        '''#endif //$guard\n'''
        '''\n''')

    _namespace_open_template = string.Template(
        '''namespace $name\n'''
        '''{''')

    _cpp_file_template = string.Template(
        '''#include <map>\n'''
        '''#include <set>\n'''
        '''\n'''
        '''#include "$header_file_name"\n'''
        '''#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"\n'''
        '''#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"\n'''
        '''#include "SmartObjects/CArraySchemaItem.hpp"\n'''
        '''#include "SmartObjects/CBoolSchemaItem.hpp"\n'''
        '''#include "SmartObjects/CObjectSchemaItem.hpp"\n'''
        '''#include "SmartObjects/CStringSchemaItem.hpp"\n'''
        '''#include "SmartObjects/TEnumSchemaItem.hpp"\n'''
        '''#include "SmartObjects/TNumberSchemaItem.hpp"\n'''
        '''#include "SmartObjects/TSchemaItemParameter.hpp"\n'''
        '''\n'''
        '''using namespace NsAppLink::NsSmartObjects;\n'''
        '''\n'''
        '''$namespace::$class_name::$class_name()\n'''
        ''' : CSmartFactory<FunctionID::eType, messageType::eType>()\n'''
        '''{\n'''
        '''    TStructsSchemaItems structsSchemaItems;\n'''
        '''    initStructSchemaItems(structsSchemaItems);\n'''
        '''    initSchemas(structsSchemaItems);\n'''
        '''}\n'''
        '''\n'''
        '''TSharedPtr<ISchemaItem> $namespace::$class_name::'''
        '''provideObjectSchemaItemForStruct('''
        '''TStructsSchemaItems & StructsSchemaItems, '''
        '''const std::string & StructName)\n'''
        '''{\n'''
        '''    const TStructsSchemaItems::const_iterator it = '''
        '''StructsSchemaItems.find(StructName);\n'''
        '''    if (it != StructsSchemaItems.end())\n'''
        '''    {\n'''
        '''        return it->second;\n'''
        '''    }\n'''
        '''\n'''
        '''    return NsAppLink::NsSmartObjects::'''
        '''CAlwaysFalseSchemaItem::create();\n'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::initStructSchemaItems('''
        '''TStructsSchemaItems & StructsSchemaItems)\n'''
        '''{\n'''
        '''$struct_schema_items'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::initSchemas(TStructsSchemaItems & '''
        '''StructsSchemaItems)\n'''
        '''{\n'''
        '''$function_schemas'''
        '''}\n'''
        '''\n'''
        '''//------------- Functions schema initialization ---------------\n'''
        '''\n'''
        '''$init_function_impls'''
        '''\n'''
        '''//---------- Structs schema items initialization --------------\n'''
        '''\n'''
        '''$init_structs_impls'''
        '''\n'''
        '''//---------- Structs schema items initialization --------------\n'''
        '''\n'''
        '''namespace NsAppLink\n'''
        '''{\n'''
        '''    namespace NsSmartObjects\n'''
        '''    {\n'''
        '''$enum_string_coversions'''
        '''    }\n'''
        '''}\n'''
        '''\n''')

    _enum_to_str_converter_template = string.Template(
        '''template <>\n'''
        '''const std::map<${namespace}::${enum}::eType, '''
        '''std::string> & TEnumSchemaItem<${namespace}::${enum}::eType>::'''
        '''getEnumElementsStringRepresentation(void)\n'''
        '''{\n'''
        '''    static bool isInitialized = false;\n'''
        '''    static std::map<${namespace}::${enum}::eType, '''
        '''std::string> enumStringRepresentationMap;\n'''
        '''\n'''
        '''    if (false == isInitialized)\n'''
        '''    {\n'''
        '''${mapping}'''
        '''\n'''
        '''        isInitialized = true;\n'''
        '''    }\n'''
        '''\n'''
        '''    return enumStringRepresentationMap;\n'''
        '''}''')

    _enum_to_str_mapping_template = string.Template(
        '''enumStringRepresentationMap.insert(std::make_pair(${namespace}::'''
        '''${enum_name}::${enum_value}, "${string}"));''')

    _struct_schema_item_template = string.Template(
        '''StructsSchemaItems.insert(std::make_pair("${name}", '''
        '''initStructSchemaItem_${name}('''
        '''StructsSchemaItems)));''')

    _function_schema_template = string.Template(
        '''mSchemas.insert(std::make_pair(NsAppLink::NsJSONHandler::'''
        '''SmartSchemaKey<FunctionID::eType, messageType::eType>'''
        '''(FunctionID::$function_id, messageType::$message_type), '''
        '''initFunction_${function_id}_${message_type}('''
        '''StructsSchemaItems)));''')

    _struct_impl_template = string.Template(
        '''TSharedPtr<ISchemaItem> $namespace::$class_name::'''
        '''initStructSchemaItem_${struct_name}('''
        '''TStructsSchemaItems & StructsSchemaItems)\n'''
        '''{\n'''
        '''$code'''
        '''}\n''')

    _struct_impl_code_tempate = string.Template(
        '''${schema_loc_decl}'''
        '''${schema_items_decl}'''
        '''std::map<std::string, CObjectSchemaItem::SMember> '''
        '''schemaMembersMap;\n\n'''
        '''${schema_item_fill}'''
        '''return CObjectSchemaItem::create(schemaMembersMap);''')

    _impl_code_loc_decl_enum_template = string.Template(
        '''std::set<${type}::eType> ${var_name};''')

    _impl_code_loc_decl_enum_insert_template = string.Template(
        '''${var_name}.insert(${enum}::${value});''')

    _impl_code_item_decl_temlate = string.Template(
        '''${comment}'''
        '''TSharedPtr<ISchemaItem> ${var_name} = ${item_decl};''')

    _impl_code_integer_item_template = string.Template(
        '''TNumberSchemaItem<${type}>::create(${params})''')

    _impl_code_bool_item_template = string.Template(
        '''CBoolSchemaItem::create(${params})''')

    _impl_code_string_item_template = string.Template(
        '''CStringSchemaItem::create(${params})''')

    _impl_code_array_item_template = string.Template(
        '''CArraySchemaItem::create(${params})''')

    _impl_code_struct_item_template = string.Template(
        '''provideObjectSchemaItemForStruct(StructsSchemaItems, "${name}")''')

    _impl_code_enum_item_template = string.Template(
        '''TEnumSchemaItem<${type}::eType>::create(${params})''')

    _impl_code_item_param_value_template = string.Template(
        '''TSchemaItemParameter<$type>($value)''')

    _impl_code_item_fill_template = string.Template(
        '''schemaMembersMap["${name}"] = CObjectSchemaItem::'''
        '''SMember(${var_name}, ${is_mandatory});''')

    _function_impl_template = string.Template(
        '''CSmartSchema $namespace::$class_name::'''
        '''initFunction_${function_id}_${message_type}('''
        '''TStructsSchemaItems & StructsSchemaItems)\n'''
        '''{\n'''
        '''$code'''
        '''}\n''')

    _function_impl_code_tempate = string.Template(
        '''${schema_loc_decl}'''
        '''${schema_items_decl}'''
        '''std::map<std::string, CObjectSchemaItem::SMember> '''
        '''schemaMembersMap;\n\n'''
        '''${schema_item_fill}'''
        '''return CSmartSchema(CObjectSchemaItem::'''
        '''create(schemaMembersMap));''')

    _class_hpp_template = string.Template(
        '''$comment\n'''
        '''class $class_name : public NsAppLink::NsJSONHandler::'''
        '''CSmartFactory<FunctionID::eType, messageType::eType>\n'''
        '''{\n'''
        '''public:\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Constructor.\n'''
        '''     */\n'''
        '''    $class_name(void);\n'''
        '''\n'''
        '''protected:\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Type that maps of struct names to schema items.\n'''
        '''     */\n'''
        '''    typedef std::map<std::string, NsAppLink::NsSmartObjects::'''
        '''TSharedPtr<NsAppLink::NsSmartObjects::'''
        '''ISchemaItem> > TStructsSchemaItems;\n'''
        '''\n'''
        '''   /**\n'''
        '''    * @brief Helper that allows to make reference to struct\n'''
        '''    *\n'''
        '''    * @param StructName Name of structure to provide.\n'''
        '''    *\n'''
        '''    * @return TSharedPtr of strucute\n'''
        '''    */\n'''
        '''    static NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::'''
        '''NsSmartObjects::ISchemaItem> '''
        '''provideObjectSchemaItemForStruct(TStructsSchemaItems & '''
        '''StructsSchemaItems, const std::string & StructName);\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Initializes all struct schema items.\n'''
        '''     */\n'''
        '''    void initStructSchemaItems(TStructsSchemaItems & '''
        '''StructsSchemaItems);\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Initializes all schemas.\n'''
        '''     */\n'''
        '''    void initSchemas(TStructsSchemaItems & StructsSchemaItems);\n'''
        '''\n'''
        '''$init_function_decls'''
        '''\n'''
        '''$init_struct_decls'''
        '''};''')

    _function_return_comment = ''' * @return NsAppLink::NsSmartObjects::''' \
                               '''CSmartSchema\n'''

    _function_decl_template = string.Template(
        '''$comment\n'''
        '''static NsAppLink::NsSmartObjects::CSmartSchema '''
        '''initFunction_${function_id}_${message_type}('''
        '''TStructsSchemaItems & StructsSchemaItems);''')

    _struct_decl_template = string.Template(
        '''$comment\n'''
        '''static NsAppLink::NsSmartObjects::'''
        '''TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> '''
        '''initStructSchemaItem_${struct_name}('''
        '''TStructsSchemaItems & StructsSchemaItems);''')

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

    _comment_cpp_template = string.Template(
        '''$brief_description'''
        '''$description'''
        '''$design_description'''
        '''$issues'''
        '''$todos'''
        '''$returns''')

    _enum_template = string.Template(
        '''namespace $name\n'''
        '''{\n'''
        '''$comment'''
        '''    enum eType\n'''
        '''    {\n'''
        '''$enum_items    };\n'''
        '''}\n''')

    _enum_element_with_value_template = string.Template(
        '''$comment\n'''
        '''$name = $value''')

    _enum_element_with_no_value_template = string.Template(
        '''$comment\n'''
        '''$name''')

    _indent_template = "    "
