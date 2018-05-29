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

#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sstream>

#include <dirent.h>
#include <unistd.h>
// TODO(VS): lint error: Streams are highly discouraged.
#include <algorithm>
#include <boost/filesystem.hpp>
#include <cstddef>
#include <cstdio>
#include <fstream>

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

// Easier reference
namespace fs = boost::filesystem;
using boost::system::error_code;

uint64_t file_system::GetAvailableDiskSpace(const std::string& path) {
  error_code ec;
  fs::space_info si = fs::space(path, ec);

  if (ec) {
    // If something went wrong, assume no free space
    return 0;
  } else {
    return si.free;
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
  size_t dir_size = 0;
  boost::system::error_code ec;
  fs::recursive_directory_iterator iter(path, ec);
  // Directory does not exist
  if (ec) {
    return 0;
  }
  // default constructor gives end iterator
  fs::recursive_directory_iterator end;
  while (iter != end) {
    size_t fsize = fs::file_size(iter->path(), ec);
    if (ec) {
      // do nothing, can't get size of file
    } else {
      dir_size += fsize;
    }
    iter++;  // next entry
  }
  return dir_size;
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
  boost::system::error_code ec;
  // Create directory and all parents
  fs::create_directories(path, ec);

  if (ec) {
    LOG4CXX_WARN_WITH_ERRNO(logger_,
                            "Unable to create directory recursively: "
                                << path << " reason: " << ec.message());
    std::cerr << "Unable to create directory recursively: " << path
              << " reason: " << ec.message() << std::endl;
    return false;
  }
  // return true if we made something or if it already existed
  return true;
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
  error_code ec;
  return fs::exists(name, ec);
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
  error_code ec;
  fs::path currpath = fs::current_path(ec);
  if (ec) {
    LOG4CXX_WARN(logger_, "Could not get CWD");
  }
  return currpath.string();
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
  error_code ec;
  bool success = fs::remove(name, ec);
  if (ec || !success) {
    // File could not be removed
    return false;
  }
  return true;
}

void file_system::remove_directory_content(const std::string& directory_name) {
  error_code ec;
  fs::directory_iterator dir_iter(directory_name, ec);

  if (ec) {
    LOG4CXX_WARN_WITH_ERRNO(logger_,
                            "Unable to empty directory: " << directory_name);
  }

  // According to Boost's documentation, removing shouldn't invalidate the
  // iterator
  for (auto& dirent : dir_iter) {
    boost::uintmax_t num_removed = fs::remove_all(dirent, ec);
    if (num_removed == 0 || ec) {
      LOG4CXX_WARN_WITH_ERRNO(
          logger_, "Unable to remove file: " << dirent.path().string());
    }
  }
}

bool file_system::RemoveDirectory(const std::string& directory_name,
                                  bool is_recursively) {
  // Make sure the directory exists
  if (!DirectoryExists(directory_name)) {
    return false;
  }
  error_code ec;
  // If recursive, just force full remove
  if (is_recursively) {
    boost::uintmax_t num_removed = fs::remove_all(directory_name, ec);
    if (ec || num_removed == 0) {
      return false;
    }
  } else {
    // Otherwise try to remove
    bool success = fs::remove(directory_name, ec);
    if (!success || ec) {
      return false;
    }
  }
  // Return true on success
  return true;
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
  error_code ec;

  fs::directory_iterator dir_iter(directory_name, ec);
  std::vector<std::string> listFiles;

  // In case the directory doesn't exist / can't be read, second check may be
  // redundant
  if (ec || !DirectoryExists(directory_name)) {
    return listFiles;
  }

  for (auto& dirent : dir_iter) {
    listFiles.push_back(dirent.path().filename().string());
  }
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

// NOTE: this seems to have two flaws
// It returns only the ns _component_ of modify time, not the whole thing
// It relies on C11 / C++17 behaviour
uint64_t file_system::GetFileModificationTime(const std::string& path) {
  struct stat info;
  if (0 != stat(path.c_str(), &info)) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "Could not get file mod time: " << path);
  }
  return static_cast<uint64_t>(info.st_mtim.tv_nsec);
}

bool file_system::CopyFile(const std::string& src, const std::string& dst) {
  if (!FileExists(src) || FileExists(dst) || !CreateFile(dst)) {
    return false;
  }
  error_code ec;
  fs::copy_file(src, dst, ec);
  if (ec) {
    // something failed
    return false;
  }
  return true;
}

bool file_system::MoveFile(const std::string& src, const std::string& dst) {
  error_code ec;

  if (std::rename(src.c_str(), dst.c_str()) == 0) {
    return true;
  } else {
    // In case of src and dst on different file systems std::rename returns
    // an error (at least on QNX).
    // Instead, copy the file over and delete the old one
    bool success = CopyFile(src, dst);
    if (!success) {
      return false;
    }
    DeleteFile(src);

    return true;
  }
  // NOTE this could probably shouldn't be hit if we did it right

  return false;
}
