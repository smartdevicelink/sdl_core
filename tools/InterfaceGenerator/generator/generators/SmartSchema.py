"""SmartSchema generator.

Generator for SmartObjects schema source code.

"""
# pylint: disable=W0402
# pylint: disable=C0302
import os
import string
import uuid
import codecs

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
        self._structs_add_code = u""

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

        namespace = unicode(namespace)

        if interface is None:
            raise GenerateError("Given interface is None.")

        self._generated_structs = []
        self._structs_add_code = ""

        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)

        indent_level = 0
        namespace_open = u""
        namespace_close = u""

        if namespace:
            parts = namespace.split(u"::")
            for part in parts:
                namespace_open = u"".join([namespace_open, self._indent_code(
                    self._namespace_open_template.substitute(name=part),
                    indent_level)])
                namespace_close = "".join([namespace_close, self._indent_code(
                    "}", len(parts) - indent_level - 1)])
                indent_level = indent_level + 1

        class_name = unicode(os.path.splitext(filename)[0])
        guard = u"__CSMARTFACTORY_{0}_{1}_HPP__".format(
            class_name.upper(),
            unicode(uuid.uuid1().hex.capitalize()))
        header_file_name = "".join("{0}.hpp".format(class_name))

        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            f_h.write(self._hpp_file_tempalte.substitute(
                class_name=class_name,
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

        function_id_items = u""
        if "FunctionID" in interface.enums:
            function_id = interface.enums["FunctionID"]
            function_id_items = u"\n".join(
                [self._impl_code_loc_decl_enum_insert_template.substitute(
                    var_name="FunctionIDItems",
                    enum=function_id.name,
                    value=x.primary_name)
                 for x in function_id.elements.values()])

        message_type_items = u""
        if "messageType" in interface.enums:
            message_type = interface.enums["messageType"]
            message_type_items = u"\n".join(
                [self._impl_code_loc_decl_enum_insert_template.substitute(
                    var_name="MessageTypeItems",
                    enum=message_type.name,
                    value=x.primary_name)
                 for x in message_type.elements.values()])

        with codecs.open(os.path.join(destination_dir,
                                      "".join("{0}.cpp".format(class_name))),
                         encoding="utf-8", mode="w") as f_s:
            f_s.write(self._cpp_file_template.substitute(
                header_file_name=unicode(header_file_name),
                namespace=namespace,
                class_name=class_name,
                function_id_items=self._indent_code(function_id_items, 1),
                message_type_items=self._indent_code(message_type_items, 1),
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
                    interface.enums.values(),
                    namespace)))

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

        return u"\n".join([self._indent_code(
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

        return u"\n".join([self._enum_to_str_mapping_template.substitute(
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

        return u"\n".join([self._indent_code(
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

        return u"\n".join([self._indent_code(
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

        self._structs_add_code = u"".join(
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

        return u"".join([self._indent_code(
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

        return u"\n".join([self._gen_struct_impl(
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

        result = u""
        for member in members:
            if type(member.param_type) is Model.Enum and \
               member.param_type.name not in processed_enums:
                local_var = self._gen_schema_loc_emum_var_name(
                    member.param_type)
                result = u"\n".join(
                    [u"".join(
                        [result, self._impl_code_loc_decl_enum_template.
                            substitute(
                                type=member.param_type.name,
                                var_name=local_var)]),
                        u"\n".join(
                            [self._impl_code_loc_decl_enum_insert_template.
                                substitute(
                                    var_name=local_var,
                                    enum=member.param_type.name,
                                    value=x.primary_name)
                             for x in member.param_type.elements.values()])])
                processed_enums.append(member.param_type.name)
                result = u"".join([result, u"\n\n"]) if result else u""
            elif type(member.param_type) is Model.EnumSubset:
                local_var = self._gen_schema_loc_emum_s_var_name(member.name)
                result = u"\n".join(
                    [u"".join(
                        [result, self._impl_code_loc_decl_enum_template.
                            substitute(
                                type=member.param_type.enum.name,
                                var_name=local_var)]),
                        u"\n".join(
                            [self._impl_code_loc_decl_enum_insert_template.
                                substitute(
                                    var_name=local_var,
                                    enum=member.param_type.enum.name,
                                    value=x.primary_name)
                             for x in member.param_type.
                             allowed_elements.values()])])
                result = "".join([result, u"\n\n"]) if result else u""
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

        result = u"\n\n".join(
            [self._gen_schema_item_decl(x) for x in members])

        return u"".join([result, u"\n\n"]) if result else u""

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

        code = u""
        if type(param) is Model.Boolean:
            code = self._impl_code_bool_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [[u"bool", None if default_value is None
                      else u"true" if default_value is True else u"false"]]))
        elif type(param) is Model.Integer:
            code = self._impl_code_integer_item_template.substitute(
                type=u"int",
                params=self._gen_schema_item_param_values(
                    [[u"int", param.min_value],
                     [u"int", param.max_value],
                     [u"int", default_value]]))
        elif type(param) is Model.Double:
            code = self._impl_code_integer_item_template.substitute(
                type=u"double",
                params=self._gen_schema_item_param_values(
                    [[u"double", param.min_value],
                     [u"double", param.max_value],
                     [u"double", default_value]]))
        elif type(param) is Model.String:
            code = self._impl_code_string_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [[u"size_t", param.max_length],
                     [u"std::string", default_value]]))
        elif type(param) is Model.Array:
            code = self._impl_code_array_item_template.substitute(
                params=u"".join(
                    [u"".join(
                        [self._gen_schema_item_decl_code(
                            param.element_type,
                            param.element_type.name if type(param.element_type)
                            is Model.EnumSubset else u"",
                            None),
                            u", "]),
                        self._gen_schema_item_param_values(
                            [[u"size_t", param.min_size],
                             [u"size_t", param.max_size]])]))
        elif type(param) is Model.Struct:
            code = self._impl_code_struct_item_template.substitute(
                name=param.name)
        elif type(param) is Model.Enum:
            code = self._impl_code_enum_item_template.substitute(
                type=param.name,
                params="".join(
                    [self._gen_schema_loc_emum_var_name(param),
                     u", ",
                     self._gen_schema_item_param_values(
                         [[u"".join([param.name, u"::eType"]),
                          default_value.primary_name if default_value
                          is not None else None]])]))
        elif type(param) is Model.EnumSubset:
            code = self._impl_code_enum_item_template.substitute(
                type=param.enum.name,
                params=u"".join(
                    [self._gen_schema_loc_emum_s_var_name(member_name),
                     u", ",
                     self._gen_schema_item_param_values(
                         [["".join([param.enum.name, u"::eType"]),
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

        result = u""
        for param in params:
            value = self._impl_code_item_param_value_template.substitute(
                type=param[0],
                value=str(param[1] if param[1] is not None else ""))
            result = u"".join([result, u"".join(
                [u", ", value])
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

        result = u"\n".join(
            [self._gen_schema_item_fill(x) for x in members])

        return u"".join([result, u"\n\n"]) if result else u""

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
            is_mandatory=u"true" if member.is_mandatory is True else u"false")

    @staticmethod
    def _gen_schema_item_var_name(member):
        """Generate schema item variable name.

        Generates variable name for local declarations.

        Keyword arguments:
        member -- struct member/function parameter to process.

        Returns:
        String with schema item variable name.

        """

        return u"".join([member.name, u"_SchemaItem"])

    @staticmethod
    def _gen_schema_loc_emum_var_name(param_type):
        """Generate name of enum local variable.

        Generates name of local variable that defines allowed enum elements.

        Keyword arguments:
        param_type -- parameter type object.

        Returns:
        String with name of enum local variable.

        """

        return u"".join([param_type.name, u"_allEnumValues"])

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

        return u"".join([member_name, "_allowedEnumSubsetValues"])

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

        return u"\n".join([self._gen_function_impl(
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
                [u" *     {0} - {1}\n".format(x[0],
                 x[1]) for x in params.items()])
            if params else u" *    none\n")

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
                                for x in interface_item_base.description])
        if description is not u"":
            description = u"".join([u" *\n" if use_doxygen
                                    is True else u"//\n", description])

        design_description = u"".join([(u" * {0}\n" if use_doxygen is
                                       True else u"// {0}\n").format(x)
                                       for x in
                                       interface_item_base.design_description])
        if design_description is not u"":
            design_description = u"".join([u" *\n" if use_doxygen is
                                           True else "//\n",
                                           design_description])

        issues = u"".join([(u" * @note {0}\n" if use_doxygen is
                           True else u"// Note: {0}\n").format(x.value)
                           for x in interface_item_base.issues])
        if issues is not u"":
            issues = u"".join([u" *\n" if use_doxygen is
                              True else u"//\n", issues])

        todos = u"".join([(u" * @todo {0}\n" if use_doxygen is
                          True else u"// ToDo: {0}\n").format(x)
                          for x in interface_item_base.todos])
        if todos is not u"":
            todos = u"".join([u" *\n" if use_doxygen is
                              True else u"//\n", todos])

        returns = u""
        if type(interface_item_base) is Model.Function:
            returns = "".join([u" *\n", self._function_return_comment])

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
        return u"".join(
            [u"{0}{1}\n".format(
                self._indent_template * indent_level,
                x) if x is not u"" else u"\n" for x in code_lines])

    _model_types_briefs = dict(
        {u"EnumElement": u"",
         u"Enum": u"Enumeration ",
         u"Function": u"Method that generates schema for function ",
         u"Struct": u"Method that generates schema item for structure ",
         u"Param": u"Struct member ",
         u"FunctionParam": u"Function parameter "})

    _hpp_file_tempalte = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.hpp\n'''
        u''' * @brief Generated class ${class_name} header file.\n'''
        u''' *\n'''
        u''' * This class is a part of SmartObjects solution. It provides\n'''
        u''' * factory functionallity which allows client to use '''
        u'''SmartSchemas\n'''
        u''' * in accordance with definitions from ${class_name}.xml file\n'''
        u''' */\n'''
        u'''// Copyright (c) 2013, Ford Motor Company\n'''
        u'''// All rights reserved.\n'''
        u'''//\n'''
        u'''// Redistribution and use in source and binary forms, '''
        u'''with or without\n'''
        u'''// modification, are permitted provided that the following '''
        u'''conditions are met:\n'''
        u'''//\n'''
        u'''// Redistributions of source code must retain the above '''
        u'''copyright notice, this\n'''
        u'''// list of conditions and the following disclaimer.\n'''
        u'''//\n'''
        u'''// Redistributions in binary form must reproduce '''
        u'''the above copyright notice,\n'''
        u'''// this list of conditions and the following\n'''
        u'''// disclaimer in the documentation and/or other materials '''
        u'''provided with the\n'''
        u'''// distribution.\n'''
        u'''//\n'''
        u'''// Neither the name of the Ford Motor Company nor the names '''
        u'''of its contributors\n'''
        u'''// may be used to endorse or promote products derived '''
        u'''from this software\n'''
        u'''// without specific prior written permission.\n'''
        u'''//\n'''
        u'''// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND '''
        u'''CONTRIBUTORS "AS IS"\n'''
        u'''// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT '''
        u'''LIMITED TO, THE\n'''
        u'''// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR '''
        u''''A PARTICULAR PURPOSE\n'''
        u'''// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER '''
        u'''OR CONTRIBUTORS BE\n'''
        u'''// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, '''
        u'''EXEMPLARY, OR\n'''
        u'''// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, '''
        u'''PROCUREMENT OF\n'''
        u'''// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; '''
        u'''OR BUSINESS\n'''
        u'''// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, '''
        u'''WHETHER IN\n'''
        u'''// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE '''
        u'''OR OTHERWISE)\n'''
        u'''// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF '''
        u'''ADVISED OF THE\n'''
        u'''// POSSIBILITY OF SUCH DAMAGE.\n\n'''
        u'''#ifndef $guard\n'''
        u'''#define $guard\n'''
        u'''\n'''
        u'''#include "JSONHandler/CSmartFactory.hpp"\n'''
        u'''#include "SmartObjects/CSmartSchema.hpp"\n'''
        u'''#include "SmartObjects/ISchemaItem.hpp"\n'''
        u'''#include "Utils/shared_ptr.h"\n'''
        u'''\n'''
        u'''$namespace_open'''
        u'''$enums_content'''
        u'''$class_content'''
        u'''$namespace_close'''
        u'''#endif //$guard\n'''
        u'''\n\n''')

    _namespace_open_template = string.Template(
        u'''namespace $name\n'''
        u'''{''')

    _cpp_file_template = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.cpp\n'''
        u''' * @brief Generated class ${class_name} source file.\n'''
        u''' *\n'''
        u''' * This class is a part of SmartObjects solution. It provides\n'''
        u''' * factory functionallity which allows client to use '''
        u'''SmartSchemas\n'''
        u''' * in accordance with definitions from ${class_name}.xml file\n'''
        u''' */\n'''
        u'''// Copyright (c) 2013, Ford Motor Company\n'''
        u'''// All rights reserved.\n'''
        u'''//\n'''
        u'''// Redistribution and use in source and binary forms, '''
        u'''with or without\n'''
        u'''// modification, are permitted provided that the following '''
        u'''conditions are met:\n'''
        u'''//\n'''
        u'''// Redistributions of source code must retain the above '''
        u'''copyright notice, this\n'''
        u'''// list of conditions and the following disclaimer.\n'''
        u'''//\n'''
        u'''// Redistributions in binary form must reproduce '''
        u'''the above copyright notice,\n'''
        u'''// this list of conditions and the following\n'''
        u'''// disclaimer in the documentation and/or other materials '''
        u'''provided with the\n'''
        u'''// distribution.\n'''
        u'''//\n'''
        u'''// Neither the name of the Ford Motor Company nor the names '''
        u'''of its contributors\n'''
        u'''// may be used to endorse or promote products derived '''
        u'''from this software\n'''
        u'''// without specific prior written permission.\n'''
        u'''//\n'''
        u'''// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND '''
        u'''CONTRIBUTORS "AS IS"\n'''
        u'''// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT '''
        u'''LIMITED TO, THE\n'''
        u'''// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR '''
        u''''A PARTICULAR PURPOSE\n'''
        u'''// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER '''
        u'''OR CONTRIBUTORS BE\n'''
        u'''// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, '''
        u'''EXEMPLARY, OR\n'''
        u'''// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, '''
        u'''PROCUREMENT OF\n'''
        u'''// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; '''
        u'''OR BUSINESS\n'''
        u'''// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, '''
        u'''WHETHER IN\n'''
        u'''// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE '''
        u'''OR OTHERWISE)\n'''
        u'''// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF '''
        u'''ADVISED OF THE\n'''
        u'''// POSSIBILITY OF SUCH DAMAGE.\n\n'''
        u'''#include <map>\n'''
        u'''#include <set>\n'''
        u'''\n'''
        u'''#include "$header_file_name"\n'''
        u'''#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/CArraySchemaItem.hpp"\n'''
        u'''#include "SmartObjects/CBoolSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/CObjectSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/CStringSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/TEnumSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/TNumberSchemaItem.hpp"\n'''
        u'''#include "SmartObjects/TSchemaItemParameter.hpp"\n'''
        u'''\n'''
        u'''using namespace NsSmartDeviceLink::NsSmartObjects;\n'''
        u'''\n'''
        u'''$namespace::$class_name::$class_name()\n'''
        u''' : CSmartFactory<FunctionID::eType, messageType::eType>()\n'''
        u'''{\n'''
        u'''    TStructsSchemaItems structsSchemaItems;\n'''
        u'''    initStructSchemaItems(structsSchemaItems);\n'''
        u'''\n'''
        u'''    std::set<FunctionID::eType> FunctionIDItems;\n'''
        u'''${function_id_items}'''
        u'''\n'''
        u'''    std::set<messageType::eType> MessageTypeItems;\n'''
        u'''${message_type_items}'''
        u'''\n'''
        u'''    initSchemas(structsSchemaItems, '''
        u'''FunctionIDItems, MessageTypeItems);\n'''
        u'''}\n'''
        u'''\n'''
        u'''utils::SharedPtr<ISchemaItem> $namespace::$class_name::'''
        u'''provideObjectSchemaItemForStruct('''
        u'''const TStructsSchemaItems & StructsSchemaItems, '''
        u'''const std::string & StructName)\n'''
        u'''{\n'''
        u'''    const TStructsSchemaItems::const_iterator it = '''
        u'''StructsSchemaItems.find(StructName);\n'''
        u'''    if (it != StructsSchemaItems.end())\n'''
        u'''    {\n'''
        u'''        return it->second;\n'''
        u'''    }\n'''
        u'''\n'''
        u'''    return NsSmartDeviceLink::NsSmartObjects::'''
        u'''CAlwaysFalseSchemaItem::create();\n'''
        u'''}\n'''
        u'''\n'''
        u'''void $namespace::$class_name::initStructSchemaItems('''
        u'''TStructsSchemaItems & StructsSchemaItems)\n'''
        u'''{\n'''
        u'''$struct_schema_items'''
        u'''}\n'''
        u'''\n'''
        u'''void $namespace::$class_name::initSchemas(TStructsSchemaItems & '''
        u'''StructsSchemaItems, '''
        u'''const std::set<FunctionID::eType> & FunctionIDItems, '''
        u'''const std::set<messageType::eType> & MessageTypeItems)\n'''
        u'''{\n'''
        u'''$function_schemas'''
        u'''}\n'''
        u'''\n'''
        u'''//------------- Functions schema initialization --------------\n'''
        u'''\n'''
        u'''$init_function_impls'''
        u'''\n'''
        u'''//---------- Structs schema items initialization -------------\n'''
        u'''\n'''
        u'''$init_structs_impls'''
        u'''\n'''
        u'''//---------- Structs schema items initialization -------------\n'''
        u'''\n'''
        u'''namespace NsSmartDeviceLink\n'''
        u'''{\n'''
        u'''    namespace NsSmartObjects\n'''
        u'''    {\n'''
        u'''$enum_string_coversions'''
        u'''    }\n'''
        u'''}\n'''
        u'''\n''')

    _enum_to_str_converter_template = string.Template(
        u'''template <>\n'''
        u'''const std::map<${namespace}::${enum}::eType, '''
        u'''std::string> & TEnumSchemaItem<${namespace}::${enum}::eType>::'''
        u'''getEnumElementsStringRepresentation(void)\n'''
        u'''{\n'''
        u'''    static bool isInitialized = false;\n'''
        u'''    static std::map<${namespace}::${enum}::eType, '''
        u'''std::string> enumStringRepresentationMap;\n'''
        u'''\n'''
        u'''    if (false == isInitialized)\n'''
        u'''    {\n'''
        u'''${mapping}'''
        u'''\n'''
        u'''        isInitialized = true;\n'''
        u'''    }\n'''
        u'''\n'''
        u'''    return enumStringRepresentationMap;\n'''
        u'''}''')

    _enum_to_str_mapping_template = string.Template(
        u'''enumStringRepresentationMap.insert(std::make_pair(${namespace}::'''
        u'''${enum_name}::${enum_value}, "${string}"));''')

    _struct_schema_item_template = string.Template(
        u'''StructsSchemaItems.insert(std::make_pair("${name}", '''
        u'''initStructSchemaItem_${name}('''
        u'''StructsSchemaItems)));''')

    _function_schema_template = string.Template(
        u'''mSchemas.insert(std::make_pair(NsSmartDeviceLink::'''
        u'''NsJSONHandler::'''
        u'''SmartSchemaKey<FunctionID::eType, messageType::eType>'''
        u'''(FunctionID::$function_id, messageType::$message_type), '''
        u'''initFunction_${function_id}_${message_type}('''
        u'''StructsSchemaItems, FunctionIDItems, MessageTypeItems)));''')

    _struct_impl_template = string.Template(
        u'''utils::SharedPtr<ISchemaItem> $namespace::$class_name::'''
        u'''initStructSchemaItem_${struct_name}('''
        u'''TStructsSchemaItems & StructsSchemaItems)\n'''
        u'''{\n'''
        u'''$code'''
        u'''}\n''')

    _struct_impl_code_tempate = string.Template(
        u'''${schema_loc_decl}'''
        u'''${schema_items_decl}'''
        u'''std::map<std::string, CObjectSchemaItem::SMember> '''
        u'''schemaMembersMap;\n\n'''
        u'''${schema_item_fill}'''
        u'''return CObjectSchemaItem::create(schemaMembersMap);''')

    _impl_code_loc_decl_enum_template = string.Template(
        u'''std::set<${type}::eType> ${var_name};''')

    _impl_code_loc_decl_enum_insert_template = string.Template(
        u'''${var_name}.insert(${enum}::${value});''')

    _impl_code_item_decl_temlate = string.Template(
        u'''${comment}'''
        u'''utils::SharedPtr<ISchemaItem> ${var_name} = ${item_decl};''')

    _impl_code_integer_item_template = string.Template(
        u'''TNumberSchemaItem<${type}>::create(${params})''')

    _impl_code_bool_item_template = string.Template(
        u'''CBoolSchemaItem::create(${params})''')

    _impl_code_string_item_template = string.Template(
        u'''CStringSchemaItem::create(${params})''')

    _impl_code_array_item_template = string.Template(
        u'''CArraySchemaItem::create(${params})''')

    _impl_code_struct_item_template = string.Template(
        u'''provideObjectSchemaItemForStruct('''
        u'''StructsSchemaItems, "${name}")''')

    _impl_code_enum_item_template = string.Template(
        u'''TEnumSchemaItem<${type}::eType>::create(${params})''')

    _impl_code_item_param_value_template = string.Template(
        u'''TSchemaItemParameter<$type>($value)''')

    _impl_code_item_fill_template = string.Template(
        u'''schemaMembersMap["${name}"] = CObjectSchemaItem::'''
        u'''SMember(${var_name}, ${is_mandatory});''')

    _function_impl_template = string.Template(
        u'''CSmartSchema $namespace::$class_name::'''
        u'''initFunction_${function_id}_${message_type}('''
        u'''const TStructsSchemaItems & StructsSchemaItems, '''
        u'''const std::set<FunctionID::eType> & FunctionIDItems, '''
        u'''const std::set<messageType::eType> & MessageTypeItems)\n'''
        u'''{\n'''
        u'''$code'''
        u'''}\n''')

    _function_impl_code_tempate = string.Template(
        u'''${schema_loc_decl}'''
        u'''${schema_items_decl}'''
        u'''std::map<std::string, CObjectSchemaItem::SMember> '''
        u'''schemaMembersMap;\n\n'''
        u'''${schema_item_fill}'''
        u'''std::map<std::string, CObjectSchemaItem::SMember> '''
        u'''paramsMembersMap;\n'''
        u'''paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_FUNCTION_ID] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<FunctionID::eType>::'''
        u'''create(FunctionIDItems), true);\n'''
        u'''paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_MESSAGE_TYPE] = CObjectSchemaItem::'''
        u'''SMember(TEnumSchemaItem<messageType::eType>::'''
        u'''create(MessageTypeItems), true);\n'''
        u'''paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_CORRELATION_ID] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n'''
        u'''paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(1, 2), true);\n'''
        u'''paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::'''
        u'''SMember(TNumberSchemaItem<int>::create(), true);\n\n'''
        u'''std::map<std::string, CObjectSchemaItem::SMember> '''
        u'''rootMembersMap;\n'''
        u'''rootMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_MSG_PARAMS] = '''
        u'''CObjectSchemaItem::SMember(CObjectSchemaItem::'''
        u'''create(schemaMembersMap), true);\n'''
        u'''rootMembersMap[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_PARAMS] = '''
        u'''CObjectSchemaItem::SMember(CObjectSchemaItem::'''
        u'''create(paramsMembersMap), true);\n\n'''
        u'''return CSmartSchema(CObjectSchemaItem::'''
        u'''create(rootMembersMap));''')

    _class_hpp_template = string.Template(
        u'''$comment\n'''
        u'''class $class_name : public NsSmartDeviceLink::NsJSONHandler::'''
        u'''CSmartFactory<FunctionID::eType, messageType::eType>\n'''
        u'''{\n'''
        u'''public:\n'''
        u'''\n'''
        u'''    /**\n'''
        u'''     * @brief Constructor.\n'''
        u'''     */\n'''
        u'''    $class_name(void);\n'''
        u'''\n'''
        u'''protected:\n'''
        u'''\n'''
        u'''    /**\n'''
        u'''     * @brief Type that maps of struct names to schema items.\n'''
        u'''     */\n'''
        u'''    typedef std::map<std::string, '''
        u'''utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::'''
        u'''ISchemaItem> > TStructsSchemaItems;\n'''
        u'''\n'''
        u'''    /**\n'''
        u'''     * @brief Helper that allows to make reference to struct\n'''
        u'''     *\n'''
        u'''     * @param StructName Name of structure to provide.\n'''
        u'''     *\n'''
        u'''     * @return utils::SharedPtr of strucute\n'''
        u'''     */\n'''
        u'''    static '''
        u'''utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> '''
        u'''provideObjectSchemaItemForStruct(const TStructsSchemaItems & '''
        u'''StructsSchemaItems, const std::string & StructName);\n'''
        u'''\n'''
        u'''    /**\n'''
        u'''     * @brief Initializes all struct schema items.\n'''
        u'''     *\n'''
        u'''     * @param StructsSchemaItems Map of struct schema items.\n'''
        u'''     */\n'''
        u'''    void initStructSchemaItems(TStructsSchemaItems & '''
        u'''StructsSchemaItems);\n'''
        u'''\n'''
        u'''    /**\n'''
        u'''     * @brief Initializes all schemas.\n'''
        u'''     *\n'''
        u'''     * @param StructsSchemaItems Map of struct schema items.\n'''
        u'''     * @param FunctionIDItems Set of all elements '''
        u'''of FunctionID enum.\n'''
        u'''     * @param MessageTypeItems Set of all elements '''
        u'''of messageType enum.\n'''
        u'''     */\n'''
        u'''    void initSchemas(TStructsSchemaItems & StructsSchemaItems, '''
        u'''const std::set<FunctionID::eType> & FunctionIDItems, '''
        u'''const std::set<messageType::eType> & MessageTypeItems);\n'''
        u'''\n'''
        u'''$init_function_decls'''
        u'''\n'''
        u'''$init_struct_decls'''
        u'''};''')

    _function_return_comment = u''' * @return NsSmartDeviceLink::''' \
                               u'''NsSmartObjects::CSmartSchema\n'''

    _function_decl_template = string.Template(
        u'''$comment\n'''
        u'''static NsSmartDeviceLink::NsSmartObjects::CSmartSchema '''
        u'''initFunction_${function_id}_${message_type}('''
        u'''const TStructsSchemaItems & StructsSchemaItems, '''
        u'''const std::set<FunctionID::eType> & FunctionIDItems, '''
        u'''const std::set<messageType::eType> & MessageTypeItems);''')

    _struct_decl_template = string.Template(
        u'''$comment\n'''
        u'''static '''
        u'''utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> '''
        u'''initStructSchemaItem_${struct_name}('''
        u'''TStructsSchemaItems & StructsSchemaItems);''')

    _class_comment_template = string.Template(
        u'''/**\n'''
        u''' * @brief Class $class_name.\n'''
        u''' *\n'''
        u''' * Params:\n'''
        u'''$class_params'''
        u''' */''')

    _comment_doxygen_template = string.Template(
        u'''/**\n'''
        u'''$brief_description'''
        u'''$description'''
        u'''$design_description'''
        u'''$issues'''
        u'''$todos'''
        u'''$returns */''')

    _comment_cpp_template = string.Template(
        u'''$brief_description'''
        u'''$description'''
        u'''$design_description'''
        u'''$issues'''
        u'''$todos'''
        u'''$returns''')

    _enum_template = string.Template(
        u'''namespace $name\n'''
        u'''{\n'''
        u'''$comment'''
        u'''    enum eType\n'''
        u'''    {\n'''
        u'''$enum_items    };\n'''
        u'''}\n''')

    _enum_element_with_value_template = string.Template(
        u'''$comment\n'''
        u'''$name = $value''')

    _enum_element_with_no_value_template = string.Template(
        u'''$comment\n'''
        u'''$name''')

    _indent_template = u"    "
