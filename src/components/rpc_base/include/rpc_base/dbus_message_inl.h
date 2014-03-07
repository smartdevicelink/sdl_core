/* Copyright (c) 2014, Ford Motor Company
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

#ifndef DBUS_MESSAGE_INL_H
#define DBUS_MESSAGE_INL_H

#include "rpc_base/dbus_message.h"

namespace dbus {

template<>
inline bool MessageReader::NextIs<bool>() const {
  return NextValueType() == DBUS_TYPE_BOOLEAN;
}

template<>
inline bool MessageReader::NextIs<int8_t>() const {
  return NextValueType() == DBUS_TYPE_BYTE;
}

template<>
inline bool MessageReader::NextIs<uint8_t>() const {
  return NextValueType() == DBUS_TYPE_BYTE;
}

template<>
inline bool MessageReader::NextIs<int16_t>() const {
  return NextValueType() == DBUS_TYPE_INT16;
}

template<>
inline bool MessageReader::NextIs<uint16_t>() const {
  return NextValueType() == DBUS_TYPE_UINT16;
}

template<>
inline bool MessageReader::NextIs<int32_t>() const {
  return NextValueType() == DBUS_TYPE_INT32;
}

template<>
inline bool MessageReader::NextIs<uint32_t>() const {
  return NextValueType() == DBUS_TYPE_UINT32;
}

template<>
inline bool MessageReader::NextIs<int64_t>() const {
  return NextValueType() == DBUS_TYPE_INT64;
}

template<>
inline bool MessageReader::NextIs<uint64_t>() const {
  return NextValueType() == DBUS_TYPE_UINT64;
}

template<>
inline bool MessageReader::NextIs<double>() const {
  return NextValueType() == DBUS_TYPE_DOUBLE;
}

template<>
inline bool MessageReader::NextIs<std::string>() const {
  return NextValueType() == DBUS_TYPE_STRING;
}

// Reader functions
template<>
inline bool MessageReader::Read<bool>() {
  dbus_bool_t value = 0;
  ReadNextValue(DBUS_TYPE_BOOLEAN, &value);
  return value != 0;
}

template<>
inline uint8_t MessageReader::Read<uint8_t>() {
  uint8_t value = 0;
  ReadNextValue(DBUS_TYPE_BYTE, &value);
  return value;
}

template<>
inline int8_t MessageReader::Read<int8_t>() {
  int8_t value = 0;
  ReadNextValue(DBUS_TYPE_BYTE, &value);
  return value;
}

template<>
inline int16_t MessageReader::Read<int16_t>() {
  int16_t value = 0;
  ReadNextValue(DBUS_TYPE_INT16, &value);
  return value;
}

template<>
inline uint16_t MessageReader::Read<uint16_t>() {
  uint16_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT16, &value);
  return value;
}

template<>
inline int32_t MessageReader::Read<int32_t>() {
  int32_t value = 0;
  ReadNextValue(DBUS_TYPE_INT32, &value);
  return value;
}

template<>
inline uint32_t MessageReader::Read<uint32_t>() {
  uint32_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT32, &value);
  return value;
}

template<>
inline int64_t MessageReader::Read<int64_t>() {
  int64_t value = 0;
  ReadNextValue(DBUS_TYPE_INT64, &value);
  return value;
}

template<>
inline uint64_t MessageReader::Read<uint64_t>() {
  uint64_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT64, &value);
  return value;
}

template<>
inline double MessageReader::Read<double>() {
  double value = 0.;
  ReadNextValue(DBUS_TYPE_DOUBLE, &value);
  return value;
}

template<>
inline std::string MessageReader::Read<std::string>() {
  const char* value = "";
  ReadNextValue(DBUS_TYPE_STRING, &value);
  return value;
}



}  // namespace dbus

#endif // DBUS_MESSAGE_INL_H
