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

#include "rpc_base/dbus_message.h"

#include <algorithm>
#include <cassert>

#include <dbus/dbus.h>

namespace dbus {

MessageRefKeeper::MessageRefKeeper()
  : raw_message_(NULL) {
}

MessageRefKeeper::MessageRefKeeper(DBusMessage* message)
  : raw_message_(message) {
  assert(raw_message_);
  dbus_message_ref(raw_message_);
}

MessageRefKeeper::~MessageRefKeeper() {
  if (raw_message_) {
    dbus_message_unref(raw_message_);
  }
}

MessageRefKeeper::MessageRefKeeper(const MessageRefKeeper& other)
  : raw_message_(other.raw_message_) {
  assert(raw_message_);
  dbus_message_ref(raw_message_);
}

MessageRefKeeper& MessageRefKeeper::operator=(MessageRefKeeper other) {
  other.swap(*this);
  return *this;
}

DBusMessage* MessageRefKeeper::get() const {
  return raw_message_;
}

DBusMessage* MessageRefKeeper::Pass() {
  DBusMessage* message = raw_message_;
  raw_message_ = NULL;
  return message;
}

void MessageRefKeeper::swap(MessageRefKeeper& other) {
  std::swap(raw_message_, other.raw_message_);
}


MessageRef::MessageRef(DBusMessage* message)
  : raw_message_ref_(message) {
}

MessageRef::~MessageRef(){
}

std::string MessageRef::GetInterface() const {
  return dbus_message_get_interface(raw_message_ref_.get());
}

std::string MessageRef::GetMember() const {
  return dbus_message_get_member(raw_message_ref_.get());
}

// Message writer
MessageReader::MessageReader(const MessageRef& message)
  : parent_reader_(NULL),
    failed_(false) {
  dbus_bool_t success =
      dbus_message_iter_init(message.raw_message_ref_.get(),
                             &iterator_);
  if (!success) {
    MarkFailed();
  }
}

MessageReader::MessageReader(MessageReader* reader,
                             DataType container_data_type)
  : parent_reader_(reader),
    failed_(reader->has_failed()) {
  assert(parent_reader_);
  if (!failed_) {
    if (reader->NextValueType() != container_data_type) {
      MarkFailed();
    } else {
      dbus_message_iter_recurse(&reader->iterator_, &iterator_);
    }
  }
}

bool MessageReader::has_failed() const {
  return failed_;
}

bool MessageReader::HasNext() const {
  DBusMessageIter* iter = const_cast<DBusMessageIter*>(&iterator_);
  return !failed_ && dbus_message_iter_has_next(iter) != 0;
}

void MessageReader::Next() {
  if (dbus_message_iter_next(&iterator_) == 0) {
    MarkFailed();
  }
}

bool MessageReader::NextIsInvalid() const {
  return NextValueType() == DBUS_TYPE_INVALID;
}

bool MessageReader::NextIsBool() const {
  return NextValueType() == DBUS_TYPE_BOOLEAN;
}

bool MessageReader::NextIsByte() const {
  return NextValueType() == DBUS_TYPE_BYTE;
}

bool MessageReader::NextIsInt16() const {
  return NextValueType() == DBUS_TYPE_INT16;
}

bool MessageReader::NextIsUint16() const {
  return NextValueType() == DBUS_TYPE_UINT16;
}

bool MessageReader::NextIsInt32() const {
  return NextValueType() == DBUS_TYPE_INT32;
}

bool MessageReader::NextIsUint32() const{
  return NextValueType() == DBUS_TYPE_UINT32;
}

bool MessageReader::NextIsInt64() const {
  return NextValueType() == DBUS_TYPE_INT64;
}

bool MessageReader::NextIsUint64() const {
  return NextValueType() == DBUS_TYPE_UINT64;
}

bool MessageReader::NextIsDouble() const {
  return NextValueType() == DBUS_TYPE_DOUBLE;
}

bool MessageReader::NextIsString() const {
  return NextValueType() == DBUS_TYPE_STRING;
}

bool MessageReader::NextIsArray() const {
  return NextValueType() == DBUS_TYPE_ARRAY;
}

bool MessageReader::NextIsStruct() const {
  return NextValueType() == DBUS_TYPE_STRUCT;
}

bool MessageReader::NextIsDictEntry() const {
  return NextValueType() == DBUS_TYPE_DICT_ENTRY;
}

bool MessageReader::ReadBool() {
  dbus_bool_t value = 0;
  ReadNextValue(DBUS_TYPE_BOOLEAN, &value);
  return value != 0;
}

uint8_t MessageReader::ReadByte() {
  uint8_t value = 0;
  ReadNextValue(DBUS_TYPE_BYTE, &value);
  return value;
}

int16_t MessageReader::ReadInt16() {
  int16_t value = 0;
  ReadNextValue(DBUS_TYPE_INT16, &value);
  return value;
}

uint16_t MessageReader::ReadUin16() {
  uint16_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT16, &value);
  return value;
}

int32_t MessageReader::ReadInt32() {
  int32_t value = 0;
  ReadNextValue(DBUS_TYPE_INT32, &value);
  return value;
}

uint32_t MessageReader::ReadUint32() {
  uint32_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT32, &value);
  return value;
}

