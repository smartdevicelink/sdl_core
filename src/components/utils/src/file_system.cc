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

SDL_CREATE_LOG_VARIABLE("Utils::FileSystem")

// Easier reference
namespace fs = boost::filesystem;
using boost::system::error_code;

uint64_t file_system::GetAvailableDiskSpace(const std::string& path) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  fs::space_info si = {0, 0, 0};
  si = fs::space(path, ec);

  if (ec) {
    // If something went wrong, assume no free space
    SDL_LOG_ERROR_WITH_ERRNO("Unable to get available disk space: '"
                             << path << "', reason: " << ec.message());
  }
  return si.free;
}

uint64_t file_system::FileSize(const std::string& path) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  // Boost returns sizes as unsigned
  const uint64_t fsize = static_cast<uint64_t>(fs::file_size(path, ec));

  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO(

        "Unable to get file size: '" << path << "', reason: " << ec.message());
    return 0;
  }
  return fsize;
}

size_t file_system::DirectorySize(const std::string& path) {
  SDL_LOG_AUTO_TRACE();
  size_t dir_size = 0;
  error_code ec;
  // Recursively iterate through directory to accumulate file sizes
  fs::recursive_directory_iterator iter(path, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to get directory size: '"
                             << path << "', reason: " << ec.message());
    return 0;
  }

  // default constructor gives end iterator
  fs::recursive_directory_iterator end;
  while (end != iter) {
    const bool is_directory = fs::is_directory(iter->path(), ec);
    if (ec) {
      SDL_LOG_WARN_WITH_ERRNO("Failed check if '"
                              << iter->path()
                              << "' is directory, reason: " << ec.message());
    }

    if (!is_directory && !ec) {
      const size_t fsize = fs::file_size(iter->path(), ec);
      if (ec) {
        SDL_LOG_WARN_WITH_ERRNO("Failed to get file_size: '"
                                << path << "', reason: " << ec.message());
      } else {
        // No error means we can add the file
        dir_size += fsize;
        SDL_LOG_DEBUG("Adding: " << fsize << ", total: " << dir_size);
      }
    }

    // Increment the iterator to point to next entry in recursive iteration
    iter.increment(ec);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO("Failed to increment iterator for path '"
                               << path << "', reason: " << ec.message());
      return dir_size;
    }
  }

  return dir_size;
}

// NOTE that boost makes 0777 permissions by default
bool file_system::CreateDirectory(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  const bool success = fs::create_directory(name, ec);
  if (!success || ec) {
    SDL_LOG_WARN_WITH_ERRNO("Unable to create directory: " << name);
  } else {
    // Set 0700 permissions to maintain previous API
    fs::permissions(name, fs::perms::owner_all, ec);
  }
  return success;
}

bool file_system::CreateDirectoryRecursively(const std::string& path) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  // Create directory and all parents
  fs::create_directories(path, ec);

  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to create directory recursively: '"
                             << path << "', reason: " << ec.message());

    return false;
  }
  // return true if we made something or if it already existed
  return true;
}

bool file_system::IsDirectory(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  const bool is_directory = fs::is_directory(name, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to check if it is directory: "
                             << name << " reason: " << ec.message());
  }
  return is_directory;
}

// NOTE this may be a duplicate of IsDirectory since it already checks
// existence
bool file_system::DirectoryExists(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  const bool exists = FileExists(name) && IsDirectory(name);
  SDL_LOG_DEBUG(

      "Directory '" << name << "' " << (exists ? "exists" : "NOT exists"));
  return exists;
}

bool file_system::FileExists(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  const bool exists = fs::exists(name, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to check that file exists: "
                             << name << " reason: " << ec.message());
  }
  return exists;
}

bool file_system::Write(const std::string& file_name,
                        const std::vector<uint8_t>& data,
                        std::ios_base::openmode mode) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
  std::ofstream* file = new std::ofstream();
  file->open(file_name.c_str(), std::ios_base::binary | mode);
  if (file->is_open()) {
    return file;
  }

  delete file;
  return nullptr;
}

bool file_system::Write(std::ofstream* const file_stream,
                        const uint8_t* data,
                        uint32_t data_size) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
  if (file_stream) {
    file_stream->close();
  }
}

