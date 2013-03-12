"""Interface model.

Interface model is represented by Interface class.
Parser must provide an instance of this model as a result of parsing
interface definition file.
Generator must take an instance of this model as input for generating
output files.
"""


class Boolean(object):

    """Boolean type."""

    pass


class Integer(object):

    """Integer type.

    Instance variables:
    min_value -- minunum allowed value
    max_value -- maximum allowed value

    """

    def __init__(self, min_value=None, max_value=None):
        self.min_value = min_value
        self.max_value = max_value


class Double(object):

    """Floating-point type.

    Instance variables:
    min_value -- minunum allowed value
    max_value -- maximum allowed value

    """

    def __init__(self, min_value=None, max_value=None):
        self.min_value = min_value
        self.max_value = max_value


class String(object):

    """String type.

    Instance variables:
    max_length -- maximum string length

    """

    def __init__(self, max_length=None):
        self.max_length = max_length


class Array(object):

    """Array type.

    Instance variables:
    min_size -- minimum array size
    max_size -- maximum array size
    element_type -- type of array element

    """

    def __init__(self, min_size=None, max_size=None, element_type=None):
        self.min_size = min_size
        self.max_size = max_size
        self.element_type = element_type


class Issue(object):

    """Issue.

    Instance variables:
    creator -- issue creator
    value -- issue text

    """

    def __init__(self, creator=None, value=None):
        self.creator = creator
        self.value = value


class InterfaceItemBase(object):

    """Base class for interface item.

    Instance variables:
    name -- item name
    description -- list of string description elements
    design_description -- list of string design description elements
    issues -- list of issues
    todos -- list of string todo elements

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None):
        self.name = name
        self.description = description if description is not None else []
        self.design_description = \
            design_description if design_description is not None else []
        self.issues = issues if issues is not None else []
        self.todos = todos if todos is not None else []


class EnumElement(InterfaceItemBase):

    """Element of enumeration.

    Instance variables:
    internal_name -- internal name of an element must be used by a
                     generator if it is provided (not None)
    value -- optional element value

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, internal_name=None, value=None):
        super(EnumElement, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos)
        self.internal_name = internal_name
        self.value = value


class Enum(InterfaceItemBase):

    """Enumeration.

    Instance variables:
    internal_scope -- optional internal scope
    elements -- enumeration elements

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, internal_scope=None, elements=None):
        super(Enum, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos)

        self.internal_scope = internal_scope
        self.elements = elements if elements is not None else {}


class EnumSubset(InterfaceItemBase):

    """Enumeration subset.

    Instance variables:
    enum -- enumeration
    allowed_elements -- dictionary of elements of enumeration
                        which are allowed in this subset

    """

    def __init__(self, name, enum, description=None, design_description=None,
                 issues=None, todos=None, allowed_elements=None):
        super(EnumSubset, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos)

        self.enum = enum
        self.allowed_elements = \
            allowed_elements if allowed_elements is not None else {}


class Param(InterfaceItemBase):

    """Parameter.

    Instance variables:
    is_mandatory -- boolean value indicating whether
                    this parameter is mandatory
    param_type -- parameter type

    """

    def __init__(self, name, param_type, description=None,
                 design_description=None, issues=None, todos=None,
                 is_mandatory=True):
        super(Param, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos)

        self.is_mandatory = is_mandatory
        self.param_type = param_type


class FunctionParam(Param):

    """Function parameter.

    Instance variables:
    platform -- optional platform (string or None)
    default_value -- optional default value of this parameter

    """

    def __init__(self, name, param_type, description=None,
                 design_description=None, issues=None, todos=None,
                 is_mandatory=True, platform=None, default_value=None):
        super(FunctionParam, self).__init__(
            name, param_type=param_type, description=description,
            design_description=design_description, issues=issues, todos=todos)

        self.platform = platform
        self.default_value = default_value


class Struct(InterfaceItemBase):

    """Structure.

    Instance variables:
    members -- dictionary of structure members (instances of Param class)

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, members=None):
        super(Struct, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos)

        self.members = members if members is not None else {}


class Function(InterfaceItemBase):

    """Function.

    Instance variables:
    function_id -- function identifier (string)
    message_type -- message type must be "request",
                    "response" or "notification"
    platform -- optional platform (string or None)
    params -- function parameters

    """

    def __init__(self, name, function_id, message_type, description=None,
                 design_description=None, issues=None, todos=None,
                 platform=None, params=None):
        super(Function, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos)

        self.function_id = function_id
        self.message_type = message_type
        self.platform = platform
        self.params = params if params is not None else {}


class Interface(object):

    """Interface.

    Instance variables:
    enums -- dictionary of enumerations
    structs -- dictionary of structures
    functions -- dictionary of functions
    params -- dictionary of interface parameters (name, version, etc.)

    """

    def __init__(self, enums=None, structs=None, functions=None, params=None):
        self.enums = enums if enums is not None else {}
        self.structs = structs if structs is not None else {}
        self.functions = functions if functions is not None else {}
        self.params = params if params is not None else {}