int64_t MessageReader::ReadInt64() {
  int64_t value = 0;
  ReadNextValue(DBUS_TYPE_INT64, &value);
  return value;
}

uint64_t MessageReader::ReadUint64() {
  uint64_t value = 0;
  ReadNextValue(DBUS_TYPE_UINT64, &value);
  return value;
}

double MessageReader::ReadDouble() {
  double value = 0.;
  ReadNextValue(DBUS_TYPE_DOUBLE, &value);
  return value;
}

std::string MessageReader::ReadString() {
  const char* value = "";
  ReadNextValue(DBUS_TYPE_STRING, &value);
  return value;
}

MessageReader MessageReader::GetArrayReader() {
  return MessageReader(this, DBUS_TYPE_ARRAY);
}

MessageReader MessageReader::GetStructReader() {
  return MessageReader(this, DBUS_TYPE_STRUCT);
}

MessageReader MessageReader::GetDictEntryReader() {
  return MessageReader(this, DBUS_TYPE_DICT_ENTRY);
}

void MessageReader::MarkFailed() {
  if (!failed_) {
    failed_ = true;
    if (parent_reader_) {
      parent_reader_->MarkFailed();
    }
  }
}

MessageReader::DataType MessageReader::NextValueType() const {
  DBusMessageIter* iter = const_cast<DBusMessageIter*>(&iterator_);
  return failed_ ? DBUS_TYPE_INVALID : dbus_message_iter_get_arg_type(iter);
}

void MessageReader::ReadNextValue(MessageReader::DataType type,
                                  void* value) {
  assert(value);
  if (NextValueType() != type) {
    MarkFailed();
  } else if (!failed_) {
    dbus_message_iter_get_basic(&iterator_, value);
    if (HasNext()) {
      Next();
    }
  }
}

// DbusMessageWriter
MessageWriter::MessageWriter(const MessageRef& message)
  : has_opened_subcontainer_(false),
    parent_writer_(NULL) {
  dbus_message_iter_init_append(message.raw_message_ref_.get(),
                                &iterator_);
}

MessageWriter::MessageWriter(
    MessageWriter* parent,
    ContainerType container_type,
    const char* array_signature)
  : has_opened_subcontainer_(false),
    parent_writer_(parent) {
  assert(parent_writer_);
  assert(!parent_writer_->has_opened_subcontainer_);
  bool enough_memory =
      dbus_message_iter_open_container(&parent_writer_->iterator_,
                                       container_type,
                                       array_signature,
                                       &iterator_);
  assert(enough_memory);
  parent_writer_->has_opened_subcontainer_ = true;
}

MessageWriter::~MessageWriter() {
  assert(!has_opened_subcontainer_);
  if (parent_writer_ != NULL) {
    CloseWriter();
  }
}

void MessageWriter::WriteBool(bool value) {
  dbus_bool_t dbus_value = value;
  WriteAndCheck(DBUS_TYPE_BOOLEAN, &dbus_value);
}

void MessageWriter::WriteByte(uint8_t value) {
  WriteAndCheck(DBUS_TYPE_BYTE, &value);
}

void MessageWriter::WriteInt16(int16_t value) {
  WriteAndCheck(DBUS_TYPE_INT16, &value);
}

void MessageWriter::WriteUint16(uint16_t value) {
  WriteAndCheck(DBUS_TYPE_UINT16, &value);
}

void MessageWriter::WriteInt32(int32_t value) {
  WriteAndCheck(DBUS_TYPE_INT32, &value);
}

void MessageWriter::WriteUint32(uint32_t value) {
  WriteAndCheck(DBUS_TYPE_UINT32, &value);
}

void MessageWriter::WriteInt64(int64_t value) {
  WriteAndCheck(DBUS_TYPE_INT64, &value);
}

void MessageWriter::WriteUint64(uint64_t value) {
  WriteAndCheck(DBUS_TYPE_UINT64, &value);
}

void MessageWriter::WriteDouble(double value) {
  WriteAndCheck(DBUS_TYPE_DOUBLE, &value);
}

void MessageWriter::WriteString(const std::string& value) {
  const char* pointer = value.c_str();
  WriteAndCheck(DBUS_TYPE_STRING, &pointer);
}

void MessageWriter::WriteAndCheck(
    MessageWriter::DataType value_type, const void* value) {
  assert(!has_opened_subcontainer_);
  dbus_bool_t enough_memory =
      dbus_message_iter_append_basic(&iterator_, value_type, value);
  assert(enough_memory);
}

void MessageWriter::CloseWriter() {
  assert(parent_writer_);
  assert(parent_writer_->has_opened_subcontainer_);
  dbus_bool_t enough_memory =
      dbus_message_iter_close_container(&parent_writer_->iterator_,
                                        &iterator_);
  assert(enough_memory);
  parent_writer_->has_opened_subcontainer_ = false;
}

// Construction functions

MessageRef MethodCall(const char* bus_name,
                      const char* path,
                      const char* interface,
                      const char* method) {
  return MessageRef(dbus_message_new_method_call(
                      bus_name,
                      path,
                      interface,
                      method));
}

MessageRef Signal(const char* path,
                  const char* interface,
                  const char* name) {
  return MessageRef(dbus_message_new_signal(path, interface, name));
}


} // namespace dbus
