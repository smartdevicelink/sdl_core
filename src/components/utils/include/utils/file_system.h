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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_

#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

namespace file_system {


/**
 * @brief Get available disc space.
 *
 * @param path to directory
 * @return free disc space.
 */
uint64_t GetAvailableDiskSpace(const std::string& path);

/*
 * @brief Get size of current directory
 *
 * @param path to directory
 */
size_t DirectorySize(const std::string& path);

/*
 * @brief Get size of current file
 *
 * @param path to file
 * @return size of file, return 0 if file not exist
 */
int64_t FileSize(const std::string& path);

/**
 * @brief Creates directory
 * @param name path to directory
 * @return path to created directory.
 */
std::string CreateDirectory(const std::string& name);

/**
 * @brief Creates directory recursively
 * @param path - full path to directory
 * @return return true if directory was created or already exist
 */
bool CreateDirectoryRecursively(const std::string& path);

/**
  * @brief Checks the file to see whether the file is a directory
  * @param name path to file
  * @return returns true if file is directory.
  */
bool IsDirectory(const std::string& name);

/**
  * @brief Is directory exist
  * @param name path to directory
  * @return returns true if directory is exists.
  */
bool DirectoryExists(const std::string& name);

/**
  * @brief Is file exist
  * @param name path to file
  * @return returns true if file is exists.
  */
bool FileExists(const std::string& name);

/**
  * @brief Writes to file
  *
  * @remark - create file if it doesn't exist
  * @param name path to file
  * @param data data to write
  * @return returns true if the operation is successfully.
  */
bool Write(const std::string& file_name,
           const std::vector<uint8_t>& data,
           std::ios_base::openmode mode = std::ios_base::out);

/**
  * @brief Opens file stream for writing
  * @param file_name path to file to write data to
  * @return returns pointer to opened stream in case of success;
  * otherwise returns NULL
  */
std::ofstream* Open(const std::string& file_name,
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
           uint32_t data_size);

/**
  * @brief Closes file stream
  * @param file_stream file stream to be closed
  */
void Close(std::ofstream* file_stream);

/**
  * @brief Returns current working directory path
  * If filename begins with "/", return unchanged filename
  * @param name file name
  * @return returns full file path.
  */
std::string CurrentWorkingDirectory();

/**
  * @brief Removes file
  *
  * @param name path to file
  * @return returns true if the file is successfully deleted.
  */
bool DeleteFile(const std::string& name);

/**
 * @brief Removes directory.
 *
 * @param name path to directory.
 * @param is_recursively true if you need delete directory recursively, otherwise false.
 * @return returns true if the directory is successfully deleted.
 */
bool RemoveDirectory(const std::string& directory_name,
                     bool is_recursively = true);

/**
  * @brief Check access rights
  *
  * @param name path to file.
  * @param how Read/write attribute.
  * @return returns true if file has the given mode.
  */
bool IsAccessible(const std::string& name, int32_t how);

/**
  * @brief Check access rights for writing
  *
  * @param name path to file or folder
  * @return returns true if has access rights.
  */
bool IsWritingAllowed(const std::string& name);

/**
  * @brief Check access rights for reading
  *
  * @param name path to file.
  * @return returns true if file has access rights.
  */
bool IsReadingAllowed(const std::string& name);

/**
  * @brief Lists all files in given directory
  *
  * @param name path to directory.
  * @return returns list of files.
  */
std::vector<std::string> ListFiles(const std::string& directory_name);

/**
 * @brief Creates or overwrites file with given binary contents
 * @param name path to the file
 * @param contents data to be written into the file
 * @returns true if file write succeeded
 */
bool WriteBinaryFile(const std::string& name,
                     const std::vector<uint8_t>& contents);

/**
  * @brief Reads from file
  *
  * @param name path to file
  * @param result read data
  * @return returns true if the operation is successfully.
  */
bool ReadBinaryFile(const std::string& name,
                    std::vector<uint8_t>& result);

bool ReadFile(const std::string& name, std::string& result);

/**
  * @brief Convert special symbols in system path to percent-encoded
  *
  * @param name path to file
  * @return returns converted path.
*/
const std::string ConvertPathForURL(const std::string& path);

/**
  * @brief Create empty file
  *
  * @param name path to file
  * @return if result success return true
*/
bool CreateFile(const std::string& path);

void remove_directory_content(const std::string& directory_name);

}  // namespace file_system

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
