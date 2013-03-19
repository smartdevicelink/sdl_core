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

        with open(os.path.join(destination_dir,
                               "".join("{0}.cpp".format(class_name))),
                  "w") as f_s:
            f_s.write(self._cpp_file_template.substitute(
                header_file_name=header_file_name,
                namespace=namespace,
                class_name=class_name,
                function_schemas=self._generate_function_schemas(
                    interface.functions.values(),
                    class_name),
                init_function_impls=self._generate_function_impls(
                    interface.functions.values(),
                    namespace,
                    class_name)))

    def _generate_hpp_class(self, class_name, params, functions, structs):
        """Generate source code of class for header file.

        Generates source code of class that should be used in the
        header file.

        Keyword arguments:
        class_name -- Name of the class to generate.
        params -- class parameters.
        functions -- list of functions to generate methods for.
        structs -- structures to generate methods for.

        Returns:
        String with complete *.hpp file source code.

        """

        return self._class_hpp_template.substitute(
            comment=self._generate_class_comment(class_name, params),
            class_name=class_name,
            init_function_decls=self._generate_function_decls(functions))

    def _generate_function_decls(self, functions):
        """Generate method prototypes for functions for header file.

        Generates method prototypes for functions that should be used in the
        header file.

        Keyword arguments:
        functions -- list of functions to generate methods for.

        Returns:
        String with function declarations source code.

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

    def _generate_function_schemas(self, functions, class_name):
        """Generate functions initialization code for source file.

        Generates functions schema initialization code that should be used
        in the source file.

        Keyword arguments:
        functions -- list of functions to generate methods for.
        class_name -- Name of the class to generate.

        Returns:
        String with functions schema initialization source code.

        """

        if functions is None:
            raise GenerateError("Functions is None")

        return "".join([self._indent_code(
            self._generate_function_schema(x, class_name), 1)
            for x in functions])

    def _generate_function_schema(self, function, class_name):
        """Generate function initialization code for source file.

        Generates function schema initialization code that should be used
        in the source file.

        Keyword arguments:
        function -- function to generate method for.
        class_name -- Name of the class to generate.

        Returns:
        String with function schema initialization source code.

        """

        return self._function_schema_template.substitute(
            class_name=class_name,
            function_id=function.function_id.primary_name,
            message_type=function.message_type.primary_name)

    def _generate_function_impls(self, functions, namespace, class_name):
        """Generate functions implementation for source file.

        Generates implementation code of methods that provide schema for
        functions. This code should be used in the source file.

        Keyword arguments:
        functions -- list of functions to generate methods for.
        namespace -- name of destination namespace.
        class_name -- Name of the class to generate.

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
        class_name -- Name of the class to generate.

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
        class_name -- Name of the class.
        params -- Class parameters.

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
        interface_item_base -- Object to generate doxygen comment for.

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
        code -- Given source code.
        indent_level -- Desired indentation level.

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
         "Function": "Method that generates schema for function "})

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
        '''#include "$header_file_name\n"'''
        '''#include "SmartObjects/validation/CAlwaysTrueValidator.hpp\n"'''
        '''\n'''
        '''using namespace NsAppLink::NsSmartObjects;\n'''
        '''\n'''
        '''$namespace::$class_name::$class_name()\n'''
        ''': CSmartFactory<FunctionID, messageType>()\n'''
        '''{\n'''
        '''    initSchemas();\n'''
        '''}\n'''
        '''\n'''
        '''void $namespace::$class_name::initSchemas()\n'''
        '''{\n'''
        '''$function_schemas'''
        '''}\n'''
        '''\n'''
        '''$init_function_impls'''
        '''\n''')

    _function_schema_template = string.Template(
        '''mSchemas.insert(std::make_pair(NsAppLink::NsJSONHandler::'''
        '''SmartSchemaKey<FunctionID, messageType>($function_id, '''
        '''$message_type), $class_name::initFunction_${function_id}_'''
        '''${message_type}()));''')

    _function_impl_template = string.Template(
        '''CSmartSchema $namespace::$class_name::'''
        '''initFunction_${function_id}_${message_type}()\n'''
        '''{\n'''
        '''    return CSmartSchema(Validation::CAlwaysTrueValidator());\n'''
        '''}\n''')

    _class_hpp_template = string.Template(
        '''$comment\n'''
        '''class $class_name : public NsAppLink::NsJSONHandler::'''
        '''CSmartFactory<FunctionID, messageType>\n'''
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
        '''    /**\n'''
        '''     * @brief Initializes all schemas.\n'''
        '''     */\n'''
        '''    void initSchemas();\n'''
        '''\n'''
        '''$init_function_decls};''')

    _function_return_comment = ''' * @return NsAppLink::NsSmartObjects::''' \
                               '''CSmartSchema\n'''

    _function_decl_template = string.Template(
        '''$comment\n'''
        '''static NsAppLink::NsSmartObjects::CSmartSchema '''
        '''initFunction_${function_id}_${message_type}();''')

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
