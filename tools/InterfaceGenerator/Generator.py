"""
Generator application that generates c++ interfaces code from xml description

usage: Generator.py [-h] --parser-type {sdlrpcv2}
                    source-xml namespace output-dir

SmartSchema interface generator

positional arguments:
  source-xml
  namespace
  output-dir

optional arguments:
  -h, --help            show this help message and exit
  --parser-type {sdlrpcv2}
"""

import os.path
import argparse
import errno
import sys

import generator.parsers.SDLRPCV1
import generator.parsers.SDLRPCV2
import generator.parsers.JSONRPC
import generator.generators.SmartFactorySDLRPC
import generator.generators.SmartFactoryJSONRPC
import generator.generators.PolicyTypes
import MsgVersionGenerate

from generator.parsers.RPCBase import ParseError
from generator.generators.SmartFactoryBase import GenerateError

SUPPORTED_FORMATS = {
    "sdlrpcv1": (generator.parsers.SDLRPCV1.Parser,
                 generator.generators.SmartFactorySDLRPC.CodeGenerator),
    "sdlrpcv2": (generator.parsers.SDLRPCV2.Parser,
                 generator.generators.SmartFactorySDLRPC.CodeGenerator),
    "jsonrpc": (generator.parsers.JSONRPC.Parser,
                generator.generators.SmartFactoryJSONRPC.CodeGenerator),
    "mobile-policy-types": (generator.parsers.SDLRPCV2.Parser,
                generator.generators.PolicyTypes.CodeGenerator),
    "hmi-policy-types": (generator.parsers.JSONRPC.Parser,
                generator.generators.PolicyTypes.CodeGenerator)
}


def _create_parser():
    """Create parser for parsing command-line arguments.

    Returns an instance of argparse.ArgumentParser

    """

    parser = argparse.ArgumentParser(
        description="SmartSchema interface generator"
    )
    parser.add_argument("source-xml")
    parser.add_argument("namespace")
    parser.add_argument("output-dir")
    parser.add_argument("--parser-type",
                        choices=SUPPORTED_FORMATS.keys(),
                        required=True)
    return parser


def _handle_fatal_error(error):
    """Handle fatal error during parsing or code generation.

    Keyword arguments:
    error -- base exception to handle.

    """

    print(error.message)
    print
    sys.exit(errno.EINVAL)

def main():
    """Main function of the generator that does actual work."""

    args = vars(_create_parser().parse_args())

    src_xml = args["source-xml"]
    src_xml_name = os.path.splitext(os.path.basename(src_xml))[0]
    namespace = args["namespace"]
    output_dir = args["output-dir"]
    parser_type = args["parser_type"]

    print("""
Generating interface source code with following parameters:
    Source xml      : {0}
    Namespace       : {1}
    Output directory: {2}
    Parser type     : {3}
""".format(src_xml, namespace, output_dir, parser_type))

    # Select required parser and code generator
    parser = SUPPORTED_FORMATS[parser_type][0]()
    code_generator = SUPPORTED_FORMATS[parser_type][1]()

    # Convert incoming xml to internal model
    try:
        interface = parser.parse(args["source-xml"])
    except ParseError as error:
        _handle_fatal_error(error)

    # Parse sdl version from MOBILE_API.xml and create source file with this version
    if src_xml_name == "MOBILE_API":
        try:
            MsgVersionGenerate.generate_msg_version(src_xml, output_dir)
        except ParseError as error:
            _handle_fatal_error(error)

    # Generate SmartFactory source code from internal model
    try:
        code_generator.generate(interface,
                                src_xml_name,
                                namespace,
                                output_dir)
    except GenerateError as error:
        _handle_fatal_error(error)

    print("Done.")

if __name__ == '__main__':
    main()
