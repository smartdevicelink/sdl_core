"""JSON RPC parser.

Contains parser for JSON RPC XML format.

"""

from generator.parsers import RPCBase


class Parser(RPCBase.Parser):

    """JSON RPC parser."""

    def _parse_root(self, root):
        """Parse root XML element.

        This implementation parses root as interfaces element with multiple
        interfaces in it.

        Keyword arguments:
        root -- root element.

        """

        self._params = root.attrib

        for element in root:
            if element.tag != "interface":
                raise RPCBase.ParseError("Subelement '" + element.tag +
                                         "' is unexpected in interfaces")

            if "name" not in element.attrib:
                raise RPCBase.ParseError(
                    "Name is not specified for interface")

            self._parse_interface(element, element.attrib["name"] + "_")
