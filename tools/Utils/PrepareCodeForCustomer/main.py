#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# Script to prepare sources for defined customer
# Removes code that markes for other customers
# Leaves only common code and defined customer code
#

import sys
import customer_prepare

def is_diff(lines1, lines2):
#  @brief Checks if line arrays are different
#  @param lines1 array of strings containes first lines
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
#  @brief Save linearray to files
#  @param f_name array of strings containes first lines
#  @param lines2 array of strings containes second lines
#  @return True if line arrays are different otherwise return False
  f = open(f_name,"w+")
  for l in lines:
    f.write(l)
  f.close()

def main():
#  main fincton
#  First argument is customer name,
#  Others : file names 
  customer = sys.argv[1]
  files = sys.argv[2:]
  print "Start preparing code for customer:" , customer
  for input_file_name in files:
    print "Process: ", input_file_name,
    input_file = open(input_file_name, "rb")
    input_lines = input_file.readlines()
    input_file.close()
    try:
      parced_lines, warnings = customer_prepare.get_parsed_lines(input_lines, customer)
      if is_diff(input_lines, parced_lines):
        print "Changes"
        save_lines(input_file_name, parced_lines)
      else :
        print ""
    except:
      print "Error"
  return 0

if __name__ == '__main__':
  main()
