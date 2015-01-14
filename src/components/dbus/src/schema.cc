/**
 * Copyright (c) 2013, Ford Motor Company
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
#include "dbus/schema.h"

#include <algorithm>
#include <functional>

namespace dbus {

struct Description {
  MessageId id;
  MessageName name;
  MessageType type;
  ListArgs args;
};

struct IsId : public std::unary_function<const Description*, bool> {
  explicit IsId(MessageId id)
      : id_(id) {
  }
  bool operator()(const Description* desc) {
    return desc->id == id_;
  }
 private:
  MessageId id_;
};

struct IsName : public std::unary_function<const Description*, bool> {
  explicit IsName(const MessageName& name)
      : name_(name) {
  }
  bool operator()(const Description* desc) {
    return desc->name == name_;
  }
 private:
  MessageName name_;
};

struct IsIdType : public std::unary_function<const Description*, bool> {
  explicit IsIdType(const MessageId& id, const MessageType& type)
      : id_(id),
        type_(type) {
  }
  bool operator()(const Description* desc) {
    return desc->id == id_ && desc->type == type_;
  }
 private:
  MessageId id_;
  MessageType type_;
};

struct IsNameType : public std::unary_function<const Description*, bool> {
  explicit IsNameType(const MessageName& name, const MessageType& type)
      : name_(name),
        type_(type) {
  }
  bool operator()(const Description* desc) {
    return desc->name == name_ && desc->type == type_;
  }
 private:
  MessageName name_;
  MessageType type_;
};

DBusSchema::DBusSchema(const MessageDescription** array) {
  const MessageDescription** msg = array;
  while (*msg != NULL) {
    Description *desc = new Description();
    desc->id = (*msg)->function_id;
    desc->name = std::make_pair((*msg)->interface, (*msg)->name);
    desc->type = (*msg)->message_type;
    const ParameterDescription** param;
    param = (*msg)->parameters;
    while (*param != NULL) {
      desc->args.push_back(*param);
      param++;
    }
    msgs_.push_back(desc);
    msg++;
  }
}

MessageName DBusSchema::getMessageName(MessageId id) const {
  Messages::const_iterator it = std::find_if(msgs_.begin(), msgs_.end(),
                                             IsId(id));
  if (msgs_.end() != it) {
    return (*it)->name;
  }
  return MessageName();
}

MessageId DBusSchema::getMessageId(const MessageName& name) const {
  Messages::const_iterator it = std::find_if(msgs_.begin(), msgs_.end(),
                                             IsName(name));
  if (msgs_.end() != it) {
    return (*it)->id;
  }
  return hmi_apis::FunctionID::eType::INVALID_ENUM;
}

ListArgs DBusSchema::getListArgs(MessageId id, MessageType type) const {
  Messages::const_iterator it = std::find_if(msgs_.begin(), msgs_.end(),
                                             IsIdType(id, type));
  if (msgs_.end() != it) {
    return (*it)->args;
  }
  return ListArgs();
}

ListArgs DBusSchema::getListArgs(const MessageName& name,
                                 MessageType type) const {
  Messages::const_iterator it = std::find_if(msgs_.begin(), msgs_.end(),
                                             IsNameType(name, type));
  if (msgs_.end() != it) {
    return (*it)->args;
  }
  return ListArgs();
}

}  // namespace dbus_schema
