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
// TODO(AK): lint error: Use int16/int64/etc, rather than the C type long
unsigned long int available_space();
std::string create_directory(const std::string& name);
bool is_directory_exists(const std::string& name);
bool is_file_exist(const std::string& name);

/**
 * @remark - create file if it doesn't exist
 */
bool write(const std::string& file_name,
           const std::vector<unsigned char>& data);

std::string full_path(const std::string& name);
bool delete_file(const std::string& name);
std::vector<std::string> list_files(const std::string& directory_name);
bool read_binary_file(const std::string& name,
                      std::vector<unsigned char>& result);
}  // namespace file_system

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_FILE_SYSTEM_H_
