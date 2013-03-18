"""SmartSchema generator.

Generator for SmartObjects schema source code.

"""

import os
import string
import uuid

from generator import Model

class GenerateError(Exception):

    """Generate error.

    This exception is raised when SmartSchema generator is unable to create output from given model.

    """

    pass

class SmartSchema(object):
    
    """SmartSchema generator."""
    
    def __init__(self):
        """Constructor"""
        
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
            raise GenerateError("Given interface is None")

        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)
                        
        indent_level = 0
        namespace_open = ""
        namespace_close = ""
        
        if namespace:
            parts = namespace.split("::")
            for part in parts:
                namespace_open = "".join([namespace_open, self._indent_code(self._namespace_open_template.substitute(name = part), indent_level)])
                namespace_close = "".join([namespace_close, self._indent_code("}", len(parts) - indent_level - 1)])            
                indent_level = indent_level + 1 
        
        class_name = os.path.splitext(filename)[0]
        guard = "__CSMARTFACTORY_{0}_{1}_HPP__".format(class_name.upper(), uuid.uuid1().hex.capitalize())
        header_file_name = "".join("{0}.hpp".format(class_name))
                
        with open(os.path.join(destination_dir, header_file_name) , "w") as f:
            f.write(self._hpp_file_tempalte.substitute(guard = guard,
                                                       namespace_open = namespace_open,
                                                       enums_content = self._indent_code(self._generate_enums(interface.enums.values()), indent_level),
                                                       class_content = self._indent_code(self._generate_hpp_class(class_name,
                                                                                                                  interface.params,
                                                                                                                  interface.functions.values(),
                                                                                                                  interface.structs.values()), indent_level),
                                                       namespace_close = namespace_close))
        
        with open(os.path.join(destination_dir, "".join("{0}.cpp".format(class_name))), "w") as f:
            f.write(self._cpp_file_template.substitute(header_file_name = header_file_name,
                                                       namespace = namespace,
                                                       class_name = class_name,
                                                       function_schemas = self._generate_function_schemas(interface.functions.values(), 
                                                                                                          class_name),
                                                       init_function_impls = self._generate_function_impls(interface.functions.values(), 
                                                                                                           namespace,
                                                                                                           class_name)))

    def _generate_hpp_class(self, class_name, params, functions, structs):
        return self._class_hpp_template.substitute(comment = self._generate_class_comment(class_name, params),
                                                   class_name = class_name,
                                                   init_function_decls = self._generate_function_decls(functions))
        
    def _generate_function_decls(self, functions):
        if functions is None:
            raise GenerateError("Functions is None")
        
        return "\n".join(map(lambda x: self._indent_code(self._generate_function_decl(x), 1), functions))
    
    def _generate_function_decl(self, function):
        return self._function_decl_template.substitute(comment = self._generate_comment(function),
                                                       function_id = function.function_id.primary_name,
                                                       message_type = function.message_type.primary_name)

    def _generate_function_schemas(self, functions, class_name):
        if functions is None:
            raise GenerateError("Functions is None")
        
        return "".join(map(lambda x: self._indent_code(self._generate_function_schema(x, class_name), 1), functions))
    
    def _generate_function_schema(self, function, class_name):
        return self._function_schemas_template.substitute(class_name = class_name,
                                                          function_id = function.function_id.primary_name,
                                                          message_type = function.message_type.primary_name)

    def _generate_function_impls(self, functions, namespace, class_name):
        if functions is None:
            raise GenerateError("Functions is None")
        
        return "\n".join(map(lambda x: self._generate_function_impl(x, namespace, class_name), functions))
    
    def _generate_function_impl(self, function, namespace, class_name):
        return self._function_impl_template.substitute(namespace = namespace,
                                                       class_name = class_name,
                                                       function_id = function.function_id.primary_name,
                                                       message_type = function.message_type.primary_name)
        
    def _generate_enums(self, enums):
        if enums is None:
            raise GenerateError("Enums is None")

        return "\n".join(map(lambda x: self._generate_enum(x), enums))
            
    def _generate_enum(self, enum):
        enum_elements = enum.elements.values()
        enum_elements.insert(0, Model.EnumElement("INVALID_ENUM", None, None, None, None, None, "-1"))
        return self._enum_template.substitute(comment = self._generate_comment(enum),
                                              name = enum.name,
                                              enum_items = self._indent_code(self._generate_enum_elements(enum_elements), 1))

    def _generate_enum_elements(self, enum_elements):
        return ",\n\n".join(map(lambda x: self._generate_enum_element(x), enum_elements))

    def _generate_enum_element(self, enum_element):
        if enum_element.value is not None:
            return self._enum_element_with_value_template.substitute(comment = self._generate_comment(enum_element),
                                                                     name = enum_element.primary_name,
                                                                     value = enum_element.value)
        else:
            return self._enum_element_with_no_value_template.substitute(comment = self._generate_comment(enum_element),
                                                                        name = enum_element.primary_name)

    def _generate_class_comment(self, class_name, params):
        return self._class_comment_template.substitute(class_name = class_name,
                                                       class_params = "".join(map(lambda x: " *     {0} - {1}\n".format(x[0], x[1]), params.items())) if params else " *    none\n")

    def _generate_comment(self, interface_item_base):
        brief_type_title = None
        interface_item_base_classname = interface_item_base.__class__.__name__ 
        if interface_item_base_classname in self._model_types_briefs:
            brief_type_title = self._model_types_briefs[interface_item_base_classname]
        else:
            raise GenerateError("Unable to create comment for unknown type " +
                                interface_item_base_classname)
        
        name = interface_item_base.primary_name if type(interface_item_base) is Model.EnumElement else interface_item_base.name
        brief_description = " * @brief {0}{1}.\n".format(brief_type_title, name)
                
        description = "".join(map(lambda x: " * {0}\n".format(x),
                                  interface_item_base.description))
        if description is not "":
            description = "".join([" *\n", description])
        
        design_description = "".join(map(lambda x: " * {0}\n".format(x),
                                         interface_item_base.design_description))
        if design_description is not "":
            design_description = "".join([" *\n", design_description])
        
        issues = "".join(map(lambda x: " * @note {0}\n".format(x),
                             interface_item_base.issues))
        if issues is not "":
            issues = "".join([" *\n", issues])
        
        todos = "".join(map(lambda x: " * @todo {0}\n".format(x),
                            interface_item_base.todos))
        if todos is not "":
            todos = "".join([" *\n", todos])

        returns = ""
        if type(interface_item_base) is Model.Function:
            returns = "".join([" *\n", self._function_return_comment])
        
        return self._comment_template.substitute(brief_description = brief_description,
                                                 description = description,
                                                 design_description = design_description,
                                                 issues = issues,
                                                 todos = todos,
                                                 returns = returns)

    def _indent_code(self, code, indent_level):
        code_lines = code.split("\n")
        return "".join(map(lambda x: "{0}{1}\n".format(self._indent_template * indent_level, x) if x is not "" else "\n", code_lines))

    _model_types_briefs = dict({"EnumElement" : "", 
                                "Enum" : "Enumeration ",
                                "Function" : "Method that generates schema for function "})

    _hpp_file_tempalte = string.Template(
'''#ifndef $guard
#define $guard

#include "JSONHandler/CSmartFactory.hpp"
#include "SmartObjects/CSmartSchema.hpp"

$namespace_open$enums_content$class_content$namespace_close
#endif //$guard

''')

    _namespace_open_template = string.Template(
"""namespace $name
{""")

    _cpp_file_template = string.Template(
'''#include "$header_file_name"
#include "SmartObjects/validation/CAlwaysTrueValidator.hpp"

using namespace NsAppLink::NsSmartObjects;

$namespace::$class_name::$class_name()
: CSmartFactory<FunctionID, messageType>()
{
    initSchemas();
}

void $namespace::$class_name::initSchemas()
{
$function_schemas}

$init_function_impls
''')
    
    _function_schemas_template = string.Template(
"""mSchemas.insert(std::make_pair(NsAppLink::NsJSONHandler::SmartSchemaKey<FunctionID, messageType>($function_id, $message_type), $class_name::initFunction_${function_id}_${message_type}()));""")

    _function_impl_template = string.Template(
"""CSmartSchema $namespace::$class_name::initFunction_${function_id}_${message_type}()
{
    return CSmartSchema(Validation::CAlwaysTrueValidator());
}
""")
    
    _class_hpp_template = string.Template(
"""$comment
class $class_name : public NsAppLink::NsJSONHandler::CSmartFactory<FunctionID, messageType>
{
public:

    /**
     * @brief Constructor.
     */
    $class_name();

protected:

    /**
     * @brief Initializes all schemas.     
     */
    void initSchemas();

$init_function_decls};""")

    _function_return_comment = " * @return NsAppLink::NsSmartObjects::CSmartSchema\n"

    _function_decl_template = string.Template(
"""$comment
static NsAppLink::NsSmartObjects::CSmartSchema initFunction_${function_id}_${message_type}();""")

    _class_comment_template = string.Template(
"""/**
 * @brief Class $class_name.
 *
 * Params:
$class_params */""")

    _comment_template = string.Template(
"""/**
$brief_description$description$design_description$issues$todos$returns */""")

    _enum_template = string.Template(
"""$comment
enum $name
{
$enum_items};
""")

    _enum_element_with_value_template = string.Template(
"""$comment
$name = $value""")

    _enum_element_with_no_value_template = string.Template(
"""$comment
$name""")

    _indent_template = "    "
