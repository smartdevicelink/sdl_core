"""SmartSchema generator.

Generator for SmartObjects schema source code.

"""
# pylint: disable=W0402
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
                enums_content=self._indent_code(self._generate_enums(
                    interface.enums.values()), indent_level),
                class_content=self._indent_code(self._generate_hpp_class(
                    class_name,
                    interface.params,
                    interface.functions.values(),
                    interface.structs.values()), indent_level),
                namespace_close=namespace_close))

        self._generate_struct_schema_items(interface.structs.values())

        with open(os.path.join(destination_dir,
                               "".join("{0}.cpp".format(class_name))),
                  "w") as f_s:
            f_s.write(self._cpp_file_template.substitute(
                header_file_name=header_file_name,
                namespace=namespace,
                class_name=class_name,
                struct_schema_items=self._structs_add_code,
                function_schemas=self._generate_function_schemas(
                    interface.functions.values()),
                init_function_impls=self._generate_function_impls(
                    interface.functions.values(),
                    namespace,
                    class_name),
                init_structs_impls=self._generate_sturct_impls(
                    interface.structs.values(),
                    namespace,
                    class_name)))

    def _generate_hpp_class(self, class_name, params, functions, structs):
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
            comment=self._generate_class_comment(class_name, params),
            class_name=class_name,
            init_function_decls=self._generate_function_decls(functions),
            init_struct_decls=self._generate_structs_decls(structs))

    def _generate_structs_decls(self, structs):
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
            self._generate_struct_decl(x), 1) for x in structs])

    def _generate_struct_decl(self, struct):
        """Generate method prototype for struct for header file.

        Generates method prototype for struct that should be used in the
        header file.

        Keyword arguments:
        struct -- struct to generate method for.

        Returns:
        String with struct inin method declaration source code.

        """

        return self._struct_decl_template.substitute(
            comment=self._generate_comment(struct),
            struct_name=struct.name)

    def _generate_function_decls(self, functions):
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
            self._generate_function_decl(x), 1) for x in functions])

    def _generate_function_decl(self, function):
        """Generate method prototype for function for header file.

        Generates method prototype for function that should be used in the
        header file.

        Keyword arguments:
        function -- function to generate method for.

        Returns:
        String with function declaration source code.

        """

        return self._function_decl_template.substitute(
            comment=self._generate_comment(function),
            function_id=function.function_id.primary_name,
            message_type=function.message_type.primary_name)

    def _generate_struct_schema_items(self, structs):
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
        if struct.name in self._generated_structs:
            return

        for member in struct.members.values():
            self._process_struct_member(member)

        self._structs_add_code = "".join(
            [self._structs_add_code, self._indent_code(
                self._generate_struct_schema_item(struct), 1)])
        self._generated_structs.append(struct.name)

    def _process_struct_member(self, member):
        if type(member.param_type) is Model.Struct:
            self._ensure_struct_generated(member.param_type)

    def _ensure_struct_generated(self, struct):
        if struct.name in self._generated_structs:
            return

        self._process_struct(struct)

    def _generate_struct_schema_item(self, struct):
        """Generate struct schema item initialization code for source file.

        Generates struct schema item initialization code that should be used
        in the source file.

        Keyword arguments:
        struct -- struct to generate code for.

        Returns:
        String with struct schema item initialization source code.

        """

        return self._struct_schema_item_template.substitute(name=struct.name)

    def _generate_function_schemas(self, functions):
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
            self._generate_function_schema(x), 1)
            for x in functions])

    def _generate_function_schema(self, function):
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

    def _generate_sturct_impls(self, structs, namespace, class_name):
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

        return "\n".join([self._generate_struct_impl(
            x, namespace, class_name) for x in structs])

    def _generate_struct_impl(self, struct, namespace, class_name):
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

        return self._struct_impl_template.substitute(
            namespace=namespace,
            class_name=class_name,
            struct_name=struct.name,
            code=self._indent_code(
                self._struct_impl_code_tempate.substitute(
                    schema_local_decl=self._generate_schema_local_decls(
                        struct.members.values()),
                    schema_items_decl=self._generate_schema_items_decls(
                        struct.members.values()),
                    schema_item_fill=self._generate_struct_items_fill(
                        struct.members.values())),
                1))

    def _generate_schema_local_decls(self, members):
        result = ""
        processed_enums = []
        for member in members:
            if type(member.param_type) is Model.Enum and \
               member.param_type.name not in processed_enums:
                local_var = self._generate_schema_local_emum_var_name(member)
                result = "\n".join(
                    ["".join(
                        [result, self._impl_code_local_decl_enum_template.
                            substitute(
                                type=member.param_type.name,
                                var_name=local_var)]),
                        "\n".join(
                            [self._impl_code_local_decl_enum_insert_template.
                                substitute(
                                    var_name=local_var,
                                    value=x.primary_name)
                             for x in member.param_type.elements.values()])])
                processed_enums.append(member.param_type.name)
            # if type(member.param_type) is Model.EnumSubset:

        return "".join([result, "\n\n"]) if result else ""

    def _generate_schema_items_decls(self, members):
        result = "\n".join(
            [self._generate_schema_item_decl(x) for x in members])

        return "".join([result, "\n\n"]) if result else ""

    def _generate_schema_item_decl(self, member):
        code = ""

        if type(member.param_type) is Model.Boolean:
            code = self._impl_code_bool_item_template.substitute(params="")
        elif type(member.param_type) is Model.Integer:
            code = self._impl_code_integer_item_template.substitute(
                type="int",
                params=self._generate_schema_item_param_values(
                    [["int", member.param_type.min_value],
                     ["int", member.param_type.max_value]]))
        elif type(member.param_type) is Model.Double:
            code = self._impl_code_integer_item_template.substitute(
                type="double",
                params=self._generate_schema_item_param_values(
                    [["double", member.param_type.min_value],
                     ["double", member.param_type.max_value]]))
        elif type(member.param_type) is Model.String:
            code = self._impl_code_string_item_template.safe_substitute(
                param=self._generate_schema_item_param_values(
                    [["size_t", member.param_type.max_length]]))
        elif type(member.param_type) is Model.Array:
            code = "ARRAY"
        elif type(member.param_type) is Model.Struct:
            code = self._impl_code_struct_item_template.substitute(
                name=member.param_type.name)
        elif type(member.param_type) is Model.Enum:
            code = self._impl_code_enum_item_template.substitute(
                type=member.param_type.name,
                params=self._generate_schema_local_emum_var_name(member))
        elif type(member.param_type) is Model.EnumSubset:
            code = self._impl_code_enum_item_template.substitute(
                type=member.param_type.name,
                params="")
        else:
            raise GenerateError("Unexpected type of parameter: " +
                                str(type(member.param_type)))

        return self._impl_code_item_decl_temlate.substitute(
            var_name=self._generate_schema_item_var_name(member),
            item_decl=code)

    def _generate_schema_item_param_values(self, params):
        result = ""
        for param in params:
            value = self._impl_code_item_param_value_template.substitute(
                type=param[0],
                value=str(param[1] if param[1] is not None else ""))
            result = "".join([result, "".join(
                [", ", value])
                if result else value])

        return result

    def _generate_struct_items_fill(self, members):
        result = "\n".join(
            [self._generate_schema_item_fill(x) for x in members])

        return "".join([result, "\n\n"]) if result else ""

    def _generate_schema_item_fill(self, member):
        return self._impl_code_item_fill_template.substitute(
            name=member.name,
            var_name=self._generate_schema_item_var_name(member),
            is_mandatory=member.is_mandatory)

    def _generate_schema_item_var_name(self, member):
        return "".join([member.name, "_SchemaItem"])

    def _generate_schema_local_emum_var_name(self, member):
        return "".join([member.param_type.name, "_allowedEnumValues"])

    def _generate_function_impls(self, functions, namespace, class_name):
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

        return "\n".join([self._generate_function_impl(
            x, namespace, class_name) for x in functions])

    def _generate_function_impl(self, function, namespace, class_name):
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

        return self._function_impl_template.substitute(
            namespace=namespace,
            class_name=class_name,
            function_id=function.function_id.primary_name,
            message_type=function.message_type.primary_name)

    def _generate_enums(self, enums):
        """Generate enums for header file.

        Generates declaration of enumerations for the header file.

        Keyword arguments:
        enums -- list of enums to generate.

        Returns:
        String with enums declaration source code.

        """

        if enums is None:
            raise GenerateError("Enums is None")

        return "\n".join([self._generate_enum(x) for x in enums])

    def _generate_enum(self, enum):
        """Generate enum for header file.

        Generates declaration of enumeration for the header file.

        Keyword arguments:
        enum -- enum to generate.

        Returns:
        String with enum declaration source code.

        """

        enum_elements = enum.elements.values()
        enum_elements.insert(0, Model.EnumElement(
            "INVALID_ENUM", None, None, None, None, None, "-1"))
        return self._enum_template.substitute(
            comment=self._indent_code(self._generate_comment(enum), 1),
            name=enum.name,
            enum_items=self._indent_code(self._generate_enum_elements(
                enum_elements), 2))

    def _generate_enum_elements(self, enum_elements):
        """Generate enum elements for header file.

        Generates declaration of enumeration elements for the header file.

        Keyword arguments:
        enum_elements -- list of enum elements to generate.

        Returns:
        String with enum elements declaration source code.

        """

        return ",\n\n".join([self._generate_enum_element(x)
                             for x in enum_elements])

    def _generate_enum_element(self, enum_element):
        """Generate enum element for header file.

        Generates declaration of enumeration element for the header file.

        Keyword arguments:
        enum_element -- enum element to generate.

        Returns:
        String with enum element declaration source code.

        """

        if enum_element.value is not None:
            return self._enum_element_with_value_template.substitute(
                comment=self._generate_comment(enum_element),
                name=enum_element.primary_name,
                value=enum_element.value)
        else:
            return self._enum_element_with_no_value_template.substitute(
                comment=self._generate_comment(enum_element),
                name=enum_element.primary_name)

    def _generate_class_comment(self, class_name, params):
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

    def _generate_comment(self, interface_item_base):
        """Generate doxygen comment for iterface_item_base for header file.

        Generates doxygen comment for any iterface_item_base for the header
        file.

        Keyword arguments:
        interface_item_base -- object to generate doxygen comment for.

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
        brief_description = " * @brief {0}{1}.\n".format(
            brief_type_title, name)

        description = "".join([" * {0}\n".format(x)
                              for x in interface_item_base.description])
        if description is not "":
            description = "".join([" *\n", description])

        design_description = "".join([" * {0}\n".format(x)
                                     for x in
                                     interface_item_base.design_description])
        if design_description is not "":
            design_description = "".join([" *\n", design_description])

        issues = "".join([" * @note {0}\n".format(x)
                         for x in interface_item_base.issues])
        if issues is not "":
            issues = "".join([" *\n", issues])

        todos = "".join([" * @todo {0}\n".format(x)
                        for x in interface_item_base.todos])
        if todos is not "":
            todos = "".join([" *\n", todos])

        returns = ""
        if type(interface_item_base) is Model.Function:
            returns = "".join([" *\n", self._function_return_comment])

        return self._comment_template.substitute(
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
         "Struct": "Method that generates schema item for structure "})

    _hpp_file_tempalte = string.Template(
        '''#ifndef $guard\n'''
        '''#define $guard\n'''
        '''\n'''
        '''#include "JSONHandler/CSmartFactory.hpp"\n'''
        '''#include "SmartObjects/CSmartSchema.hpp"\n'''
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
        '''#include "$header_file_name"\n'''
        '''#include "SmartObjects/validation/CAlwaysTrueValidator.hpp"\n'''
        '''\n'''
        '''using namespace NsAppLink::NsSmartObjects;\n'''
        '''\n'''
        '''$namespace::$class_name::$class_name()\n'''
        ''' : CSmartFactory<FunctionID::eType, messageType::eType>(),\n'''
        '''   mStructSchemaItems()\n'''
        '''{\n'''
        '''    initStructSchemaItems();\n'''
        '''    initSchemas();\n'''
        '''}\n'''
        '''\n'''
        '''TSharedPtr<CObjectSchemaItem> $namespace::$class_name::'''
        '''provideObjectSchemaItemForStruct('''
        '''const std::string & StructName) const\n'''
        '''{\n'''
        '''    const TStructsSchemaItems::const_iterator it = '''
        '''mStructSchemaItems.find(StructName);\n'''
        '''    if (it != mStructSchemaItems.end())\n'''
        '''    {\n'''
        '''        return it->second;\n'''
        '''    }\n'''
        '''\n'''
        '''    return NsAppLink::NsSmartObjects::CAlwaysFalse();\n'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::initStructSchemaItems()\n'''
        '''{\n'''
        '''$struct_schema_items'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::initSchemas()\n'''
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
        '''\n''')

    _struct_schema_item_template = string.Template(
        '''mStructSchemaItems.insert(std::make_pair("${name}", '''
        '''initStructSchemaItem_${name}());''')

    _function_schema_template = string.Template(
        '''mSchemas.insert(std::make_pair(NsAppLink::NsJSONHandler::'''
        '''SmartSchemaKey<FunctionID::eType, messageType::eType>'''
        '''(FunctionID::$function_id, messageType::$message_type), '''
        '''initFunction_${function_id}_${message_type}()));''')

    _struct_impl_template = string.Template(
        '''CObjectValidator $namespace::$class_name::'''
        '''initStructSchemaItem_${struct_name}()\n'''
        '''{\n'''
        '''$code'''
        '''}\n''')

    _struct_impl_code_tempate = string.Template(
        '''${schema_local_decl}'''
        '''${schema_items_decl}'''
        '''std::map<std::string, CObjectSchemaItem::SMember> '''
        '''schemaMembersMap;\n\n'''
        '''${schema_item_fill}'''
        '''return CObjectSchemaItem::create(schemaMembersMap);''')

    _impl_code_local_decl_enum_template = string.Template(
        '''std::set<${type}> ${var_name};''')

    _impl_code_local_decl_enum_insert_template = string.Template(
        '''${var_name}.isert(${value});''')

    _impl_code_item_decl_temlate = string.Template(
        '''TSharedPtr<ISchemaItem> $var_name = $item_decl''')

    _impl_code_integer_item_template = string.Template(
        '''TNumberSchemaItem<${type}>::create(${params});''')

    _impl_code_bool_item_template = string.Template(
        '''CBoolSchemaItem::create(${params});''')

    _impl_code_string_item_template = string.Template(
        '''CStringSchemaItem::create(${params})''')

    _impl_code_array_item_template = string.Template(
        '''CArraySchemaItem::create(${params})''')

    _impl_code_struct_item_template = string.Template(
        '''provideObjectSchemaItemForStruct("${name}");''')

    _impl_code_enum_item_template = string.Template(
        '''TEnumSchemaItem<${type}::eType>::create(${params});''')

    _impl_code_item_param_value_template = string.Template(
        '''TSchemaItemParameter<$type>($value)''')

    _impl_code_item_fill_template = string.Template(
        '''schemaMembersMap["${name}"] = CObjectSchemaItem::'''
        '''SMember(${var_name}, ${is_mandatory});''')

    _function_impl_template = string.Template(
        '''CSmartSchema $namespace::$class_name::'''
        '''initFunction_${function_id}_${message_type}()\n'''
        '''{\n'''
        '''    return CSmartSchema(Validation::CAlwaysTrueValidator());\n'''
        '''}\n''')

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
        '''    $class_name();\n'''
        '''\n'''
        '''protected:\n'''
        '''\n'''
        '''   /**\n'''
        '''    * @brief Helper that allows to make reference to struct\n'''
        '''    *\n'''
        '''    * @param StructName Name of structure to provide.\n'''
        '''    *\n'''
        '''    * @return TSharedPtr of strucute\n'''
        '''    */\n'''
        '''    NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::'''
        '''NsSmartObjects::CObjectSchemaItem> '''
        '''provideObjectSchemaItemForStruct(const std::string & '''
        '''StructName) const;\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Initializes all struct schema items.\n'''
        '''     */\n'''
        '''    void initStructSchemaItems();\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Initializes all schemas.\n'''
        '''     */\n'''
        '''    void initSchemas();\n'''
        '''\n'''
        '''$init_function_decls'''
        '''\n'''
        '''$init_struct_decls'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Type that maps of struct names to schema items.\n'''
        '''     */\n'''
        '''    typedef std::map<std::string, NsAppLink::NsSmartObjects::'''
        '''TSharedPtr<CObjectSchemaItem> > TStructsSchemaItems;\n'''
        '''\n'''
        '''    /**\n'''
        '''     * @brief Map of struct names to their schema items.\n'''
        '''     */\n'''
        '''    TStructsSchemaItems mStructSchemaItems;\n'''
        '''};''')

    _function_return_comment = ''' * @return NsAppLink::NsSmartObjects::''' \
                               '''CSmartSchema\n'''

    _function_decl_template = string.Template(
        '''$comment\n'''
        '''static NsAppLink::NsSmartObjects::CSmartSchema '''
        '''initFunction_${function_id}_${message_type}();''')

    _struct_decl_template = string.Template(
        '''$comment\n'''
        '''static NsAppLink::NsSmartObjects::CObjectValidator '''
        '''initStructSchemaItem_${struct_name}();''')

    _class_comment_template = string.Template(
        '''/**\n'''
        ''' * @brief Class $class_name.\n'''
        ''' *\n'''
        ''' * Params:\n'''
        '''$class_params'''
        ''' */''')

    _comment_template = string.Template(
        '''/**\n'''
        '''$brief_description'''
        '''$description'''
        '''$design_description'''
        '''$issues'''
        '''$todos'''
        '''$returns */''')

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
