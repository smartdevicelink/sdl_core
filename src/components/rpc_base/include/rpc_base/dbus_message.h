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

class Message {
 public:
  Message(DBusMessage* message);
  Message(const Message& that);
  static Message MethodCall(const char* bus_name,
                            const char* path,
                            const char* interface,
                            const char* method);
  static Message Signal(const char  *path,
                        const char  *interface,
                        const char  *name);
  ~Message();
  Message& operator=(const Message& that);
  std::string GetInterface() const;
  std::string GetMember() const;
 protected:
  DBusMessage* raw_message_;
};

class MessageIterator {
 public:
  MessageIterator(DBusMessage* message);
  bool IsNull() const;
  bool IsBool() const;
  bool GetBool();
  bool IsInt32() const;
  int32_t GetInt32();
  void PutInt32(int32_t value);
  bool IsDouble() const;
  double GetDouble();
  void PutDouble(double value);
  bool IsString() const;
  std::string GetString();
  void PutString(const std::string& value);
  void PutString(const char* value);
  bool IsArray() const;
  MessageIterator GetArrayReadMessageIterator();
  MessageIterator GetArrayWriteMessageIterator(
      const char* item_type_signature);
  bool IsStruct() const;
  MessageIterator GetStructReadMessageIterator();
  MessageIterator GetStructWriteMessageIterator();
 private:
  MessageIterator();
  int GetCurrentArgType() const;
  DBusMessageIter iterator_;
};

} // namespace dbus

#endif // DBUS_DBUS_MESSAGE_H
