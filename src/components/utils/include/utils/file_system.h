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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_

#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

namespace file_system {

typedef uint64_t FileSizeType;

const std::string kCurrentDirectoryEntry = ".";
const std::string kParentDirectoryEntry = "..";

/**
 * @brief Get available disc space.
 * @param utf8_path path to directory
 * @return free disc space.
 */
FileSizeType GetAvailableDiskSpace(const std::string& utf8_path);

/*
 * @brief Get size of current directory
 * @param utf8_path path to directory
 */
FileSizeType DirectorySize(const std::string& utf8_path);

/*
 * @brief Get size of current file
 * @param utf8_path path to file
 * @return size of file, return 0 if file not exist
 */
FileSizeType FileSize(const std::string& utf8_path);

/**
 * @brief Creates directory
 * @param utf8_path path to directory
 * @return path to created directory.
 */
std::string CreateDirectory(const std::string& utf8_path);

/**
 * @brief Creates directory recursively
 * @param utf8_path full path to directory
 * @return return true if directory was created or already exist
 */
bool CreateDirectoryRecursively(const std::string& utf8_path);

/**
  * @brief Is directory exist
  * @param utf8_path path to directory
  * @return returns true if directory is exists.
  */
bool DirectoryExists(const std::string& utf8_path);

/**
  * @brief Is file exist
  * @param utf8_path path to file
  * @return returns true if file is exists.
  */
bool FileExists(const std::string& utf8_path);

/**
  * @brief Writes to file
  *
  * @remark - create file if it doesn't exist
  * @param utf8_path path to file
  * @param data data to write
  * @return returns true if the operation is successfully.
  */
bool Write(const std::string& utf8_path,
           const std::vector<uint8_t>& data,
           std::ios_base::openmode mode = std::ios_base::out);

/**
  * @brief Opens file stream for writing
  * @param utf8_path path to file to write data to
  * @return returns pointer to opened stream in case of success;
  * otherwise returns NULL
  */
std::ofstream* Open(const std::string& utf8_path,
                    std::ios_base::openmode mode = std::ios_base::out);

/**
  * @brief Writes to file stream
  * @param file_stream file stream to be written to
  * @param data data to be written to file
  * @param data_size size of data to be written to file
  * @return returns true if the operation is successfully.
  */
bool Write(std::ofstream* const file_stream,
           const uint8_t* data,
           std::size_t data_size);

/**
  * @brief Closes file stream
  * @param file_stream file stream to be closed
  */
void Close(std::ofstream* file_stream);

/**
  * @brief Returns current working directory path
  * @return Current working directory path
  */
std::string CurrentWorkingDirectory();

/**
  * @brief Removes file
  *
  * @param utf8_path path to file
  * @return returns true if the file is successfully deleted.
  */
bool DeleteFile(const std::string& utf8_path);

/**
 * @brief Removes directory.
 *
 * @param utf8_path path to directory.
 * @param is_recursively true if you need delete directory recursively,
 * otherwise false.
 * @return returns true if the directory is successfully deleted.
 */
bool RemoveDirectory(const std::string& utf8_path, bool is_recursively);

/**
  * @brief Checks access rights for file or directory
  *
  * @param utf8_path Path to file or directory
  * @param access_rights Access rights to be checked
  * @return True if file or directory has requested rights, false otherwise
  */
bool IsAccessible(const std::string& utf8_path, int32_t access_rights);

/**
  * @brief Check access rights for writing
  *
  * @param utf8_path path to file or folder
  * @return returns true if has access rights.
  */
bool IsWritingAllowed(const std::string& utf8_path);

/**
  * @brief Check access rights for reading
  *
  * @param utf8_path path to file.
  * @return returns true if file has access rights.
  */
bool IsReadingAllowed(const std::string& utf8_path);

/**
  * @brief Lists all files in given directory
  *
  * @param utf8_path path to directory.
  * @return returns list of files.
  */
std::vector<std::string> ListFiles(const std::string& utf8_path);

/**
 * @brief Creates or overwrites file with given binary contents
 * @param utf8_path path to the file
 * @param data data to be written into the file
 * @returns true if file write succeeded
 */
bool WriteBinaryFile(const std::string& utf8_path,
                     const std::vector<uint8_t>& data);

/**
  * @brief Reads from file
  *
  * @param utf8_path path to file
  * @param result read data
  * @return returns true if the operation is successfully.
  */
bool ReadBinaryFile(const std::string& utf8_path, std::vector<uint8_t>& result);

bool ReadFile(const std::string& utf8_path, std::string& result);

/**
  * @brief Convert special symbols in system path to percent-encoded
  *
  * @param utf8_path path to file
  * @return returns converted path.
*/
const std::string ConvertPathForURL(const std::string& utf8_path);

/**
  * @brief Create empty file
  *
  * @param utf8_path path to file
  * @return if result success return true
*/
bool CreateFile(const std::string& utf8_path);

/**
 * @brief Get modification time of file
 * @param utf8_path Path to file
 * @return Modification time in seconds
 */
uint64_t GetFileModificationTime(const std::string& utf8_path);

/**
  * @brief Copy file from source to destination
  *
  * @param utf8_src_path Source file path
  * @param utf8_dst_path Destination file path
  * @return if result success return true
*/
bool CopyFile(const std::string& utf8_src_path,
              const std::string& utf8_dst_path);

/**
  * @brief Move file from source to destination
  *
  * @param utf8_src_path Source file path
  * @param utf8_dst_path Destination file path
  * @return if result success return true
*/
bool MoveFile(const std::string& utf8_src_path,
              const std::string& utf8_dst_path);

/**
  * @brief Removes files and subdirectories of specified directory
  * @param utf8_path Path to directory
  */
void RemoveDirectoryContent(const std::string& utf8_path);

/**
  * @brief Checks if path is relative or not
  * @param utf8_path Path to check
  * @return True if path is relative, otherwise false
  */
bool IsRelativePath(const std::string& utf8_path);

/**
  * @brief Returns platform specific path delimiter
  * @return Delimiter string
  */
std::string GetPathDelimiter();

/**
  * @brief Concatenates strings to platform specific path
  * @param utf8_path Strings to be concatenated
  * @return Concatenated path string
  */
std::string ConcatPath(const std::string& utf8_path1,
                       const std::string& utf8_path2);
std::string ConcatPath(const std::string& utf8_path1,
                       const std::string& utf8_path2,
                       const std::string& utf8_path3);

/**
  * @brief Concatenates path to current working directory path
  * @param utf8_path Path to be concatenated
  * @return Concatenated path string
  */
std::string ConcatCurrentWorkingPath(const std::string& utf8_path);

/**
  * @brief Retrieves file name from path by
  * removing all before last path delimiter
  * @param utf8_path Path to process
  * @return File name without path
  */
std::string RetrieveFileNameFromPath(const std::string& utf8_path);

}  // namespace file_system

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
