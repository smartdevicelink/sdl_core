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

#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif

#ifdef OS_WIN32 
#include <sstream>
#include <Windows.h>
#else
#ifdef OS_ANDROID
#include <sys/statfs.h>
#include <sys/stat.h>
#include <sstream>
#else
#include <sys/statvfs.h>
#include <sys/stat.h>
#endif

#include <sys/types.h>
#include <sstream>

#include <dirent.h>
#endif

#include <unistd.h>


#include "utils/file_system.h"
#include "utils/logger.h"

// TODO(VS): lint error: Streams are highly discouraged.
#include <fstream>
#include <cstddef>
#include <algorithm>
#ifdef OS_WINCE
#include "utils/global.h"
#endif
CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

uint64_t file_system::GetAvailableDiskSpace(const std::string& path) {
#ifdef OS_WIN32
	return 1024 * 1024 * 1024;
#else

#ifdef OS_ANDROID
  struct statfs fsInfo;//statvfs
#else
	struct statvfs fsInfo;
#endif
  memset(reinterpret_cast<void*>(&fsInfo), 0, sizeof(fsInfo));
#ifdef OS_ANDROID
  if (statfs(path.c_str(), &fsInfo) == 0) {
#else
  if (statvfs(path.c_str(), &fsInfo) == 0) {
#endif
    return fsInfo.f_bsize * fsInfo.f_bfree;
  } else {
    return 0;
  }
#endif
}

uint32_t file_system::FileSize(const std::string &path) {
#ifdef OS_WIN32
	return 1024 * 1024;
#else
  if (file_system::FileExists(path)) {
    struct stat file_info;
    memset(reinterpret_cast<void*>(&file_info), 0, sizeof(file_info));
    stat(path.c_str(), &file_info);
    return file_info.st_size;
  }
  return 0;
#endif
}

uint32_t file_system::DirectorySize(const std::string& path) {
#ifdef OS_WIN32
	return 1024 * 1024;
#else
  uint32_t size = 0;
  int32_t return_code = 0;
  DIR* directory = NULL;

#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer =
      new char[offsetof(struct dirent, d_name) +
               pathconf(path.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new(direntbuffer) dirent;
#endif
  struct dirent* result = NULL;
  struct stat file_info;
  directory = opendir(path.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);
    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if (0 == strcmp(result->d_name, "..")
          || 0 == strcmp(result->d_name, ".")) {
        continue;
      }
      std::string full_element_path = path + "/" + result->d_name;
      if (file_system::IsDirectory(full_element_path)) {
        size += DirectorySize(full_element_path);
      } else {
        memset(reinterpret_cast<void*>(&file_info), 0, sizeof(file_info));
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
#endif
}

std::string file_system::CreateDirectory(const std::string& name) {
	if (!DirectoryExists(name)) {
#ifdef OS_WIN32
#ifdef OS_WINCE
		wchar_string strUnicodeData;
		Global::toUnicode(name, CP_ACP, strUnicodeData);
		::CreateDirectory(strUnicodeData.c_str(), NULL);
#else
		::CreateDirectory(name.c_str(), NULL);
#endif
#else
		mkdir(name.c_str(), S_IRWXU);
#endif
	}
	return name;

}

bool file_system::CreateDirectoryRecursively(const std::string& path) {
  size_t pos = 0;
  bool ret_val = true;

  while (ret_val == true && pos <= path.length()) {
    pos = path.find('/', pos + 1);
    if (!DirectoryExists(path.substr(0, pos))) {
#ifdef OS_WIN32
#ifdef OS_WINCE
		wchar_string strUnicodeData;
		Global::toUnicode(path.substr(0, pos), CP_ACP, strUnicodeData);
	  if (0 != ::CreateDirectory(strUnicodeData.c_str(), NULL)) {
#else
      if (0 != ::CreateDirectory(path.substr(0, pos).c_str(), NULL)) {
#endif
#else
      if (0 != mkdir(path.substr(0, pos).c_str(), S_IRWXU)) {
#endif
        ret_val = false;
      }
    }
  }

  return ret_val;
}

bool file_system::IsDirectory(const std::string& name) {
#ifdef OS_WIN32
	//bool b = ::SetCurrentDirectory(name.c_str()) == TRUE ? true : false;
	//return b;
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(name, CP_ACP, strUnicodeData);
	int fileAttri = GetFileAttributes(strUnicodeData.c_str());
#else
	int fileAttri = GetFileAttributes(name.c_str());
#endif
	if(fileAttri != -1){
		if (fileAttri & FILE_ATTRIBUTE_DIRECTORY){
			return true;
		}	
	}
	return false;
#else
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }

  return S_ISDIR(status.st_mode);
#endif
}

bool file_system::DirectoryExists(const std::string& name) {
#ifdef OS_WIN32
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(name, CP_ACP, strUnicodeData);
	int fileAttri = ::GetFileAttributes(strUnicodeData.c_str());
#else
	int fileAttri = ::GetFileAttributes(name.c_str());
#endif
	if(fileAttri == -1){
		return false;
	}
	return true;
#else
  struct stat status;
  memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status) || !S_ISDIR(status.st_mode)) {
    return false;
  }

  return true;
#endif
}

bool file_system::FileExists(const std::string& name) {
#ifdef OS_WIN32
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(name, CP_ACP, strUnicodeData);
	HANDLE file = ::CreateFile(strUnicodeData.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	HANDLE file = ::CreateFile(name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
	bool b = !(file == (HANDLE)-1);
	if (b)
		::CloseHandle((HANDLE)file);
	return b;
#else
	struct stat status;
	memset(&status, 0, sizeof(status));

  if (-1 == stat(name.c_str(), &status)) {
    return false;
  }
  return true;
#endif
}

bool file_system::Write(
  const std::string& file_name, const std::vector<uint8_t>& data,
  std::ios_base::openmode mode) {
  std::ofstream file(file_name.c_str(), std::ios_base::binary | mode);
  if (file.is_open()) {
    for (int32_t i = 0; i < data.size(); ++i) {
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
  file->open( file_name.c_str(),std::ios_base::binary | mode);
  if (file->is_open()) {

    return file;
  }

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
#ifdef OS_WIN32
#ifdef OS_WINCE
	wchar_t szPath[MAX_PATH];
	::GetModuleFileName( NULL, szPath, MAX_PATH );
	wchar_t *lpszPath = wcsrchr(szPath, '\\');
	*lpszPath = 0;
	std::string strData;
	Global::fromUnicode(szPath, CP_ACP, strData);
	const char *szPre = strData.c_str();
#else
	char szPre[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szPre);
#endif

	char path[MAX_PATH];
	memset(path, 0, MAX_PATH);
	sprintf_s(path, MAX_PATH - 1, "%s", szPre);
#else
  size_t filename_max_lenght = 1024;
  char currentAppPath[filename_max_lenght];
  memset(currentAppPath, 0, filename_max_lenght);
  getcwd(currentAppPath, filename_max_lenght);

  char path[filename_max_lenght];
  memset(path, 0, filename_max_lenght);
  snprintf(path, filename_max_lenght - 1, "%s", currentAppPath);
#endif
  return std::string(path);
}

bool file_system::DeleteFile(const std::string& name) {
#ifdef OS_WIN32
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(name, CP_ACP, strUnicodeData);
	return ::DeleteFile(strUnicodeData.c_str()) == TRUE ? true : false;
#else
	return ::DeleteFile(name.c_str()) == TRUE ? true : false;
#endif
#else
	if (FileExists(name) && IsAccessible(name, W_OK)) {
		return !remove(name.c_str());
	}
	return false;
#endif
}

void file_system::remove_directory_content(const std::string& directory_name) {
#ifdef OS_WIN32
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(directory_name, CP_ACP, strUnicodeData);
	::RemoveDirectory(strUnicodeData.c_str());
	::CreateDirectory(strUnicodeData.c_str(), NULL);
#else
	::RemoveDirectory(directory_name.c_str());
	::CreateDirectory(directory_name.c_str(), NULL);
#endif
#else
  int32_t return_code = 0;
  DIR* directory = NULL;
#ifndef __QNXNTO__
  struct dirent dir_element_;
  struct dirent* dir_element = &dir_element_;
#else
  char* direntbuffer =
      new char[offsetof(struct dirent, d_name) +
               pathconf(directory_name.c_str(), _PC_NAME_MAX) + 1];
  struct dirent* dir_element = new(direntbuffer) dirent;
#endif
  struct dirent* result = NULL;

  directory = opendir(directory_name.c_str());

  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if (0 == strcmp(result->d_name, "..")
          || 0 == strcmp(result->d_name, ".")) {
        continue;
      }

      std::string full_element_path = directory_name + "/" + result->d_name;

      if (file_system::IsDirectory(full_element_path)) {
        remove_directory_content(full_element_path);
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
#endif
}

bool file_system::RemoveDirectory(const std::string& directory_name,
                                  bool is_recursively) {
#ifdef OS_WIN32
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(directory_name, CP_ACP, strUnicodeData);
	return ::RemoveDirectory(strUnicodeData.c_str()) == TRUE ? true : false;
#else
	return ::RemoveDirectory(directory_name.c_str()) == TRUE ? true : false;
#endif
#else
	if (DirectoryExists(directory_name)
		&& IsAccessible(directory_name, W_OK)) {
		if (is_recursively) {
			remove_directory_content(directory_name);
		}

    return !rmdir(directory_name.c_str());
  }
  return false;
#endif
}

bool file_system::IsAccessible(const std::string& name, int32_t how) {
#ifdef OS_WIN32
	return true;
#else
	return !access(name.c_str(), how);
#endif
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
#ifdef OS_WIN32
  WIN32_FIND_DATA ffd;
#ifdef OS_WINCE
	wchar_string strUnicodeData;
	Global::toUnicode(directory_name, CP_ACP, strUnicodeData);
  HANDLE hFind = ::FindFirstFile(strUnicodeData.c_str(), &ffd);
#else
  HANDLE hFind = ::FindFirstFile(directory_name.c_str(), &ffd);
#endif

  if (INVALID_HANDLE_VALUE == hFind)
  {
	  return listFiles;
  }

  // List all the files in the directory with some info about them.

  do
  {
	  if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	  {
	  }
	  else
	  {
#ifdef OS_WINCE
		  std::string strData;
		  Global::fromUnicode(ffd.cFileName, CP_ACP, strData);
		  listFiles.push_back(strData.c_str());
#else
		  listFiles.push_back(ffd.cFileName);
#endif
	  }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
#else
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
  struct dirent* dir_element = new(direntbuffer) dirent;
#endif
  struct dirent* result = NULL;

  directory = opendir(directory_name.c_str());
  if (NULL != directory) {
    return_code = readdir_r(directory, dir_element, &result);

    for (; NULL != result && 0 == return_code;
         return_code = readdir_r(directory, dir_element, &result)) {
      if (0 == strcmp(result->d_name, "..")
          || 0 == strcmp(result->d_name, ".")) {
        continue;
      }

      listFiles.push_back(std::string(result->d_name));
    }

    closedir(directory);
#ifdef __QNXNTO__
    delete[] direntbuffer;
#endif
  }
#endif
  return listFiles;
}

bool file_system::WriteBinaryFile(const std::string& name,
                                  const std::vector<uint8_t>& contents) {
  using namespace std;
  ofstream output(name.c_str(), ios_base::binary|ios_base::trunc);
  output.write(reinterpret_cast<const char*>(&contents.front()),
               contents.size());
  return output.good();
}

bool file_system::ReadBinaryFile(const std::string& name,
                                 std::vector<uint8_t>& result) {
#ifdef OS_WIN32
	if (!FileExists(name) || !IsAccessible(name, 0)) {
#else
	if (!FileExists(name) || !IsAccessible(name, R_OK)) {
#endif
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
#ifdef OS_WIN32
	if (!FileExists(name) || !IsAccessible(name, 0)) {
#else
	if (!FileExists(name) || !IsAccessible(name, R_OK)) {
#endif
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
  std::string::const_iterator it_sym = reserved_symbols.begin();
  std::string::const_iterator it_sym_end = reserved_symbols.end();

  std::string converted_path;
  while (it_path != it_path_end) {

    it_sym = reserved_symbols.begin();
    for (; it_sym != it_sym_end; ++it_sym) {

      if (*it_path == *it_sym) {
#ifdef OS_WIN32
		  const size_t size = 100;
		  char percent_value[size];
		  sprintf_s(percent_value, size, "%%%x", *it_path);
#else
        size_t size = 100;
        char percent_value[size];
        snprintf(percent_value, size, "%%%x", *it_path);
#endif
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

bool file_system::CreateFile(const std::string& path) {
#if defined(OS_WINCE) || defined(OS_MAC)
	std::ofstream file(path.c_str());
#else
  std::ofstream file(path);
#endif
  if (!(file.is_open())) {
    return false;
  } else {
    file.close();
    return true;
  }
}


uint64_t file_system::GetFileModificationTime(const std::string& path) {
#ifdef OS_WIN32
	return 0;
#else
  struct stat info;
  stat(path.c_str(), &info);
#ifndef __QNXNTO__
  return static_cast<uint64_t>(info.st_mtim.tv_nsec);
#else
  return static_cast<uint64_t>(info.st_mtime);
#endif
#endif
}

bool file_system::CopyFile(const std::string& src,
                           const std::string& dst) {
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

bool file_system::MoveFile(const std::string& src,
                           const std::string& dst) {
  if (!CopyFile(src, dst)) {
    return false;
  }
  if (!DeleteFile(src)) {
    DeleteFile(dst);
    return false;
  }
  return true;
}
