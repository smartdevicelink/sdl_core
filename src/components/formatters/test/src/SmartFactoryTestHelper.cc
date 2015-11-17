/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "SmartFactoryTestHelper.h"

using namespace test::components::formatters;

template<>
const EnumConversionHelper<TestType::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::TestType::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::TestType::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<TestType::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::TestType::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::TestType::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<TestType::eType>::cstring_values_[] = {
    "APPLICATION_NOT_REGISTERED", "SUCCESS", "TOO_MANY_PENDING_REQUESTS",
    "REJECTED", "INVALID_DATA", "OUT_OF_MEMORY", "ABORTED", "USER_DISALLOWED",
    "GENERIC_ERROR", "DISALLOWED" };

template<>
const TestType::eType EnumConversionHelper<TestType::eType>::enum_values_[] = {
    test::components::formatters::TestType::APPLICATION_NOT_REGISTERED,
    test::components::formatters::TestType::SUCCESS,
    test::components::formatters::TestType::TOO_MANY_PENDING_REQUESTS,
    test::components::formatters::TestType::REJECTED,
    test::components::formatters::TestType::INVALID_DATA,
    test::components::formatters::TestType::OUT_OF_MEMORY,
    test::components::formatters::TestType::ABORTED,
    test::components::formatters::TestType::USER_DISALLOWED,
    test::components::formatters::TestType::GENERIC_ERROR,
    test::components::formatters::TestType::DISALLOWED };

template<>
const EnumConversionHelper<FunctionIdTest::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::FunctionIdTest::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::FunctionIdTest::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<FunctionIdTest::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::FunctionIdTest::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::FunctionIdTest::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<FunctionIdTest::eType>::cstring_values_[] =
    { "Function1", "Function2", "Function3" };

template<>
const FunctionIdTest::eType EnumConversionHelper<FunctionIdTest::eType>::enum_values_[] =
    { test::components::formatters::FunctionIdTest::Function1,
        test::components::formatters::FunctionIdTest::Function2,
        test::components::formatters::FunctionIdTest::Function3 };

template<>
const EnumConversionHelper<MessageTypeTest::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::MessageTypeTest::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::MessageTypeTest::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<MessageTypeTest::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::MessageTypeTest::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::MessageTypeTest::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<MessageTypeTest::eType>::cstring_values_[] =
    { "request", "response", "notification" };

template<>
const MessageTypeTest::eType EnumConversionHelper<MessageTypeTest::eType>::enum_values_[] =
    { test::components::formatters::MessageTypeTest::request,
        test::components::formatters::MessageTypeTest::response,
        test::components::formatters::MessageTypeTest::notification };

CSmartFactoryTest::CSmartFactoryTest()
    : CSmartFactory<FunctionIdTest::eType, MessageTypeTest::eType,
        StructIdentifiersTest::eType>() {
  TStructsSchemaItems struct_schema_items;
  InitStructSchemes(struct_schema_items);
  std::set<FunctionIdTest::eType> function_id_items;
  function_id_items.insert(FunctionIdTest::Function1);
  function_id_items.insert(FunctionIdTest::Function2);
  function_id_items.insert(FunctionIdTest::Function3);

  std::set<MessageTypeTest::eType> message_type_items;
  message_type_items.insert(MessageTypeTest::request);
  message_type_items.insert(MessageTypeTest::response);
  message_type_items.insert(MessageTypeTest::notification);
  message_type_items.insert(MessageTypeTest::error_response);
  InitFunctionSchemes(struct_schema_items, function_id_items,
                      message_type_items);
}

void CSmartFactoryTest::InitStructSchemes(
    TStructsSchemaItems &struct_schema_items) {
  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_1 =
      InitStructSchemaItem_Common_1(struct_schema_items);
  struct_schema_items.insert(
      std::make_pair(StructIdentifiersTest::Common_1,
                     struct_schema_item_Common_1));
  structs_schemes_.insert(
      std::make_pair(StructIdentifiersTest::Common_1,
                     CSmartSchema(struct_schema_item_Common_1)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_2 =
      InitStructSchemaItem_Common_2();
  struct_schema_items.insert(
      std::make_pair(StructIdentifiersTest::Common_2,
                     struct_schema_item_Common_2));
  structs_schemes_.insert(
      std::make_pair(StructIdentifiersTest::Common_2,
                     CSmartSchema(struct_schema_item_Common_2)));
}

void CSmartFactoryTest::InitFunctionSchemes(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {
  CObjectSchemaItem::Members params_members;
  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[kCode] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[kMessage] = CObjectSchemaItem::SMember(
      CStringSchemaItem::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] =
      CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members),
                                 true);

  CSmartSchema error_response_schema(
      CObjectSchemaItem::create(root_members_map));

  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function1, MessageTypeTest::error_response),
          error_response_schema));
  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function1, MessageTypeTest::request),
          InitFunction_Function1_request(function_id_items,
                                         message_type_items)));
  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function1, MessageTypeTest::response),
          InitFunction_Function1_response(struct_schema_items,
                                          function_id_items,
                                          message_type_items)));

  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function2, MessageTypeTest::error_response),
          error_response_schema));
  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function2, MessageTypeTest::request),
          InitFunction_Function2_request(function_id_items,
                                         message_type_items)));
  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function2, MessageTypeTest::response),
          InitFunction_Function2_response(struct_schema_items,
                                          function_id_items,
                                          message_type_items)));

  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function3, MessageTypeTest::error_response),
          error_response_schema));
  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function3, MessageTypeTest::request),
          InitFunction_Function3_request(function_id_items,
                                         message_type_items)));
  functions_schemes_.insert(
      std::make_pair(
          SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>(
              FunctionIdTest::Function3, MessageTypeTest::response),
          InitFunction_Function3_response(struct_schema_items,
                                          function_id_items,
                                          message_type_items)));
}

