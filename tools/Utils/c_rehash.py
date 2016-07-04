#!/usr/bin/python

import os
import argparse
import subprocess

def process_file (dirname, file) :
    # Old full file ocation
    full_path = dirname + "\\" + file
    # Calculate hash using "OpenSSL x509" functionality
    cmd = subprocess.Popen('openssl x509 -hash -noout -in ' + full_path, shell=True, stdout=subprocess.PIPE)
    for hash in cmd.stdout:
        hahs_filename = hash.rstrip('\n') + ".0"
        print file + " => " + hahs_filename
        # New cert name with hash 
        new_path = dirname + "\\" + hahs_filename
        subprocess.call('copy ' + full_path + " " + new_path + " >NUL", shell=True)

def process_dir (dir) :
    files = os.listdir(dir)
    for filename in files:
        if (".crt" in filename) and not (".req" in filename) :
            process_file( dir, filename )

def main():
    arg_parser = argparse.ArgumentParser(description='"c_rehash" functionality realization.')
    arg_parser.add_argument('-i', '--input_dir', help="Directory which contains ceritificate files.")
    args = arg_parser.parse_args()
	
    if not os.path.exists(args.input_dir):
        raise OSError("Input directory does not exist")
		
    directory = args.input_dir
    print "\nRehash certificates in : " + directory
    process_dir ( directory )

		
if __name__ == "__main__":
    main()