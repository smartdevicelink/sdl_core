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

#include "utils/file_system.h"
#include "utils/logger.h"

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

#include <dirent.h>
#include <unistd.h>
// TODO(VS): lint error: Streams are highly discouraged.
#include <fstream>
#include <cstddef>
#include <cstdio>
#include <algorithm>

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

uint64_t file_system::GetAvailableDiskSpace(const std::string& path) {
  struct statvfs fsInfo = {0};
  if (statvfs(path.c_str(), &fsInfo) == 0) {
    return fsInfo.f_bsize * fsInfo.f_bfree;
  } else {
    return 0;
  }
}

int64_t file_system::FileSize(const std::string& path) {
  if (file_system::FileExists(path)) {
    struct stat file_info = {0};
    if (0 != stat(path.c_str(), &file_info)) {
      LOG4CXX_WARN_WITH_ERRNO(logger_, "Could not get file size: " << path);
    } else {
      return file_info.st_size;
    }
  }
  return 0;
}

size_t file_system::DirectorySize(const std::string& path) {
  size_t size = 0;
  int32_t return_code = 0;
  DIR* directory = NULL;

#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer = new char[offsetof(struct dirent, d_name) +
                                pathconf(path.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new (direntbuffer) dirent;
#endif
  struct dirent* result = NULL;
  struct stat file_info = {0};
  directory = opendir(path.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);
    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if (0 == strcmp(result->d_name, "..") ||
          0 == strcmp(result->d_name, ".")) {
        continue;
      }
      std::string full_element_path = path + "/" + result->d_name;
      if (file_system::IsDirectory(full_element_path)) {
        size += DirectorySize(full_element_path);
      } else {
        if (0 != stat(full_element_path.c_str(), &file_info)) {
          LOG4CXX_WARN_WITH_ERRNO(
              logger_, "Could not get file info: " << full_element_path);
        } else {
          size += file_info.st_size;
        }
      }
    }
  }
  closedir(directory);
#ifdef __QNXNTO__
  delete[] direntbuffer;
#endif
  return size;
}

std::string file_system::CreateDirectory(const std::string& name) {
  if (!DirectoryExists(name)) {
    if (0 != mkdir(name.c_str(), S_IRWXU)) {
      LOG4CXX_WARN_WITH_ERRNO(logger_, "Unable to create directory: " << name);
    }
  }

  return name;
}

bool file_system::CreateDirectoryRecursively(const std::string& path) {
  size_t pos = 0;
  bool ret_val = true;

  while (ret_val == true && pos <= path.length()) {
    pos = path.find('/', pos + 1);
    if (!DirectoryExists(path.substr(0, pos))) {
      if (0 != mkdir(path.substr(0, pos).c_str(), S_IRWXU)) {
        ret_val = false;
      }
    }
  }

  return ret_val;
}

bool file_system::IsDirectory(const std::string& name) {
  struct stat status = {0};

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }

  return S_ISDIR(status.st_mode);
}

bool file_system::DirectoryExists(const std::string& name) {
  struct stat status = {0};

  if (-1 == stat(name.c_str(), &status) || !S_ISDIR(status.st_mode)) {
    return false;
  }

  return true;
}

bool file_system::FileExists(const std::string& name) {
  struct stat status = {0};

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }
  return true;
}

bool file_system::Write(const std::string& file_name,
                        const std::vector<uint8_t>& data,
                        std::ios_base::openmode mode) {
  std::ofstream file(file_name.c_str(), std::ios_base::binary | mode);
  if (file.is_open()) {
    for (uint32_t i = 0; i < data.size(); ++i) {
      file << data[i];
    }
    file.close();
    return true;
  }
  return false;
}

std::ofstream* file_system::Open(const std::string& file_name,
                                 std::ios_base::openmode mode) {
  std::ofstream* file = new std::ofstream();
  file->open(file_name.c_str(), std::ios_base::binary | mode);
  if (file->is_open()) {
    return file;
  }

  delete file;
  return NULL;
}

bool file_system::Write(std::ofstream* const file_stream,
                        const uint8_t* data,
                        uint32_t data_size) {
  bool result = false;
  if (file_stream) {
    for (size_t i = 0; i < data_size; ++i) {
      (*file_stream) << data[i];
    }
    result = true;
  }
  return result;
}

void file_system::Close(std::ofstream* file_stream) {
  if (file_stream) {
    file_stream->close();
  }
}

std::string file_system::CurrentWorkingDirectory() {
  const size_t filename_max_length = 1024;
  char path[filename_max_length];
  if (0 == getcwd(path, filename_max_length)) {
    LOG4CXX_WARN(logger_, "Could not get CWD");
  }
  return std::string(path);
}

std::string file_system::GetAbsolutePath(const std::string& path) {
  char abs_path[PATH_MAX];
  if (NULL == realpath(path.c_str(), abs_path)) {
    return std::string();
  }

  return std::string(abs_path);
}

bool file_system::IsFileNameValid(const std::string& file_name) {
  return file_name.end() == std::find(file_name.begin(), file_name.end(), '/');
}

bool file_system::DeleteFile(const std::string& name) {
  if (FileExists(name) && IsAccessible(name, W_OK)) {
    return !remove(name.c_str());
  }
  return false;
}

