/*
 * Copyright (c) 2018, Livio
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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_SEMANTIC_VERSION_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_SEMANTIC_VERSION_H_

#include <boost/algorithm/string.hpp>

namespace utils {

struct SemanticVersion {

  SemanticVersion(uint16_t major = 0, uint16_t minor = 0, uint16_t patch = 0) {
    major_version = major;
    minor_version = minor;
    patch_version = patch;
  }

  SemanticVersion(const std::string& str_version) {
    std::vector<std::string> str_array;
    boost::split(str_array, str_version, boost::is_any_of("."));
    if(str_array.size() == 3) {
        major_version = atoi(str_array[0].c_str());
        minor_version = atoi(str_array[1].c_str());
        patch_version = atoi(str_array[2].c_str());
    } else {
      exit(1);
    }
  }

  bool operator==(const SemanticVersion& version) const {
    if (major_version == version.major_version && minor_version == version.minor_version && patch_version == version.patch_version) {
      return true;
    } else {
      return false;
    }
  }

  bool operator<(const SemanticVersion& version) const {
    if(major_version < version.major_version) {
      return true;
    } else if (minor_version < version.minor_version) {
      return true;
    } else if (patch_version < version.patch_version) {
      return true;
    } else {
      return false;
    }
  }

  bool operator<=(const SemanticVersion& version) const {
    if (this->operator==(version)) {
      return true;
    } else if (this->operator<(version)) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>(const SemanticVersion& version) const {
    if(major_version > version.major_version) {
      return true;
    } else if (minor_version > version.minor_version) {
      return true;
    } else if (patch_version > version.patch_version) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>=(const SemanticVersion& version) const {
    if (this->operator==(version)) {
      return true;
    } else if (this->operator>(version)) {
      return true;
    } else {
      return false;
    }
  }

  const std::string toString() {
    std::string result = "";
    result += std::to_string(major_version);
    result += ".";
    result += std::to_string(minor_version);
    result += ".";
    result += std::to_string(patch_version);
    printf("toString Result!!!: %s\n", result.c_str());
    return result;
  }

  bool isValid() const{
    return major_version > 0 || minor_version > 0 || patch_version > 0;
  }

  uint16_t major_version = 0;
  uint16_t minor_version = 0;
  uint16_t patch_version = 0;

};

}

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_CALLABLE_H