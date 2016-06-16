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

    This exception is raised when SmartFactory generator is unable to create
    output from given model.

    """

    pass


class CodeGenerator(object):

    """Base SmartFactory generator.

    This class provides service which allows to generate pair of *.h and
    *.cc files by given interface model.

    """

    def __init__(self):
        """Construct new object."""

        self._generated_structs = []
        self._structs_add_code = u""

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

        namespace = unicode(namespace)

        if interface is None:
            raise GenerateError("Given interface is None.")

        self._generated_structs = []
        self._structs_add_code = ""

        if "messageType" in interface.enums:
            interface.enums["messageType"] = self._preprocess_message_type(
                interface.enums["messageType"])

        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)

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

        class_name = unicode(os.path.splitext(filename)[0])
        guard = u"__CSMARTFACTORY_{0}_{1}_H__".format(
            class_name.upper(),
            unicode(uuid.uuid1().hex.capitalize()))
        header_file_name = u"".join("{0}.h".format(class_name))

        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            f_h.write(self._h_file_tempalte.substitute(
                class_name=class_name,
                guard=guard,
                namespace_open=namespace_open,
                enums_content=self._gen_enums(
                    interface.enums.values(),
                    interface.structs.values()),
                namespace_close=namespace_close))

        self._gen_struct_schema_items(interface.structs.values())

        function_id_items = u""
        if "FunctionID" in interface.enums:
            function_id = interface.enums["FunctionID"]
            function_id_items = u"\n".join(
                [self._impl_code_loc_decl_enum_insert_template.substitute(
                    var_name="function_id_items",
                    enum=function_id.name,
                    value=x.primary_name)
                 for x in function_id.elements.values()])

        message_type_items = u""
        if "messageType" in interface.enums:
            message_type = interface.enums["messageType"]
            message_type_items = u"\n".join(
                [self._impl_code_loc_decl_enum_insert_template.substitute(
                    var_name="message_type_items",
                    enum=message_type.name,
                    value=x.primary_name)
                 for x in message_type.elements.values()])

        header_file_name = "".join("{0}_schema.h".format(class_name))
        guard = u"__CSMARTFACTORY_{0}_{1}_HPP__".format(
            class_name.upper(),
            unicode(uuid.uuid1().hex.capitalize()))
        with codecs.open(os.path.join(destination_dir, header_file_name),
                         encoding="utf-8",
                         mode="w") as f_h:
            f_h.write(self._hpp_schema_file_tempalte.substitute(
                class_name=class_name,
                guard=guard,
                header_file_name=unicode("".join("{0}.h".format(class_name))),
                namespace_open=namespace_open,
                class_content=self._gen_h_class(
                    class_name,
                    interface.params,
                    interface.functions.values(),
                    interface.structs.values()),
                namespace_close=namespace_close))

        with codecs.open(os.path.join(destination_dir,
                                      u"".join("{0}_schema.cc".format(class_name))),
                         encoding="utf-8", mode="w") as f_s:
            f_s.write(self._cc_file_template.substitute(
                header_file_name=unicode(header_file_name),
                namespace=namespace,
                class_name=class_name,
                function_id_items=self._indent_code(function_id_items, 1),
                message_type_items=self._indent_code(message_type_items, 1),
                struct_schema_items=self._structs_add_code,
                pre_function_schemas=self._gen_pre_function_schemas(
                    interface.functions.values()),
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

        return u"\n".join([self._enum_to_str_converter_template.substitute(
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
        return u",\n".join(['"' + x.name + '"' for x in enum.elements.values()])

    def _gen_enum_enum_values(self, enum, namespace):
        """Generate list of all enum values.
        Keyword arguments:
        enum -- enum to generate the list.
        namespace -- namespace to address enum.
        Returns:
        String value with enum values.
        """
        return u",\n".join([namespace + "::" + enum.name + "::" + x.primary_name for x in enum.elements.values()])

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

        self._structs_add_code = u"\n".join(
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
                result = u"".join([result, u"\n\n"]) if result else u""
            elif type(member.param_type) is Model.Array:
                result = u"".join(
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
        code = u""
        if type(param) is Model.Boolean:
            code = self._impl_code_bool_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [[u"bool", None if param.default_value is None
                      else u"true" if param.default_value is True else u"false"]]))
        elif type(param) is Model.Integer:
            if param.max_value < 2 ** 31:
                code = self._impl_code_integer_item_template.substitute(
                    type=u"int32_t",
                    params=self._gen_schema_item_param_values(
                        [[u"int32_t", param.min_value],
                         [u"int32_t", param.max_value],
                         [u"int32_t", param.default_value]]))
            elif param.max_value < 2 ** 63:
                code = self._impl_code_integer_item_template.substitute(
                    type=u"int64_t",
                    params=self._gen_schema_item_param_values(
                        [[u"int64_t", param.min_value],
                         [u"int64_t", str(param.max_value) + u"LL"],
                         [u"int64_t", param.default_value]]))
            else:
                raise GenerateError("Parameter value too large: " + str(param.max_value))
        elif type(param) is Model.Double:
            code = self._impl_code_integer_item_template.substitute(
                type=u"double",
                params=self._gen_schema_item_param_values(
                    [[u"double", param.min_value],
                     [u"double", param.max_value],
                     [u"double", param.default_value]]))
        elif type(param) is Model.String:
            code = self._impl_code_string_item_template.substitute(
                params=self._gen_schema_item_param_values(
                    [[u"size_t", param.min_length],
                     [u"size_t", param.max_length],
                     [u"std::string", u"".join(
                     [u'"', param.default_value, u'"']) if param.default_value
                         is not None else u""]]))
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
                params=u"".join(
                    [self._gen_schema_loc_emum_var_name(param),
                     u", ",
                     self._gen_schema_item_param_values(
                         [[u"".join([param.name, u"::eType"]),
                          u"".join([param.name, u"::",
                                    default_value.primary_name]) if
                           default_value is not None else None]])]))
        elif type(param) is Model.EnumSubset:
            code = self._impl_code_enum_item_template.substitute(
                type=param.enum.name,
                params=u"".join(
                    [self._gen_schema_loc_emum_s_var_name(member_name),
                     u", ",
                     self._gen_schema_item_param_values(
                         [[u"".join([param.enum.name, u"::eType"]),
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
        String with function schema items fill code.

        """

        result = u"\n".join(
            [self._gen_schema_item_fill(x) for x in members])

        return u"".join([result, u"\n\n"]) if result else u""

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

        return u"".join([param_type.name, u"_all_enum_values"])

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

        return u"".join([member_name, "_allowed_enum_subset_values"])

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
                        function.params.values()),
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
                struct_id_enum_elements[struct.name] = Model.EnumElement(
                    name=struct.name)
            return u"\n".join(
                [self._gen_enum(
                    Model.Enum(name="StructIdentifiers",
                               elements=struct_id_enum_elements)),
                 u"\n".join([self._gen_enum(x) for x in enums])])

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
            class_params=u"".join(
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

    _h_file_tempalte = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.h\n'''
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
        u'''A PARTICULAR PURPOSE\n'''
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
        u'''$namespace_open'''
        u'''$enums_content'''
        u'''$namespace_close'''
        u'''#endif //$guard\n'''
        u'''\n\n''')

    _hpp_schema_file_tempalte = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.h\n'''
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
        u'''A PARTICULAR PURPOSE\n'''
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
        u'''#include "formatters/CSmartFactory.h"\n'''
        u'''#include "smart_objects/smart_schema.h"\n'''
        u'''#include "smart_objects/schema_item.h"\n'''
        u'''#include "utils/shared_ptr.h"\n'''
        u'''#include "$header_file_name"\n'''
        u'''\n'''
        u'''$namespace_open'''
        u'''\n\n'''
        u'''$class_content'''
        u'''\n\n'''
        u'''$namespace_close'''
        u'''\n'''
        u'''#endif //$guard\n'''
        u'''\n''')

    _namespace_open_template = string.Template(
        u'''namespace $name {\n''')

    _cc_file_template = string.Template(
        u'''/**\n'''
        u''' * @file ${class_name}.cc\n'''
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
        u'''#include "smart_objects/always_true_schema_item.h"\n'''
        u'''#include "smart_objects/always_false_schema_item.h"\n'''
        u'''#include "smart_objects/array_schema_item.h"\n'''
        u'''#include "smart_objects/bool_schema_item.h"\n'''
        u'''#include "smart_objects/object_schema_item.h"\n'''
        u'''#include "smart_objects/string_schema_item.h"\n'''
        u'''#include "smart_objects/enum_schema_item.h"\n'''
        u'''#include "smart_objects/number_schema_item.h"\n'''
        u'''#include "smart_objects/schema_item_parameter.h"\n'''
        u'''\n'''
        u'''using namespace NsSmartDeviceLink::NsSmartObjects;\n'''
        u'''\n'''
        u'''$namespace::$class_name::$class_name()\n'''
        u''' : NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, '''
        u'''messageType::eType, StructIdentifiers::eType>() {\n'''
        u'''  TStructsSchemaItems struct_schema_items;\n'''
        u'''  InitStructSchemes(struct_schema_items);\n'''
        u'''\n'''
        u'''  std::set<FunctionID::eType> function_id_items;\n'''
        u'''${function_id_items}'''
        u'''\n'''
        u'''  std::set<messageType::eType> message_type_items;\n'''
        u'''${message_type_items}'''
        u'''\n'''
        u'''  InitFunctionSchemes(struct_schema_items, function_id_items, '''
        u'''message_type_items);\n'''
        u'''}\n'''
        u'''\n'''
        u'''utils::SharedPtr<ISchemaItem> $namespace::$class_name::'''
        u'''ProvideObjectSchemaItemForStruct(\n'''
        u'''    const TStructsSchemaItems &struct_schema_items,\n'''
        u'''    const StructIdentifiers::eType struct_id) {\n'''
        u'''  const TStructsSchemaItems::const_iterator it = '''
        u'''struct_schema_items.find(struct_id);\n'''
        u'''  if (it != struct_schema_items.end()) {\n'''
        u'''    return it->second;\n'''
        u'''  }\n'''
        u'''\n'''
        u'''  return NsSmartDeviceLink::NsSmartObjects::'''
        u'''CAlwaysFalseSchemaItem::create();\n'''
        u'''}\n'''
        u'''\n'''
        u'''void $namespace::$class_name::InitStructSchemes(\n'''
        u'''    TStructsSchemaItems &struct_schema_items) {'''
        u'''$struct_schema_items'''
        u'''}\n'''
        u'''\n'''
        u'''void $namespace::$class_name::InitFunctionSchemes(\n'''
        u'''    const TStructsSchemaItems &struct_schema_items,\n'''
        u'''    const std::set<FunctionID::eType> &function_id_items,\n'''
        u'''    const std::set<messageType::eType> &message_type_items) {\n'''
        u'''$pre_function_schemas'''
        u'''$function_schemas'''
        u'''}\n'''
        u'''\n'''
        u'''//------------- Functions schemes initialization -------------\n'''
        u'''\n'''
        u'''$init_function_impls'''
        u'''\n'''
        u'''//----------- Structs schema items initialization ------------\n'''
        u'''\n'''
        u'''$init_structs_impls'''
        u'''\n'''
        u'''//-------------- String to value enum mapping ----------------\n'''
        u'''\n'''
        u'''namespace NsSmartDeviceLink {\n'''
        u'''namespace NsSmartObjects {\n'''
        u'''\n'''
        u'''$enum_string_coversions'''
        u'''\n'''
        u'''} // NsSmartObjects\n'''
        u'''} // NsSmartDeviceLink\n'''
        u'''\n''')

    _enum_to_str_converter_template = string.Template(
        u'''template<>\n'''
        u'''const EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''EnumToCStringMap\n'''
        u'''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''enum_to_cstring_map_ =\n'''
        u'''  EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''InitEnumToCStringMap();\n'''
        u'''\n'''
        u'''template<>\n'''
        u'''const EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''CStringToEnumMap\n'''
        u'''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''cstring_to_enum_map_ =\n'''
        u'''  EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''InitCStringToEnumMap();\n'''
        u'''\n'''
        u'''template<>\n'''
        u'''const char* const\n'''
        u'''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''cstring_values_[] = {\n'''
        u'''${cstringvalues}'''
        u'''};\n'''
        u'''\n'''
        u'''template<>\n'''
        u'''const ${namespace}::${enum}::eType\n'''
        u'''EnumConversionHelper<${namespace}::${enum}::eType>::'''
        u'''enum_values_[] = {\n'''
        u'''${enumvalues}'''
        u'''};\n'''
        u'''\n''')

    _struct_schema_item_template = string.Template(
        u'''utils::SharedPtr<ISchemaItem> struct_schema_item_${name} = '''
        u'''InitStructSchemaItem_${name}(struct_schema_items);\n'''
        u'''struct_schema_items.insert(std::make_pair('''
        u'''StructIdentifiers::${name}, struct_schema_item_${name}));\n'''
        u'''structs_schemes_.insert(std::make_pair('''
        u'''StructIdentifiers::${name}, CSmartSchema('''
        u'''struct_schema_item_${name})));''')

    _function_schema_template = string.Template(
        u'''functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::'''
        u'''NsJSONHandler::'''
        u'''SmartSchemaKey<FunctionID::eType, messageType::eType>'''
        u'''(FunctionID::$function_id, messageType::$message_type), '''
        u'''InitFunction_${function_id}_${message_type}('''
        u'''struct_schema_items, function_id_items, message_type_items)));''')

    _struct_impl_template = string.Template(
        u'''utils::SharedPtr<ISchemaItem> $namespace::$class_name::'''
        u'''InitStructSchemaItem_${struct_name}(\n'''
        u'''    const TStructsSchemaItems &struct_schema_items) {\n'''
        u'''$code'''
        u'''}\n''')

    _struct_impl_code_tempate = string.Template(
        u'''${schema_loc_decl}'''
        u'''${schema_items_decl}'''
        u'''CObjectSchemaItem::Members '''
        u'''schema_members;\n\n'''
        u'''${schema_item_fill}'''
        u'''return CObjectSchemaItem::create(schema_members);''')

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
        u'''ProvideObjectSchemaItemForStruct(struct_schema_items, '''
        u'''StructIdentifiers::${name})''')

    _impl_code_enum_item_template = string.Template(
        u'''TEnumSchemaItem<${type}::eType>::create(${params})''')

    _impl_code_item_param_value_template = string.Template(
        u'''TSchemaItemParameter<$type>($value)''')

    _impl_code_item_fill_template = string.Template(
        u'''schema_members["${name}"] = CObjectSchemaItem::'''
        u'''SMember(${var_name}, ${is_mandatory});''')

    _function_impl_template = string.Template(
        u'''CSmartSchema $namespace::$class_name::'''
        u'''InitFunction_${function_id}_${message_type}(\n'''
        u'''    const TStructsSchemaItems &struct_schema_items,\n'''
        u'''    const std::set<FunctionID::eType> &function_id_items,\n'''
        u'''    const std::set<messageType::eType> &message_type_items) {\n'''
        u'''$code'''
        u'''}\n''')

    _function_impl_code_tempate = string.Template(
        u'''${schema_loc_decl}'''
        u'''${schema_items_decl}'''
        u'''CObjectSchemaItem::Members '''
        u'''schema_members;\n\n'''
        u'''${schema_item_fill}'''
        u'''CObjectSchemaItem::Members '''
        u'''params_members;\n'''
        u'''${schema_params_fill}'''
        u'''\n'''
        u'''CObjectSchemaItem::Members '''
        u'''root_members_map;\n'''
        u'''root_members_map[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_MSG_PARAMS] = '''
        u'''CObjectSchemaItem::SMember(CObjectSchemaItem::'''
        u'''create(schema_members), true);\n'''
        u'''root_members_map[NsSmartDeviceLink::NsJSONHandler::'''
        u'''strings::S_PARAMS] = '''
        u'''CObjectSchemaItem::SMember(CObjectSchemaItem::'''
        u'''create(params_members), true);\n\n'''
        u'''return CSmartSchema(CObjectSchemaItem::'''
        u'''create(root_members_map));''')

    _class_h_template = string.Template(
        u'''$comment\n'''
        u'''class $class_name : public NsSmartDeviceLink::NsJSONHandler::'''
        u'''CSmartFactory<FunctionID::eType, messageType::eType, '''
        u'''StructIdentifiers::eType> {\n'''
        u''' public:\n'''
        u'''  /**\n'''
        u'''   * @brief Constructor.\n'''
        u'''   */\n'''
        u'''  $class_name();\n'''
        u'''\n'''
        u''' protected:\n'''
        u'''  /**\n'''
        u'''   * @brief Type that maps of struct IDs to schema items.\n'''
        u'''   */\n'''
        u'''  typedef std::map<const StructIdentifiers::eType, '''
        u'''utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::'''
        u'''ISchemaItem> > TStructsSchemaItems;\n'''
        u'''\n'''
        u'''  /**\n'''
        u'''   * @brief Helper that allows to make reference to struct\n'''
        u'''   *\n'''
        u'''   * @param struct_schema_items Struct schema items.\n'''
        u'''   * @param struct_id ID of structure to provide.\n'''
        u'''   *\n'''
        u'''   * @return utils::SharedPtr of strucute\n'''
        u'''   */\n'''
        u'''  static '''
        u'''utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> '''
        u'''ProvideObjectSchemaItemForStruct(\n'''
        u'''        const TStructsSchemaItems &struct_schema_items,\n'''
        u'''        const StructIdentifiers::eType struct_id);\n'''
        u'''\n'''
        u'''  /**\n'''
        u'''   * @brief Initializes all struct schemes.\n'''
        u'''   */\n'''
        u'''  void InitStructSchemes('''
        u'''TStructsSchemaItems &struct_schema_items);\n'''
        u'''\n'''
        u'''  /**\n'''
        u'''   * @brief Initializes all function schemes.\n'''
        u'''   *\n'''
        u'''   * @param struct_schema_items Struct schema items.\n'''
        u'''   * @param function_id_items Set of all elements '''
        u'''of FunctionID enum.\n'''
        u'''   * @param message_type_items Set of all elements '''
        u'''of messageType enum.\n'''
        u'''   */\n'''
        u'''  void InitFunctionSchemes(\n'''
        u'''      const TStructsSchemaItems &struct_schema_items,\n'''
        u'''      const std::set<FunctionID::eType> &function_id_items,\n'''
        u'''      const std::set<messageType::eType> '''
        u'''&message_type_items);\n'''
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
        u'''InitFunction_${function_id}_${message_type}(\n'''
        u'''    const TStructsSchemaItems &struct_schema_items,\n'''
        u'''    const std::set<FunctionID::eType> &function_id_items,\n'''
        u'''    const std::set<messageType::eType> &message_type_items);''')

    _struct_decl_template = string.Template(
        u'''$comment\n'''
        u'''static '''
        u'''utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> '''
        u'''InitStructSchemaItem_${struct_name}(\n'''
        u'''    const TStructsSchemaItems &struct_schema_items);''')

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

    _comment_cc_template = string.Template(
        u'''$brief_description'''
        u'''$description'''
        u'''$design_description'''
        u'''$issues'''
        u'''$todos'''
        u'''$returns''')

    _enum_template = string.Template(
        u'''namespace $name {\n'''
        u'''$comment\n'''
        u'''enum eType {\n'''
        u'''$enum_items};\n'''
        u'''} // $name\n''')

    _enum_element_with_value_template = string.Template(
        u'''$comment\n'''
        u'''$name = $value''')

    _enum_element_with_no_value_template = string.Template(
        u'''$comment\n'''
        u'''$name''')

    _indent_template = u"  "
