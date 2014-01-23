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

#include "model/interface.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "model/constant.h"
#include "model/scope.h"
#include "pugixml.hpp"
#include "utils/safeformat.h"
#include "utils/stl_utils.h"
#include "utils/xml_utils.h"

using std::cerr;
using std::endl;
using std::make_pair;
using std::pair;
using std::set;
using std::string;
using typesafe_format::format;
using typesafe_format::strmfmt;

namespace codegen {

Interface::Interface(BuiltinTypeRegistry* builtin_type_registry)
    : builtin_type_registry_(builtin_type_registry),
      type_registry_(builtin_type_registry_),
      requests_deleter_(&requests_),
      responses_deleter_(&responses_),
      notifications_deleter_(&notifications_) {
}

Interface::~Interface() {
}

bool codegen::Interface::init(const pugi::xml_node& xml) {
  name_ = xml.attribute("name").value();
  if (name_.empty()) {
    std::cerr << "Interface must have 'name' attribute specified" << '\n';
  }
  if (!type_registry_.init(xml) || !AddFunctions(xml))
    return false;
  return true;
}

const std::string& Interface::name() const {
  return name_;
}

const Interface::FunctionsList& Interface::functions() const {
  return functions_list_;
}

const Interface::NotificationList& Interface::notifications() const {
  return notifications_list_;
}

const Interface::ResponseList& Interface::generic_responses() const {
  return generic_responses_list_;
}

const Interface::EnumList& Interface::enums() const {
  return type_registry_.enums();
}

const Interface::StructList& Interface::structs() const {
  return type_registry_.structs();
}

const Enum* Interface::function_id_enum() const {
  return type_registry_.GetFunctionIDEnum();
}

bool codegen::Interface::AddFunctions(const pugi::xml_node& xml_interface) {
  for (pugi::xml_node i = xml_interface.child("function"); i; i =
      i.next_sibling("function")) {
    std::string message_type_str = i.attribute("messagetype").value();
    FunctionMessage::MessageType message_type =
        FunctionMessage::MessageTypeFromLiteral(message_type_str);
    MessagesMap* map_for_message = NULL;
    switch (message_type) {
      case FunctionMessage::kRequest: {
        map_for_message = &requests_;
        break;
      }
      case FunctionMessage::kResponse: {
        map_for_message = &responses_;
        break;
      }
      case FunctionMessage::kNotification: {
        map_for_message = &notifications_;
        break;
      }
      default: {
        std::cerr << "Invalid message type: " << message_type_str << std::endl;
        return false;
      }
    }
    if (!AddFunctionMessage(map_for_message, message_type, i)) {
      return false;
    }
  }
  return SeparateFunctionMessages();
}

bool Interface::AddFunctionMessage(MessagesMap* list,
                                   FunctionMessage::MessageType message_type,
                                   const pugi::xml_node& xml_message) {
  std::string name = xml_message.attribute("name").value();
  std::string func_id_str = xml_message.attribute("functionID").value();
  if (name.empty()) {
    std::cerr << "Message with empty function name found\n";
    return false;
  }
  if (func_id_str.empty()) {
    std::cerr << "Message with empty functionID found\n";
    return false;
  }
  const Enum* func_id_enum = type_registry_.GetFunctionIDEnum();
  if (!func_id_enum) {
    return false;
  }
  const Enum::Constant* func_id = static_cast<const Enum::Constant*>(func_id_enum
      ->ConstantFor(func_id_str));
  if (!func_id) {
    std::cerr << "Function " << name << " has invalid functionID: "
              << func_id_str << std::endl;
    return false;
  }
  Description description = CollectDescription(xml_message);
  Scope scope = ScopeFromLiteral(xml_message.attribute("scope").value());
  pair<MessagesMap::iterator, bool> res = list->insert(
      make_pair(
          name,
          new FunctionMessage(this, name, func_id, message_type, description,
                              scope)));
  if (!res.second) {
    strmfmt(cerr, "Duplicate function message {0} of type {1}", name,
            message_type) << endl;
    return false;
  }
  return AddFunctionMessageParameters(res.first->second, xml_message);
}

bool Interface::AddFunctionMessageParameters(
    FunctionMessage* function_message, const pugi::xml_node& xml_message) {
  for (pugi::xml_node i = xml_message.child("param"); i;
      i = i.next_sibling("param")) {
    std::string name = i.attribute("name").value();
    if (name.empty()) {
      strmfmt(cerr, "Function message {0}, has parameter with empty name",
              function_message->name()) << endl;
      return false;
    }
    const Type* type = NULL;
    if (!type_registry_.GetType(i, &type)) {
      return false;
    }
    const Constant* default_value = NULL;
    std::string default_value_literal = i.attribute("defvalue").value();
    if (!default_value_literal.empty()) {
      const ConstantsCreator* const_creator = type->SupportsConstants();
      if (!const_creator) {
        strmfmt(cerr, "Can not use {0} as a default value",
                default_value_literal) << endl;
        return false;
      }
      default_value = const_creator->ConstantFor(default_value_literal);
      if (!default_value) {
        strmfmt(cerr, "Incorrect default value: {0}",
               default_value_literal) << '\n';
        return false;
      }
    }
    bool is_mandatory = TypeRegistry::IsMandatoryParam(i);
    Description description = CollectDescription(i);
    Scope scope = ScopeFromLiteral(i.attribute("scope").value());
    Platform platform = PlatformFromLiteral(i.attribute("platform").value());
    if (!function_message->AddParameter(name, type, default_value, is_mandatory,
                                        description, scope, platform)) {
      return false;
    }
  }
  return true;
}

bool Interface::SeparateFunctionMessages() {
  set<string> requests = utils::MapKeys(requests_);
  set<string> response_names = utils::MapKeys(responses_);
  // Generic responses are responses that do not have matching request
  set<string> responses;
  std::set_difference(response_names.begin(), response_names.end(),
                      requests.begin(), requests.end(),
                      std::inserter(responses, responses.begin()));
  for (set<string>::iterator i = requests.begin(), end = requests.end();
      i != end; ++i) {
    const string& name = *i;
    assert(requests_.find(name) != requests_.end());
    const FunctionMessage* request = requests_.find(name)->second;
    MessagesMap::iterator have_response = responses_.find(name);
    if (have_response == responses_.end()) {
      strmfmt(cerr, "Request {0} doesn't have matching response", name);
      return false;
    }
    const FunctionMessage* response = have_response->second;
    functions_list_.push_back(Function(request, response));
  }
  for (set<string>::iterator i = responses.begin(), end = responses.end();
      i != end; ++i) {
    const std::string& name = *i;
    assert(responses_.find(name) != responses_.end());
    generic_responses_list_.push_back(responses_.find(name)->second);
  }
  for (MessagesMap::iterator i = notifications_.begin(), end = notifications_
      .end(); i != end; ++i) {
    notifications_list_.push_back(i->second);
  }
  return true;
}

}  // namespace codegen
