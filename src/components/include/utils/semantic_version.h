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

#include <string>
namespace utils {

struct SemanticVersion {
  SemanticVersion(uint16_t major = 0, uint16_t minor = 0, uint16_t patch = 0) {
    major_version_ = major;
    minor_version_ = minor;
    patch_version_ = patch;
  }

  SemanticVersion(const SemanticVersion& other) {
    major_version_ = other.major_version_;
    minor_version_ = other.minor_version_;
    patch_version_ = other.patch_version_;
  }

  SemanticVersion(const std::string& versionString)
      : major_version_(0), minor_version_(0), patch_version_(0) {
    unsigned int major_int, minor_int, patch_int;
    int readElements = sscanf(
        versionString.c_str(), "%u.%u.%u", &major_int, &minor_int, &patch_int);
    if (readElements != 3) {
      // LOG4CXX_WARN(logger_,
      //             "Error while parsing version string: " << versionString);
    } else {
      major_version_ = static_cast<uint8_t>(major_int);
      minor_version_ = static_cast<uint8_t>(minor_int);
      patch_version_ = static_cast<uint8_t>(patch_int);
    }
  }

  static inline int16_t cmp(const SemanticVersion& version1,
                            const SemanticVersion& version2) {
    int16_t diff =
        static_cast<int16_t>(version1.major_version_ - version2.major_version_);
    if (diff == 0) {
      diff = static_cast<int16_t>(version1.minor_version_ -
                                  version2.minor_version_);
      if (diff == 0) {
        diff = static_cast<int16_t>(version1.patch_version_ -
                                    version2.patch_version_);
      }
    }
    return diff;
  }

  bool operator==(const SemanticVersion& other) const {
    return SemanticVersion::cmp(*this, other) == 0;
  }
  bool operator<(const SemanticVersion& other) const {
    return SemanticVersion::cmp(*this, other) < 0;
  }
  bool operator>(const SemanticVersion& other) const {
    return SemanticVersion::cmp(*this, other) > 0;
  }
  bool operator<=(const SemanticVersion& other) const {
    return SemanticVersion::cmp(*this, other) <= 0;
  }
  bool operator>=(const SemanticVersion& other) const {
    return SemanticVersion::cmp(*this, other) >= 0;
  }
  static inline SemanticVersion* min(SemanticVersion& version1,
                                     SemanticVersion& version2) {
    return (version1 < version2) ? &version1 : &version2;
  }

  const std::string toString() const {
    std::string result = "";
    result += std::to_string(major_version_);
    result += ".";
    result += std::to_string(minor_version_);
    result += ".";
    result += std::to_string(patch_version_);
    return result;
  }

  bool isValid() const {
    return major_version_ > 0 || minor_version_ > 0 || patch_version_ > 0;
  }

  uint16_t major_version_ = 0;
  uint16_t minor_version_ = 0;
  uint16_t patch_version_ = 0;
};

extern const SemanticVersion base_rpc_version;
}

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_CALLABLE_H