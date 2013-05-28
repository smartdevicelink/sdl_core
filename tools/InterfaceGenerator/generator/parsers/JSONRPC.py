"""JSON RPC parser.

Contains parser for JSON RPC XML format.

"""

from generator.parsers import RPCBase


class Parser(RPCBase.Parser):

    """JSON RPC parser."""

    def __init__(self):
        """Constructor."""
        super(Parser, self).__init__()
        self._interface_name = None

    def _parse_root(self, root):
        """Parse root XML element.

        This implementation parses root as interfaces element with multiple
        interfaces in it.

        Keyword arguments:
        root -- root element.

        """

        self._params = root.attrib
        self._interface_name = None

        for element in root:
            if element.tag != "interface":
                raise RPCBase.ParseError("Subelement '" + element.tag +
                                         "' is unexpected in interfaces")

            if "name" not in element.attrib:
                raise RPCBase.ParseError(
                    "Name is not specified for interface")

            self._interface_name = element.attrib["name"]
            self._parse_interface(element, self._interface_name + "_")

    def _provide_enum_element_for_function(self, enum_name, element_name):
        """Provide enum element for functions.

        This implementation replaces the underscore separating interface and
        function name with dot and sets it as name of enum element leaving
        the name with underscore as internal_name. For enums other than
        FunctionID the base implementation is called.

        Returns EnumElement.

        """

        name = element_name
        internal_name = None

        if "FunctionID" == enum_name:
            prefix_length = len(self._interface_name) + 1
            if element_name[:prefix_length] != self._interface_name + '_':
                raise RPCBase.ParseError(
                    "Unexpected prefix for function id '" +
                    element_name + "'")
            name = self._interface_name + "." + element_name[prefix_length:]
            internal_name = element_name

        element = super(Parser, self)._provide_enum_element_for_function(
            enum_name,
            name)

        if internal_name is not None:
            element.internal_name = internal_name

        return element

    def _check_function_param_name(self, function_param_name):
        """Check function param name.

        This method is called to check whether the newly parsed function
        parameter name conflicts with some predefined name.
        """

        if function_param_name in ['method', 'code']:
            raise RPCBase.ParseError(
                "'" + function_param_name +
                "' is a predefined name and can't be used" +
                " as a function parameter name")
