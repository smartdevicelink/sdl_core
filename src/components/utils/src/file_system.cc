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

#include <iostream>
#include <fstream>
#include <cstddef>
#include <algorithm>
#include <string>
#include <sstream>
#include "utils/file_system.h"

uint64_t file_system::available_space() {
  char currentAppPath[FILENAME_MAX];
  memset(reinterpret_cast<void*>(currentAppPath), 0, FILENAME_MAX);
  getcwd(currentAppPath, FILENAME_MAX - 1);

  struct statvfs fsInfo;
  memset(reinterpret_cast<void*>(&fsInfo), 0, sizeof(fsInfo));
  statvfs(currentAppPath, &fsInfo);
  return fsInfo.f_bsize * fsInfo.f_bfree;
}

std::string file_system::create_directory(const std::string & name) {
  if (!is_directory_exists(name)) {
    mkdir(name.c_str(), S_IRWXU);
  }

  return name;
}

bool file_system::is_directory(const std::string & name) {
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }

  return S_ISDIR(status.st_mode);
}

bool file_system::is_directory_exists(const std::string & name) {
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status) || !S_ISDIR(status.st_mode)) {
    return false;
  }

  return true;
}

bool file_system::is_file_exist(const std::string & name) {
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }
  return true;
}

bool file_system::write(
    const std::string & file_name, const std::vector<unsigned char>& data) {
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

std::string file_system::full_path(const std::string & file) {
  char currentAppPath[FILENAME_MAX];
  memset(currentAppPath, 0, FILENAME_MAX);
  getcwd(currentAppPath, FILENAME_MAX);

  char path[FILENAME_MAX];
  memset(path, 0, FILENAME_MAX);
  snprintf(path, FILENAME_MAX - 1, "%s/%s", currentAppPath, file.c_str());
  return std::string(path);
}

bool file_system::delete_file(const std::string & name) {
  if (is_file_exist(name)) {
    return !remove(name.c_str());
  }
  return false;
}

void remove_directory_content(const std::string& directory_name) {
  int return_code;
  DIR * directory = NULL;
  struct dirent dirElement;
  struct dirent *result = NULL;

  directory = opendir(directory_name.c_str());

  if (NULL != directory) {
    return_code = readdir_r(directory, &dirElement, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, &dirElement, &result)) {
      if (0 == strcmp(dirElement.d_name, "..")
          || 0 == strcmp(dirElement.d_name, ".")) {
        continue;
      }

      std::string full_element_path = directory_name + "/" + dirElement.d_name;

      if (file_system::is_directory(full_element_path)) {
        remove_directory_content(full_element_path);
        rmdir(full_element_path.c_str());
      } else {
        remove(full_element_path.c_str());
      }
    }
  }

  closedir(directory);
}

bool file_system::remove_directory(const std::string& directory_name,
                                   bool is_recursively) {
  if (is_directory_exists(directory_name)
      && has_access(directory_name, W_OK)) {
    if (is_recursively) {
      remove_directory_content(directory_name);
    }

    return !rmdir(directory_name.c_str());
  }
  return false;
}

bool file_system::has_access(const std::string& name, int how) {
  return !access(name.c_str(), how);
}

std::vector<std::string> file_system::list_files(
    const std::string & directory_name) {
  std::vector<std::string> listFiles;
  if (!is_directory_exists(directory_name)) {
    return listFiles;
  }

  int return_code;
  DIR * directory = NULL;
  struct dirent dirElement;
  struct dirent *result = NULL;

  directory = opendir(directory_name.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, &dirElement, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, &dirElement, &result)) {
      if (0 == strcmp(dirElement.d_name, "..")
          || 0 == strcmp(dirElement.d_name, ".")) {
        continue;
      }

      listFiles.push_back(std::string(dirElement.d_name));
    }

    closedir(directory);
  }

  return listFiles;
}

bool file_system::read_binary_file(const std::string& name,
                                   std::vector<unsigned char>& result) {
  if (!is_file_exist(name))
    return false;

  std::ifstream file(name.c_str(), std::ios_base::binary);
  std::ostringstream ss;
  ss << file.rdbuf();
  const std::string& s = ss.str();

  result.resize(s.length());
  std::copy(s.begin(), s.end(), result.begin());
  return true;
}
