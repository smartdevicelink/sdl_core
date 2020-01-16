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

import argparse
import os
import sys
from inspect import getfile
from os.path import basename
from pathlib import Path
from re import findall

sys.path.append(Path(__file__).absolute().parents[1].joinpath('rpc_spec/InterfaceParser').as_posix())

try:
    from parsers import sdl_rpc_v1, sdl_rpc_v2, json_rpc
    from parsers.parse_error import ParseError
    from generators import SmartFactorySDLRPC, SmartFactoryJSONRPC, PolicyTypes
    from generators.SmartFactoryBase import GenerateError
    from MsgVersionGenerate import generate_msg_version
except ModuleNotFoundError as error:
    print('{}.\nProbably you did not initialize submodule'.format(error))
    sys.exit(1)


class Generator:
    """Generator application that generates c++ interfaces code from xml description

    """

    def __init__(self):
        self._supported_formats = {
            'sdlrpcv1': (sdl_rpc_v1.Parser, SmartFactorySDLRPC.CodeGenerator),
            'sdlrpcv2': (sdl_rpc_v2.Parser, SmartFactorySDLRPC.CodeGenerator),
            'jsonrpc': (json_rpc.Parser, SmartFactoryJSONRPC.CodeGenerator),
            'mobile-policy-types': (sdl_rpc_v2.Parser, PolicyTypes.CodeGenerator),
            'hmi-policy-types': (json_rpc.Parser, PolicyTypes.CodeGenerator)
        }

    @property
    def supported_formats(self):
        """
        :return: dictionary with supported_formats
        """
        return self._supported_formats

    def _create_parser(self):
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
                            choices=self.supported_formats.keys(),
                            required=True)
        return parser

    def versions_compatibility_validating(self, parser_type):
        """version of generator script requires the same or lesser version of parser script.
        if the parser script needs to fix a bug (and becomes, e.g. 1.0.1) and the generator script stays at 1.0.0.
        As long as the generator script is the same or greater major version, it should be parsable.
        This requires some level of backward compatibility. E.g. they have to be the same major version.

        :param parser_type: parser-type argument provided to script input
        """

        parser = self.supported_formats[parser_type][0]()
        generator = self.supported_formats[parser_type][1]()

        regex = r'(\d+\.\d+).(\d)'

        parser_origin = parser.get_version
        generator_origin = generator.get_version
        parser_split = findall(regex, parser_origin).pop()
        generator_split = findall(regex, generator_origin).pop()

        parser_major = float(parser_split[0])
        generator_major = float(generator_split[0])

        if parser_major > generator_major:
            print('Generator ({}) requires the same or lesser version of Parser ({})'
                  .format(generator_origin, parser_origin))
            sys.exit(1)

        print('Parser type: {}, version {}.\tGenerator type: {}, version {}'
              .format(basename(getfile(parser.__class__)), parser_origin,
                      basename(getfile(generator.__class__)), generator_origin))

    def main(self):
        """Main function of the generator that does actual work."""

        args = vars(self._create_parser().parse_args())

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
        parser = self.supported_formats[parser_type][0]()
        code_generator = self.supported_formats[parser_type][1]()

        # Convert incoming xml to internal model
        try:
            interface = parser.parse(args["source-xml"])

            # Parse sdl version from MOBILE_API.xml and create source file with this version
            if src_xml_name == "MOBILE_API":
                generate_msg_version(src_xml, output_dir)

            # Generate SmartFactory source code from internal model
            code_generator.generate(interface,
                                    src_xml_name,
                                    namespace,
                                    output_dir)
        except (ParseError, GenerateError) as error1:
            print(error1)
            sys.exit(1)

        print('Done.')


if __name__ == '__main__':
    Generator().main()
