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
#include "utils/file_system.h"

namespace test  {
namespace components  {
namespace utils  {

TEST(FileSystemTest, CommonFileSystemTest)  {
   // Directory creation
   ASSERT_FALSE(file_system::DirectoryExists("./Test directory"));

  file_system::CreateDirectory("./Test directory");

  ASSERT_TRUE(file_system::DirectoryExists("./Test directory"));

  ASSERT_TRUE(file_system::IsDirectory("./Test directory"));

  // File creation
  ASSERT_FALSE(file_system::FileExists("./Test directory/test file"));

  std::vector<unsigned char> data;
  data.push_back('t');
  data.push_back('e');
  data.push_back('s');
  data.push_back('t');

  ASSERT_TRUE(file_system::Write("./Test directory/test file", data));

  ASSERT_TRUE(file_system::FileExists("./Test directory/test file"));

  ASSERT_FALSE(file_system::IsDirectory("./Test directory/test file"));

  // Read data from file
  std::vector<unsigned char> result;

  ASSERT_TRUE(file_system::ReadBinaryFile("./Test directory/test file",
                                            result));
  ASSERT_FALSE(result.empty());

    // list files
  ASSERT_TRUE(file_system::Write("./Test directory/test file 2", data));

  std::vector<std::string> list;
  list = file_system::ListFiles("./Test directory");

  ASSERT_FALSE(list.empty());
  std::sort(list.begin(), list.end());
  ASSERT_EQ("test file", list[0]);
  ASSERT_EQ("test file 2", list[1]);

  // Delete file
  ASSERT_TRUE(file_system::FileExists("./Test directory/test file 2"));
  ASSERT_TRUE(file_system::DeleteFile("./Test directory/test file 2"));
  ASSERT_FALSE(file_system::FileExists("./Test directory/test file 2"));

  // Delete empty directory
  file_system::CreateDirectory("./Test directory/Empty directory");

  ASSERT_TRUE(file_system::DirectoryExists(
      "./Test directory/Empty directory"));
  ASSERT_TRUE(file_system::RemoveDirectory(
      "./Test directory/Empty directory", false));
  ASSERT_FALSE(file_system::DirectoryExists(
      "./Test directory/Empty directory"));

    ASSERT_FALSE(file_system::RemoveDirectory("./Test directory", false));
    ASSERT_TRUE(file_system::DirectoryExists("./Test directory"));

    // Delete directory recursively
  file_system::CreateDirectory("./Test directory/Test directory 2");
  ASSERT_TRUE(file_system::Write(
        "./Test directory/Test directory 2/test file 2", data));
  ASSERT_TRUE(file_system::RemoveDirectory("./Test directory", true));

  ASSERT_FALSE(file_system::DirectoryExists("./Test directory"));
}
}  // namespace utils
}  // namespace components
}  // namespace test
