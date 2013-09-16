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
#ifndef SRC_COMPONENTS_DBUS_SCHEMA_INCLUDE_DBUS_SCHEMA_SCHEMA_H_
#define SRC_COMPONENTS_DBUS_SCHEMA_INCLUDE_DBUS_SCHEMA_SCHEMA_H_

#include <string>
#include <vector>
#include <utility>

#include "dbus/ford_message_descriptions.h"
#include "interfaces/HMI_API.h"

namespace dbus_schema {

using ford_message_descriptions::ParameterDescription;
using ford_message_descriptions::MessageDescription;

struct Description;

typedef hmi_apis::FunctionID::eType MessageId;
typedef hmi_apis::messageType::eType MessageType;
typedef std::pair<std::string, std::string> MessageName;
typedef std::vector<const ParameterDescription*> ListArgs;
typedef std::vector<const Description*> Messages;

class DBusSchema {
 public:
  DBusSchema();

  MessageName getMessageName(MessageId id);
  MessageId getMessageId(MessageName name);
  ListArgs getListArgs(MessageId id, MessageType type);
  ListArgs getListArgs(MessageName name, MessageType type);

 private:
  Messages msgs_;
};

}  // namespace dbus_schema

#endif  // SRC_COMPONENTS_DBUS_SCHEMA_INCLUDE_DBUS_SCHEMA_SCHEMA_H_
