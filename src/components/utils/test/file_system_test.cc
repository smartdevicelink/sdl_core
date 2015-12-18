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

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace utils {

typedef std::vector<std::string> StringArray;

using namespace file_system;

namespace {
StringArray MergeStringsToArray(const std::string& first,
                                const std::string& second) {
  StringArray array_of_strings;
  array_of_strings.reserve(2);

  array_of_strings.push_back(first);
  array_of_strings.push_back(second);

  return array_of_strings;
}
}

TEST(FileSystemTest, CreateDeleteDirectory) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  // Directory creation
  CreateDirectory("./Test directory");

  EXPECT_TRUE(DirectoryExists("./Test directory"));
  EXPECT_TRUE(IsDirectory("./Test directory"));

  // Directory removing
  EXPECT_TRUE(RemoveDirectory("./Test directory", false));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, CreateDirectoryTwice) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  // Directory creation
  CreateDirectory("./Test directory");

  EXPECT_TRUE(DirectoryExists("./Test directory"));
  EXPECT_TRUE(IsDirectory("./Test directory"));

  // Create directory second time
  CreateDirectory("./Test directory");
  EXPECT_TRUE(DirectoryExists("./Test directory"));

  // Directory removing
  EXPECT_TRUE(RemoveDirectory("./Test directory", false));
  // Try to delete directory again
  EXPECT_FALSE(RemoveDirectory("./Test directory", false));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, DeleteDirectoryRecursively) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  // Create directories
  CreateDirectory("./Test directory");
  CreateDirectory("./Test directory/Test directory 2");

  // Create file inside directory
  EXPECT_TRUE(CreateFile("./Test directory/test file"));

  EXPECT_FALSE(RemoveDirectory("./Test directory", false));
  EXPECT_TRUE(DirectoryExists("./Test directory"));
  EXPECT_TRUE(IsDirectory("./Test directory"));

  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, CreateDirectoryRecursivelyDeleteRecursively) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  // Create directories recursively
  CreateDirectoryRecursively(
      "./Test directory/Test directory 2/Test directory 3");

  EXPECT_TRUE(DirectoryExists("./Test directory"));
  EXPECT_TRUE(IsDirectory("./Test directory"));

  EXPECT_TRUE(DirectoryExists("./Test directory/Test directory 2"));
  EXPECT_TRUE(IsDirectory("./Test directory/Test directory 2"));

  EXPECT_TRUE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));
  EXPECT_TRUE(
      IsDirectory("./Test directory/Test directory 2/Test directory 3"));

  // Delete recursively
  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
  EXPECT_FALSE(DirectoryExists("./Test directory/Test directory 2"));
  EXPECT_FALSE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));
}

TEST(FileSystemTest, TwiceCreateDirectoryRecursivelyDeleteRecursivelyOnce) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  // Create directories recursively
  EXPECT_TRUE(CreateDirectoryRecursively(
      "./Test directory/Test directory 2/Test directory 3"));

  // Check that all directories are created
  EXPECT_TRUE(DirectoryExists("./Test directory"));
  EXPECT_TRUE(IsDirectory("./Test directory"));

  EXPECT_TRUE(DirectoryExists("./Test directory/Test directory 2"));
  EXPECT_TRUE(IsDirectory("./Test directory/Test directory 2"));

  EXPECT_TRUE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));
  EXPECT_TRUE(
      IsDirectory("./Test directory/Test directory 2/Test directory 3"));

  // Create directories recursively second time
  EXPECT_TRUE(CreateDirectoryRecursively(
      "./Test directory/Test directory 2/Test directory 3"));

  EXPECT_TRUE(DirectoryExists("./Test directory"));

  EXPECT_TRUE(DirectoryExists("./Test directory/Test directory 2"));

  EXPECT_TRUE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));

  // Delete recursively
  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
  // Delete recursively again is impossible
  EXPECT_FALSE(RemoveDirectory("./Test directory", true));

  EXPECT_FALSE(DirectoryExists("./Test directory"));
  EXPECT_FALSE(DirectoryExists("./Test directory/Test directory 2"));
  EXPECT_FALSE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));
}

