"""ALRPCV1 parser.

Contains parser for ALRPCV1 XML format.

"""

import collections

from generator import Model
from generator.parsers import ALRPCBase


class Parser(ALRPCBase.Parser):

    """ALRPCV1 parser."""

    def parse(self, filename):
        """Parse XML in ALRPCV1 format.

        Returns an instance of generator.Model.Interface containing parsed
        interface or raises ParseError if input XML contains errors
        and can't be parsed.

        Keyword arguments:
        filename -- name of input XML file.

        """
        return self._do_parse(filename)

    def _initialize_enums(self):
        """Initialize enums.

        This implementation returns an OrderedDict with two empty enums:
        "FunctionID" and "messageType". In ALRPCV1 these enums must be
        generated automatically according to the declared in XML functions.

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
            raise ALRPCBase.ParseError(
                "Enum '" + enum.name +
                "' is generated automatically in ALRPCV1 and"
                " must not be declared in xml file")

    def _parse_function_id_type(self, function_name, attrib):
        """Parse function id and message type according to XML format.

        This implementation takes function name as function id and extracts
        attribute "messagetype" as message type and searches them in enums
        "FunctionID" and "messageType" adding the missing elements if
        necessary.

        Returns function id and message type as an instances of EnumElement.

        """
        if "messagetype" not in attrib:
            raise ALRPCBase.ParseError(
                "No messagetype specified for function '" +
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
            raise ALRPCBase.ParseError("Enum '" + enum_name +
                                       "' is not initialized")

        enum = self._types[enum_name]

        if not isinstance(enum, Model.Enum):
            raise ALRPCBase.ParseError("'" + enum_name + "' is not an enum")

        if element_name not in enum.elements:
            enum.elements[element_name] = Model.EnumElement(name=element_name)

        return enum.elements[element_name]
