# !/usr/bin/env python
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

def is_diff(lines1, lines2):
    # @brief Checks if line arrays are different
    # @param lines1 array of strings containes first lines
    #  @param lines2 array of strings containes second lines
    #  @return True if line arrays are different otherwise return False
    length1 = len(lines1)
    length2 = len(lines2)
    if (length1 != length2):
        return True
    for i in range(length1):
        l1 = lines1[i]
        l2 = lines2[i]
        if (l1 != l2):
            return True
    return False


def save_lines(f_name, lines):
    # @brief Save linearray to files
    #  @param f_name array of strings containes first lines
    #  @param lines2 array of strings containes second lines
    #  @return True if line arrays are different otherwise return False
    f = open(f_name, "w+")
    for l in lines:
        f.write(l)
    f.close()

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
        print data


if __name__ == '__main__':
    main()
