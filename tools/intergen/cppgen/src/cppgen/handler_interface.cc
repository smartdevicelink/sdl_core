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

#include "cppgen/handler_interface.h"

#include <cassert>
#include <memory>

#include "cppgen/cpp_file.h"
#include "cppgen/naming_convention.h"
#include "model/interface.h"

namespace codegen {

HandlerInterface::HandlerInterface(FunctionMessage::MessageType type,
                                   const Interface* interface,
                                   CppFile* header_file)
    : CppClass("Handler"),
      type_(type),
      interface_(interface),
      header_file_(header_file),
      methods_deleter_(&methods_) {
  assert(interface);
  CollectMethods();
}

const CppClass::MethodsList& HandlerInterface::methods(){
  return methods_;
}

void HandlerInterface::CollectMethods() {
  switch(type_) {
    case FunctionMessage::kNotification: {
      AddFunctionMessageHandlers(interface_->notifications());
      break;
    }
    case FunctionMessage::kRequest: {
      const Interface::FunctionsList& functions =
          interface_->functions();
      AddFunctionMessageHandlers(interface_->all_requests());
      break;
    }
    case FunctionMessage::kResponse: {
      AddFunctionMessageHandlers(interface_->all_responses());
      break;
    }
    default: {
      assert(!"Unexpected function message type");
    }
  }
  std::unique_ptr<CppClass::Method> destructor(
        new CppClass::Method(this, CppClass::kPublic,
                             "~"+name(), "",
                             CppFunction::kVirtual));
  methods_.push_back(destructor.get());
  destructor.release();
}

void HandlerInterface::AddFunctionMessageHandlers(
    const FunctionMessages& function_messages) {
  for (FunctionMessages::const_iterator i = function_messages.begin(),
       end = function_messages.end(); i != end; ++i) {
    const FunctionMessage* func_msg = *i;
    Namespace& message_ns = header_file_->NamespaceByMessageType(type_);
    message_ns.ForwardDeclare(Namespace::ForwardDeclaration(
                              Namespace::ForwardDeclaration::kStruct,
                              func_msg->name()));
    std::unique_ptr<CppClass::Method> method(
        new CppClass::Method(
          this, CppClass::kPublic, "Handle" + func_msg->name(),
          "void", CppFunction::kVirtual | CppFunction::kAbstract));
    method->Add(CppClass::Method::Parameter(
                  "params", "const " + func_msg->name() + "&"));
    methods_.push_back(method.get());
    method.release();
  }
}

}  // namespace codegen
