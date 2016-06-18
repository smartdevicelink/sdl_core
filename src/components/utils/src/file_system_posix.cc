/*
 * Copyright (c) 2016, Ford Motor Company
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
#include "utils/file_system.h"

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

#include <dirent.h>
#include <unistd.h>
// TODO(VS): lint error: Streams are highly discouraged.
#include <fstream>
#include <cstddef>
#include <algorithm>

file_system::FileSizeType file_system::GetAvailableDiskSpace(
    const std::string& utf8_path) {
  struct statvfs fsInfo = {0};
  if (statvfs(utf8_path.c_str(), &fsInfo) == 0) {
    return fsInfo.f_bsize * fsInfo.f_bfree;
  }
  return 0u;
}

file_system::FileSizeType file_system::FileSize(const std::string& utf8_path) {
  if (file_system::FileExists(utf8_path)) {
    struct stat file_info = {0};
    stat(utf8_path.c_str(), &file_info);
    return file_info.st_size;
  }
  return 0u;
}

file_system::FileSizeType file_system::DirectorySize(
    const std::string& utf8_path) {
  FileSizeType size = 0u;
  int32_t return_code = 0;
  DIR* directory = NULL;

#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer = new char[offsetof(struct dirent, d_name) +
                                pathconf(utf8_path.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new (direntbuffer) dirent;
#endif
  struct dirent* result = NULL;
  struct stat file_info = {0};
  directory = opendir(utf8_path.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);
    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if ((kCurrentDirectoryEntry == result->d_name) ||
          (kParentDirectoryEntry == result->d_name)) {
        continue;
      }
      std::string full_element_path = ConcatPath(utf8_path, result->d_name);
      if (file_system::DirectoryExists(full_element_path)) {
        size += DirectorySize(full_element_path);
      } else {
        stat(full_element_path.c_str(), &file_info);
        size += file_info.st_size;
      }
    }
  }
  closedir(directory);
#ifdef __QNXNTO__
  delete[] direntbuffer;
#endif
  return size;
}

bool file_system::CreateDirectory(const std::string& utf8_path) {
  return DirectoryExists(utf8_path) || 0 == mkdir(utf8_path.c_str(), S_IRWXU);
}

bool file_system::CreateDirectoryRecursively(const std::string& utf8_path) {
  std::size_t pos = 0;
  bool ret_val = true;

  while (ret_val == true && pos <= utf8_path.length()) {
    pos = utf8_path.find(GetPathDelimiter(), pos + 1);
    if (!DirectoryExists(utf8_path.substr(0, pos))) {
      if (0 != mkdir(utf8_path.substr(0, pos).c_str(), S_IRWXU)) {
        ret_val = false;
      }
    }
  }

  return ret_val;
}

bool file_system::DirectoryExists(const std::string& utf8_path) {
  struct stat status = {0};

  if (-1 == stat(utf8_path.c_str(), &status) || !S_ISDIR(status.st_mode)) {
    return false;
  }

  return true;
}

bool file_system::FileExists(const std::string& utf8_path) {
  struct stat status = {0};

  if (-1 == stat(utf8_path.c_str(), &status)) {
    return false;
  }
  return true;
}

bool file_system::Write(const std::string& utf8_path,
                        const std::vector<uint8_t>& data,
                        std::ios_base::openmode mode) {
  if (data.empty()) {
    return false;
  }
  std::ofstream file(utf8_path, std::ios_base::binary | mode);
  if (!file.is_open()) {
    return false;
  }
  file.write(reinterpret_cast<const char*>(&data[0]), data.size());
  file.close();
  return file.good();
}

std::ofstream* file_system::Open(const std::string& utf8_path,
                                 std::ios_base::openmode mode) {
  std::ofstream* file = new std::ofstream();
  file->open(utf8_path, std::ios_base::binary | mode);
  if (!file->is_open()) {
    delete file;
    return NULL;
  }
  return file;
}

bool file_system::Write(std::ofstream* const file_stream,
                        const uint8_t* data,
                        std::size_t data_size) {
  if (!file_stream || !data) {
    return false;
  }
  file_stream->write(reinterpret_cast<const char*>(&data[0]), data_size);
  return file_stream->good();
}

void file_system::Close(std::ofstream* file_stream) {
  if (file_stream) {
    file_stream->close();
  }
}

std::string file_system::CurrentWorkingDirectory() {
  const std::size_t filename_max_length = 1024;
  char path[filename_max_length];
  if (0 == getcwd(path, filename_max_length)) {
    return "";
  }
  return std::string(path);
}

bool file_system::DeleteFile(const std::string& utf8_path) {
  if (FileExists(utf8_path) && IsAccessible(utf8_path, W_OK)) {
    return 0 == remove(utf8_path.c_str());
  }
  return false;
}

void file_system::RemoveDirectoryContent(const std::string& utf8_path) {
  int32_t return_code = 0;
  DIR* directory = NULL;
#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer = new char[offsetof(struct dirent, d_name) +
                                pathconf(utf8_path.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new (direntbuffer) dirent;
#endif
  struct dirent* result = NULL;

  directory = opendir(utf8_path.c_str());

  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if ((kCurrentDirectoryEntry == result->d_name) ||
          (kParentDirectoryEntry == result->d_name)) {
        continue;
      }

      std::string full_element_path = ConcatPath(utf8_path, result->d_name);
      if (file_system::DirectoryExists(full_element_path)) {
        RemoveDirectoryContent(full_element_path);
        rmdir(full_element_path.c_str());
      } else {
        remove(full_element_path.c_str());
      }
    }
  }

  closedir(directory);
#ifdef __QNXNTO__
  delete[] direntbuffer;
#endif
}

bool file_system::RemoveDirectory(const std::string& utf8_path,
                                  bool is_recursively) {
  if (DirectoryExists(utf8_path) && IsAccessible(utf8_path, W_OK)) {
    if (is_recursively) {
      RemoveDirectoryContent(utf8_path);
    }

    return 0 == rmdir(utf8_path.c_str());
  }
  return false;
}

bool file_system::IsAccessible(const std::string& utf8_path,
                               int32_t access_rights) {
  return 0 == access(utf8_path.c_str(), access_rights);
}

bool file_system::IsWritingAllowed(const std::string& utf8_path) {
  return IsAccessible(utf8_path, W_OK);
}

bool file_system::IsReadingAllowed(const std::string& utf8_path) {
  return IsAccessible(utf8_path, R_OK);
}

std::vector<std::string> file_system::ListFiles(const std::string& utf8_path) {
  std::vector<std::string> listFiles;
  if (!DirectoryExists(utf8_path)) {
    return listFiles;
  }

  int32_t return_code = 0;
  DIR* directory = NULL;
#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer = new char[offsetof(struct dirent, d_name) +
                                pathconf(utf8_path.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new (direntbuffer) dirent;
#endif
  struct dirent* result = NULL;

  directory = opendir(utf8_path.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if (0 == strcmp(result->d_name, "..") ||
          0 == strcmp(result->d_name, ".")) {
        continue;
      }

      listFiles.push_back(std::string(result->d_name));
    }

    closedir(directory);
  }

#ifdef __QNXNTO__
  delete[] direntbuffer;
#endif

  return listFiles;
}

bool file_system::WriteBinaryFile(const std::string& utf8_path,
                                  const std::vector<uint8_t>& data) {
  if (data.empty()) {
    return false;
  }
  std::ofstream output(utf8_path.c_str(),
                       std::ios_base::binary | std::ios_base::trunc);
  output.write(reinterpret_cast<const char*>(&data[0]), data.size());
  return output.good();
}

bool file_system::ReadBinaryFile(const std::string& utf8_path,
                                 std::vector<uint8_t>& result) {
  if (!FileExists(utf8_path) || !IsAccessible(utf8_path, R_OK)) {
    return false;
  }

  std::ifstream file(utf8_path.c_str(), std::ios_base::binary);
  std::ostringstream ss;
  ss << file.rdbuf();
  const std::string& s = ss.str();

  result.resize(s.length());
  std::copy(s.begin(), s.end(), result.begin());
  return true;
}

bool file_system::ReadFile(const std::string& utf8_path, std::string& result) {
  if (!FileExists(utf8_path) || !IsAccessible(utf8_path, R_OK)) {
    return false;
  }

  std::ifstream file(utf8_path.c_str());
  std::ostringstream ss;
  ss << file.rdbuf();
  result = ss.str();
  return true;
}

const std::string file_system::ConvertPathForURL(const std::string& utf8_path) {
  std::string::const_iterator it_path = utf8_path.begin();
  std::string::const_iterator it_path_end = utf8_path.end();

  const std::string reserved_symbols = "!#$&'()*+,:;=?@[] ";
  std::string::const_iterator it_sym = reserved_symbols.begin();
  std::string::const_iterator it_sym_end = reserved_symbols.end();

  std::string converted_path;
  while (it_path != it_path_end) {
    it_sym = reserved_symbols.begin();
    for (; it_sym != it_sym_end; ++it_sym) {
      if (*it_path == *it_sym) {
        const std::size_t size = 100;
        char percent_value[size];
        snprintf(percent_value, size, "%%%x", *it_path);
        converted_path += percent_value;
        ++it_path;
        continue;
      }
    }

    converted_path += *it_path;
    ++it_path;
  }

  return converted_path;
}

bool file_system::CreateFile(const std::string& utf8_path) {
  std::ofstream file(utf8_path);
  if (!(file.is_open())) {
    return false;
  } else {
    file.close();
    return true;
  }
}

uint64_t file_system::GetFileModificationTime(const std::string& utf8_path) {
  struct stat info;
  stat(utf8_path.c_str(), &info);
#ifndef __QNXNTO__
  return static_cast<uint64_t>(info.st_mtim.tv_sec);
#else
  return static_cast<uint64_t>(info.st_mtime);
#endif
}

bool file_system::CopyFile(const std::string& utf8_src_path,
                           const std::string& utf8_dst_path) {
  if (!FileExists(utf8_src_path) || FileExists(utf8_dst_path) ||
      !CreateFile(utf8_dst_path)) {
    return false;
  }
  std::vector<uint8_t> data;
  if (!ReadBinaryFile(utf8_src_path, data) ||
      !WriteBinaryFile(utf8_dst_path, data)) {
    DeleteFile(utf8_dst_path);
    return false;
  }
  return true;
}

bool file_system::MoveFile(const std::string& utf8_src_path,
                           const std::string& utf8_dst_path) {
  if (!CopyFile(utf8_src_path, utf8_dst_path)) {
    return false;
  }
  if (!DeleteFile(utf8_src_path)) {
    DeleteFile(utf8_dst_path);
    return false;
  }
  return true;
}

bool file_system::IsRelativePath(const std::string& utf8_path) {
  if (utf8_path.empty()) {
    return false;
  }
  return '/' != utf8_path[0];
}

std::string file_system::GetPathDelimiter() {
  return "/";
}

std::string file_system::ConcatPath(const std::string& utf8_path1,
                                    const std::string& utf8_path2) {
  return utf8_path1 + GetPathDelimiter() + utf8_path2;
}
std::string file_system::ConcatPath(const std::string& utf8_path1,
                                    const std::string& utf8_path2,
                                    const std::string& utf8_path3) {
  return ConcatPath(ConcatPath(utf8_path1, utf8_path2), utf8_path3);
}

std::string file_system::ConcatCurrentWorkingPath(
    const std::string& utf8_path) {
  if (!IsRelativePath(utf8_path)) {
    return utf8_path;
  }
  return ConcatPath(CurrentWorkingDirectory(), utf8_path);
}

std::string file_system::RetrieveFileNameFromPath(
    const std::string& utf8_path) {
  std::size_t slash_pos = utf8_path.find_last_of("/", utf8_path.length());
  std::size_t back_slash_pos = utf8_path.find_last_of("\\", utf8_path.length());
  return utf8_path.substr(
      std::max(slash_pos != std::string::npos ? slash_pos + 1 : 0,
               back_slash_pos != std::string::npos ? back_slash_pos + 1 : 0));
}
