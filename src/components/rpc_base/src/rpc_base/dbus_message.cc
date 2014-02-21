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

#include <cassert>

#include <dbus/dbus.h>

namespace dbus {

Message::Message(DBusMessage* message)
  : raw_message_(message) {
}

Message::Message(const Message& that):
  raw_message_(that.raw_message_) {
  dbus_message_ref(raw_message_);
}

Message Message::MethodCall(const char* bus_name,
                            const char* path,
                            const char* interface,
                            const char* method) {
  return Message(dbus_message_new_method_call(bus_name,
                                              path,
                                              interface,
                                              method));
}

Message Message::Signal(const char* path,
                        const char* interface,
                        const char* name) {
  return Message(dbus_message_new_signal(path, interface, name));
}

Message::~Message(){
  dbus_message_unref(raw_message_);
}

Message&Message::operator=(const Message& that) {
  dbus_message_ref(that.raw_message_);
  dbus_message_unref(this->raw_message_);
  this->raw_message_ = that.raw_message_;
  return *this;
}

std::string Message::GetInterface() const {
  return dbus_message_get_interface(raw_message_);
}

std::string Message::GetMember() const {
  return dbus_message_get_member(raw_message_);
}

MessageIterator::MessageIterator(DBusMessage* message) {
  dbus_bool_t success = dbus_message_iter_init(message,
                                               &iterator_);
  assert(success);
}

bool MessageIterator::IsNull() const {
  return GetCurrentArgType() == DBUS_TYPE_INVALID;
}

bool MessageIterator::IsBool() const {
  return GetCurrentArgType() == DBUS_TYPE_BOOLEAN;
}

bool MessageIterator::GetBool() {
  assert(IsBool());
  dbus_bool_t res;
  dbus_message_iter_get_basic(&iterator_, &res);
  return static_cast<bool>(res);
}

bool MessageIterator::IsInt32() const {
  return GetCurrentArgType() == DBUS_TYPE_INT32;
}

int32_t MessageIterator::GetInt32() {
  assert(IsInt32());
  dbus_int32_t res;
  dbus_message_iter_get_basic(&iterator_, &res);
  return res;
}

void MessageIterator::PutInt32(int32_t value) {
  dbus_bool_t res = dbus_message_iter_append_basic(&iterator_,
                                                   DBUS_TYPE_INT32,
                                                   &value);
  assert(res && "Not enough memory");
}

bool MessageIterator::IsDouble() const {
  return GetCurrentArgType() == DBUS_TYPE_DOUBLE;
}

double MessageIterator::GetDouble() {
  assert(IsDouble());
  double res;
  dbus_message_iter_get_basic(&iterator_, &res);
  return res;
}

void MessageIterator::PutDouble(double value) {
  dbus_bool_t res = dbus_message_iter_append_basic(&iterator_,
                                                   DBUS_TYPE_DOUBLE,
                                                   &value);
  assert(res && "Not enough memory");
}

bool MessageIterator::IsString() const {
  return GetCurrentArgType() == DBUS_TYPE_STRING;
}

std::string MessageIterator::GetString() {
  assert(IsString());
  const char* res;
  dbus_message_iter_get_basic(&iterator_, &res);
  return res;
}

void MessageIterator::PutString(const std::string& value) {
  return PutString(value.c_str());
}

void MessageIterator::PutString(const char* value) {
  dbus_bool_t res = dbus_message_iter_append_basic(&iterator_,
                                                   DBUS_TYPE_STRING,
                                                   &value);
  assert(res && "Not enough memory");
}

bool MessageIterator::IsArray() const {
  return GetCurrentArgType() == DBUS_TYPE_ARRAY;
}

MessageIterator MessageIterator::GetArrayReadMessageIterator() {
  assert(IsArray());
  MessageIterator array_read_MessageIterator;
  dbus_message_iter_recurse(&iterator_,
                            &array_read_MessageIterator.iterator_);
  return array_read_MessageIterator;
}

MessageIterator MessageIterator::GetArrayWriteMessageIterator(
    const char* item_type_signature) {
  assert(IsArray());
  MessageIterator array_write_iterator;
  dbus_bool_t success = dbus_message_iter_open_container(
                      &iterator_,
                      DBUS_TYPE_ARRAY,
                      item_type_signature,
                      &array_write_iterator.iterator_);
  assert(success && "Not enough memory");

  return array_write_iterator;
}

bool MessageIterator::IsStruct() const {
  return GetCurrentArgType() == DBUS_TYPE_STRUCT;
}

MessageIterator MessageIterator::GetStructReadMessageIterator() {
  assert(IsStruct());
  MessageIterator array_read_iterator;
  dbus_message_iter_recurse(&iterator_,
                            &array_read_iterator.iterator_);
  return array_read_iterator;
}

MessageIterator MessageIterator::GetStructWriteMessageIterator() {
  assert(IsStruct());
  MessageIterator struct_write_iterator;
  dbus_bool_t success = dbus_message_iter_open_container(
                          &iterator_,
                          DBUS_TYPE_STRUCT,
                          NULL,
                          &struct_write_iterator.iterator_);
  assert(success && "Not enough memory");
  return struct_write_iterator;
}

MessageIterator::MessageIterator() {
  // Do-nothing
}

int MessageIterator::GetCurrentArgType() const {
  DBusMessageIter* iter = const_cast<DBusMessageIter*>(&iterator_);
  return dbus_message_iter_get_arg_type(iter);
}

} // namespace dbus
