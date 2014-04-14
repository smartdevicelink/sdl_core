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

#include "cppgen/message_interface.h"

#include "cppgen/naming_convention.h"
#include "model/interface.h"
#include "utils/safeformat.h"

using typesafe_format::strmfmt;

namespace codegen {

InterfaceStringIdMethod::InterfaceStringIdMethod(
    const MessageInterface* message_interface,
    const Interface* interface)
    : CppClass::Method(message_interface, CppClass::kPublic,
         "interface_string_id", "const char*", kConst),
      interface_(interface) {
}

void InterfaceStringIdMethod::DefineBody(std::ostream* os) const {
  strmfmt(*os, "return \"{0}\";\n", interface_->name());
}

MessageInterface::MessageInterface(const Interface* interface,
                                   FunctionMessage::MessageType message_type)
  : CppClass(Capitalize(FunctionMessage::MessageTypeToString(message_type))),
    constructor_(this, CppClass::kPublic, name(), "", Method::kExplicit),
    handle_with_method_(this, kPublic,
                        "HandleWith", "void",
                        Method::kVirtual|Method::kAbstract),
    interface_string_id_method_(this, interface){
  std::string superclass_name = "rpc::" + name() + "Base";
  Add(Superclass(superclass_name, kPublic));
  constructor_.Add(Method::Parameter("init_state", "InitializationState"));
  constructor_.Add(Method::Initializer(superclass_name, "init_state"));

  handle_with_method_.Add(Method::Parameter(
                            "handler", "Handler*"));

  methods_.push_back(&constructor_);
  methods_.push_back(&handle_with_method_);
  methods_.push_back(&interface_string_id_method_);
}

const CppClass::MethodsList& MessageInterface::methods() {
  return methods_;
}

} // namespace codegen
