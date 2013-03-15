import os.path
import argparse

import generator.parsers.ALRPCV2
import generator.Model
import generator.generators.SmartSchema

_supported_parsers = {
    "alrpcv2": generator.parsers.ALRPCV2.Parser
}

def _create_parser():
    parser = argparse.ArgumentParser(description="SmartSchema interface generator")
    parser.add_argument("source-xml")
    parser.add_argument("namespace")
    parser.add_argument("output-dir")
    parser.add_argument("--parser-type", choices=_supported_parsers.keys(), required=True)
    return parser


def _print_introduction(sourceXml, namespace, outputDir, parserType):
    print("""
Generating interface source code with following parameters:
    Source xml      : {0}
    Namespace       : {1}
    Output directory: {2}
    Parser type     : {3}
""".format(sourceXml, namespace, outputDir, parserType))

def main():
    args = vars(_create_parser().parse_args())

    sourceXml = args["source-xml"]
    sourceXmlName = os.path.splitext(os.path.basename(sourceXml))[0]
    namespace = args["namespace"]
    outputDir = args["output-dir"]
    parserType = args["parser_type"]

    _print_introduction(sourceXml, namespace, outputDir, parserType)

    # Converting incoming xml to internal model
    parser = _supported_parsers[parserType]()
    interface = parser.parse(args["source-xml"])

    # There is only one generator available now
    schemaGenerator = generator.generators.SmartSchema.SmartSchema()
    schemaGenerator.generate(interface, sourceXmlName, namespace, outputDir)

    print("Done.")

if __name__ == '__main__':
    main()