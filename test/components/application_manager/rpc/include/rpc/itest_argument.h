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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ITEST_ARGUMENT_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ITEST_ARGUMENT_H_

#include <string>

/**
 * @brief Interface provides access Gtest to different tests.
 * In GMockClassContainer contains vector pointer of
 * InterfaceOfAccessOfGTestToDifferentArgumentsTest and when Gtest run, he call
 * Check() in all class contains in GMockClassContainer.
 */

class  ITestArgument {
 public:
  /**
   * @brief Constructor
   *
   * @param first - test case name
   *
   * @param second - test name
   */
  ITestArgument(
      const std::string& in_unit_test_name,
      const std::string& in_test_name);

  /**
   * @brief Calls Gtest func
   *
   * @param
   *
   * @return bool
   */
  virtual bool Check() = 0;

  /**
   * @brief Virtual destructor
   */
  virtual ~ITestArgument();

  /**
   * @brief Set unitTestName
   *
   * @param first - test case name
   *
   * @return void
   */
  inline void SetUnitTestName(const std::string& unittestname);

  /**
   * @brief Get test case name
   *
   * @param
   *
   * @return const std::string &
   */
  inline const std::string& GetUnitTestName() const;

  /**
   * @brief Set test name
   *
   * @param first - test name
   *
   * @return void
   */
  inline void SetTestName(const std::string& testname);

  /**
   * @brief Get test name
   *
   * @param
   *
   * @return const std::string &
   */
  inline const std::string& GetTestName() const;

 protected:
  /**
   * @brief Test case name
   */
  std::string unit_test_name_;

  /**
   * @brief Test name
   */
  std::string test_name_;
};

void ITestArgument::SetUnitTestName(
    const std::string& unit_test_name) {
  unit_test_name_.clear();
  unit_test_name_.append(unit_test_name);
}

const std::string& ITestArgument::
    GetUnitTestName() const {
  return unit_test_name_;
}

void ITestArgument::SetTestName(
    const std::string& test_name) {
  test_name_.clear();
  test_name_.append(test_name);
}

const std::string& ITestArgument::
    GetTestName() const {
  return test_name_;
}

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ITEST_ARGUMENT_H_