std::string file_system::CurrentWorkingDirectory() {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  const fs::path currpath = fs::current_path(ec);
  if (ec) {
    SDL_LOG_WARN_WITH_ERRNO("Unable to get current working directory: '"
                            << currpath << "' reason: " << ec.message());
  }
  return currpath.string();
}

std::string file_system::GetAbsolutePath(const std::string& path) {
  SDL_LOG_AUTO_TRACE();
  error_code ec;
  const fs::path absolute = fs::canonical(path, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to get absolute path: '"
                             << path << "', reason: " << ec.message());
    return std::string();  // invalid path
  }
  return absolute.string();
}

bool file_system::IsFileNameValid(const std::string& file_name) {
  SDL_LOG_AUTO_TRACE();
  return file_name.end() == std::find(file_name.begin(), file_name.end(), '/');
}

// Does not remove if file is write-protected
bool file_system::DeleteFile(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  if (FileExists(name) && IsAccessible(name, W_OK)) {
    error_code ec;
    const bool success = fs::remove(name.c_str(), ec);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO("Unable to delete file: '"
                               << name << "', reason: " << ec.message()
                               << "success: " << success);
    }

    return success && !ec;
  }
  SDL_LOG_WARN(
      "Unable to delete file either doesn't exist or is not accessible");
  return false;
}

void file_system::remove_directory_content(const std::string& directory_name) {
  SDL_LOG_AUTO_TRACE();

  error_code ec;
  fs::directory_iterator dir_iter(directory_name, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to remove directory contents: "
                             << directory_name << " reason: " << ec.message());
  }

  // According to Boost's documentation, removing shouldn't invalidate the
  // iterator, although it may cause the removed entry to appear again,
  // duplicating the warning message. See here:
  // https://www.boost.org/doc/libs/1_67_0/libs/filesystem/doc/reference.html#Class-directory_iterator
  const fs::directory_iterator end;
  while (dir_iter != end) {
    fs::remove_all(dir_iter->path(), ec);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO(

          "Unable to remove file: " << dir_iter->path().string() << " reason "
                                    << ec.message());
    }
    dir_iter.increment(ec);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO("Unable to increment dir_iter: reason "
                               << ec.message());
      break;
    }
  }
}

bool file_system::RemoveDirectory(const std::string& directory_name,
                                  const bool is_recursively) {
  SDL_LOG_AUTO_TRACE();
  // Make sure the directory exists
  if (!DirectoryExists(directory_name) && IsAccessible(directory_name, W_OK)) {
    SDL_LOG_WARN(

        "Unable to remove directory either doesn't exist or is not accessible");
    return false;
  }
  error_code ec;
  bool success;
  // If recursive, just force full remove
  if (is_recursively) {
    success = (fs::remove_all(directory_name, ec) != 0);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO("Unable to remove all: '" << directory_name
                                                         << "', reason "
                                                         << ec.message());
    }
  } else {
    // Otherwise try to remove
    success = fs::remove(directory_name, ec);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO("Unable to remove: '" << directory_name
                                                     << "', reason "
                                                     << ec.message());
    }
  }
  return success && !ec;
}

bool file_system::IsAccessible(const std::string& name, const int32_t how) {
  SDL_LOG_AUTO_TRACE();
  return !access(name.c_str(), how);
}

bool file_system::IsWritingAllowed(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  return IsAccessible(name, W_OK);
}

bool file_system::IsReadingAllowed(const std::string& name) {
  SDL_LOG_AUTO_TRACE();
  return IsAccessible(name, R_OK);
}

