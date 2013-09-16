from argparse import ArgumentParser

arg_parser = ArgumentParser()
arg_parser.add_argument('--infile', required=True)
arg_parser.add_argument('--outfile', required=True)
args = arg_parser.parse_args()

in_file = open(args.infile, "rb")
out_file = open(args.outfile, "w")

out_file.write("const char introspection_xml[] = {")

cnt = 0
for char in in_file.read():
    if cnt % 12 == 0:
        out_file.write("\n  ")
    else:
        out_file.write(" ")
    out_file.write("0x%02x," % ord(char))
    cnt = cnt + 1

out_file.write(" 0x00\n")
out_file.write("};")

