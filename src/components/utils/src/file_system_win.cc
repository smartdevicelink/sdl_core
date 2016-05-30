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
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <io.h>
// TODO(VS): lint error: Streams are highly discouraged.
#include <sstream>
#include <fstream>
#include <cstddef>
#include <algorithm>

#include "utils/winhdr.h"
#include "utils/file_system.h"
#include "utils/string_utils.h"
#include "utils/date_time.h"

namespace {

/**
  * Path prefix required by OS Windows to allow
  * processing file names longer than MAX_PATH (260) characters
  */
const std::string kPlatformPathPrefix = "\\\\?\\";

/**
  * @brief Converts UTF-8 string to wide string
  * @param str String to be converted
  * @return Result wide string
  */
std::wstring ConvertUTF8ToWString(const std::string& utf8_str) {
  if (utf8_str.empty()) {
    return std::wstring();
  }
  std::string extended_utf8_str(utf8_str);
  if (!file_system::IsRelativePath(utf8_str)) {
    extended_utf8_str = kPlatformPathPrefix + extended_utf8_str;
  }
  int size = MultiByteToWideChar(CP_UTF8,
                                 0,
                                 &extended_utf8_str[0],
                                 static_cast<int>(extended_utf8_str.size()),
                                 NULL,
                                 0);
  std::wstring wide_str(size, 0);
  MultiByteToWideChar(CP_UTF8,
                      0,
                      &extended_utf8_str[0],
                      static_cast<int>(extended_utf8_str.size()),
                      &wide_str[0],
                      size);
  return wide_str;
}

/**
  * @brief Converts wide string to UTF-8 string
  * @param str String to be converted
  * @return Result UTF-8 string
  */
std::string ConvertWStringToUTF8(const std::wstring& wide_str) {
  if (wide_str.empty()) {
    return std::string();
  }
  int size = WideCharToMultiByte(CP_UTF8,
                                 0,
                                 &wide_str[0],
                                 static_cast<int>(wide_str.size()),
                                 NULL,
                                 0,
                                 NULL,
                                 NULL);
  std::string utf8_str(size, 0);
  WideCharToMultiByte(CP_UTF8,
                      0,
                      &wide_str[0],
                      static_cast<int>(wide_str.size()),
                      &utf8_str[0],
                      size,
                      NULL,
                      NULL);
  return utf8_str;
}

}  // namespace

file_system::FileSizeType file_system::GetAvailableDiskSpace(
    const std::string& utf8_path) {
  DWORD sectors_per_cluster;
  DWORD bytes_per_sector;
  DWORD number_of_free_clusters;

  const BOOL res = GetDiskFreeSpaceW(ConvertUTF8ToWString(utf8_path).c_str(),
                                     &sectors_per_cluster,
                                     &bytes_per_sector,
                                     &number_of_free_clusters,
                                     NULL);
  if (0 == res) {
    return 0u;
  }
  return number_of_free_clusters * sectors_per_cluster * bytes_per_sector;
}

file_system::FileSizeType file_system::FileSize(const std::string& utf8_path) {
  WIN32_FIND_DATAW ffd;
  HANDLE find = FindFirstFileW(ConvertUTF8ToWString(utf8_path).c_str(), &ffd);
  if (INVALID_HANDLE_VALUE == find) {
    return 0u;
  }

  FileSizeType file_size =
      (ffd.nFileSizeHigh * (MAXDWORD + 1)) + ffd.nFileSizeLow;
  FindClose(find);
  return file_size;
}

file_system::FileSizeType file_system::DirectorySize(
    const std::string& utf8_path) {
  FileSizeType size = 0u;
  if (!DirectoryExists(utf8_path)) {
    return size;
  }

  const std::string find_string = ConcatPath(utf8_path, "*");
  WIN32_FIND_DATAW ffd;

  HANDLE find = FindFirstFileW(ConvertUTF8ToWString(find_string).c_str(), &ffd);
  if (INVALID_HANDLE_VALUE == find) {
    return size;
  }

  do {
    if (FILE_ATTRIBUTE_DIRECTORY == ffd.dwFileAttributes) {
      const std::string utf8_file_name = ConvertWStringToUTF8(ffd.cFileName);
      if (utf8_file_name.compare(kCurrentDirectoryEntry) != 0 &&
          utf8_file_name.compare(kParentDirectoryEntry) != 0) {
        size += DirectorySize(utf8_file_name);
      }
    } else {
      size += (ffd.nFileSizeHigh * (MAXDWORD + 1)) + ffd.nFileSizeLow;
    }
  } while (FindNextFileW(find, &ffd) != 0);

  FindClose(find);
  return size;
}

bool file_system::CreateDirectory(const std::string& utf8_path) {
  return DirectoryExists(utf8_path) ||
      0 == _wmkdir(ConvertUTF8ToWString(utf8_path).c_str());
}

