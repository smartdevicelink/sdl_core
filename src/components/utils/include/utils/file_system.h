/*
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

#include <stdint.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "utils/macro.h"

namespace file_system {

/**
 * @brief Get available disc space of directory. Returns 0 if the directory does
 * not exist.
 *
 * @param path to directory
 * @return free disc space.
 */
uint64_t GetAvailableDiskSpace(const std::string& path);

/*
 * @brief Get size of given directory
 *
 * @param path to directory
 * @return size of directory, return 0 if dir not exist
 */
size_t DirectorySize(const std::string& path);

/*
 * @brief Get size of given file
 *
 * @param path to file
 * @return size of file, return 0 if file not exist
 */
uint64_t FileSize(const std::string& path);

/**
 * @brief Creates directory with owner_all permissions
 * @param name directory to create
 * @return true if directory was created or already existed.
 */
bool CreateDirectory(const std::string& name);

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
 * @brief Does directory exist
 * @param name path to directory
 * @return returns true if the file exists and is a directory.
 */
bool DirectoryExists(const std::string& name);

/**
 * @brief Does file exist
 * @param name path to file
 * @return returns true if the file exists.
 */
bool FileExists(const std::string& name);

/**
 * @brief Writes to file
 *
 * @remark - create file if it doesn't exist
 * @param name path to file
 * @param data data to write
 * @return returns true if the operation is successful.
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
 * @return returns true if the operation is successful.
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
 * @return returns full file path.
 */
std::string CurrentWorkingDirectory();

/**
 * @brief Convert a path to its absolute form.
 * @param path the file name to convert.
 * @return corresponding absolute path for a valid path, otherwise an empty
 * string.
 */
std::string GetAbsolutePath(const std::string& path);

/**
 * @brief Checks if file name contains invalid symbols e.g. '/'
 * @param file_name file name to check
 * @return true if file name does not contain any invalid symbol otherwise
 * returns false
 */
bool IsFileNameValid(const std::string& file_name);

/**
 * @brief Removes file
 *
 * @param name path to file
 * @return returns true if the file is successfully deleted.
 */
bool DeleteFile(const std::string& name);

/**
 * @brief Removes contents of directory but not directory itself
 *
 * @param directory_name path to directory.
 */
void remove_directory_content(const std::string& directory_name);

/**
 * @brief Removes directory.
 *
 * @param directory_name path to directory.
 * @param is_recursively true if you need delete directory recursively,
 * otherwise false. A non-empty directory with is_recursively == false will
 * return false.
 * @return returns true if the directory is successfully deleted.
 */
bool RemoveDirectory(const std::string& directory_name,
                     const bool is_recursively = true);

/**
 * @brief Check access rights
 *
 * @param name path to file.
 * @param how Read/write attribute.
 * @return returns true if file has the given mode.
 */
bool IsAccessible(const std::string& name, const int32_t how);

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
bool ReadBinaryFile(const std::string& name, std::vector<uint8_t>& result);

/**
 * @brief Reads from file
 *
 * @param name path to file
 * @param result read data
 * @param offset number of bytes to be ignored from begining of file
 * @return returns true if the operation is successfully.
 */
bool ReadBinaryFile(const std::string& name,
                    std::vector<uint8_t>& result,
                    uint32_t offset,
                    uint32_t length);

bool ReadFile(const std::string& name, std::string& result);

DEPRECATED
const std::string ConvertPathForURL(const std::string& path);

/**
 * @brief Create empty file
 *
 * @param name path to file
 * @return if result success return true
 */
bool CreateFile(const std::string& path);

/**
 * @brief Get modification time of file
 * @param path Path to file
 * @return Modification time in nanoseconds
 */
time_t GetFileModificationTime(const std::string& path);

/**
 * @brief Copy file from source to destination
 *
 * @param src Source file path
 * @param dst Destination file path
 * @return if result success return true
 */
bool CopyFile(const std::string& src, const std::string& dst);

/**
 * @brief Move file from source to destination
 *
 * @param src Source file path
 * @param dst Destination file path
 * @return if result success return true
 */
bool MoveFile(const std::string& src, const std::string& dst);

/**
 * @brief Get filename from full path
 *
 * @param full_path Source file path
 * @return parsed filename
 */
std::string GetFileName(const std::string& full_path);

}  // namespace file_system

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
