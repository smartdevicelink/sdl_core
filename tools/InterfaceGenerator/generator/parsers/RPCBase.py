"""RPC XML base parser.

Contains base parser for SDLRPC v1/v2 and JSON RPC XML format.

"""

import collections
import xml.etree.ElementTree

from generator import Model


class ParseError(Exception):

    """Parse error.

    This exception is raised when XML contains errors and can't be parsed.

    """

    pass


class Parser(object):

    """RPC XML Parser base.

    This class must not be used directly. One of its subclasses must be used
    instead.

    """

    def __init__(self):
        """Constructor."""
        self._types = {}
        self._enums = collections.OrderedDict()
        self._structs = collections.OrderedDict()
        self._functions = collections.OrderedDict()
        self._params = {}

    def parse(self, filename):
        """Parse XML.

        Returns an instance of generator.Model.Interface containing parsed
        interface or raises ParseError if input XML contains errors
        and can't be parsed.

        Keyword arguments:
        filename -- name of input XML file.

        """

        tree = xml.etree.ElementTree.parse(filename)
        root = tree.getroot()

        self._enums = self._initialize_enums()
        self._structs = collections.OrderedDict()
        self._functions = collections.OrderedDict()
        self._params = {}

        self._types = dict(self._enums.items())

        self._parse_root(root)

        return Model.Interface(enums=self._enums, structs=self._structs,
                               functions=self._functions, params=self._params)

    def _initialize_enums(self):
        """Initialize enums.

        The default implementation returns an OrderedDict with two empty
        enums: "FunctionID" and "messageType". Required for formats where
        these enums must be generated automatically according to the declared
        in the XML functions.

        These enums are filled during the parsing of the functions.

        """
        return collections.OrderedDict(
            [("FunctionID", Model.Enum(name="FunctionID")),
             ("messageType", Model.Enum(name="messageType"))])

    def _check_enum_name(self, enum):
        """Check enum name.

        This method is called to check whether the newly parsed enum's name
        conflicts with some predefined enum.

        This implementation raises an error if enum name is one of the
        predefined enums "FunctionID" or "messageType" which must not be
        declared explicitly in the XML.

        """
        if enum.name in ["FunctionID", "messageType"]:
            raise ParseError(
                "Enum '" + enum.name +
                "' is generated automatically in SDLRPCV1 and"
                " must not be declared in xml file")

    def _check_function_param_name(self, function_param_name):
        """Check function param name.

        This method is called to check whether the newly parsed function
        parameter name conflicts with some predefined name.

        This implementation doesn't check anything because there is no
        predefined names in base RPC XML.
        """

        pass

    def _parse_root(self, root):
        """Parse root XML element.

        Default implementation parses root as interface element without a
        prefix.

        Keyword arguments:
        root -- root element.

        """

        self._parse_interface(root, "")

    def _parse_interface(self, interface, prefix):
        """Parse interface element.

        Keyword arguments:
        interface -- interface element.
        prefix -- string prefix for all types of the interface.

        """
        if interface.tag != "interface":
            raise ParseError("Invalid interface tag: " + interface.tag)

        params, subelements, attrib = self._parse_base_item(interface, "")

        for param in ["description", "design_description", "todos"]:
            if 0 != len(params[param]):
                attrib[param] = "\n".join(params[param])

        if 0 != len(params["issues"]):
            attrib["issues"] = "\n".join(i.value for i in params["issues"])

        self._params = dict(
            self._params.items() +
            [(prefix + p[0], p[1]) for p in attrib.items()])

        for element in subelements:
            if element.tag == "enum":
                enum = self._parse_enum(element, prefix)
                self._check_enum_name(enum)
                self._add_item(self._enums, enum)
                self._add_type(enum)
            elif element.tag == "struct":
                struct = self._parse_struct(element, prefix)
                self._add_item(self._structs, struct)
                self._add_type(struct)
            elif element.tag == "function":
                function = self._parse_function(element, prefix)
                self._add_item(self._functions, function,
                               (function.function_id, function.message_type))
            else:
                raise ParseError("Unexpected element: " + element.tag)

    @staticmethod
    def _add_item(items, item, key=None):
        """Add new item in the items dictionary with given key.

        Performs additional check for presence in the dictionary and throws
        ParseError exception if key already exist.

        """
        if key is None:
            key = item.name

        if key in items:
            raise ParseError(type(item).__name__ + " '" + str(key) +
                             "' is declared more than once")
        items[key] = item

    def _add_type(self, _type):
        """Add new type in the internal types dictionary.

        Performs additional check for presence type with same name in the
        dictionary and throws ParseError exception if key already exist.

        """
        if _type.name in self._types:
            raise ParseError("Type '" + _type.name +
                             "' is declared as both struct and enum")

        self._types[_type.name] = _type

    def _parse_enum(self, element, prefix):
        """Parse element as enumeration.

        Returns an instance of generator.Model.Enum

        """
        params, subelements, attributes = \
            self._parse_base_item(element, prefix)

        internal_scope = None
        scope = None
        for attribute in attributes:
            if attribute == "internal_scope":
                internal_scope = attributes[attribute]
            elif attribute == "scope":
                scope = attributes[attribute]
            else:
                raise ParseError("Unexpected attribute '" + attribute +
                                 "' in enum '" + params["name"] + "'")
        params["internal_scope"] = internal_scope
        params["scope"] = scope

        elements = collections.OrderedDict()
        for subelement in subelements:
            if subelement.tag == "element":
                self._add_item(elements, self._parse_enum_element(subelement))
            else:
                raise ParseError("Unexpected element '" + subelement.tag +
                                 "' in enum '" + params["name"] + "'")
        params["elements"] = elements

        # Magic usage is correct
        # pylint: disable=W0142
        return Model.Enum(**params)

    def _parse_struct(self, element, prefix):
        """Parse element as structure.

        Returns an instance of generator.Model.Struct

        """
        params, subelements, attrib = self._parse_base_item(element, prefix)

        scope = None
        for attribute in attrib:
            if attribute == "scope":
                scope = attrib[attribute]
            else:
                raise ParseError("Unexpected attribute '" + attribute +
                                 "' in struct '" + params["name"] + "'")
        params["scope"] = scope

        members = collections.OrderedDict()
        for subelement in subelements:
            if subelement.tag == "param":
                self._add_item(members, self._parse_param(subelement, prefix))
            else:
                raise ParseError("Unexpected subelement '" + subelement.name +
                                 "' in struct '" + params["name"] + "'")
        params["members"] = members

        # Magic usage is correct
        # pylint: disable=W0142
        return Model.Struct(**params)

    def _parse_function(self, element, prefix):
        """Parse element as function.

        Returns an instance of generator.Model.Function

        """
        params, subelements, attributes = \
            self._parse_base_item(element, prefix)

        function_id, message_type = self._parse_function_id_type(
            params["name"],
            attributes)

        scope = None
        for attribute in attributes:
            if attribute == "scope":
                scope = attributes[attribute]

        params["function_id"] = function_id
        params["message_type"] = message_type
        params["scope"] = scope

        function_params = collections.OrderedDict()
        for subelement in subelements:
            if subelement.tag == "param":
                function_param = self._parse_function_param(subelement,
                                                            prefix)
                self._check_function_param_name(function_param.name)
                if function_param.name in function_params:
                    raise ParseError("Parameter '" + function_param.name +
                                     "' is specified more than once" +
                                     " for function '" + params["name"] + "'")
                function_params[function_param.name] = function_param
            else:
                raise ParseError("Unexpected subelement '" + subelement.tag +
                                 "' in function '" + params["name"] + "'")
        params["params"] = function_params

        # Magic usage is correct
        # pylint: disable=W0142
        return Model.Function(**params)

    def _parse_function_id_type(self, function_name, attrib):
        """Parse function id and message type according to XML format.

        This implementation takes function name as function id and extracts
        attribute "messagetype" as message type and searches them in enums
        "FunctionID" and "messageType" adding the missing elements if
        necessary.

        Returns function id and message type as an instances of EnumElement.

        """
        if "messagetype" not in attrib:
            raise ParseError("No messagetype specified for function '" +
                             function_name + "'")

        function_id = self._provide_enum_element_for_function(
            "FunctionID",
            function_name)

        message_type = self._provide_enum_element_for_function(
            "messageType",
            self._extract_attrib(attrib, "messagetype"))

        return function_id, message_type

    def _provide_enum_element_for_function(self, enum_name, element_name):
        """Provide enum element for functions.

        Search an element in an enum and add it if it is missing.

        Returns EnumElement.

        """
        if enum_name not in self._types:
            raise ParseError("Enum '" + enum_name +
                             "' is not initialized")

        enum = self._types[enum_name]

        if not isinstance(enum, Model.Enum):
            raise ParseError("'" + enum_name + "' is not an enum")

        if element_name not in enum.elements:
            enum.elements[element_name] = Model.EnumElement(name=element_name)

        return enum.elements[element_name]

    def _parse_base_item(self, element, prefix):
        """Parse element as base item.

        Returns an params, sub-elements and attributes of the element

        """
        params = {}

        description = []
        design_description = []
        issues = []
        todos = []
        subelements = []

        if "name" not in element.attrib:
            raise ParseError("Name is not specified for " + element.tag)

        params["name"] = prefix + element.attrib["name"]
        attrib = dict(element.attrib.items())
        del attrib["name"]

        params["platform"] = self._extract_attrib(attrib, "platform")

        for subelement in element:
            if subelement.tag == "description":
                description.append(self._parse_simple_element(subelement))
            elif subelement.tag == "designdescription":
                design_description.append(
                    self._parse_simple_element(subelement))
            elif subelement.tag == "todo":
                todos.append(self._parse_simple_element(subelement))
            elif subelement.tag == "issue":
                issues.append(self._parse_issue(subelement))
            else:
                subelements.append(subelement)

        params["description"] = description
        params["design_description"] = design_description
        params["issues"] = issues
        params["todos"] = todos

        return params, subelements, attrib

    @staticmethod
    def _parse_simple_element(element):
        """Parse element as simple element and returns it's text.

        Element is simple when it contains no subelements and attributes.

        Returns element text if present or empty string if not

        """
        if len(element) != 0:
            raise ParseError("Unexpected subelements in '" +
                             element.tag + "'")
        if len(element.attrib) != 0:
            raise ParseError("Unexpected attributes in '" +
                             element.tag + "'")
        return element.text if element.text is not None else ""

    @staticmethod
    def _parse_issue(element):
        """Parse element as issue.

        Issue must not contain subelements and attributes.

        Returns an instance of generator.Model.Issue

        """
        if len(element) != 0:
            raise ParseError("Unexpected subelements in issue")
        if "creator" not in element.attrib:
            raise ParseError("No creator in issue")
        if len(element.attrib) != 1:
            raise ParseError("Unexpected attributes in issue")

        return Model.Issue(
            creator=element.attrib["creator"],
            value=element.text if element.text is not None else "")

    def _parse_enum_element(self, element):
        """Parse element as element of enumeration.

        Returns an instance of generator.Model.EnumElement

        """
        params, subelements, attributes = self._parse_base_item(element, "")

        if len(subelements) != 0:
            raise ParseError("Unexpected subelements in enum element")

        self._ignore_attribute(attributes, "hexvalue")
        self._ignore_attribute(attributes, "scope")
        self._ignore_attribute(attributes, "rootscreen")

        internal_name = None
        value = None
        for attribute in attributes:
            if attribute == "internal_name":
                internal_name = attributes[attribute]
            elif attribute == "value":
                try:
                    value = int(attributes[attribute])
                except:
                    raise ParseError("Invalid value for enum element: '" +
                                     attributes[attribute] + "'")
        params["internal_name"] = internal_name
        params["value"] = value

        # Magic usage is correct
        # pylint: disable=W0142
        return Model.EnumElement(**params)

    def _parse_param(self, element, prefix):
        """Parse element as structure parameter.

        Returns an instance of generator.Model.Param

        """
        params, subelements, attrib = \
            self._parse_param_base_item(element, prefix)

        if len(attrib) != 0:
            raise ParseError("""Unknown attribute(s) {0} in param {1}
                """.format(attrib, params["name"]))

        if len(subelements) != 0:
            raise ParseError("Unknown subelements in param '" +
                             params["name"] + "'")

        # Magic usage is correct
        # pylint: disable=W0142
        return Model.Param(**params)

    def _parse_function_param(self, element, prefix):
        """Parse element as function parameter.

        Returns an instance of generator.Model.FunctionParam

        """
        params, subelements, attrib = \
            self._parse_param_base_item(element, prefix)

        default_value = None
        default_value_string = self._extract_attrib(attrib, "defvalue")
        if default_value_string is not None:
            param_type = params["param_type"]
            if type(param_type) is Model.Boolean:
                default_value = \
                    self._get_bool_from_string(default_value_string)
            elif type(param_type) is Model.Integer:
                try:
                    default_value = int(default_value_string)
                except:
                    raise ParseError("Invalid value for integer: '" +
                                     default_value_string + "'")
            elif type(param_type) is Model.Double:
                try:
                    default_value = float(default_value_string)
                except:
                    raise ParseError("Invalid value for float: '" +
                                     default_value_string + "'")
            elif type(param_type) is Model.String:
                default_value = default_value_string
            elif type(param_type) is Model.Enum or \
                    type(param_type) is Model.EnumSubset:
                if type(param_type) is Model.EnumSubset:
                    allowed_elements = param_type.allowed_elements
                else:
                    allowed_elements = param_type.elements
                if default_value_string not in allowed_elements:
                    raise ParseError("Default value '" + default_value_string +
                                     "' for parameter '" + params["name"] +
                                     "' is not a member of " +
                                     type(param_type).__name__ +
                                     "'" + params["name"] + "'")
                default_value = allowed_elements[default_value_string]
            else:
                raise ParseError("Default value specified for " +
                                 type(param_type).__name__)
        params["default_value"] = default_value

        if len(attrib) != 0:
            raise ParseError("Unexpected attributes in parameter '" +
                             params["name"] + "'")

        if len(subelements) != 0:
            raise ParseError("Unexpected subelements in parameter '" +
                             params["name"] + "'")

        # Magic usage is correct
        # pylint: disable=W0142
        return Model.FunctionParam(**params)

    def _parse_param_base_item(self, element, prefix):
        """Parse base param items.

        Returns params, other subelements and attributes.

        """
        params, subelements, attrib = self._parse_base_item(element, "")

        params["is_mandatory"] = self._extract_optional_bool_attrib(
            attrib, "mandatory", True)

        scope = self._extract_attrib(attrib, "scope")
        if scope is not None:
            params["scope"] = scope

        default_value = None;
        param_type = None
        type_name = self._extract_attrib(attrib, "type")
        if type_name is None:
            raise ParseError("Type is not specified for parameter '" +
                             params["name"] + "'")
        if type_name == "Boolean":
            default_value = self._extract_attrib(
                attrib, "defvalue")
            if default_value != None:
                default_value = self._get_bool_from_string(default_value);
            param_type = Model.Boolean(default_value=default_value)
        elif type_name == "Integer" or \
                type_name == "Float" or \
                type_name == "Double" :
            min_value = self._extract_optional_number_attrib(
                attrib, "minvalue", int if type_name == "Integer" else float)
            max_value = self._extract_optional_number_attrib(
                attrib, "maxvalue", int if type_name == "Integer" else float)
            default_value = self._extract_optional_number_attrib(
                attrib, "defvalue", int if type_name == "Integer" else float)

            param_type = \
                (Model.Integer if type_name == "Integer" else Model.Double)(
                    min_value=min_value,
                    max_value=max_value,
                    default_value=default_value)
        elif type_name == "String":
            min_length = self._extract_optional_number_attrib(
                attrib, "minlength")
            # if minlength is not defined default value is 1
            if min_length is None:
                min_length = 1
            max_length = self._extract_optional_number_attrib(
                attrib, "maxlength")
            default_value = self._extract_attrib(attrib, "defvalue")
            param_type = Model.String(min_length=min_length, max_length=max_length, default_value=default_value)
        else:
            if 1 == type_name.count("."):
                custom_type_name = type_name.replace(".", "_")
            else:
                custom_type_name = prefix + type_name

            if custom_type_name in self._types:
                param_type = self._types[custom_type_name]
                default_value = self._extract_attrib(attrib, "defvalue")
                if default_value != None:
                    if default_value not in param_type.elements:
                        raise ParseError("Default value '" + default_value +
                                     "' for parameter '" + params["name"] +
                                     "' is not a member of " +
                                     type(param_type).__name__ +
                                     "'" + params["name"] + "'")
                    default_value = param_type.elements[default_value]
            else:
                raise ParseError("Unknown type '" + type_name + "'")

        if self._extract_optional_bool_attrib(attrib, "array", False):
            min_size = self._extract_optional_number_attrib(attrib,
                                                            "minsize")
            max_size = self._extract_optional_number_attrib(attrib,
                                                            "maxsize")
            param_type = Model.Array(element_type=param_type,
                                     min_size=min_size,
                                     max_size=max_size)
            
        base_type = \
            param_type.element_type if isinstance(param_type, Model.Array) \
            else param_type

        other_subelements = []
        for subelement in subelements:
            if subelement.tag == "element":
                if type(base_type) is not Model.Enum and \
                   type(base_type) is not Model.EnumSubset:
                    raise ParseError("Elements specified for parameter '" +
                                     params["name"] + "' of type " +
                                     type(base_type).__name__)
                if type(base_type) is Model.Enum:
                    base_type = Model.EnumSubset(
                        name=params["name"],
                        enum=base_type,
                        description=params["description"],
                        design_description=params["design_description"],
                        issues=params["issues"],
                        todos=params["todos"],
                        allowed_elements={})
                if "name" not in subelement.attrib:
                    raise ParseError(
                        "Element name is not specified for parameter '" +
                        params["name"] + "'")
                element_name = subelement.attrib["name"]
                if len(subelement.attrib) != 1:
                    raise ParseError("Unexpected attributes for element '" +
                                     element_name + "' of parameter '" +
                                     params["name"])
                if len(subelement.getchildren()) != 0:
                    raise ParseError("Unexpected subelements for element '" +
                                     element_name + "' of parameter '" +
                                     params["name"])
                if element_name in base_type.allowed_elements:
                    raise ParseError("Element '" + element_name +
                                     "' is specified more than once for" +
                                     " parameter '" + params["name"] + "'")
                if element_name not in base_type.enum.elements:
                    raise ParseError("Element '" + element_name +
                                     "' is not a member of enum '" +
                                     base_type.enum.name + "'")
                base_type.allowed_elements[element_name] = \
                    base_type.enum.elements[element_name]
            else:
                other_subelements.append(subelement)

        if isinstance(param_type, Model.Array):
            param_type.element_type = base_type
        else:
            param_type = base_type

        params["param_type"] = param_type
        if default_value is not None:
            params["default_value"] = default_value

        return params, other_subelements, attrib

    def _extract_optional_bool_attrib(self, attrib, name, default):
        """Extract boolean attribute with given name.

        Returns value of the attribute.

        """
        value = self._extract_attrib(attrib, name)

        if value is None:
            value = default
        else:
            value = self._get_bool_from_string(value)

        return value

    def _extract_optional_number_attrib(self, attrib, name, _type=int):
        """Extract number attribute with given name.

        Returns value of the attribute.

        """
        value = self._extract_attrib(attrib, name)

        if value is not None:
            try:
                value = _type(value)
            except:
                raise ParseError("Invlaid value for " + _type.__name__ +
                                 ": '" + value + "'")

        return value

    @staticmethod
    def _extract_attrib(attrib, name):
        """Extract attribute with given name.

        Returns value of the attribute.

        """
        value = None

        if name in attrib:
            value = attrib[name]
            del attrib[name]

        return value

    @staticmethod
    def _get_bool_from_string(bool_string):
        """Convert string representation of boolean to real bool value.

        Returns converted value.

        """
        value = None

        if bool_string in ['0', 'false']:
            value = False
        elif bool_string in ['1', 'true']:
            value = True
        else:
            raise ParseError("Invalid value for bool: '" +
                             bool_string + "'")

        return value

    def _ignore_attribute(self, attrib, name):
        """To be called when attribute is meaningless in terms
        of code generation but it's presence is not issue.

        Removes this attribute from attribute list.

        """
        if name in attrib:
            del attrib[name]
            print ("Ignoring attribute '" +
                   name + "'")
        return True
