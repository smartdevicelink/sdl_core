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

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <unistd.h>
// TODO(VS): lint error: Streams are highly discouraged.
#include <iostream>
#include <fstream>
#include <cstddef>
#include <algorithm>
#include <string>
#include <sstream>
#include "utils/file_system.h"
#include "config_profile/profile.h"

uint64_t file_system::AvailableSpace() {
  char currentAppPath[FILENAME_MAX];
  memset(reinterpret_cast<void*>(currentAppPath), 0, FILENAME_MAX);
  getcwd(currentAppPath, FILENAME_MAX - 1);

  struct statvfs fsInfo;
  memset(reinterpret_cast<void*>(&fsInfo), 0, sizeof(fsInfo));
  statvfs(currentAppPath, &fsInfo);
  return fsInfo.f_bsize * fsInfo.f_bfree;
}

unsigned int SizeDirectory(const std::string& path) {
  unsigned int size = 0;
  int return_code = 0;
  DIR* directory = NULL;
  struct dirent dir_element;
  struct dirent* result = NULL;
  struct stat file_info;
  directory = opendir(path.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, &dir_element, &result);
    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, &dir_element, &result)) {
      if (0 == strcmp(dir_element.d_name, "..")
          || 0 == strcmp(dir_element.d_name, ".")) {
        continue;
      }
      std::string full_element_path = path + "/" + dir_element.d_name;
      if (file_system::IsDirectory(full_element_path)) {
        size += SizeDirectory(full_element_path);
      } else {
        memset(reinterpret_cast<void*>(&file_info), 0, sizeof(file_info));
        stat(full_element_path.c_str(), &file_info);
        size += file_info.st_size;
      }
    }
  }
  closedir(directory);
  return size;
}

unsigned int file_system::AvailableSpaceApp(const std::string& name) {
  unsigned int available_space_app = profile::Profile::instance()
  ->space_available();
  std::string full_path;
  unsigned int size_of_directory = 0;
  uint64_t available_space = 0;
  if(DirectoryExists(name)) {
    full_path = FullPath(name);
    size_of_directory = SizeDirectory(full_path);
    if(available_space_app < size_of_directory) {
      return 0;
    }
    available_space_app -= size_of_directory;
    available_space = AvailableSpace();
    if(available_space_app > available_space) {
      return available_space;
    } else {
      return available_space_app;
    }
  } else {
    return available_space_app;
  }
}

std::string file_system::CreateDirectory(const std::string& name) {
  if (!DirectoryExists(name)) {
    mkdir(name.c_str(), S_IRWXU);
  }

  return name;
}

bool file_system::IsDirectory(const std::string& name) {
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }

  return S_ISDIR(status.st_mode);
}

bool file_system::DirectoryExists(const std::string& name) {
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status) || !S_ISDIR(status.st_mode)) {
    return false;
  }

  return true;
}

bool file_system::FileExists(const std::string& name) {
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }
  return true;
}

bool file_system::Write(
  const std::string& file_name, const std::vector<unsigned char>& data) {
  std::ofstream file(file_name.c_str(), std::ios_base::binary);
  if (file.is_open()) {
    for (int i = 0; i < data.size(); ++i) {
      file << data[i];
    }
    file.close();
    return true;
  }
  return false;
}

std::string file_system::FullPath(const std::string& file) {
  char currentAppPath[FILENAME_MAX];
  memset(currentAppPath, 0, FILENAME_MAX);
  getcwd(currentAppPath, FILENAME_MAX);

  char path[FILENAME_MAX];
  memset(path, 0, FILENAME_MAX);
  snprintf(path, FILENAME_MAX - 1, "%s/%s", currentAppPath, file.c_str());
  return std::string(path);
}

bool file_system::DeleteFile(const std::string& name) {
  if (FileExists(name) && IsAccessible(name, W_OK)) {
    return !remove(name.c_str());
  }
  return false;
}

void remove_directory_content(const std::string& directory_name) {
  int return_code = 0;
  DIR* directory = NULL;
  struct dirent dir_element;
  struct dirent* result = NULL;

  directory = opendir(directory_name.c_str());

  if (NULL != directory) {
    return_code = readdir_r(directory, &dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, &dir_element, &result)) {
      if (0 == strcmp(dir_element.d_name, "..")
          || 0 == strcmp(dir_element.d_name, ".")) {
        continue;
      }

      std::string full_element_path = directory_name + "/" + dir_element.d_name;

      if (file_system::IsDirectory(full_element_path)) {
        remove_directory_content(full_element_path);
        rmdir(full_element_path.c_str());
      } else {
        remove(full_element_path.c_str());
      }
    }
  }

  closedir(directory);
}

bool file_system::RemoveDirectory(const std::string& directory_name,
                                  bool is_recursively) {
  if (DirectoryExists(directory_name)
      && IsAccessible(directory_name, W_OK)) {
    if (is_recursively) {
      remove_directory_content(directory_name);
    }

    return !rmdir(directory_name.c_str());
  }
  return false;
}

bool file_system::IsAccessible(const std::string& name, int how) {
  return !access(name.c_str(), how);
}

std::vector<std::string> file_system::ListFiles(
  const std::string& directory_name) {
  std::vector<std::string> listFiles;
  if (!DirectoryExists(directory_name)) {
    return listFiles;
  }

  int return_code = 0;
  DIR* directory = NULL;
  struct dirent dir_element;
  struct dirent* result = NULL;

  directory = opendir(directory_name.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, &dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, &dir_element, &result)) {
      if (0 == strcmp(dir_element.d_name, "..")
          || 0 == strcmp(dir_element.d_name, ".")) {
        continue;
      }

      listFiles.push_back(std::string(dir_element.d_name));
    }

    closedir(directory);
  }

  return listFiles;
}

bool file_system::ReadBinaryFile(const std::string& name,
                                 std::vector<unsigned char>& result) {
  if (!FileExists(name) || !IsAccessible(name, R_OK)) {
    return false;
  }

  std::ifstream file(name.c_str(), std::ios_base::binary);
  std::ostringstream ss;
  ss << file.rdbuf();
  const std::string& s = ss.str();

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