void file_system::remove_directory_content(const std::string& directory_name) {
  int32_t return_code = 0;
  DIR* directory = NULL;
#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer =
      new char[offsetof(struct dirent, d_name) +
               pathconf(directory_name.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new (direntbuffer) dirent;
#endif
  struct dirent* result = NULL;

  directory = opendir(directory_name.c_str());

  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if (0 == strcmp(result->d_name, "..") ||
          0 == strcmp(result->d_name, ".")) {
        continue;
      }

      std::string full_element_path = directory_name + "/" + result->d_name;

      if (file_system::IsDirectory(full_element_path)) {
        remove_directory_content(full_element_path);
        rmdir(full_element_path.c_str());
      } else {
        if (0 != remove(full_element_path.c_str())) {
          LOG4CXX_WARN_WITH_ERRNO(
              logger_, "Unable to remove file: " << full_element_path);
        }
      }
    }
  }

  closedir(directory);
#ifdef __QNXNTO__
  delete[] direntbuffer;
#endif
}

bool file_system::RemoveDirectory(const std::string& directory_name,
                                  bool is_recursively) {
  if (DirectoryExists(directory_name) && IsAccessible(directory_name, W_OK)) {
    if (is_recursively) {
      remove_directory_content(directory_name);
    }

    return !rmdir(directory_name.c_str());
  }
  return false;
}

bool file_system::IsAccessible(const std::string& name, int32_t how) {
  return !access(name.c_str(), how);
}

bool file_system::IsWritingAllowed(const std::string& name) {
  return IsAccessible(name, W_OK);
}

bool file_system::IsReadingAllowed(const std::string& name) {
  return IsAccessible(name, R_OK);
}

std::vector<std::string> file_system::ListFiles(
    const std::string& directory_name) {
  std::vector<std::string> listFiles;
  if (!DirectoryExists(directory_name)) {
    return listFiles;
  }

  int32_t return_code = 0;
  DIR* directory = NULL;
#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer =
      new char[offsetof(struct dirent, d_name) +
               pathconf(directory_name.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new (direntbuffer) dirent;
#endif
  struct dirent* result = NULL;

  directory = opendir(directory_name.c_str());
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

bool file_system::WriteBinaryFile(const std::string& name,
                                  const std::vector<uint8_t>& contents) {
  using namespace std;
  ofstream output(name.c_str(), ios_base::binary | ios_base::trunc);
  output.write(reinterpret_cast<const char*>(&contents.front()),
               contents.size());
  return output.good();
}

bool file_system::ReadBinaryFile(const std::string& name,
                                 std::vector<uint8_t>& result) {
  if (!FileExists(name) || !IsAccessible(name, R_OK)) {
    return false;
  }

  std::ifstream file(name.c_str(), std::ios_base::binary);
  std::ostringstream ss;
  ss << file.rdbuf();
  const std::string s = ss.str();

  result.resize(s.length());
  std::copy(s.begin(), s.end(), result.begin());
  return true;
}

bool file_system::ReadFile(const std::string& name, std::string& result) {
  if (!FileExists(name) || !IsAccessible(name, R_OK)) {
    return false;
  }

  std::ifstream file(name.c_str());
  std::ostringstream ss;
  ss << file.rdbuf();
  result = ss.str();
  return true;
}

const std::string file_system::ConvertPathForURL(const std::string& path) {
  std::string::const_iterator it_path = path.begin();
  std::string::const_iterator it_path_end = path.end();

  const std::string reserved_symbols = "!#$&'()*+,:;=?@[] ";
  size_t pos = std::string::npos;
  std::string converted_path;

  for (; it_path != it_path_end; ++it_path) {
    pos = reserved_symbols.find_first_of(*it_path);
    if (pos != std::string::npos) {
      const size_t size = 100;
      char percent_value[size];
      snprintf(percent_value, size, "%%%x", *it_path);
      converted_path += percent_value;
    } else {
      converted_path += *it_path;
    }
  }
  return converted_path;
}

bool file_system::CreateFile(const std::string& path) {
  std::ofstream file(path);
  if (!(file.is_open())) {
    return false;
  } else {
    file.close();
    return true;
  }
}

uint64_t file_system::GetFileModificationTime(const std::string& path) {
  struct stat info;
  if (0 != stat(path.c_str(), &info)) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "Could not get file mod time: " << path);
  }
#ifndef __QNXNTO__
  return static_cast<uint64_t>(info.st_mtim.tv_nsec);
#else
  return static_cast<uint64_t>(info.st_mtime);
#endif
}

bool file_system::CopyFile(const std::string& src, const std::string& dst) {
  if (!FileExists(src) || FileExists(dst) || !CreateFile(dst)) {
    return false;
  }
  std::vector<uint8_t> data;
  if (!ReadBinaryFile(src, data) || !WriteBinaryFile(dst, data)) {
    DeleteFile(dst);
    return false;
  }
  return true;
}

bool file_system::MoveFile(const std::string& src, const std::string& dst) {
  if (std::rename(src.c_str(), dst.c_str()) == 0) {
    return true;
  } else {
    // In case of src and dst on different file systems std::rename returns
    // an error (at least on QNX).
    // Seems, streams are not recommended for use, so have
    // to find another way to do this.
    std::ifstream s_src(src, std::ios::binary);
    if (!s_src.good()) {
      return false;
    }
    std::ofstream s_dst(dst, std::ios::binary);
    if (!s_dst.good()) {
      return false;
    }
    s_dst << s_src.rdbuf();
    s_dst.close();
    s_src.close();
    DeleteFile(src);
    return true;
  }
  return false;
}
