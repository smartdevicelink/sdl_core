/*
 * \file bluetooth_uuid_win.h
 * \brief Header for classes which stores UUID of bluetooth-service.
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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BLUETOOTH_BLUETOOTH_UUID_WIN_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BLUETOOTH_BLUETOOTH_UUID_WIN_H_

#include "utils/bluetooth/bluetooth_utils_win.h"

namespace utils {
/**
* @brief Information about UUID of bluetooth-service.
*/
class BluetoothUUID {
 public:
  explicit BluetoothUUID(const std::string& uuid) {
    utils::GetCanonicalUuid(uuid, value_, canonical_value_, format_);
  }

  BluetoothUUID() : format_(utils::UUIDFormat::kFormatInvalid) {}

  /**
  * @brief  Returns true, if the UUID is in a valid canonical format.
  */
  bool IsValid() const {
    return format_ != utils::UUIDFormat::kFormatInvalid;
  }

  /**
  * @brief Returns the representation format of the UUID. This reflects the
  * format
  * that was provided during construction.
  */
  utils::UUIDFormat Format() const {
    return format_;
  }

  /**
  * @brief Returns the value of the UUID as a string. The representation format
  * is
  *     based on what was passed in during construction. For the supported
  * sizes,
  *     this representation can have the following formats:
  *   - 16 bit:  xxxx
  *   - 32 bit:  xxxxxxxx
  *   - 128 bit: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
  *     where x is a lowercase hex digit.
  */
  const std::string& Value() const {
    return value_;
  }

  /**
  * @brief Returns the underlying 128-bit value as a string in the following
  * format:
  *   xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
  *   where x is a lowercase hex digit.
  */
  const std::string& CanonicalValue() const {
    return canonical_value_;
  }

  /**
  * @brief Permit sufficient comparison to allow a UUID to be used as a key in a
  *   std::map.
  */
  bool operator<(const BluetoothUUID& uuid) const {
    return canonical_value_ < uuid.canonical_value_;
  }

  /**
  * @brief Equality operators.
  */
  bool operator==(const BluetoothUUID& uuid) const {
    return canonical_value_ == uuid.canonical_value_;
  }
  bool operator!=(const BluetoothUUID& uuid) const {
    return canonical_value_ != uuid.canonical_value_;
  }

 private:
  utils::UUIDFormat format_;
  std::string value_;

  /**
  * @brief The 128-bit string representation of the UUID.
  */
  std::string canonical_value_;
};
}  // namespace utils
#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BLUETOOTH_BLUETOOTH_UUID_WIN_H_
