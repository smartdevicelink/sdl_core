"""Interface model.

Interface model is represented by Interface class.
Parser must provide an instance of this model as a result of parsing
interface definition file.
Generator must take an instance of this model as input for generating
output files.
"""

# In this module there are classes that are used as data container
# pylint: disable=R0903

import collections


class Boolean(object):

    """Boolean type.

    default_value -- default value

    """

    def __init__(self, default_value=None):
        self.default_value = default_value


class Integer(object):

    """Integer type.

    Instance variables:
    min_value -- minimum allowed value
    max_value -- maximum allowed value
    default_value -- default value

    """

    def __init__(self, min_value=None, max_value=None, default_value=None):
        self.min_value = min_value
        self.max_value = max_value
        self.default_value = default_value


class Double(object):

    """Floating-point type.

    Instance variables:
    min_value -- minimum allowed value
    max_value -- maximum allowed value
    default_value -- default value

    """

    def __init__(self, min_value=None, max_value=None, default_value=None):
        self.min_value = min_value
        self.max_value = max_value
        self.default_value = default_value

class String(object):

    """String type.

    Instance variables:
    min_length -- minimum string length
    max_length -- maximum string length
    default_value -- default value

    """

    def __init__(self, min_length=None, max_length=None, default_value=None):
        self.min_length = min_length
        self.max_length = max_length
        self.default_value = default_value


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
    platform -- optional platform (string or None)
    default_value -- default value
    scope -- optional scope: internal, partner or none (none by defaul, means public)

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, platform=None, default_value=None, scope=None,
                 since=None, until=None, deprecated=None, removed=None, history=None):
        self.name = name
        self.description = description if description is not None else []
        self.design_description = \
            design_description if design_description is not None else []
        self.issues = issues if issues is not None else []
        self.todos = todos if todos is not None else []
        self.platform = platform
        self.default_value = default_value
        self.scope = scope
        self.since = since
        self.until = until
        self.deprecated = deprecated
        self.removed = removed
        self.history = history


class EnumElement(InterfaceItemBase):

    """Element of enumeration.

    Instance variables:
    internal_name -- internal name of an element must be used by a
                     generator if it is provided (not None)
    value -- optional element value

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, platform=None, internal_name=None,
                 value=None, since=None, until=None, deprecated=None, removed=None, history=None):
        super(EnumElement, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, history=history)
        self.internal_name = internal_name
        self.value = value
        self.since = since
        self.until = until
        self.deprecated = deprecated
        self.removed = removed


    @property
    def primary_name(self):
        """Primary name of the EnumElement.

        Return the 'internal_name' property if presented or 'name' property
        otherwise.

        """
        return self.name if self.internal_name is None else self.internal_name


class Enum(InterfaceItemBase):

    """Enumeration.

    Instance variables:
    internal_scope -- optional internal scope
    elements -- enumeration elements

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, platform=None, internal_scope=None,
                 elements=None, scope=None, since=None, until=None, deprecated=None, removed=None, history=None):
        super(Enum, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, scope=scope, history=history)

        self.internal_scope = internal_scope
        self.elements = \
            elements if elements is not None else collections.OrderedDict()
        self.since = since
        self.until = until
        self.deprecated = deprecated
        self.removed = removed


class EnumSubset(InterfaceItemBase):

    """Enumeration subset.

    Instance variables:
    enum -- enumeration
    allowed_elements -- dictionary of elements of enumeration
                        which are allowed in this subset

    """

    def __init__(self, name, enum, description=None, design_description=None,
                 issues=None, todos=None, platform=None,
                 allowed_elements=None, since=None, until=None, deprecated=None, removed=None, history=None):
        super(EnumSubset, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, history=history)

        self.enum = enum
        self.allowed_elements = \
            allowed_elements if allowed_elements is not None else {}
        self.since = since
        self.until = until
        self.deprecated = deprecated
        self.removed = removed


class Param(InterfaceItemBase):

    """Parameter.

    Instance variables:
    is_mandatory -- boolean value indicating whether
                    this parameter is mandatory
    param_type -- parameter type
    default_value -- default value

    """

    def __init__(self, name, param_type, description=None,
                 design_description=None, issues=None, todos=None,
                 platform=None, is_mandatory=True, default_value=None, scope=None,
                 since=None, until=None, deprecated=None, removed=None, history=None):
        super(Param, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, default_value=default_value, scope=scope, history=history)

        self.is_mandatory = is_mandatory
        self.param_type = param_type
        self.default_value = default_value
        self.since = since
        self.until = until
        self.deprecated = deprecated
        self.removed=removed


class FunctionParam(Param):

    """Function parameter.

    Instance variables:
    default_value -- optional default value of this parameter

    """

    def __init__(self, name, param_type, description=None,
                 design_description=None, issues=None, todos=None,
                 platform=None, is_mandatory=True, default_value=None, scope=None,
                 since=None, until=None, deprecated=None, removed=None, history=None):
        super(FunctionParam, self).__init__(
            name, param_type=param_type, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, is_mandatory=is_mandatory, default_value=default_value, 
            scope=scope, since=since, until=until, deprecated=deprecated, removed=removed, history=history)

        self.default_value = default_value


class Struct(InterfaceItemBase):

    """Structure.

    Instance variables:
    members -- dictionary of structure members (instances of Param class)

    """

    def __init__(self, name, description=None, design_description=None,
                 issues=None, todos=None, platform=None, members=None, scope=None, 
                 since=None, until=None, deprecated=None, removed=None, history=None):
        super(Struct, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, scope=scope, since=since, until=until,
            deprecated=deprecated, removed=removed, history=history)

        self.members = \
            members if members is not None else collections.OrderedDict()


class Function(InterfaceItemBase):

    """Function.

    Instance variables:
    function_id -- function identifier (EnumElement from Enum "FunctionID")
    message_type -- message type (EnumElement from Enum "messageType")
    params -- function parameters

    """

    def __init__(self, name, function_id, message_type, description=None,
                 design_description=None, issues=None, todos=None,
                 platform=None, params=None, scope=None, since=None, until=None, deprecated=None, removed=None, history=None):
        super(Function, self).__init__(
            name, description=description,
            design_description=design_description, issues=issues, todos=todos,
            platform=platform, scope=scope, since=since, until=until, deprecated=deprecated, removed=removed, history=history)

        self.function_id = function_id
        self.message_type = message_type
        self.params = \
            params if params is not None else collections.OrderedDict()


class Interface(object):

    """Interface.

    Instance variables:
    enums -- dictionary of enumerations
    structs -- dictionary of structures
    functions -- dictionary of functions
    params -- dictionary of interface parameters (name, version, etc.)

    """

    def __init__(self, enums=None, structs=None, functions=None, params=None):
        self.enums = enums if enums is not None else collections.OrderedDict()
        self.structs = \
            structs if structs is not None else collections.OrderedDict()
        self.functions = \
            functions if functions is not None else collections.OrderedDict()
        self.params = params if params is not None else {}
