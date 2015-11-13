/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_BINARY_ARGUMENT_TEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_BINARY_ARGUMENT_TEST_H_

#include <string>

#include "rpc/itest_argument.h"

/**
 * @brief Need only AddTest.
 * Pattern class inherited from interface
 * InterfaceOfAccessOfGTestToDifferentArgumentsTest, added to
 * GMockClassContainer and method Check called GTest of Google. In method
 * Check call binary functor and passed two parameters, then return values
 * compared with ReturnValues. If equal return true else false.
 */
template <typename BinFunc, typename FirstArgument, typename SecondArgument,
    typename ReturnValues>
class BinaryArgumentTest : public ITestArgument {
 public:
  /**
   * @brief Constructor
   *
   * @param first  - test case name
   * @param second - test name
   * @param third  - Binfunctor
   * @param fourth - first argument Binfanc
   * @param fifth  - second argument Binfanc,
   * @param the 6th - expected return value Binfanc
   *
   */
      BinaryArgumentTest(
      const std::string& test_case_name,
      const std::string& test_name,
      BinFunc func,
      FirstArgument first,
      SecondArgument second,
      ReturnValues ret);

  /**
   * @brief Destructor
   */
  ~BinaryArgumentTest();

  /**
   * @brief Calls Gtest func
   *
   * @param
   *
   * @return bool
   */
  virtual bool Check();

 private:
  /**
   * @brief copy constructor
   *
   * @param first - const ref BinaryFuncFord
   */
  BinaryArgumentTest(const BinaryArgumentTest&);

  /**
   * @brief Operator assignment
   *
   * @param first - const ref BinaryFuncFord
   *
   * @return point to BinaryFuncFord
   */
  BinaryArgumentTest& operator=(const BinaryArgumentTest&);

  /**
   * @brief BinFunc
   */
  BinFunc func_;

  /**
   * @brief FirstArgument
   */
  FirstArgument first_;

  /**
   * @brief SecondArgument
   */
  SecondArgument second_;

  /**
   * @brief ReturnValues
   */
  ReturnValues ret_;
};

template <typename BinFunc, typename FirstArgument, typename SecondArgument,
typename ReturnValues>
BinaryArgumentTest<BinFunc, FirstArgument, SecondArgument, ReturnValues>::
  BinaryArgumentTest(
    const std::string& test_case_name,
    const std::string& test_name,
    BinFunc func,
    FirstArgument first,
    SecondArgument second,
    ReturnValues ret)
    : ITestArgument(test_case_name, test_name),
      func_(func),
      first_(first),
      second_(second),
      ret_(ret) {
}

template <typename BinFunc, typename FirstArgument, typename SecondArgument,
    typename ReturnValues>
BinaryArgumentTest<BinFunc, FirstArgument, SecondArgument, ReturnValues>::
    ~BinaryArgumentTest() {
}

template <typename BinFunc, typename FirstArgument, typename SecondArgument,
    typename ReturnValues>
bool BinaryArgumentTest<BinFunc, FirstArgument, SecondArgument,
    ReturnValues>::Check() {
  return (ret_ == (*func_)(first_, second_));
}

template <typename BinFunc, typename FirstArgument, typename SecondArgument,
    typename ReturnValues>
BinaryArgumentTest<BinFunc, FirstArgument, SecondArgument, ReturnValues>::
    BinaryArgumentTest(const BinaryArgumentTest& in)
  : ITestArgument(in.unit_test_name, in.test_name) {
}

template <typename BinFunc, typename FirstArgument, typename SecondArgument,
    typename ReturnValues>
BinaryArgumentTest<BinFunc, FirstArgument, SecondArgument, ReturnValues>&
BinaryArgumentTest<BinFunc, FirstArgument, SecondArgument,
    ReturnValues>
    ::operator=(const BinaryArgumentTest&) {
  return *this;
}

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_BINARY_ARGUMENT_TEST_H_
