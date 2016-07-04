#!/usr/bin/python

import os
import argparse
import subprocess


def process_file(dirname, filename):
## process_file () function
#   - creates copy of certificate named with hash of inner certificate data
#  @filename - file for processing 

    # Old full file ocation
    full_path = os.path.join(dirname, filename)
    # Calculate hash using "OpenSSL x509" functionality
    cmd = subprocess.Popen(
        'openssl x509 -hash -noout -in {}'.format(full_path), 
        shell=True, 
        stdout=subprocess.PIPE)
    for hash in cmd.stdout:
        hash_filename = hash.rstrip('\n') + ".0"
        print("{} => {}".format(filename, hash_filename))
        # New cert name with hash
        new_path = os.path.join(dirname, hash_filename)
        subprocess.call(
            'copy {} {} {}'.format(full_path, new_path, ">NUL"),  
            shell=True)


def process_dir(dir):
## process_dir () function
#   - finds all crt files in directory and call process_file() for each
#  @dir - directory name for processing
    files = os.listdir(dir)
    for filename in files:
        if (".crt" in filename) and not (".req" in filename):
            process_file(dir, filename)


def main():
    arg_parser = argparse.ArgumentParser(
        description="""Creates symlinks for certificates named with hash 
                    of inner certificate data""")
    arg_parser.add_argument(
        '-i', '--input_dir', required=True,   
        help="Directory which contains ceritificate files.")
    args = arg_parser.parse_args()

    if not os.path.exists(args.input_dir):
        raise OSError("Input directory does not exist")

    directory = args.input_dir
    print("\nRehash certificates in : {}".format(directory))
    process_dir(directory)


if __name__ == "__main__":
    main()
