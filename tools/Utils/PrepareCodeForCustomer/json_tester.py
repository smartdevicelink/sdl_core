#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import customer_prepare

def toBool(value):
  return True if value == "True" else False if value == "False" else value
  
class JsonTester:
#  Class to test Validator util with json tests
#  Each test contains preconditions and postconditions and result
  def __init__(self, file_name):
    self.file_name = file_name
    test_file = open(file_name, "rb")
    self.test_cases = json.load(test_file)
    self.log = False
    
  def run(self):
#	Run all tests
    print "Run test"
    self.Validator = customer_prepare.Validator(None)
    for test in self.test_cases:
      self.run_test(test)
      
  def run_test(self, test):
#	run one test 'test'
#	@param test is json test object
    print 80 * "="
    test_name = test["name"]
    preconditions = test["preconditions"]
    line = test["line"]   
    expected_result = toBool(test["expected_result"])
    postconditions = test["postconditions"]   
    print test_name
    if (self.log == True):
      print "expected_result: ", expected_result
    customer_name = preconditions["customer_name"]
    my_customer_block =  toBool(preconditions["my_customer_block"]) 
    
    other_customer_block = toBool(preconditions["other_customer_block"])
    if_deep = preconditions["if_deep"]
    
    self.Validator.customer_name = customer_name
    self.Validator.my_customer_block = my_customer_block
    self.Validator.other_customer_block = other_customer_block
    self.Validator.if_deep = int(if_deep)
    
    if (self.log == True):
      print "Preconditions:"
      print "\t customer_name: ", self.Validator.customer_name
      print "\t my_customer_block: ", self.Validator.my_customer_block
      print "\t other_customer_block: ", self.Validator.other_customer_block
      print "\t if_deep: ", self.Validator.if_deep
    
    try:
      result = self.Validator.validate(line)
    except BaseException, err:
      result = "error"
      if (self.log == True):
        print "Exeption: ", err
    if (expected_result == result):
      if (self.check_post_conditions(postconditions) == True):
        print "PASSED"
      else:
        print "FAILED POSTCONDITIONS"
    else:
      print "FAILED"
    if (self.log == True):
      print "result :", result
      self.print_status()
      
      
    print 80 * "="
  
  def check_post_conditions(self, postconditions):
#	 Validate post conditions 
    if (self.log == True):
      print "Expected Postconditions:"
    
    if (postconditions == "None"):
      if (self.log == True):
        print "\t None"
      return True
    r = self.Validator
    my_customer_block =  toBool(postconditions["my_customer_block"]) 
    other_customer_block = toBool(postconditions["other_customer_block"])
    if_deep = int(postconditions["if_deep"])
    if (self.log == True):
      print "\t my_customer_block: ", my_customer_block
      print "\t other_customer_block: ", other_customer_block
      print "\t if_deep: ", if_deep
    if ((my_customer_block == r.my_customer_block) and
       (other_customer_block == r.other_customer_block) and
       (if_deep == r.if_deep)):
         return True
    return False
  
  def print_status(self):
#	Print current state of Validator
    r = self.Validator
    print "Real status:"
    print "\tcustomer_name", r.customer_name
    print "\tmy_customer_block", r.my_customer_block
    print "\tother_customer_block", r.other_customer_block
    print "\tif_deep", r.if_deep

def main():
  t = JsonTester("test.json")
  t.run()
  return 0

if __name__ == '__main__':
  main()

