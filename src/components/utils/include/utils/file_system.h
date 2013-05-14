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
#include <string>
#include <vector>

namespace file_system {
  /**
   * @brief Get available disc space.
   *
   * @return free disc space.
   */
  uint64_t available_space();

  /**
    * @brief Creates directory
    * @param name path to directory
    * @return path to created directory.
    */
  std::string create_directory(const std::string& name);

  /**
    * @brief Checks the file to see whether the file is a directory
    * @param name path to file
    * @return returns true if file is directory.
    */
  bool is_directory(const std::string & name);

  /**
    * @brief Is directory exist
    * @param name path to directory
    * @return returns true if directory is exists.
    */
  bool directory_exists(const std::string& name);

  /**
    * @brief Is file exist
    * @param name path to file
    * @return returns true if file is exists.
    */
  bool file_exists(const std::string& name);

  /**
    * @brief Writes to file
    *
    * @remark - create file if it doesn't exist
    * @param name path to file
    * @param data data to write
    * @return returns true if the operation is successfully.
    */
  bool write(const std::string& file_name,
             const std::vector<unsigned char>& data);

  /**
    * @brief Returns full file path
    *
    * @param name file name
    * @return returns full file path.
    */
  std::string full_path(const std::string& name);

  /**
    * @brief Removes file
    *
    * @param name path to file
    * @return returns true if the file is successfully deleted.
    */
  bool delete_file(const std::string& name);

  /**
   * @brief Removes directory.
   *
   * @param name path to directory.
   * @param is_recursively true if you need delete directory recursively, otherwise false.
   * @return returns true if the directory is successfully deleted.
   */
  bool remove_directory(const std::string& directory_name,
                        bool is_recursively = true);

  /**
    * @brief Check access rights
    *
    * @param name path to file.
    * @param how Read/write attribute.
    * @return returns true if file has the given mode.
    */
  bool is_accessible(const std::string& name, int how);

  /**
    * @brief Lists all files in given directory
    *
    * @param name path to directory.
    * @return returns list of files.
    */
  std::vector<std::string> list_files(const std::string& directory_name);

  /**
    * @brief Reads from file
    *
    * @param name path to file
    * @param result read data
    * @return returns true if the operation is successfully.
    */
  bool read_binary_file(const std::string& name,
                        std::vector<unsigned char>& result);
}  // namespace file_system

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
