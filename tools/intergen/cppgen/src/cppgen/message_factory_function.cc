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

#include "cppgen/message_factory_function.h"

#include "cppgen/literal_generator.h"
#include "cppgen/naming_convention.h"
#include "model/interface.h"
#include "utils/safeformat.h"
#include "utils/string_utils.h"

using typesafe_format::format;
using typesafe_format::strmfmt;

namespace codegen {

MessageFactoryFunction::MessageFactoryFunction(
    const Interface* interface,
    SerializationType serialization_type,
    FunctionMessage::MessageType factory_type)
    : CppFunction("",
                  "NewFromJson",
                  Capitalize(
                    FunctionMessage::MessageTypeToString(
                      factory_type)) + "*"),
      interface_(interface),
      factory_type_(factory_type) {
  Add(MessageFactoryFunction::Parameter(
        "json",
        "const Json::Value*"));
  Add(MessageFactoryFunction::Parameter("function_id", "FunctionID"));
}

void MessageFactoryFunction::DefineBody(std::ostream* os) const {
  *os << "switch (function_id) {\n";
  {
    Indent indent(*os);
    switch(factory_type_) {
      case FunctionMessage::kRequest: {
        DefineCases(os, interface_->all_requests());
        break;
      }
      case FunctionMessage::kResponse: {
        DefineCases(os, interface_->all_responses());
        break;
      }
      case FunctionMessage::kNotification: {
        DefineCases(os, interface_->notifications());
        break;
      }
    }
  }
  *os << "}\n";
}

void MessageFactoryFunction::DefineCases(
    std::ostream* os,
    const MessageList& functions) const {
  for (MessageList::const_iterator i = functions.begin(),
       end = functions.end(); i != end; ++i) {
    const FunctionMessage* message = *i;
    strmfmt(*os, "case {0}: return new {1}({2});\n",
            LiteralGenerator(*message->id()).result(),
            message->name(),
            parameters_[0].name);
  }
  *os << "default: return NULL;\n";
}

} // namespace codegen