TEST(FileSystemTest, CreateDeleteFile) {
  ASSERT_FALSE(FileExists("./test file"));
  // File creation
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_FALSE(IsDirectory("./test file"));

  // Delete file
  EXPECT_TRUE(DeleteFile("./test file"));
  // Try to delete file again
  EXPECT_FALSE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CheckIsDirectory) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  // Create directory and check that IsDirectory=true
  CreateDirectory("./Test directory");
  EXPECT_TRUE(IsDirectory("./Test directory"));

  // Delete directory and check, that IsDirectory=false
  EXPECT_TRUE(RemoveDirectory("./Test directory", false));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
  EXPECT_FALSE(IsDirectory("./Test directory"));

  // Create file and check that IsDirectory=false
  ASSERT_FALSE(FileExists("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_FALSE(IsDirectory("./test file"));

  // Delete file and check that IsDirectory=false
  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
  EXPECT_FALSE(IsDirectory("./test file"));
}

TEST(FileSystemTest, CreateFileTwice) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create file first time
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(FileExists("./test file"));

  // Create file second time
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(FileExists("./test file"));

  // Delete file
  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateOpenCloseFileStream) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());
  Close(test_file);
  EXPECT_FALSE(test_file->is_open());
  delete test_file;

  EXPECT_TRUE(FileExists("./test file"));

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateAndOpenFileStreamTwice) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());
  Close(test_file);
  EXPECT_FALSE(test_file->is_open());
  delete test_file;

  EXPECT_TRUE(FileExists("./test file"));

  // Create file second time
  EXPECT_TRUE(CreateFile("./test file"));

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, OpenFileWriteInFileStream) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  uint8_t* data = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data[i] = i;
  }
  EXPECT_TRUE(Write(test_file, data, data_size));
  Close(test_file);
  EXPECT_FALSE(test_file->is_open());
  delete test_file;

  // Read data from file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Check data
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_EQ(data[i], result[i]);
  }
  delete data;

  // Delete file
  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CannotWriteInClosedFileStream) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());
  Close(test_file);
  EXPECT_FALSE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  uint8_t* data = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data[i] = i;
  }
  EXPECT_TRUE(Write(test_file, data, data_size));

  delete data;
  delete test_file;

  // Read data from file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_TRUE(result.empty());

  // Delete file
  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateWriteInFileStream_CreateFileAgain_FileRewritten) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  uint8_t* data = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data[i] = i;
  }
  EXPECT_TRUE(Write(test_file, data, data_size));

  Close(test_file);
  delete test_file;

  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  delete data;
  EXPECT_TRUE(CreateFile("./test file"));

  // Now file is empty
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_TRUE(result.empty());

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateFileStream_WriteInFile_FileStreamNotClosed) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  std::vector<uint8_t> data;
  for (uint i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  // Write data in file
  EXPECT_TRUE(Write("./test file", data));
  EXPECT_TRUE(test_file->is_open());

  // Close filestream
  Close(test_file);
  delete test_file;

  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest,
     CreateFileStream_WriteInFileWriteInFileStream_FileIncludeLastData) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  std::vector<uint8_t> data;
  for (uint i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  // Write data in file
  EXPECT_TRUE(Write("./test file", data));

  EXPECT_TRUE(test_file->is_open());

  // Write in filestream
  uint8_t* data_2 = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data_2[i] = i + data_size;
  }
  EXPECT_TRUE(Write(test_file, data_2, data_size));
  // Close filestream
  Close(test_file);

  delete test_file;
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Check data
  EXPECT_EQ(result.size(), data_size);
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_NE(data[i], result[i]);
    EXPECT_EQ(data_2[i], result[i]);
  }

  delete data_2;

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteInFilestreamTwice_FileRewritten) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  // Open file second time
  std::ofstream* test_file_2 = Open("./test file");
  EXPECT_TRUE(test_file_2->is_open());

  uint32_t data_size = 4;
  uint8_t* data = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data[i] = i;
  }
  uint8_t* data_2 = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data_2[i] = i + 4;
  }

  // Write data in file
  EXPECT_TRUE(Write(test_file, data, data_size));

  EXPECT_TRUE(Write(test_file_2, data_2, data_size));

  Close(test_file);
  Close(test_file_2);

  EXPECT_FALSE(test_file->is_open());
  EXPECT_FALSE(test_file_2->is_open());

  delete test_file;
  delete test_file_2;
  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());
  // Check data
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_NE(data[i], result[i]);
    EXPECT_EQ(data_2[i], result[i]);
  }

  delete data;
  delete data_2;

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteInFilestreamConsequentially_FileRewritten) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  uint32_t data_size = 4;
  uint8_t* data = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data[i] = i;
  }

  // Write data in file
  EXPECT_TRUE(Write(test_file, data, data_size));

  Close(test_file);
  EXPECT_FALSE(test_file->is_open());

  // Open file second time
  std::ofstream* test_file_2 = Open("./test file");
  EXPECT_TRUE(test_file_2->is_open());

  // Write second time
  uint8_t* data_2 = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data_2[i] = i + 4;
  }
  EXPECT_TRUE(Write(test_file_2, data_2, data_size));

  Close(test_file_2);
  EXPECT_FALSE(test_file_2->is_open());

  delete test_file;
  delete test_file_2;
  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Check data
  EXPECT_EQ(result.size(), data_size);
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_NE(data[i], result[i]);
    EXPECT_EQ(data_2[i], result[i]);
  }

  delete data;
  delete data_2;

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateFileTwiceWriteInFileTwice) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(FileExists("./test file"));

  uint32_t data_size = 4;
  std::vector<uint8_t> data;
  for (uint i = 0; i < data_size; ++i) {
    data.push_back(i);
  }

  // Write data in file
  EXPECT_TRUE(Write("./test file", data));
  // Create file second time
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));

  std::vector<uint8_t> data_2;
  for (uint i = 0; i < data_size; ++i) {
    data_2.push_back(i + data_size);
  }

  // Write data in file
  EXPECT_TRUE(Write("./test file", data_2));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  EXPECT_EQ(data_2, result);
  EXPECT_EQ(result.size(), data_size);
  // Check data
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_NE(data[i], result[i]);
    EXPECT_EQ(data_2[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteInFileTwiceFileRewritten) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(FileExists("./test file"));

  // Write data in file
  uint32_t data_size = 4;
  std::vector<uint8_t> data;
  for (uint i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  EXPECT_TRUE(Write("./test file", data));

  // Write data to file again
  std::vector<uint8_t> data_2;
  for (uint i = 0; i < data_size; ++i) {
    data_2.push_back(i + data_size);
  }
  EXPECT_TRUE(Write("./test file", data_2));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Check data
  EXPECT_EQ(data_size, result.size());
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_NE(data[i], result[i]);
    EXPECT_EQ(data_2[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteDataInTheEndOfFile) {
  ASSERT_FALSE(FileExists("./test file"));

  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(FileExists("./test file"));

  int32_t data_size = 4;
  std::vector<uint8_t> data;
  for (int i = 0; i < data_size; ++i) {
    data.push_back(i);
  }

  // Write data in file
  EXPECT_TRUE(Write("./test file", data));

  // Write in file second time
  std::vector<uint8_t> data_2;
  for (int i = 0; i < data_size; ++i) {
    data_2.push_back(i + data_size);
  }

  // Write data in file
  EXPECT_TRUE(Write("./test file", data_2, std::ios_base::app));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  std::vector<uint8_t> data_check;
  for (int i = 0; i < 2 * data_size; ++i) {
    data_check.push_back(i);
  }

  // Check data
  EXPECT_EQ(data_check.size(), result.size());
  for (int i = 0; i < 2 * data_size; ++i) {
    EXPECT_EQ(data_check[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest,
     WriteInFileStream_WriteInFileInTheEndOfFile_FileIncludeBothData) {
  ASSERT_FALSE(FileExists("./test file"));

  // Create and open file
  std::ofstream* test_file = Open("./test file");
  EXPECT_TRUE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  std::vector<uint8_t> data;
  for (uint i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  // Write data in file
  EXPECT_TRUE(Write("./test file", data));
  EXPECT_TRUE(test_file->is_open());

  // Close filestream
  Close(test_file);

  delete test_file;
  // Write in file second time
  std::vector<uint8_t> data_2;
  for (uint i = 0; i < data_size; ++i) {
    data_2.push_back(i + data_size);
  }

  // Write data in file
  EXPECT_TRUE(Write("./test file", data_2, std::ios_base::app));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  std::vector<uint8_t> data_check;
  for (uint i = 0; i < 2 * data_size; ++i) {
    data_check.push_back(i);
  }

  // Check data
  EXPECT_EQ(data_check.size(), result.size());
  for (uint i = 0; i < 2 * data_size; ++i) {
    EXPECT_EQ(data_check[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, OpenFileStreamForRead_WriteInFileStream) {
  ASSERT_FALSE(FileExists("./test file"));
  // File creation
  EXPECT_TRUE(CreateFile("./test file"));
  std::ofstream* test_file = Open("./test file", std::ios_base::in);
  EXPECT_TRUE(test_file->is_open());

  // Write data in file
  uint32_t data_size = 4;
  uint8_t* data = new uint8_t[data_size];
  for (uint i = 0; i < data_size; ++i) {
    data[i] = i;
  }

  EXPECT_TRUE(Write(test_file, data, data_size));

  Close(test_file);
  EXPECT_FALSE(test_file->is_open());

  // Read data from file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Check data
  for (uint i = 0; i < data_size; ++i) {
    EXPECT_EQ(data[i], result[i]);
  }

  delete data;
  delete test_file;

  EXPECT_TRUE(FileExists("./test file"));

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteFileNotExists) {
  ASSERT_FALSE(FileExists("./test file"));

  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);
  EXPECT_TRUE(Write("./test file", data));
  // File now exists
  ASSERT_TRUE(FileExists("./test file"));
  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteFileReadFile) {
  ASSERT_FALSE(FileExists("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));

  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);
  EXPECT_TRUE(Write("./test file", data));

  // Read data from file
  std::string result;
  std::string check = "test";
  EXPECT_TRUE(ReadFile("./test file", result));
  EXPECT_NE(0u, result.size());
  EXPECT_EQ(check, result);

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteBinaryDataReadBinaryFile) {
  ASSERT_FALSE(FileExists("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));

  uint8_t tmp[] = {1, 2, 3, 4};
  std::vector<uint8_t> data(tmp, tmp + 4);
  EXPECT_TRUE(WriteBinaryFile("./test file", data));

  // Read data from file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());
  EXPECT_EQ(data, result);

  EXPECT_TRUE(DeleteFile("./test file"));
}

TEST(FileSystemTest, WriteBinaryDataTwice_FileRewritten) {
  ASSERT_FALSE(FileExists("./test file"));

  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_TRUE(FileExists("./test file"));

  int32_t data_size = 4;
  std::vector<uint8_t> data;
  for (int i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  // Write data in file
  EXPECT_TRUE(WriteBinaryFile("./test file", data));

  // Write in file second time
  std::vector<uint8_t> data_2;
  for (int i = 0; i < data_size; ++i) {
    data_2.push_back(i + data_size);
  }

  // Write data in file
  EXPECT_TRUE(WriteBinaryFile("./test file", data_2));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Check data
  EXPECT_EQ(data_2.size(), result.size());
  for (int i = 0; i < data_size; ++i) {
    EXPECT_EQ(data_2[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteBinaryDataFileNotExists) {
  ASSERT_FALSE(FileExists("./test file"));

  int32_t data_size = 4;
  std::vector<uint8_t> data;
  for (int i = 0; i < data_size; ++i) {
    data.push_back(i);
  }

  EXPECT_TRUE(WriteBinaryFile("./test file", data));
  ASSERT_TRUE(FileExists("./test file"));
  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteDataAsBinaryData) {
  ASSERT_FALSE(FileExists("./test file"));

  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);
  EXPECT_TRUE(WriteBinaryFile("./test file", data));
  ASSERT_TRUE(FileExists("./test file"));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  EXPECT_EQ(data.size(), result.size());

  for (uint i = 0; i < result.size(); ++i) {
    EXPECT_EQ(data[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteEmptyData) {
  ASSERT_FALSE(FileExists("./test file"));

  std::vector<unsigned char> data;
  EXPECT_TRUE(Write("./test file", data));
  ASSERT_TRUE(FileExists("./test file"));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_TRUE(result.empty());

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteEmptyDataAsBinaryData) {
  ASSERT_FALSE(FileExists("./test file"));

  // Write empty data
  std::vector<unsigned char> data;
  EXPECT_TRUE(WriteBinaryFile("./test file", data));
  ASSERT_TRUE(FileExists("./test file"));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_TRUE(result.empty());

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteBinaryData_WriteDataInTheEndOfFile) {
  ASSERT_FALSE(FileExists("./test file"));

  // Write binary file
  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);
  EXPECT_TRUE(WriteBinaryFile("./test file", data));
  ASSERT_TRUE(FileExists("./test file"));

  // Write in file second time
  int32_t data_size = 4;
  std::vector<uint8_t> data_2;
  for (int i = 0; i < data_size; ++i) {
    data_2.push_back(i);
  }

  // Write data in file
  EXPECT_TRUE(Write("./test file", data_2, std::ios_base::app));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Prepare data for check
  data.insert(data.end(), data_2.begin(), data_2.end());

  // Compare data
  EXPECT_EQ(data.size(), result.size());
  for (uint i = 0; i < result.size(); ++i) {
    EXPECT_EQ(data[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateFile_WriteDataWithFlagOpenForReading) {
  ASSERT_FALSE(FileExists("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));
  // Write data in file
  int32_t data_size = 4;
  std::vector<uint8_t> data;
  for (int i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  EXPECT_TRUE(Write("./test file", data, std::ios_base::in));
  EXPECT_TRUE(FileExists("./test file"));

  // Check file
  std::vector<uint8_t> result;
  EXPECT_TRUE(ReadBinaryFile("./test file", result));
  EXPECT_FALSE(result.empty());

  // Compare data
  EXPECT_EQ(data.size(), result.size());
  for (uint i = 0; i < result.size(); ++i) {
    EXPECT_EQ(data[i], result[i]);
  }

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest,
     FileIsntCreated_WriteFileWithFlagOpenForReadingIsImpossible) {
  ASSERT_FALSE(FileExists("./test file"));

  // Write data in file is impossible
  int32_t data_size = 4;
  std::vector<uint8_t> data;
  for (int i = 0; i < data_size; ++i) {
    data.push_back(i);
  }
  EXPECT_FALSE(Write("./test file", data, std::ios_base::in));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, WriteFileGetSize) {
  ASSERT_FALSE(FileExists("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));
  EXPECT_EQ(0, FileSize("./test file"));

  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);
  EXPECT_TRUE(Write("./test file", data));

  EXPECT_NE(0, FileSize("./test file"));

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, CreateFileCheckDefaultAccess) {
  // File creation
  ASSERT_FALSE(FileExists("./test file"));
  EXPECT_TRUE(CreateFile("./test file"));

  // Check accesses
  EXPECT_TRUE(IsAccessible("./test file", R_OK));
  EXPECT_TRUE(IsAccessible("./test file", W_OK));
  EXPECT_TRUE(IsReadingAllowed("./test file"));
  EXPECT_TRUE(IsWritingAllowed("./test file"));

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, GetFileModificationTime) {
  ASSERT_FALSE(FileExists("./test file"));

  EXPECT_TRUE(CreateFile("./test file"));

  uint64_t modif_time = GetFileModificationTime("./test file");
  EXPECT_LE(0ul, modif_time);

  std::vector<uint8_t> data(1, 1);
  EXPECT_TRUE(WriteBinaryFile("./test file", data));

  EXPECT_LE(0ul, GetFileModificationTime("./test file"));
  EXPECT_LE(modif_time, GetFileModificationTime("./test file"));

  EXPECT_TRUE(DeleteFile("./test file"));
  EXPECT_FALSE(FileExists("./test file"));
}

TEST(FileSystemTest, ListFiles) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  CreateDirectory("./Test directory");

  std::vector<std::string> list;
  list = ListFiles("./Test directory");
  EXPECT_TRUE(list.empty());

  EXPECT_TRUE(CreateFile("./Test directory/test file"));
  EXPECT_TRUE(CreateFile("./Test directory/test file 2"));

  list = ListFiles("./Test directory");
  EXPECT_FALSE(list.empty());

  std::sort(list.begin(), list.end());
  EXPECT_EQ("test file", list[0]);
  EXPECT_EQ("test file 2", list[1]);

  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));

  EXPECT_FALSE(FileExists("./Test directory/test file"));
  EXPECT_FALSE(FileExists("./Test directory/test file 2"));
}

TEST(FileSystemTest, ListFilesIncludeSubdirectory) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  CreateDirectoryRecursively("./Test directory/Test directory 2/");

  std::vector<std::string> list;
  list = ListFiles("./Test directory");
  EXPECT_FALSE(list.empty());
  EXPECT_EQ(1u, list.size());
  EXPECT_EQ("Test directory 2", list[0]);

  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, ListFilesDoesNotIncludeFilesInSubdirectory) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  CreateDirectoryRecursively("./Test directory/Test directory 2/");

  std::vector<std::string> list;
  list = ListFiles("./Test directory");
  EXPECT_FALSE(list.empty());

  EXPECT_TRUE(CreateFile("./Test directory/Test directory 2/test file"));
  EXPECT_TRUE(CreateFile("./Test directory/Test directory 2/test file 2"));

  list = ListFiles("./Test directory");
  EXPECT_FALSE(list.empty());

  std::sort(list.begin(), list.end());
  EXPECT_EQ("Test directory 2", list[0]);
  EXPECT_EQ(1u, list.size());

  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, GetAvailableDiskSpace) {
  // Get available disk space before directory with file creaction and after
  uint64_t available_space = GetAvailableDiskSpace(".");
  EXPECT_NE(0u, available_space);
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  CreateDirectory("./Test directory");

  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);
  EXPECT_TRUE(Write("./Test directory/test file", data));

  EXPECT_GE(available_space, GetAvailableDiskSpace("."));
  EXPECT_TRUE(RemoveDirectory("./Test directory"));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, ConvertPathForURL) {
  std::string path = "./Test directory";
  EXPECT_NE(path, ConvertPathForURL(path));
  std::string path_brackets = "./Test_directory_with(brackets)";
  EXPECT_NE(path_brackets, ConvertPathForURL(path));
  std::string another_path = "./Test_directory/new_directory_without_spaces";
  EXPECT_EQ(another_path, ConvertPathForURL(another_path));
}

TEST(FileSystemTest, DirectorySize) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  CreateDirectory("./Test directory");
  EXPECT_TRUE(DirectoryExists("./Test directory"));
  // Get size of empty directory
  EXPECT_EQ(0u, DirectorySize("./Test directory"));
  EXPECT_TRUE(CreateFile("./Test directory/test file"));

  // Get size of nonempty directory with empty file
  EXPECT_EQ(0u, DirectorySize("./Test directory"));

  unsigned char tmp[] = {'t', 'e', 's', 't'};
  std::vector<unsigned char> data(tmp, tmp + 4);

  EXPECT_TRUE(Write("./Test directory/test file", data));
  // Get size of nonempty directory with nonempty file
  EXPECT_NE(0u, DirectorySize("./Test directory"));

  EXPECT_TRUE(DeleteFile("./Test directory/test file"));
  EXPECT_EQ(0u, DirectorySize("./Test directory"));
  EXPECT_TRUE(RemoveDirectory("./Test directory"));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, DeleteAllContentInDirectory) {
  ASSERT_FALSE(DirectoryExists("./Test directory"));
  CreateDirectory("./Test directory");

  // Create files in directory
  EXPECT_TRUE(CreateFile("./Test directory/test file"));
  EXPECT_TRUE(CreateFile("./Test directory/test file 2"));

  EXPECT_TRUE(FileExists("./Test directory/test file"));
  EXPECT_TRUE(FileExists("./Test directory/test file 2"));

  EXPECT_TRUE(DirectoryExists("./Test directory"));

  // Create subdirectories
  CreateDirectoryRecursively(
      "./Test directory/Test directory 2/Test directory 3");

  EXPECT_TRUE(DirectoryExists("./Test directory/Test directory 2"));
  EXPECT_TRUE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));

  remove_directory_content("./Test directory");

  // Directory does not include files and subdirectories
  EXPECT_FALSE(FileExists("./Test directory/test file"));
  EXPECT_FALSE(FileExists("./Test directory/test file 2"));

  EXPECT_FALSE(
      DirectoryExists("./Test directory/Test directory 2/Test directory 3"));
  EXPECT_FALSE(DirectoryExists("./Test directory/Test directory 2"));

  std::vector<std::string> list;
  list = ListFiles("./Test directory");
  EXPECT_TRUE(list.empty());

  EXPECT_TRUE(DirectoryExists("./Test directory"));

  EXPECT_TRUE(RemoveDirectory("./Test directory", true));
  EXPECT_FALSE(DirectoryExists("./Test directory"));
}

TEST(FileSystemTest, GetAbsolutePath) {
  const std::string& abs_path = GetAbsolutePath(".");
  // Getting absolute current path from system
  const std::string& absolute_current_path = CurrentWorkingDirectory();
  EXPECT_EQ('/', abs_path[0]);
  EXPECT_EQ(absolute_current_path, abs_path);
}

TEST(FileSystemTest,
     GetAbsolutePath_InvalidOrEmptyPathName_EmptyAbsolutePathName) {
  // Array of invalid paths
  const StringArray rel_path = MergeStringsToArray("not_exists_dir", "     ");

  // Check
  for (size_t i = 0; i < rel_path.size(); ++i) {
    const std::string& path_for_check = GetAbsolutePath(rel_path[i]);
    EXPECT_EQ("", path_for_check);
  }
}

TEST(FileSystemTest, GetAbsolutePath_ValidRelPaths_CorrectAbsolutePath) {
  // Array of relative dirs
  const StringArray rel_path = MergeStringsToArray(
      "first_level_path", "first_level_path/second_level_path1");

  // Create some directories in current
  CreateDirectoryRecursively(rel_path[1]);
  // Get absolute current dir
  const std::string& absolute_current_dir = GetAbsolutePath(".");
  // Check
  for (size_t i = 0; i < rel_path.size(); ++i) {
    // Concating rel_path to current dir path
    const std::string& correct_absolute_path =
        absolute_current_dir + "/" + rel_path[i];
    // Get absolute path for rel dir
    const std::string& path_for_check = GetAbsolutePath(rel_path[i]);
    EXPECT_EQ(correct_absolute_path, path_for_check);
  }
  // Cleanup after test case
  if (DirectoryExists(rel_path[0])) {
    RemoveDirectory(rel_path[0], true);
  }
}

TEST(FileSystemTest,
     GetAbsolutePath_ValidRelPathsFromParrentDir_CorrectAbsolutePath) {
  // Array of relative dirs
  const StringArray rel_path = MergeStringsToArray(
      "../first_level_path", "../first_level_path/second_level_path1");

  // Create some directories in parrent of this
  CreateDirectoryRecursively(rel_path[1]);

  // Get absolute parrent dir
  const std::string& absolute_parrent_dir = GetAbsolutePath("../");
  // Check
  for (size_t i = 0; i < rel_path.size(); ++i) {
    // Concatenation rel_path to current dir path
    const std::string& relative_dir_name = rel_path[i].substr(3);
    const std::string& correct_absolute_path =
        absolute_parrent_dir + "/" + relative_dir_name;
    // Get absolute path for rel dir
    const std::string& path_for_check = GetAbsolutePath(rel_path[i]);
    EXPECT_EQ(correct_absolute_path, path_for_check);
  }
  // Cleanup after test case
  if (DirectoryExists(rel_path[0])) {
    RemoveDirectory(rel_path[0], true);
  }
}

TEST(FileSystemTest, GetAbsolutePath_TrickiPath_CorrectAbsolutePath) {
  // Array of relative dirs
  const StringArray rel_path =
      MergeStringsToArray("../src/../../application_manager/../utils/test",
                          "../../../components/utils/test");

  const std::string& absolute_current_path = CurrentWorkingDirectory();
  for (size_t i = 0; i < rel_path.size(); ++i) {
    // Get absolute path for rel dir
    const std::string& path_for_check = GetAbsolutePath(rel_path[i]);
    EXPECT_EQ(absolute_current_path, path_for_check);
  }
}

}  // namespace utils
}  // namespace components
}  // namespace test
