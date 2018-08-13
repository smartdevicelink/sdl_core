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

namespace utils {

struct SemanticVersion {
  SemanticVersion(uint16_t major = 0, uint16_t minor = 0, uint16_t patch = 0) {
    majorVersion = major;
    minorVersion = minor;
    patchVersion = patch;
  }

  SemanticVersion(const SemanticVersion& other) {
    majorVersion = other.majorVersion;
    minorVersion = other.minorVersion;
    patchVersion = other.patchVersion;
  }

  SemanticVersion(const std::string& versionString)
      : majorVersion(0), minorVersion(0), patchVersion(0) {
    unsigned int majorInt, minorInt, patchInt;
    int readElements = sscanf(
        versionString.c_str(), "%u.%u.%u", &majorInt, &minorInt, &patchInt);
    if (readElements != 3) {
      // LOG4CXX_WARN(logger_,
      //             "Error while parsing version string: " << versionString);
    } else {
      majorVersion = static_cast<uint8_t>(majorInt);
      minorVersion = static_cast<uint8_t>(minorInt);
      patchVersion = static_cast<uint8_t>(patchInt);
    }
  }

  static inline int16_t cmp(const SemanticVersion& version1,
                            const SemanticVersion& version2) {
    int16_t diff =
        static_cast<int16_t>(version1.majorVersion - version2.majorVersion);
    if (diff == 0) {
      diff =
          static_cast<int16_t>(version1.minorVersion - version2.minorVersion);
      if (diff == 0) {
        diff =
            static_cast<int16_t>(version1.patchVersion - version2.patchVersion);
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
    result += std::to_string(majorVersion);
    result += ".";
    result += std::to_string(minorVersion);
    result += ".";
    result += std::to_string(patchVersion);
    return result;
  }

  bool isValid() const {
    return majorVersion > 0 || minorVersion > 0 || patchVersion > 0;
  }

  uint16_t majorVersion = 0;
  uint16_t minorVersion = 0;
  uint16_t patchVersion = 0;
};
}

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_CALLABLE_H