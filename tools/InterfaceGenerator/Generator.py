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

import os
import re
import sys
from argparse import ArgumentParser
from pathlib import Path

ROOT = Path(__file__).absolute().parents[1]
sys.path.append(ROOT.joinpath('rpc_spec/InterfaceParser').as_posix())

try:
    from model.interface import Interface
    from parsers import sdl_rpc_v2
    from parsers.parse_error import ParseError
    from MsgVersionGenerate import generate_msg_version
    from generator.generators import SmartFactorySDLRPC, SmartFactoryJSONRPC, PolicyTypes
    from generator.generators.SmartFactoryBase import GenerateError
    from generator.parsers import SDLRPCV1, JSONRPC
except ModuleNotFoundError as error:
    print(str(error) + '\nPlease make sure the RPC Spec Generator Submodule is initialized.\n'
                       'Check the sdl_core source directory in `tools/rpc_spec`.\n'
                       'Try running in the source directory:\n'
                       '$ git submodule init\n'
                       '$ git submodule update')
    sys.exit(1)


class Generator:
    """Generator application that generates c++ interfaces code from xml description

    """

    def __init__(self):
        self._supported_formats = {
            'sdlrpcv1': (SDLRPCV1.Parser, SmartFactorySDLRPC.CodeGenerator),
            'sdlrpcv2': (sdl_rpc_v2.Parser, SmartFactorySDLRPC.CodeGenerator),
            'jsonrpc': (JSONRPC.Parser, SmartFactoryJSONRPC.CodeGenerator),
            'mobile-policy-types': (sdl_rpc_v2.Parser, PolicyTypes.CodeGenerator),
            'hmi-policy-types': (JSONRPC.Parser, PolicyTypes.CodeGenerator)
        }

    @property
    def supported_formats(self):
        """
        :return: dictionary with supported_formats
        """
        return self._supported_formats

    def _create_parser(self):
        """
        Parsing command-line arguments, or evaluating required Paths interactively.
        :return: an instance of argparse.ArgumentParser
        """
        if len(sys.argv) == 2 and sys.argv[1] in ('-v', '--version'):
            print('1.0.0')
            sys.exit(0)

        parser = ArgumentParser(description='Proxy Library RPC Generator')
        parser.add_argument('-xml', '--source-xml', '--input-file', required=True,
                            help='should point to MOBILE_API.xml')
        parser.add_argument('-ns', '--namespace', required=True)
        parser.add_argument('-d', '-o', '--output-dir', required=True,
                            help='define the place where the generated output should be placed')
        parser.add_argument('-t', '--parser-type', required=True, choices=self.supported_formats.keys())
        parser.add_argument('-v', '--version', action='store_true', help='print the version and exit')
        parser.add_argument('-r', '--regex-pattern', required=False,
                            help='only elements matched with defined regex pattern will be parsed and generated')
        parser.add_argument('--verbose', action='store_true', help='display additional details like logs etc')
        parser.add_argument('-e', '--enums', required=False, action='store_true',
                            help='only specified elements will be generated, if present')
        parser.add_argument('-s', '--structs', required=False, action='store_true',
                            help='only specified elements will be generated, if present')
        parser.add_argument('-m', '-f', '--functions', required=False, action='store_true',
                            help='only specified elements will be generated, if present')
        parser.add_argument('-y', '--overwrite', action='store_true',
                            help='force overwriting of existing files in output directory, ignore confirmation message')
        parser.add_argument('-n', '--skip', action='store_true',
                            help='skip overwriting of existing files in output directory, ignore confirmation message')

        args, unknown = parser.parse_known_args()

        if unknown:
            print('found unknown arguments: ' + ' '.join(unknown))
            parser.print_help(sys.stderr)
            sys.exit(1)

        if args.skip and args.overwrite or not args.skip and not args.overwrite:
            print('please select one option skip or overwrite')
            sys.exit(1)

        if not args.enums and not args.structs and not args.functions:
            args.enums = args.structs = args.functions = True

        return args

    @staticmethod
    def filter_pattern(interface, pattern):
        """
        Filtring model according to regex pattern
        :param interface: Interface model
        :param pattern: regex pattern
        :return:
        """
        match = {i: {} for i in vars(interface).keys()}
        match['params'] = interface.params
        if pattern:
            for key, value in vars(interface).items():
                if key == 'params':
                    continue
                for name, item in value.items():
                    if re.match(pattern, item.name):
                        if key in match:
                            match[key].update({name: item})
        else:
            return interface

        return Interface(**match)

    def main(self):
        """Main function of the generator that does actual work."""

        args = self._create_parser()

        print("""
Generating interface source code with following parameters:
    Source xml      : {0}
    Namespace       : {1}
    Output directory: {2}
    Parser type     : {3}
    overwrite       : {4}
""".format(args.source_xml, args.namespace, args.output_dir, args.parser_type, args.overwrite))

        # Select required parser and code generator
        parser = self.supported_formats[args.parser_type][0]()
        code_generator = self.supported_formats[args.parser_type][1]()

        # Convert incoming xml to internal model
        try:
            interface = parser.parse(args.source_xml)
            filtered = self.filter_pattern(interface, args.regex_pattern)
            src_xml_name = os.path.splitext(os.path.basename(args.source_xml))[0]
            # Parse sdl version from MOBILE_API.xml and create source file with this version
            if src_xml_name == "MOBILE_API":
                generate_msg_version(args.source_xml, args.output_dir)

            # Generate SmartFactory source code from internal model
            code_generator.generate(filtered,
                                    src_xml_name,
                                    args.namespace,
                                    args.output_dir)
        except (ParseError, GenerateError) as error1:
            print(error1)
            sys.exit(1)

        print('Done.')


if __name__ == '__main__':
    Generator().main()
