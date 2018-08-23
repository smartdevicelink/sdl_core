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

#include "cppgen/cpp_interface_code_generator.h"

#include "cppgen/generator_preferences.h"
#include "cppgen/handler_interface.h"
#include "cppgen/message_factory_function.h"
#include "cppgen/message_interface.h"
#include "cppgen/module_manager.h"
#include "model/interface.h"
#include "model/type_registry.h"

namespace codegen {

CppInterfaceCodeGenerator::CppInterfaceCodeGenerator(
    const Interface* interface,
    const TypePreferences* preferences,
    ModuleManager* module_manager)
    : interface_(interface),
      preferences_(preferences),
      module_manager_(module_manager),
      declaration_generator_(preferences, module_manager_),
      definition_generator_(preferences, module_manager_) {
}

CppInterfaceCodeGenerator::~CppInterfaceCodeGenerator() {
}

void CppInterfaceCodeGenerator::GenerateCode() {
  GenerateEnums();
  GenerateTypedefs();
  GenerateStructs();
  bool type_only_interface =
      interface_->function_id_enum()->constants().empty();
  if (!type_only_interface) {
    GenerateFunctions();
    GenerateResponses();
    GenerateNotifications();
    GenerateHandlerInterfaces();
    GenerateMessageBaseClasses();
    GenerateMessageFactories();
  }
}

void CppInterfaceCodeGenerator::GenerateEnums() {
  const Interface::EnumList& enums = interface_->enums();
  for (Interface::EnumList::const_iterator i = enums.begin(), end = enums.end();
      i != end; ++i) {
    const Enum* e = *i;
    declaration_generator_.GenerateCodeForEnum(e);
    definition_generator_.GenerateCodeForEnum(e);
  }

  const Enum* func_id_enum = interface_->function_id_enum();
  // Not all interfaces declare functions, avoid empty enum generation
  if (!func_id_enum->constants().empty()) {
    declaration_generator_.GenerateCodeForEnum(func_id_enum);
    definition_generator_.GenerateCodeForEnum(func_id_enum);
  }
}

void CppInterfaceCodeGenerator::GenerateStructs() {
  const Interface::StructList& structs = interface_->structs();
  for (Interface::StructList::const_iterator i = structs.begin(), end = structs
      .end(); i != end; ++i) {
    const Struct* s = *i;
    declaration_generator_.GenerateCodeForStruct(s);
    definition_generator_.GenerateCodeForStruct(s);
  }
}

void CppInterfaceCodeGenerator::GenerateTypedefs() {
  const Interface::TypedefList& typedefs = interface_->typedefs();
  for (Interface::TypedefList::const_iterator i = typedefs.begin(), end =
      typedefs.end(); i != end; ++i) {
    const Typedef* tdef = *i;
    declaration_generator_.GenerateCodeForTypedef(tdef);
  }
}

void CppInterfaceCodeGenerator::GenerateFunctions() {
  const Interface::FunctionsList& functions = interface_->functions();
  for (Interface::FunctionsList::const_iterator i = functions.begin(), end =
      functions.end(); i != end; ++i) {
    const Function& function = *i;
    declaration_generator_.GenerateCodeForFunction(function);
    definition_generator_.GenerateCodeForFunction(function);
  }
}

void CppInterfaceCodeGenerator::GenerateResponses() {
  const Interface::ResponseList& responses = interface_->generic_responses();
  for (Interface::ResponseList::const_iterator i = responses.begin(), end =
      responses.end(); i != end; ++i) {
    const Response& response = **i;
    declaration_generator_.GenerateCodeForResponse(response);
    definition_generator_.GenerateCodeForResponse(response);
  }
}

void CppInterfaceCodeGenerator::GenerateNotifications() {
  const Interface::NotificationList& notifications =
      interface_->notifications();
  for (Interface::NotificationList::const_iterator i = notifications.begin(),
      end = notifications.end(); i != end; ++i) {
    const Notification& notification = **i;
    declaration_generator_.GenerateCodeForNotification(notification);
    definition_generator_.GenerateCodeForNotification(notification);
  }
}

void CppInterfaceCodeGenerator::GenerateHandlerInterfaces() {
  CppFile& handler_header = module_manager_->HeaderForInterface();
  CppFile& handler_source = module_manager_->SourceForInterface();
  HandlerInterface notif_handler(
        FunctionMessage::kNotification, interface_, &handler_header);
  notif_handler.Declare(&handler_header.notifications_ns().os());
  notif_handler.Define(&handler_source.notifications_ns().os());

  HandlerInterface req_handler(
        FunctionMessage::kRequest, interface_, &handler_header);
  req_handler.Declare(&handler_header.requests_ns().os());
  req_handler.Define(&handler_source.requests_ns().os());

  HandlerInterface resp_handler(
        FunctionMessage::kResponse, interface_, &handler_header);
  resp_handler.Declare(&handler_header.responses_ns().os());
  resp_handler.Define(&handler_source.responses_ns().os());
}

void CppInterfaceCodeGenerator::GenerateMessageBaseClasses() {
  CppFile& message_base_header = module_manager_->HeaderForInterface();
  CppFile& message_base_source = module_manager_->SourceForInterface();
  MessageInterface notif_message(interface_, FunctionMessage::kNotification);
  notif_message.Declare(&message_base_header.notifications_ns().os());
  notif_message.Define(&message_base_source.notifications_ns().os());

  MessageInterface request_message(interface_, FunctionMessage::kRequest);
  request_message.Declare(&message_base_header.requests_ns().os());
  request_message.Define(&message_base_source.requests_ns().os());

  MessageInterface response_message(interface_, FunctionMessage::kResponse);
  response_message.Declare(&message_base_header.responses_ns().os());
  response_message.Define(&message_base_source.responses_ns().os());
}

void CppInterfaceCodeGenerator::GenerateMessageFactories() {
  CppFile& factories_header = module_manager_->HeaderForInterface();
  CppFile& factories_source = module_manager_->SourceForInterface();

  MessageFactoryFunction::SerializationType ser_types[2];
  size_t ser_types_count = 0;
  if (preferences_->generate_json) {
    ser_types[ser_types_count++] = MessageFactoryFunction::kJson;
  }

  for (size_t i = 0; i < ser_types_count; ++i) {
    MessageFactoryFunction request_factory(interface_,
                                           ser_types[i],
                                           FunctionMessage::kRequest);
    request_factory.Declare(&factories_header.requests_ns().os(), true);
    request_factory.Define(&factories_source.requests_ns().os(), true);
    MessageFactoryFunction response_factory(interface_,
                                            ser_types[i],
                                            FunctionMessage::kResponse);
    response_factory.Declare(&factories_header.responses_ns().os(), true);
    response_factory.Define(&factories_source.responses_ns().os(), true);
    MessageFactoryFunction notification_factory(
          interface_,
          ser_types[i],
          FunctionMessage::kNotification);
    notification_factory.Declare(&factories_header.notifications_ns().os(),
                                 true);
    notification_factory.Define(&factories_source.notifications_ns().os(), true);
  }

}

}  // namespace codegen
