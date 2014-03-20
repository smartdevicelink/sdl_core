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

#ifndef DBUS_DBUS_MESSAGE_H
#define DBUS_DBUS_MESSAGE_H

#include <stdint.h>
#include <string>

#include <dbus/dbus.h>

namespace dbus {

enum ContainerType {
  kArray = DBUS_TYPE_ARRAY,
  kStruct = DBUS_TYPE_STRUCT,
  kDictEntry = DBUS_TYPE_DICT_ENTRY
};

class MessageRefKeeper {
public:
  MessageRefKeeper();
  MessageRefKeeper(DBusMessage* message);
  ~MessageRefKeeper();
  MessageRefKeeper(const MessageRefKeeper& other);
  MessageRefKeeper& operator=(MessageRefKeeper other);
  DBusMessage* get() const;
  DBusMessage* Pass();
  void swap(MessageRefKeeper& other);
private:
  DBusMessage* raw_message_;
};

class MessageRef {
 public:
  MessageRef(DBusMessage* message);
  ~MessageRef();
  std::string GetInterface() const;
  std::string GetMember() const;
 protected:
  MessageRefKeeper raw_message_ref_;
  friend class MessageReader;
  friend class MessageWriter;
};

class MessageReader {
 public:
  // Main constructor
  MessageReader(const MessageRef& message);
  bool has_failed() const;
  bool IsAtLastElement() const;
  bool HasNext() const;

  // Type checkers
  bool NextIsInvalid() const;
  bool NextIsBool() const;
  bool NextIsByte() const;
  bool NextIsInt16() const;
  bool NextIsUint16() const;
  bool NextIsInt32() const;
  bool NextIsUint32() const;
  bool NextIsInt64() const;
  bool NextIsUint64() const;
  bool NextIsDouble() const;
  bool NextIsString() const;
  bool NextIsArray() const;
  bool NextIsStruct() const;
  bool NextIsDictEntry() const;

  // Readers
  bool     TakeBool();
  uint8_t  TakeByte();
  int16_t  TakeInt16();
  uint16_t TakeUint16();
  int32_t  TakeInt32();
  uint32_t TakeUint32();
  int64_t  TakeInt64();
  uint64_t TakeUint64();
  double   TakeDouble();
  std::string TakeString();

  MessageReader TakeArrayReader();
  MessageReader TakeStructReader();
  MessageReader TakeDictEntryReader();
 private:
  typedef int DataType;
  // Container reader constructor
  MessageReader(MessageReader* reader,
                DataType container_data_type);
  void MoveToNext();
  void MarkFailed();
  DataType NextValueType() const;
  void ReadNextValue(DataType type, void* value);
 private:
  // Fields
  MessageReader* parent_reader_;
  bool failed_;
  DBusMessageIter iterator_;
};

class MessageWriter {
 public:
  // Methods
  // Main appending constructor
  MessageWriter(const MessageRef& message);
  // Container writer constructor
  MessageWriter(MessageWriter* parent,
                ContainerType type,
                const char* array_signature);
  ~MessageWriter();
  void PutBool(bool value);
  void PutByte(uint8_t value);
  void PutInt16(int16_t value);
  void PutUint16(uint16_t value);
  void PutInt32(int32_t value);
  void PutUint32(uint32_t value);
  void PutInt64(int64_t value);
  void PutUint64(uint64_t value);
  void PutDouble(double value);
  void PutString(const std::string& value);
 private:
  typedef int DataType;
  // Main constructor
  void WriteAndCheck(DataType value_type, const void* value);
  void CloseWriter();
 private:
  //Fields
  bool has_opened_subcontainer_;
  MessageWriter* parent_writer_;
  DBusMessageIter iterator_;

  // Disallow copy and assign
  MessageWriter(const MessageWriter& other);
  MessageWriter& operator=(const MessageWriter& other);
};

MessageRef MethodCall(const char* bus_name,
                      const char* path,
                      const char* interface,
                      const char* method);
MessageRef Signal(const char  *path,
                  const char  *interface,
                  const char  *name);
} // namespace dbus

#include "dbus/dbus_message_inl.h"

#endif // DBUS_DBUS_MESSAGE_H