bool file_system::CreateDirectoryRecursively(const std::string& utf8_path) {
  size_t pos = IsRelativePath(utf8_path) ? 0 : utf8_path.find_first_of(":") + 2;
  while (pos < utf8_path.length()) {
    pos = std::min(utf8_path.find_first_of("/", pos),
        utf8_path.find_first_of("\\", pos));
    if (!CreateDirectory(utf8_path.substr(0, pos))) {
      return false;
    }
    pos = std::string::npos == pos ? pos : pos + 1;
  }
  return true;
}

bool file_system::DirectoryExists(const std::string& utf8_path) {
  const DWORD attrib =
      GetFileAttributesW(ConvertUTF8ToWString(utf8_path).c_str());
  return (attrib != INVALID_FILE_ATTRIBUTES &&
          (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool file_system::FileExists(const std::string& utf8_path) {
  const DWORD attrib =
      GetFileAttributesW(ConvertUTF8ToWString(utf8_path).c_str());
  return (attrib != INVALID_FILE_ATTRIBUTES &&
          !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool file_system::Write(const std::string& utf8_path,
                        const std::vector<uint8_t>& data,
                        std::ios_base::openmode mode) {
  if (data.empty()) {
    return false;
  }
  std::ofstream file(ConvertUTF8ToWString(utf8_path),
                     std::ios_base::binary | mode);
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
  file->open(ConvertUTF8ToWString(utf8_path), std::ios_base::binary | mode);
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
  _getcwd(path, filename_max_length);
  return std::string(path);
}

bool file_system::DeleteFile(const std::string& utf8_path) {
  if (FileExists(utf8_path) && IsWritingAllowed(utf8_path)) {
    return 0 == _wremove(ConvertUTF8ToWString(utf8_path).c_str());
  }
  return false;
}

void file_system::RemoveDirectoryContent(const std::string& utf8_path) {
  if (!DirectoryExists(utf8_path)) {
    return;
  }

  const std::string find_string = ConcatPath(utf8_path, "*");
  WIN32_FIND_DATAW ffd;

  HANDLE find = FindFirstFileW(ConvertUTF8ToWString(find_string).c_str(), &ffd);
  if (INVALID_HANDLE_VALUE == find) {
    return;
  }

  do {
    const std::string utf8_file_name = ConvertWStringToUTF8(ffd.cFileName);
    if (FILE_ATTRIBUTE_DIRECTORY == ffd.dwFileAttributes) {
      if (utf8_file_name.compare(kCurrentDirectoryEntry) != 0 &&
          utf8_file_name.compare(kParentDirectoryEntry) != 0) {
        RemoveDirectory(ConcatPath(utf8_path, utf8_file_name), true);
      }
    } else {
      DeleteFile(ConcatPath(utf8_path, utf8_file_name));
    }
  } while (FindNextFileW(find, &ffd) != 0);

  FindClose(find);
}

bool file_system::RemoveDirectory(const std::string& utf8_path,
                                  bool is_recursively) {
  if (DirectoryExists(utf8_path) && IsWritingAllowed(utf8_path)) {
    if (is_recursively) {
      RemoveDirectoryContent(utf8_path);
    }
    return 0 == _wrmdir(ConvertUTF8ToWString(utf8_path).c_str());
  }
  return false;
}

bool file_system::IsAccessible(const std::string& utf8_path,
                               int32_t access_rights) {
  BOOL result = FALSE;

  DWORD length = 0;
  if (0 == GetFileSecurityW(ConvertUTF8ToWString(utf8_path).c_str(),
                            OWNER_SECURITY_INFORMATION |
                                GROUP_SECURITY_INFORMATION |
                                DACL_SECURITY_INFORMATION,
                            NULL,
                            NULL,
                            &length) &&
      ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
    uint8_t* security_descriptor = new uint8_t[length];
    if (0 != GetFileSecurityW(
                 ConvertUTF8ToWString(utf8_path).c_str(),
                 OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                     DACL_SECURITY_INFORMATION,
                 static_cast<PSECURITY_DESCRIPTOR>(security_descriptor),
                 length,
                 &length)) {
      HANDLE process_token = NULL;
      if (OpenProcessToken(GetCurrentProcess(),
                           TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE |
                               STANDARD_RIGHTS_READ,
                           &process_token)) {
        HANDLE impersonated_token = NULL;
        if (DuplicateToken(
                process_token, SecurityImpersonation, &impersonated_token)) {
          PRIVILEGE_SET privileges = {0};
          DWORD privileges_length = sizeof(privileges);
          DWORD granted_access = 0;

          GENERIC_MAPPING mapping = {0xFFFFFFFF};
          mapping.GenericRead = FILE_GENERIC_READ;
          mapping.GenericWrite = FILE_GENERIC_WRITE;
          mapping.GenericExecute = FILE_GENERIC_EXECUTE;
          mapping.GenericAll = FILE_ALL_ACCESS;
          MapGenericMask(reinterpret_cast<PDWORD>(&access_rights), &mapping);

          AccessCheck(security_descriptor,
                      impersonated_token,
                      access_rights,
                      &mapping,
                      &privileges,
                      &privileges_length,
                      &granted_access,
                      &result);
          CloseHandle(impersonated_token);
        }
        CloseHandle(process_token);
      }
    }
    delete security_descriptor;
  }

  return TRUE == result;
}

bool file_system::IsWritingAllowed(const std::string& utf8_path) {
  return IsAccessible(utf8_path, GENERIC_WRITE);
}

bool file_system::IsReadingAllowed(const std::string& utf8_path) {
  return IsAccessible(utf8_path, GENERIC_READ);
}

std::vector<std::string> file_system::ListFiles(const std::string& utf8_path) {
  std::vector<std::string> list_files;
  if (!DirectoryExists(utf8_path)) {
    return list_files;
  }

  const std::string find_string = ConcatPath(utf8_path, "*");
  WIN32_FIND_DATAW ffd;

  HANDLE find = FindFirstFileW(ConvertUTF8ToWString(find_string).c_str(), &ffd);
  if (INVALID_HANDLE_VALUE == find) {
    return list_files;
  }

  do {
    const std::string utf8_file_name = ConvertWStringToUTF8(ffd.cFileName);
    if (utf8_file_name.compare(kCurrentDirectoryEntry) != 0 &&
        utf8_file_name.compare(kParentDirectoryEntry) != 0) {
      list_files.push_back(ConvertWStringToUTF8(ffd.cFileName));
    }
  } while (FindNextFileW(find, &ffd) != 0);

  FindClose(find);
  return list_files;
}

bool file_system::WriteBinaryFile(const std::string& utf8_path,
                                  const std::vector<uint8_t>& data) {
  if (data.empty()) {
    return false;
  }
  std::ofstream output(ConvertUTF8ToWString(utf8_path),
                       std::ios_base::binary | std::ios_base::trunc);
  output.write(reinterpret_cast<const char*>(&data[0]), data.size());
  return output.good();
}

bool file_system::ReadBinaryFile(const std::string& utf8_path,
                                 std::vector<uint8_t>& result) {
  if (!FileExists(utf8_path) || !IsReadingAllowed(utf8_path)) {
    return false;
  }

  std::ifstream file(ConvertUTF8ToWString(utf8_path), std::ios_base::binary);
  std::ostringstream ss;
  ss << file.rdbuf();
  const std::string& s = ss.str();

  result.resize(s.length());
  std::copy(s.begin(), s.end(), result.begin());
  return true;
}

bool file_system::ReadFile(const std::string& utf8_path, std::string& result) {
  if (!FileExists(utf8_path) || !IsReadingAllowed(utf8_path)) {
    return false;
  }

  std::ifstream file(ConvertUTF8ToWString(utf8_path));
  std::ostringstream ss;
  ss << file.rdbuf();
  result = ss.str();
  return true;
}

const std::string file_system::ConvertPathForURL(const std::string& utf8_path) {
  std::string::const_iterator it_path = utf8_path.begin();
  std::string::const_iterator it_path_end = utf8_path.end();

  // list of characters to be encoded from the link:
  // http://www.blooberry.com/indexdot/html/topics/urlencoding.htm
  const std::string reserved_symbols = "$+,<>%{}|\^~[]` ";
  std::string::const_iterator it_sym = reserved_symbols.begin();
  std::string::const_iterator it_sym_end = reserved_symbols.end();

  std::string converted_path;
  while (it_path != it_path_end) {
    it_sym = reserved_symbols.begin();
    for (; it_sym != it_sym_end; ++it_sym) {
      if (*it_path == *it_sym) {
        const std::size_t size = 100;
        char percent_value[size];
        _snprintf_s(percent_value, size, "%%%x", *it_path);
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
  std::ofstream file(ConvertUTF8ToWString(utf8_path));
  if (!(file.is_open())) {
    return false;
  } else {
    file.close();
    return true;
  }
}

uint64_t file_system::GetFileModificationTime(const std::string& utf8_path) {
  WIN32_FIND_DATAW ffd;
  HANDLE find = FindFirstFileW(ConvertUTF8ToWString(utf8_path).c_str(), &ffd);
  if (INVALID_HANDLE_VALUE == find) {
    return 0;
  }

  uint64_t modification_time = 0;
  modification_time |= ffd.ftLastWriteTime.dwHighDateTime;
  modification_time <<= 32;
  modification_time |= ffd.ftLastWriteTime.dwLowDateTime;

  // Convert to microseconds by dividing by 10
  modification_time /= 10;
  modification_time -= date_time::kDeltaEpochInMicrosecs;
  modification_time /= date_time::kMicrosecondsInSecond;

  FindClose(find);
  return modification_time;
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
  return std::string::npos == utf8_path.find(":");
}

std::string file_system::GetPathDelimiter() {
  return "\\";
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
