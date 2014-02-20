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

#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <map>
#include <string>

#include "model/constant.h"
#include "model/scope.h"
#include "model/type.h"
#include "utils/common_types.h"
#include "utils/macro.h"
#include "utils/stl_utils.h"

namespace codegen {
class Interface;

/*
 * Class representing single RPC message and it's parameters,
 * can be request, response or notification.
 * Message parameters are passed as structures so message is based
 * on it's implementation
 */
class FunctionMessage: public Struct {
 public:
  // Types
  enum MessageType {
    kRequest,
    kResponse,
    kNotification,
    kUnknownMessageType
  };
  // Procudes string given MessageType to be used in class names
  static const char* MessageTypeToString(MessageType message_type);

  // Message parameters are passed as structs so every parameter
  // is a struct field
  typedef Field Parameter;
  typedef FieldsList ParametersList;
 public:
  // Methods

  // Returns message type named by string literal.
  // Return kUnknownMessageType if literal names unknown message type
  static MessageType MessageTypeFromLiteral(const std::string& literal);
  // Creates Function message and associates it with given |interface|
  FunctionMessage(const Interface* interface, const std::string& name,
                  const Enum::Constant* id, MessageType message_type,
                  const Description& description, Scope scope);
  ~FunctionMessage();
  // Returns function ID which is one of the "FunctionID" enum constants
  const Enum::Constant* id() const;
  // List of function parameters
  const ParametersList& parameters() const;
  // Constructs message parameter and stores it
  bool AddParameter(const std::string& name, const Type* type,
                    const Constant* default_value, bool is_mandatory,
                    const Description& description, Scope scope,
                    Platform platform);
  const Interface* interface() const;

private:
  const Interface* interface_;
  const Enum::Constant* id_;
  MessageType message_type_;
  DISALLOW_COPY_AND_ASSIGN(FunctionMessage);
};

typedef FunctionMessage Request;
typedef FunctionMessage Response;
typedef FunctionMessage Notification;

/*
 * A container that bounds a request type to it's corresponding response
 * type
 */
class Function {
 public:
  Function(const Request* request, const Response* response);

  const Request& request() const {
    return *request_;
  }

  const Response& response() const {
    return *response_;
  }

 private:
  const Request* request_;
  const Response* response_;
};

}  // namespace codegen

#endif /* FUNCTION_H_ */
