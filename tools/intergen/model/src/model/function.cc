/**
 * Copyright (c) 2014, Ford Motor Company
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

#include "model/function.h"

#include <cassert>
#include <iostream>

#include "utils/safeformat.h"

using std::cerr;
using std::endl;
using typesafe_format::strmfmt;

namespace codegen {

// static
const char* FunctionMessage::MessageTypeToString(
    FunctionMessage::MessageType message_type) {
  switch (message_type) {
    case FunctionMessage::kRequest:
      return "request";
    case FunctionMessage::kResponse:
      return "response";
    case FunctionMessage::kNotification:
      return "notification";
    default:
      return "";
  }
}

// static
FunctionMessage::MessageType FunctionMessage::MessageTypeFromLiteral(
    const std::string& literal) {
  if (literal == "request") {
    return kRequest;
  } else if (literal == "response") {
    return kResponse;
  } else if (literal == "notification") {
    return kNotification;
  } else {
    return kUnknownMessageType;
  }
}

FunctionMessage::FunctionMessage(const Interface* interface,
                                 const std::string& name,
                                 const Enum::Constant* id,
                                 MessageType message_type,
                                 const Description& description, Scope scope)
    : Struct(interface, name, NULL, scope, description), // Functions can not be frankenmaps
      interface_(interface),
      id_(id),
      message_type_(message_type) {
}

FunctionMessage::~FunctionMessage() {
}

const Enum::Constant* FunctionMessage::id() const {
  return id_;
}

const FunctionMessage::ParametersList& FunctionMessage::parameters() const {
  return fields();
}

bool FunctionMessage::AddParameter(const std::string& param_name, const Type* type,
                                   const Constant* default_value,
                                   bool is_mandatory,
                                   const Description& description, Scope scope,
                                   Platform platformn) {
  return AddField(type, param_name, is_mandatory, scope, default_value, description, platformn);
}
const Interface* FunctionMessage::interface() const {
    return interface_;
}

Function::Function(const Request* request, const Response* response)
    : request_(request),
      response_(response) {
    assert(request_);
    assert(response_);
}


}  // namespace codegen
