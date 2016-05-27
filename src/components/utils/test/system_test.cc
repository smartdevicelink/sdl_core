/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "gmock/gmock.h"
#include "utils/system.h"

namespace test {
namespace components {
namespace utils_test {

using namespace ::utils;

TEST(SystemTest, Constructor_WithCommandName_ExpectArgsStored) {
  // Command creation without any arguments
  const std::string test_command("ls");
  System object(test_command);

  // Check if the object was created with correct command
  ASSERT_EQ(object.command(), test_command);
  int vec_size = object.argv().size();
  ASSERT_EQ(vec_size, 1);
}

TEST(SystemTest, Constructor_WithFileNameCommandName_ExpectArgsStored) {
  // Command creation with 1 argument
  const std::string test_command("ls");
  const std::string test_list_args("-la");
  System object(test_command, test_list_args);

  // Check if the object was created with correct command
  ASSERT_EQ(object.command(), test_command);

  // Check if actual number of arguments arec correct
  int vec_size = object.argv().size();
  ASSERT_EQ(vec_size, 1);  // Correct number of arguments is 1
}

TEST(SystemTest, AddTwoArgsToCommand_ExpectTwoArgsAdded) {
  const std::string test_command("echo");
  const char* args[] = {"-e", "\b"};
  System object(test_command);

  // Adding arguments
  object.Add(args[0]);
  object.Add(args[1]);

  // Check if actual number of arguments equal args stored in object
  int vec_size = object.argv().size();
  ASSERT_EQ(vec_size, 3);  // Correct number of arguments is 3
}

TEST(SystemTest, AddTwoArgsToCommand_CheckOrder_ExpectOrderCorrect) {
  const std::string test_command("echo");
  const char* args[] = {"-e", "\b"};
  System object(test_command);

  // Adding arguments
  object.Add(args[0]);
  object.Add(args[1]);

  // Check if the object was appended by correct arguments in correct order
  EXPECT_STREQ(object.argv()[1].c_str(), args[0]);
  EXPECT_STREQ(object.argv()[2].c_str(), args[1]);
}

TEST(SystemTest, SynchronousInvokeWithExistingCommand_ExpectSuccessfull) {
  const std::string test_command("./testscript.sh");
  System object(test_command);

  // Check if Execute() method is working properly with synchronous command
  // invoke
  ASSERT_TRUE(object.Execute(true));
}

TEST(SystemTest,
     SynchronousInvokeWithEmptyCommand_IncorrectCommand_ExpectFailed) {
  const std::string test_command("");  // any incorrect command
  System object(test_command);

  // Check if Execute() method will fail with not correct command (synchronous
  // command invoke)
  ASSERT_FALSE(object.Execute(true));
}

TEST(SystemTest, ASynchronousInvokeEmptyCommand_InvokeSuccessfull) {
  const std::string test_command(
      "");  // Possible to put here any command (existing or incorrect)
  const std::string test_list_args(
      "anything");  // as command will never be executed from child process
  System object(test_command, test_list_args);  // as parrent process does not
                                                // wait for child process to be
                                                // finished

  // Check if Execute() method is working properly with asynchronous command
  // invoke
  ASSERT_TRUE(object.Execute());
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
