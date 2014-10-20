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

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <vector>

#include "model/function.h"
#include "model/composite_type.h"
#include "model/type_registry.h"
#include "utils/stl_utils.h"

namespace pugi {
class xml_node;
}  // namespace pugi

namespace codegen {
class API;
class BuiltinTypeRegistry;
class Interface;
class ModelFilter;

/*
 * An interface found in XML document. Holds a registry of all the
 * structs, enums and functions found in interface definition
 */
class Interface {
 public:
  // Types
  typedef std::map<std::string, FunctionMessage*> MessagesMap;
  typedef std::vector<Function> FunctionsList;
  typedef std::vector<const Request*> RequestList;
  typedef std::vector<const Response*> ResponseList;
  typedef std::vector<const Notification*> NotificationList;
  typedef TypeRegistry::EnumList EnumList;
  typedef TypeRegistry::StructList StructList;
  typedef TypeRegistry::TypedefList TypedefList;
 public:
  // Methods
  Interface(const API* api,
            bool auto_generate_function_ids,
            BuiltinTypeRegistry* builtin_type_registry,
            const ModelFilter* model_filter);
  ~Interface();
  // API this interface belongs to
  const API& api() const;
  // Name of the interface
  const std::string& name() const;
  // List of all functions (requests and appropriate responses) in the interface
  const FunctionsList& functions() const;
  // List of all requests in the interface
  RequestList all_requests() const;
  // List of all the responses (including generic) in the interface
  ResponseList all_responses() const;
  // List of all the notifications in the interface
  const NotificationList& notifications() const;
  // List of generic responses that are not connected to particular request
  const ResponseList& generic_responses() const;
  // List of all the enums defined in the interface (exept FunctionID special purpose enum)
  const EnumList& enums() const;
  // List of all structs defined in the interface
  const StructList& structs() const;
  // List of all typedefs defined in the interface
  const TypedefList& typedefs() const;
  // Special-purpose enum that assigns numerical ID's for all API messages
  const Enum* function_id_enum() const;

  // Follows parsed |xml| document validating and constructin type tree
  bool init(const pugi::xml_node& xml);

  // Finds a type with |name| defined in this interface
  const Type* GetNamedType(const std::string& name) const;

 private:
  // Methods
  // Finds (or creates, depending on generation prefs) function id
  // enum constant given |function_id|
  const Enum::Constant* GetFunctionIdEnumConstant(
      const std::string& function_id);
  // Find and add all the functions from the given |xml_interafce|
  bool AddFunctions(const pugi::xml_node& xml_interface);
  // Validate and add single function message
  bool AddFunctionMessage(MessagesMap* list,
                          FunctionMessage::MessageType message_type,
                          const pugi::xml_node& xml_message);
  // Validate and add all the function message parameters
  bool AddFunctionMessageParameters(FunctionMessage* function_message,
                                    const pugi::xml_node& xml_message);
  bool SeparateFunctionMessages();
 private:
  // Fields
  std::string name_;
  const API* api_;
  BuiltinTypeRegistry* builtin_type_registry_;
  const ModelFilter* model_filter_;
  bool auto_generate_function_ids_;
  Enum function_ids_enum_;
  TypeRegistry type_registry_;
  MessagesMap requests_;
  utils::StdMapDeleter<MessagesMap> requests_deleter_;
  MessagesMap responses_;
  utils::StdMapDeleter<MessagesMap> responses_deleter_;
  MessagesMap notifications_;
  utils::StdMapDeleter<MessagesMap> notifications_deleter_;
  FunctionsList functions_list_;
  ResponseList generic_responses_list_;
  NotificationList notifications_list_;
 private:
  DISALLOW_COPY_AND_ASSIGN(Interface);
};

}  // namespace codegen

#endif /* INTERFACE_H_ */
