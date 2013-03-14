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
                
        parts = namespace.split("::")
        indent_level = 0
        namespace_open = ""
        namespace_close = ""
        for part in parts:
            namespace_open = "".join([namespace_open, self._indent_code(self._namespace_open_template.substitute(name = part), indent_level)])
            namespace_close = "".join([namespace_close, self._indent_code("}\n", len(parts) - indent_level - 1)])            
            indent_level = indent_level + 1 
        
        class_name = os.path.splitext(filename)[0]
        guard = "__CSMARTFACTORY_{0}_{1}_HPP__".format(class_name.upper(), uuid.uuid1().hex.capitalize())
        header_file_name = "".join("{0}.hpp".format(class_name))
                
        with open(os.path.join(destination_dir, header_file_name) , "w") as f:
            f.write(self._hpp_file_tempalte.substitute(guard = guard,
                                                       namespace_open = namespace_open,
                                                       enums_content = self._indent_code(self._generate_enums(interface.enums), indent_level),
                                                       class_content = self._indent_code(self._generate_hpp_class(class_name,
                                                                                                                  interface.params, 
                                                                                                                  interface.functions, 
                                                                                                                  interface.structs), indent_level),
                                                       namespace_close = namespace_close))
        
        with open(os.path.join(destination_dir, "".join("{0}.cpp".format(class_name))), "w") as f:
            f.write(self._cpp_file_template.substitute(header_file_name = header_file_name,
                                                       namespace = namespace,
                                                       class_name = class_name,
                                                       function_schemas = "",
                                                       init_function_impls = ""))

    def _generate_hpp_class(self, class_name, params, functions, structs):
        return ""
        
    def _generate_enums(self, enums):
        if enums is None:
            raise GenerateError("Enums is None")

        return "\n".join(map(lambda x: self._generate_enum(x), enums))
            
    def _generate_enum(self, enum):
        return self._enum_template.substitute(comment = self._generate_comment(enum),
                                                 name = enum.name,
                                                 enum_items = self._indent_code(self._generate_enum_elements(enum.elements), 1))

    def _generate_enum_elements(self, enum_elements):
        return ",\n\n".join(map(lambda x: self._generate_enum_element(x), enum_elements))

    def _generate_enum_element(self, enum_element):
        if enum_element.value is not None:
            return self._enum_element_with_value_template.substitute(comment = self._generate_comment(enum_element),
                                                                     name = enum_element.internal_name if enum_element.internal_name is not None else enum_element.name,
                                                                     value = enum_element.value)
        else:
            return self._enum_element_with_no_value_template.substitute(comment = self._generate_comment(enum_element),
                                                                        name = enum_element.internal_name if enum_element.internal_name is not None else enum_element.name)

    def _generate_comment(self, interface_item_base):
        brief_type_title = None
        interface_item_base_classname = interface_item_base.__class__.__name__ 
        if interface_item_base_classname in self._model_types_briefs:
            brief_type_title = self._model_types_briefs[interface_item_base_classname]
        else:
            raise GenerateError("Unable to create comment for unknown type " +
                                interface_item_base_classname)
        
        name = interface_item_base.internal_name if type(interface_item_base) is Model.EnumElement and interface_item_base.internal_name is not None else interface_item_base.name
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

        return self._comment_template.substitute(brief_description = brief_description,
                                                 description = description,
                                                 design_description = design_description,
                                                 issues = issues,
                                                 todos = todos)

    def _indent_code(self, code, indent_level):
        code_lines = code.split("\n")
        return "".join(map(lambda x: "{0}{1}\n".format(self._indent_template * indent_level, x) if x is not "" else "\n", code_lines))

    _model_types_briefs = dict({"EnumElement" : "", 
                                "Enum" : "Enumeration "})

    _hpp_file_tempalte = string.Template(
'''#ifndef $guard
#define $guard

#include "CSmartFactory.hpp"
#include "SmartObjects/CSmartSchema.hpp"

$namespace_open
$enums_content
$class_content
$namespace_close
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
$function_schemas
}

$init_function_impls
''')
#mSchemas.insert(std::make_pair(NsAppLink::JSONHandler::SmartSchemaKey<FunctionID, messageType>())

    _class_hpp_template = string.Template(
"""$comment
class $class_name : public NsAppLink::JSONHandler::CSmartFactory<FunctionID, messageType>
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
    
$init_function_decls   
};
""")

    _class_comment_template = string.Template(
"""/**
 * @brief Class $class_name.
 *
 * Params:
$class_params
 */ 
""")

    _comment_template = string.Template(
"""/**
$brief_description$description$design_description$issues$todos */""")

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
