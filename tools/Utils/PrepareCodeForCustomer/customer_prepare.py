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
  customer_template = "CUSTOMER_"
  ifdef_ = "#ifdef "
  startif_ = "#if"
  endif_ = "#endif"
  ifndef_ = "#ifndef"
  
  def __init__(self, customer_name):
    if (customer_name == None ):
      return
    self.customer_name = customer_name
    self.my_customer_block = False
    self.other_customer_block = False
    self.if_deep_customer_nested = 0

  def validate(self, line):
#	Make desigion if current line should be printed or not
#	and change self tate if needed
#
    if (self.if_deep_customer_nested < 0):
      raise BaseException("deep < 0")
      
    if (self.other_customer_block and self.my_customer_block):
      raise BaseException("Validator is on bad state. Can't continue processing")
    
    if (self.other_customer_block):
      if (self.any_customer_line(line)):
        raise BaseException("Recurcive customers bloks")
      if (self.start_if(line)):
        self.if_deep_customer_nested += 1
        return False
      if (self.end_if(line)):
        if (self.if_deep_customer_nested == 0):
          self.other_customer_block = False
          return False
        self.if_deep_customer_nested -= 1
        return False
      return False
    
    if (self.my_customer_block):
      if (self.any_customer_line(line)):
        raise BaseException("Recurcive customers bloks") 
      if (self.start_if(line)):
        self.if_deep_customer_nested += 1
        return True
      if (self.end_if(line)):
        if (self.if_deep_customer_nested == 0):
          self.my_customer_block = False
          return False
        self.if_deep_customer_nested -= 1
        return True
      return True
    
    if (self.my_customer_line(line)):
      if (self.if_deep_customer_nested > 0):
        raise BaseException("Can't start customer section with not null @if deep")
      self.my_customer_block = True
      self.if_deep_customer_nested = 0
      return False
      
    if (self.other_customer_line(line)):
      if (self.if_deep_customer_nested > 0):
        raise BaseException("Can't start customer section with not null @if deep")
      self.other_customer_block = True
      self.if_deep_customer_nested = 0
      return False
    return True
    
  
  def my_customer_line(self, line):
#	Return true if current line is:
#        #ifdef CUSTOMER<TARGET_CUSTOMER>
#	if it's customer line, otherwise return False
    ifdefline = self.ifdef_ + self.customer_template + self.customer_name
    pos = line.find(ifdefline)
    if (pos == -1):
      return False
    if ((len(line) > len(ifdefline)) and (line[len(ifdefline)] not in [" ",'/', "\n"])):
		return False # to avoid #ifdef <MY_CUSTOMER_NAME>SOME TEXT
    return True
  
  def any_customer_line(self, line):
#	Return true if current line is:
#        #ifdef CUSTOMER<AY_CUSTOMER>
#	Otherwise return False
    ifdefline = self.ifdef_ + self.customer_template
    if ( line.find(ifdefline) == -1):
      return False
    return True
    
  def other_customer_line(self, line):
#	Return true if current line is:
#        #ifdef CUSTOMER<any customer except TARGET_CUSTOMER>
#	Otherwise return False
    ifdefline = self.ifdef_ + self.customer_template
    pos = line.find(ifdefline)
    if ( pos == -1):
      return False
    if (line.find(self.customer_name,pos + len(ifdefline)) == 0):
      return False
    return True
    
  def start_if(self, line):
#	Return true if current line is:
#        #if<any code>
#	Otherwise return False
    return (line.find(self.startif_) != -1)

  def end_if(self, line):
#	Return true if current line is:
#        #endif<any code>
#	Otherwise return False
    return (line.find(self.endif_) != -1)


def get_parsed_lines(lines, customer_name):
#	return array of lines parced by Validator
  v = Validator(customer_name)
  output = []
  for line in lines:
    if v.validate(line):
      output.append(line)
  return output
