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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADD_TEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADD_TEST_H_

#include <string>

#include "rpc/itest_argument.h"
#include "rpc/gmock_class_container.h"
#include "rpc/without_argument_test.h"
#include "rpc/unary_argument_test.h"
#include "rpc/binary_argument_test.h"


/**
 * @brief Add your functor to test case to Gtest and after this, Gtest call this
 * functor and return value functor compare with return values - ret. If equal
 * test PASSED else FAILED. Functor has no input params.
 *
 * @param first  - test case name
 * @param second - test name
 * @param third  - functor
 * @param fourth - expected return value functor
 * 
 * @return void
 */
template<typename NoParamsFunc, typename ReturnValues>
void addTest(
    const std::string& test_case_name,
    const std::string& test_name,
    NoParamsFunc func,
    ReturnValues ret) {
  ITestArgument* test =
      new WithoutArgumentTest<
      NoParamsFunc,
      ReturnValues>(test_case_name, test_name, func, ret);
  GMockClassContainer* container  = GMockClassContainer::instance();

  container->AddTest(test);
}

/**
 * @brief Add your functor to test case to Gtest and after this, Gtest call this
 * functor and return value functor compare with return values - ret. If equal
 * test PASSED else FAILED. Functor has one input params.
 *
 * @param first  - test case name
 * @param second - test name
 * @param third  - Unfunctor
 * @param fourth - first argument Unfunctor,
 * @param fifth  - expected return value Unfunctor
 * 
 * @return void
 */
template <typename UnFunc, typename FirstArgument, typename ReturnValues>
void addTest(
    const std::string& test_case_name,
    const std::string& test_name,
    UnFunc func,
    FirstArgument first,
    ReturnValues ret) {
  ITestArgument* test =
      new UnaryArgumentTest<
      UnFunc,
      FirstArgument,
      ReturnValues>(test_case_name, test_name, func, first, ret);
  GMockClassContainer* container  = GMockClassContainer::instance();

  container->AddTest(test);
}

/**
 * @brief Add your functor to test case to Gtest and after this, Gtest call this
 * functor and return value functor compare with return values - ret. If equal
 * test PASSED else FAILED. Functor has two input params.
 *
 * @param first  - test case name
 * @param second - test name
 * @param third  - Binfunctor
 * @param fourth - first argument Binfanc
 * @param fifth  - second argument Binfanc,
 * @param the 6th - expected return value Binfanc
 *
 * @return void
 */
template <typename BinFunc, typename FirstArgument, typename SecondArgument,
    typename ReturnValues>
void addTest(
    const std::string& test_case_name,
    const std::string& test_name,
    BinFunc func,
    FirstArgument first,
    SecondArgument second,
    ReturnValues ret) {
  ITestArgument* test =
      new BinaryArgumentTest<
      BinFunc,
      FirstArgument,
      SecondArgument,
      ReturnValues>(
        test_case_name, test_name, func, first, second, ret);
  GMockClassContainer* container  = GMockClassContainer::instance();

  container->AddTest(test);
}

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADD_TEST_H_