CSmartSchema CSmartFactoryTest::InitFunction_Function1_request(
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {

  CObjectSchemaItem::Members schema_members;
  CObjectSchemaItem::Members params_members;

  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  root_members_map[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema CSmartFactoryTest::InitFunction_Function1_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {
  // Function parameter available.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(
      TSchemaItemParameter<bool>());

  CObjectSchemaItem::Members schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem,
                                                           true);

  CObjectSchemaItem::Members params_members;
  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[kCode] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  root_members_map[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema CSmartFactoryTest::InitFunction_Function2_request(
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {
  CObjectSchemaItem::Members schema_members;

  CObjectSchemaItem::Members params_members;
  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  root_members_map[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema CSmartFactoryTest::InitFunction_Function2_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {
  // Function parameter available.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(
      TSchemaItemParameter<bool>());

  CObjectSchemaItem::Members schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem,
                                                           true);

  CObjectSchemaItem::Members params_members;
  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[kCode] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  root_members_map[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema CSmartFactoryTest::InitFunction_Function3_request(
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {
  CObjectSchemaItem::Members schema_members;

  CObjectSchemaItem::Members params_members;
  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  root_members_map[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema CSmartFactoryTest::InitFunction_Function3_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionIdTest::eType> &function_id_items,
    const std::set<MessageTypeTest::eType> &message_type_items) {
  // Function parameter available.
  //
  // Must be true if VR is present and ready to communicate with SDL.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(
      TSchemaItemParameter<bool>());

  CObjectSchemaItem::Members schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem,
                                                           true);

  CObjectSchemaItem::Members params_members;
  params_members[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIdTest::eType>::create(function_id_items), true);
  params_members[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items),
      true);
  params_members[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  params_members[kCode] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  root_members_map[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

utils::SharedPtr<ISchemaItem> CSmartFactoryTest::InitStructSchemaItem_Common_1(
    TStructsSchemaItems &struct_schema_items) {
  // Struct member text.
  //
  // Text to display
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500),
      TSchemaItemParameter<std::string>());

  // Struct member image.
  //
  // Image struct
  utils::SharedPtr<ISchemaItem> image_SchemaItem =
      ProvideObjectSchemaItemForStruct(struct_schema_items,
                                       StructIdentifiersTest::Common_2);

  // Struct member position.
  //
  // Position to display item
  utils::SharedPtr<ISchemaItem> position_SchemaItem =
      TNumberSchemaItem<int32_t>::create(TSchemaItemParameter<int32_t>(1),
                                         TSchemaItemParameter<int32_t>(500),
                                         TSchemaItemParameter<int32_t>());
  CObjectSchemaItem::Members struct_members;
  struct_members["image"] = CObjectSchemaItem::SMember(image_SchemaItem, false);

  CObjectSchemaItem::Members schema_members;

  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, true);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem,
                                                          true);

  CObjectSchemaItem::Members root_members_map;
  root_members_map[""] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(struct_members), true);
  root_members_map[""] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schema_members), true);
  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> CSmartFactoryTest::InitStructSchemaItem_Common_2() {
  // Struct member text.
  //
  // Text to display
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500),
      TSchemaItemParameter<std::string>());
  // Struct member position.
  //
  // Position to display item
  utils::SharedPtr<ISchemaItem> position_SchemaItem =
      TNumberSchemaItem<int32_t>::create(TSchemaItemParameter<int32_t>(1),
                                         TSchemaItemParameter<int32_t>(500),
                                         TSchemaItemParameter<int32_t>());

  CObjectSchemaItem::Members schema_members;
  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, true);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem,
                                                          true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> CSmartFactoryTest::ProvideObjectSchemaItemForStruct(
    TStructsSchemaItems &struct_schema_items,
    const StructIdentifiersTest::eType struct_id) {
  const TStructsSchemaItems::const_iterator it = struct_schema_items.find(
      struct_id);
  if (it != struct_schema_items.end()) {
    return it->second;
  }
  return NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::create();
}

