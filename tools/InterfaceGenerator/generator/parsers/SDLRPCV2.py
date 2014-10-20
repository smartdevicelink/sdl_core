"""SDLRPCV2 parser.

Contains parser for SDLRPCV2 XML format.

"""

import collections

from generator import Model
from generator.parsers import RPCBase


class Parser(RPCBase.Parser):

    """SDLRPCV2 parser."""

    def _initialize_enums(self):
        """Initialize enums.

        This implementation returns empty OrderedDict because in SDLRPCV2
        all enums must be declared explicitly in the XML file.

        """
        return collections.OrderedDict()

    def _check_enum_name(self, enum):
        """Check enum name.

        This method is called to check whether the newly parsed enum's name
        conflicts with some predefined enum.
        As SDLRPCV2 has no predefined enums this implementation does nothing.

        """
        pass

    def _parse_function_id_type(self, function_name, attrib):
        """Parse function id and message type according to XML format.

        This implementation extracts attribute "FunctionID" as function id
        and messagetype as message type and searches them in enums
        "FunctionID" and "messageType". If at least one of them (or the entire
        enum) is missing it raises an error.

        Returns function id and message type as an instances of EnumElement.

        """
        if "functionID" not in attrib:
            raise RPCBase.ParseError(
                "No functionID specified for function '" +
                function_name + "'")

        if "messagetype" not in attrib:
            raise RPCBase.ParseError(
                "No messagetype specified for function '" +
                function_name + "'")

        function_id = self._get_enum_element_for_function(
            "FunctionID",
            self._extract_attrib(attrib, "functionID"))
        message_type = self._get_enum_element_for_function(
            "messageType",
            self._extract_attrib(attrib, "messagetype"))

        return function_id, message_type

    def _get_enum_element_for_function(self, enum_name, element_name):
        """Get enum element with given name from given enumeration.

        Returns an instance of generator.Model.EnumElement.

        """
        if enum_name not in self._types:
            raise RPCBase.ParseError(
                "Enumeration '" + enum_name +
                "' must be declared before any function")

        enum = self._types[enum_name]

        if type(enum) is not Model.Enum:
            raise RPCBase.ParseError("'" + enum_name +
                                     "' is not an enumeration")

        if element_name not in enum.elements:
            raise RPCBase.ParseError(
                "'" + element_name +
                "' is not a member of enum '" + enum_name + "'")

        return enum.elements[element_name]
