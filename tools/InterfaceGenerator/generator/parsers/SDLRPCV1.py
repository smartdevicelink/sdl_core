"""SDLRPCV1 parser.

Contains parser for SDLRPCV1 XML format.

"""
from parsers.rpc_base import RPCBase


class Parser(RPCBase):
    """SDLRPCV1 parser."""

    @property
    def get_version(self):
        return '1.0.0'
