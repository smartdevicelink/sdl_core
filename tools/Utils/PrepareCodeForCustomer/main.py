#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# Script to prepare sources for defined customer
# Removes code that markes for other customers
# Leaves only common code and defined customer code
#

import sys
import getopt

import ifdeflexer as core

LOG_FILE_NAME = "Parce_log.txt"

def help():
    print '''
    Usage:
    -h , --help : View this menu
    -i FILE , --input=FILE: Input File (mandatory)
    -c CUSTOMR_NAME , --input=CUSTOMR_NAME: Customer name (mandatory)
    '''

def main():
    # main fincton
    #  First argument is customer name,
    #  Others : file names
    options, remainder = opts, args = getopt.getopt(sys.argv[1:], "hi:vc:v", ["help", "input=","customer="])

    if len(options) == 0:
        help()
        sys.exit()

    customer = ""
    file_name = ""

    for o, a in opts:
        if o in ("-h", "--help"):
            help()
            sys.exit()
        elif o in ("-i", "--input"):
            file_name = a
        elif o in ("-c", "--customer"):
            customer = a
        else:
            assert False, "unhandled option"

    if ((customer == "") or (file_name == "")):
        help()
        sys.exit()
    try:
        input_file = open(file_name, "rb")
        data = input_file.read()
    except:
        #Read Error
        sys.exit()

    result = data
    input_file.close()
    try:
        tokens = core.split_to_tokens(data)
        tokens.reverse()
        result = core.LexicalParcer.parceSyntaxBlock(tokens)
        data = result.Code(customer)
    finally:
        print data,


if __name__ == '__main__':
    main()
