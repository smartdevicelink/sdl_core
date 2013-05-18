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
import generator.Model
import generator.generators.SmartSchema
from generator.parsers.RPCBase import ParseError

SUPPORTED_PARSERS = {
    "sdlrpcv1": generator.parsers.SDLRPCV1.Parser,
    "sdlrpcv2": generator.parsers.SDLRPCV2.Parser
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
                        choices=SUPPORTED_PARSERS.keys(),
                        required=True)
    return parser


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

    # Converting incoming xml to internal model
    parser = SUPPORTED_PARSERS[parser_type]()
    try:
        interface = parser.parse(args["source-xml"])

        # There is only one generator available now
        schema_generator = generator.generators.SmartSchema.SmartSchema()
        schema_generator.generate(interface,
                                  src_xml_name,
                                  namespace,
                                  output_dir)
    except ParseError as error:
        print(error.message)
        print
        sys.exit(errno.EINVAL)


    print("Done.")

if __name__ == '__main__':
    main()
