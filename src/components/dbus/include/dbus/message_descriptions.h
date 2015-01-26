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
#ifndef SRC_COMPONENTS_DBUS_INCLUDE_DBUS_MESSAGE_DESCRIPTIONS_H_
#define SRC_COMPONENTS_DBUS_INCLUDE_DBUS_MESSAGE_DESCRIPTIONS_H_

#include <stdint.h>
#include <stdlib.h>
#include "interfaces/HMI_API.h"

namespace ford_message_descriptions {

enum ParameterType {Integer, Boolean, Float, String, Struct, Enum, Array};

struct ParameterDescription {
  const char* name;
  const ParameterType type;
  const bool obligatory; // TODO (dchmerev@luxoft.com): rename to 'mandatory' as in HMI API description
};

struct ArrayDescription {
  const ParameterDescription base;
  const ParameterDescription* element;
  const char* element_dbus_signature;
};

struct StructDescription {
  const ParameterDescription base;
  const ParameterDescription** parameters;
};

struct MessageDescription {
  const char* interface;
  const char* name;
  const hmi_apis::messageType::eType message_type;
  const hmi_apis::FunctionID::eType function_id;
  const ParameterDescription** parameters;
};

extern const MessageDescription* message_descriptions[];

}  // namespace ford_message_descriptions

#endif  // SRC_COMPONENTS_DBUS_INCLUDE_DBUS_MESSAGE_DESCRIPTIONS_H_

