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

#include "dbus/dbus_message.h"

namespace dbus {
// MessageReader
inline bool MessageReader::has_failed() const {
  return failed_;
}

inline bool MessageReader::IsAtLastElement() const {
  DBusMessageIter* iter = const_cast<DBusMessageIter*>(&iterator_);
  return dbus_message_iter_has_next(iter) == 0;
}

inline bool MessageReader::HasNext() const {
  return !failed_ && NextValueType() != DBUS_TYPE_INVALID;
}

inline bool MessageReader::NextIsInvalid() const {
  return NextValueType() == DBUS_TYPE_INVALID;
}

inline bool MessageReader::NextIsBool() const {
  return NextValueType() == DBUS_TYPE_BOOLEAN;
}

inline bool MessageReader::NextIsByte() const {
  return NextValueType() == DBUS_TYPE_BYTE;
}

inline bool MessageReader::NextIsInt16() const {
  return NextValueType() == DBUS_TYPE_INT16;
}

inline bool MessageReader::NextIsUint16() const {
  return NextValueType() == DBUS_TYPE_UINT16;
}

inline bool MessageReader::NextIsInt32() const {
  return NextValueType() == DBUS_TYPE_INT32;
}

inline bool MessageReader::NextIsUint32() const {
  return NextValueType() == DBUS_TYPE_UINT32;
}

inline bool MessageReader::NextIsInt64() const {
  return NextValueType() == DBUS_TYPE_INT64;
}

inline bool MessageReader::NextIsUint64() const {
  return NextValueType() == DBUS_TYPE_UINT64;
}

inline bool MessageReader::NextIsDouble() const {
  return NextValueType() == DBUS_TYPE_DOUBLE;
}

inline bool MessageReader::NextIsString() const {
  return NextValueType() == DBUS_TYPE_STRING;
}

inline bool MessageReader::NextIsArray() const {
  return NextValueType() == DBUS_TYPE_ARRAY;
}

inline bool MessageReader::NextIsStruct() const {
  return NextValueType() == DBUS_TYPE_STRUCT;
}

inline bool MessageReader::TakeBool() {
  dbus_bool_t value = 0;
  ReadNextValue(DBUS_TYPE_BOOLEAN, &value);
  return value != 0;
}

inline uint8_t MessageReader::TakeByte() {
  uint8_t value = 0;
  ReadNextValue(DBUS_TYPE_BYTE, &value);
  return value;
}

inline int16_t MessageReader::TakeInt16() {
  int16_t value = 0;
  ReadNextValue(DBUS_TYPE_INT16, &value);
  return value;
}

inline uint16_t MessageReader::TakeUint16() {
  uint16_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT16, &value);
  return value;
}

inline int32_t MessageReader::TakeInt32() {
  int32_t value = 0;
  ReadNextValue(DBUS_TYPE_INT32, &value);
  return value;
}

inline uint32_t MessageReader::TakeUint32() {
  uint32_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT32, &value);
  return value;
}

inline int64_t MessageReader::TakeInt64() {
  int64_t value = 0;
  ReadNextValue(DBUS_TYPE_INT64, &value);
  return value;
}

inline uint64_t MessageReader::TakeUint64() {
  uint64_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT64, &value);
  return value;
}

inline double MessageReader::TakeDouble() {
  double value = 0.;
  ReadNextValue(DBUS_TYPE_DOUBLE, &value);
  return value;
}

inline std::string MessageReader::TakeString() {
  const char* value = "";
  ReadNextValue(DBUS_TYPE_STRING, &value);
  return value;
}


inline MessageReader::DataType MessageReader::NextValueType() const {
  DBusMessageIter* iter = const_cast<DBusMessageIter*>(&iterator_);
  return failed_ ? DBUS_TYPE_INVALID : dbus_message_iter_get_arg_type(iter);
}



// Message writer methods
inline void MessageWriter::PutBool(bool value) {
  dbus_bool_t dbus_value = value;
  WriteAndCheck(DBUS_TYPE_BOOLEAN, &dbus_value);
}

inline void MessageWriter::PutByte(uint8_t value) {
  WriteAndCheck(DBUS_TYPE_BYTE, &value);
}

inline void MessageWriter::PutInt16(int16_t value) {
  WriteAndCheck(DBUS_TYPE_INT16, &value);
}

inline void MessageWriter::PutUint16(uint16_t value) {
  WriteAndCheck(DBUS_TYPE_UINT16, &value);
}

inline void MessageWriter::PutInt32(int32_t value) {
  WriteAndCheck(DBUS_TYPE_INT32, &value);
}

inline void MessageWriter::PutUint32(uint32_t value) {
  WriteAndCheck(DBUS_TYPE_UINT32, &value);
}

inline void MessageWriter::PutInt64(int64_t value) {
  WriteAndCheck(DBUS_TYPE_INT64, &value);
}

inline void MessageWriter::PutUint64(uint64_t value) {
  WriteAndCheck(DBUS_TYPE_UINT64, &value);
}

inline void MessageWriter::PutDouble(double value) {
  WriteAndCheck(DBUS_TYPE_DOUBLE, &value);
}

inline void MessageWriter::PutString(const std::string& value) {
  const char* pointer = value.c_str();
  WriteAndCheck(DBUS_TYPE_STRING, &pointer);
}


}  // namespace dbus

#endif // DBUS_MESSAGE_INL_H