std::vector<std::string> file_system::ListFiles(
    const std::string& directory_name) {
  SDL_LOG_AUTO_TRACE();

  error_code ec;
  fs::directory_iterator iter(directory_name, ec), end;
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to get directory_iterator: "
                             << directory_name << " reason " << ec.message());
    return std::vector<std::string>();
  }

  std::vector<std::string> list_files;
  while (end != iter) {
    list_files.push_back(iter->path().filename().string());
    iter.increment(ec);
    if (ec) {
      SDL_LOG_ERROR_WITH_ERRNO("Failed to increment iterator for path '"
                               << directory_name
                               << "', reason: " << ec.message());
      return list_files;
    }
  }
  return list_files;
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
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Filename: " << name);
  if (!FileExists(name) || !IsAccessible(name, R_OK)) {
    SDL_LOG_ERROR("Not able to read binary file: " << name);
    return false;
  }

  std::ifstream file(name.c_str(), std::ios::in | std::ios_base::binary);
  if (!file.is_open()) {
    return false;
  }

  std::vector<uint8_t> content((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
  result.swap(content);

  return true;
}

bool file_system::ReadBinaryFile(const std::string& name,
                                 std::vector<uint8_t>& result,
                                 uint32_t offset,
                                 uint32_t length) {
  if (!FileExists(name) || !IsAccessible(name, R_OK)) {
    SDL_LOG_ERROR("Not able to read binary file: " << name);
    return false;
  }

  std::ifstream file(name.c_str(), std::ios_base::binary);
  if (!file.is_open()) {
    return false;
  }

  file.ignore(offset);
  std::string s;
  s.resize(length);
  file.read(&s[0], length);

  result.resize(s.length());
  std::copy(s.begin(), s.end(), result.begin());
  return true;
}

bool file_system::ReadFile(const std::string& name, std::string& result) {
  SDL_LOG_AUTO_TRACE();
  if (!FileExists(name) || !IsAccessible(name, R_OK)) {
    SDL_LOG_ERROR("Not able to read file: " << name);
    return false;
  }
  std::ifstream file(name.c_str());
  if (!file) {
    SDL_LOG_ERROR("Not able to open binary file: " << name);
    return false;
  }
  std::ostringstream ss;
  ss << file.rdbuf();
  result = ss.str();
  return true;
}

const std::string file_system::ConvertPathForURL(const std::string& path) {
  SDL_LOG_AUTO_TRACE();
  const std::string reserved_symbols = "!#$&'()*+,:;=?@[] ";
  size_t pos = std::string::npos;
  std::string converted_path;

  for (const auto symbol : path) {
    pos = reserved_symbols.find_first_of(symbol);
    if (pos != std::string::npos) {
      const size_t size = 100;
      char percent_value[size];
      snprintf(percent_value, size, "%%%x", symbol);
      converted_path += percent_value;
    } else {
      converted_path += symbol;
    }
  }
  return converted_path;
}

bool file_system::CreateFile(const std::string& path) {
  SDL_LOG_AUTO_TRACE();

  std::ofstream file(path);
  if (!(file.is_open())) {
    SDL_LOG_WARN("failed to create file: " << path);
    return false;
  }
  file.close();
  return true;
}

time_t file_system::GetFileModificationTime(const std::string& path) {
  SDL_LOG_AUTO_TRACE();

  error_code ec;
  std::time_t time = fs::last_write_time(path, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO("Unable to get file modification time: "
                             << path << " reason " << ec.message());

    return 0;
  }
  return time;
}

bool file_system::CopyFile(const std::string& src, const std::string& dst) {
  SDL_LOG_AUTO_TRACE();
  if (!FileExists(src) || FileExists(dst) || !CreateFile(dst)) {
    SDL_LOG_WARN("Failed to copy file from: '" << src << "', to: '" << dst
                                               << "'");
    return false;
  }
  error_code ec;
  fs::copy_file(src, dst, ec);
  if (ec) {
    SDL_LOG_ERROR_WITH_ERRNO(

        "Unable to copy file: '" << src << "', reason: " << ec.message());
    // something failed
    return false;
  }
  return true;
}

bool file_system::MoveFile(const std::string& src, const std::string& dst) {
  SDL_LOG_AUTO_TRACE();

  if (std::rename(src.c_str(), dst.c_str()) == 0) {
    return true;
  }
  // In case of src and dst on different file systems std::rename returns
  // an error (at least on QNX).
  // Instead, copy the file over and delete the old one
  bool success = CopyFile(src, dst);
  if (!success) {
    SDL_LOG_ERROR("Failed to copy file from: '" << src << "', to: '" << dst
                                                << "'");
    return false;
  }
  success = DeleteFile(src);
  if (!success) {
    SDL_LOG_ERROR("Failed to delete file '" << src << "'");
    return false;
  }
  return true;
}

std::string file_system::GetFileName(const std::string& full_path) {
  fs::path p(full_path);
  return p.filename().string();
}
