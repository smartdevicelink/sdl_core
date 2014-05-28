#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
args = sys.argv[1:]

class Validator:
#  Class to validate one string of code
#  Make desigion if current line should be printed or not 
#  according to self state.
#  One instance of Validator should be created for each file
#  or Validator instance should be reseted with reset() method
  customer_tamplate = "CUSTOMER_"
  ifdef_ = "#ifdef "
  startif_ = "#if"
  endif_ = "#endif"
  
  def __init__(self, customer_name):
    if (customer_name == None ):
      return
    self.customer_name = customer_name
    self.my_customer_block = False
    self.other_customer_block = False
    self.if_deep = 0

  def validate(self, line):
#	Make desigion if current line should be printed or not
#	and change self tate if needed
#
    if (self.if_deep < 0):
      raise BaseException("deep < 0")
      
    if (self.other_customer_block and self.my_customer_block):
      raise BaseException("other_customer_block == other_customer_block")
    
    if (self.other_customer_block == True):
      if (self.any_customer_line(line) == True):
        raise BaseException("Recurcive customers bloks")
      if (self.start_if(line) == True):
        self.if_deep += 1
        return False
      if (self.end_if(line) == True):
        if (self.if_deep == 0):
          self.other_customer_block = False
          return False
        self.if_deep -= 1
        return False
      return False
    
    if (self.my_customer_block == True):
      if (self.any_customer_line(line) == True):
        raise BaseException("Recurcive customers bloks") 
      if (self.start_if(line) == True):
        self.if_deep += 1
        return True
      if (self.end_if(line) == True):
        if (self.if_deep == 0):
          self.my_customer_block = False
          return False
        self.if_deep -= 1
        return True
      return True
    
    if (self.my_customer_line(line) == True):
      if (self.if_deep > 0):
        raise BaseException("Can't start customer section with not null @if deep")
      self.my_customer_block = True
      self.if_deep = 0
      return False
      
    if (self.other_customer_line(line) == True):
      if (self.if_deep > 0):
        raise BaseException("Can't start customer section with not null @if deep")
      self.other_customer_block = True
      self.if_deep = 0
      return False
    return True
    
  
  def my_customer_line(self, line):
#	Return true if current line is:
#        #ifdef CUSTOMER<TARGET_CUSTOMER>
#	if it's customer line, otherwise return False
    ifdefline = self.ifdef_ + self.customer_tamplate + self.customer_name
    if ( line.find(ifdefline) == -1):
      return False
    return True
  
  def any_customer_line(self, line):
#	Return true if current line is:
#        #ifdef CUSTOMER<AY_CUSTOMER>
#	Otherwise return False
    ifdefline = self.ifdef_ + self.customer_tamplate
    if ( line.find(ifdefline) == -1):
      return False
    return True
    
  def other_customer_line(self, line):
#	Return true if current line is:
#        #ifdef CUSTOMER<any customer except TARGET_CUSTOMER>
#	Otherwise return False
    ifdefline = self.ifdef_ + self.customer_tamplate
    pos = line.find(ifdefline)
    if ( pos == -1):
      return False
    last = line[pos + len(ifdefline):]
    if (last.find(self.customer_name) == 0):
      return False
    return True
    
  def start_if(self, line):
#	Return true if current line is:
#        #if<any code>
#	Otherwise return False
    if (line.find(self.startif_) != -1):
      return True
    return False

  def end_if(self, line):
#	Return true if current line is:
#        #endif<any code>
#	Otherwise return False
    if (line.find(self.endif_) != -1):
      return True
    return False

class Remover:
#	Class that parce each line of input file with Validator
#	With method get_parsed_lines, you can get parced code
  def __init__(self, file_name, customer_name):
    input_file = open(file_name, "rb")
    self.lines = input_file.readlines()
    input_file.close()
    self.validator = Validator(customer_name)
    
  def get_parsed_lines(self):
#	return array of lines parced by Validator
    v = self.validator
    output = []
    for line in self.lines:
      if v.validate(line):
        output.append(line)
    return output
