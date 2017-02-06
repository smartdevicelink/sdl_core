/*
 * Copyright (c) 2015, Ford Motor Company
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

#include <iostream>
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "generated_msg_version.h"
#include "utils/file_system.h"

using namespace std;

namespace test {
namespace components {
namespace utils_test {

void get_version_from_file(int& first_version, int& second_version) {
  // Supposed that major and minor version include one number
  const std::string& absolute_current_path =
      file_system::CurrentWorkingDirectory();
  std::ifstream xml_file(absolute_current_path + "/MOBILE_API.xml");

  std::string str;

  ASSERT_TRUE(xml_file.is_open());
  while (getline(xml_file, str)) {
    std::size_t isfound = str.find("interface name");
    if (isfound != std::string::npos) {
      std::size_t snd = str.find(".");
      ASSERT_TRUE(snd != std::string::npos);
      first_version = str[snd - 1] - 48;
      second_version = str[snd + 1] - 48;
      break;
    }
  }
}

TEST(GeneratorTool, GetCorrectVersion) {
  int first_version = 0;
  int second_version = 0;
  get_version_from_file(first_version, second_version);
  EXPECT_EQ(first_version, application_manager::major_version);
  EXPECT_EQ(second_version, application_manager::minor_version);
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
