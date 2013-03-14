from pprint import pprint as pp

import sys
import os.path
import argparse

import generator.parsers.ALRPCV2
import generator.Model
import generator.generators.SmartSchema

#pp(sys.argv)

def main():
    parser = argparse.ArgumentParser(description="SmartSchema interface generator")
    parser.add_argument("source-xml")
    parser.add_argument("namespace")
    parser.add_argument("output-dir")
    parser.add_argument("--parser-type", choices=["alrpcv2"], required=True)

    args = vars(parser.parse_args())

    # currently there is only one generator and parser
    parser = generator.parsers.ALRPCV2.Parser()
    interface = parser.parse(args["source-xml"])

    schemaGenerator = generator.generators.SmartSchema.SmartSchema()
    schemaGenerator.generate(
        interface,
        os.path.splitext(os.path.basename(args["source-xml"]))[0],
        args["namespace"],
        args["output-dir"]
    )


if __name__ == '__main__':
    main()