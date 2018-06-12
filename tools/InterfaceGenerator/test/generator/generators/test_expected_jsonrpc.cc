/**
 * @file Test.cc
 * @brief Generated class Test source file.
 *
 * This class is a part of SmartObjects solution. It provides
 * factory functionallity which allows client to use SmartSchemas
 * in accordance with definitions from Test.xml file
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <map>
#include <set>

#include "Test.h"
#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"
#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"
#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CObjectSchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"
#include "SmartObjects/TNumberSchemaItem.hpp"
#include "SmartObjects/TSchemaItemParameter.hpp"

using namespace NsSmartDeviceLink::NsSmartObjects;

XXX::YYY::ZZZ::Test::Test()
 : CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType>() {
  TStructsSchemaItems struct_schema_items;
  InitStructSchemes(struct_schema_items);

  std::set<FunctionID::eType> function_id_items;


  std::set<messageType::eType> message_type_items;
  message_type_items.insert(messageType::request);
  message_type_items.insert(messageType::response);
  message_type_items.insert(messageType::notification);
  message_type_items.insert(messageType::error_response);

  InitFunctionSchemes(struct_schema_items, function_id_items, message_type_items);
}

Tstd::shared_ptr<ISchemaItem> XXX::YYY::ZZZ::Test::ProvideObjectSchemaItemForStruct(
    const TStructsSchemaItems &struct_schema_items,
    const StructIdentifiers::eType struct_id) {
  const TStructsSchemaItems::const_iterator it = struct_schema_items.find(struct_id);
  if (it != struct_schema_items.end()) {
    return it->second;
  }

  return NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::create();
}

void XXX::YYY::ZZZ::Test::InitStructSchemes(
    TStructsSchemaItems &struct_schema_items) {
  Tstd::shared_ptr<ISchemaItem> struct_schema_item_Struct2 = InitStructSchemaItem_Struct2(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Struct2, struct_schema_item_Struct2));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Struct2, CSmartSchema(struct_schema_item_Struct2)));

  Tstd::shared_ptr<ISchemaItem> struct_schema_item_Struct1 = InitStructSchemaItem_Struct1(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Struct1, struct_schema_item_Struct1));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Struct1, CSmartSchema(struct_schema_item_Struct1)));
}

void XXX::YYY::ZZZ::Test::InitFunctionSchemes(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kMessage] = CObjectSchemaItem::SMember(CStringSchemaItem::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  CSmartSchema error_response_schema(CObjectSchemaItem::create(root_members_map));

  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::val_1, messageType::error_response), error_response_schema));

  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::name1, messageType::request), InitFunction_name1_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::val_1, messageType::response), InitFunction_val_1_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::val_2, messageType::notification), InitFunction_val_2_notification(struct_schema_items, function_id_items, message_type_items)));
}

//------------- Functions schemes initialization -------------

CSmartSchema XXX::YYY::ZZZ::Test::InitFunction_name1_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Enum_new4::eType> Enum_new4_all_enum_values;
  Enum_new4_all_enum_values.insert(Enum_new4::_11);
  Enum_new4_all_enum_values.insert(Enum_new4::_22);

  std::set<Enum1::eType> param2_allowed_enum_subset_values;
  param2_allowed_enum_subset_values.insert(Enum1::name1);

  // Function parameter param1.
  //
  // Description Line1
  // Description Line2
  //
  // Design Line1
  //
  // Note: Issue1
  // Note: Issue2
  // Note: Issue3
  //
  // ToDo: Do1
  // ToDo: Do2
  Tstd::shared_ptr<ISchemaItem> param1_SchemaItem = TEnumSchemaItem<Enum_new4::eType>::create(Enum_new4_all_enum_values, TSchemaItemParameter<Enum_new4::eType>(Enum_new4::_11));

  // Function parameter param2.
  Tstd::shared_ptr<ISchemaItem> param2_SchemaItem = TEnumSchemaItem<Enum1::eType>::create(param2_allowed_enum_subset_values, TSchemaItemParameter<Enum1::eType>(name1));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["param1"] = CObjectSchemaItem::SMember(param1_SchemaItem, true);
  schema_members["param2"] = CObjectSchemaItem::SMember(param2_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema XXX::YYY::ZZZ::Test::InitFunction_val_1_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema XXX::YYY::ZZZ::Test::InitFunction_val_2_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

//----------- Structs schema items initialization ------------

Tstd::shared_ptr<ISchemaItem> XXX::YYY::ZZZ::Test::InitStructSchemaItem_Struct1(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Enum1::eType> Enum1_all_enum_values;
  Enum1_all_enum_values.insert(Enum1::name1);
  Enum1_all_enum_values.insert(Enum1::internal_name2);

  std::set<Enum1::eType> enumSubset1_allowed_enum_subset_values;
  enumSubset1_allowed_enum_subset_values.insert(Enum1::name1);

  std::set<Enum_new2::eType> Enum_new2_all_enum_values;
  Enum_new2_all_enum_values.insert(Enum_new2::_1);
  Enum_new2_all_enum_values.insert(Enum_new2::_2);
  Enum_new2_all_enum_values.insert(Enum_new2::_3);

  std::set<Enum1::eType> sub1_allowed_enum_subset_values;
  sub1_allowed_enum_subset_values.insert(Enum1::name1);

  std::set<Enum1::eType> sub2_allowed_enum_subset_values;
  sub2_allowed_enum_subset_values.insert(Enum1::internal_name2);

  std::set<Enum_new4::eType> sub3_allowed_enum_subset_values;
  sub3_allowed_enum_subset_values.insert(Enum_new4::_22);

  // Struct member intParam.
  Tstd::shared_ptr<ISchemaItem> intParam_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(2), TSchemaItemParameter<int>());

  // Struct member doubleParam.
  Tstd::shared_ptr<ISchemaItem> doubleParam_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.333), TSchemaItemParameter<double>(), TSchemaItemParameter<double>());

  // Struct member boolParam.
  Tstd::shared_ptr<ISchemaItem> boolParam_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member structParam.
  Tstd::shared_ptr<ISchemaItem> structParam_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Struct2);

  // Struct member enumParam.
  Tstd::shared_ptr<ISchemaItem> enumParam_SchemaItem = TEnumSchemaItem<Enum1::eType>::create(Enum1_all_enum_values, TSchemaItemParameter<Enum1::eType>());

  // Struct member enumParam1.
  Tstd::shared_ptr<ISchemaItem> enumParam1_SchemaItem = TEnumSchemaItem<Enum1::eType>::create(Enum1_all_enum_values, TSchemaItemParameter<Enum1::eType>());

  // Struct member enumSubset1.
  Tstd::shared_ptr<ISchemaItem> enumSubset1_SchemaItem = TEnumSchemaItem<Enum1::eType>::create(enumSubset1_allowed_enum_subset_values, TSchemaItemParameter<Enum1::eType>());

  // Struct member arrayOfInt.
  Tstd::shared_ptr<ISchemaItem> arrayOfInt_SchemaItem = CArraySchemaItem::create(CBoolSchemaItem::create(TSchemaItemParameter<bool>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(20));

  // Struct member arrayOfEnum1.
  Tstd::shared_ptr<ISchemaItem> arrayOfEnum1_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Enum1::eType>::create(Enum1_all_enum_values, TSchemaItemParameter<Enum1::eType>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(20));

  // Struct member arrayOfEnum3.
  Tstd::shared_ptr<ISchemaItem> arrayOfEnum3_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Enum_new2::eType>::create(Enum_new2_all_enum_values, TSchemaItemParameter<Enum_new2::eType>()), TSchemaItemParameter<size_t>(10), TSchemaItemParameter<size_t>(40));

  // Struct member arrayOfEnum4.
  Tstd::shared_ptr<ISchemaItem> arrayOfEnum4_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Enum1::eType>::create(sub1_allowed_enum_subset_values, TSchemaItemParameter<Enum1::eType>()), TSchemaItemParameter<size_t>(10), TSchemaItemParameter<size_t>(41));

  // Struct member arrayOfEnum5.
  Tstd::shared_ptr<ISchemaItem> arrayOfEnum5_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Enum1::eType>::create(sub2_allowed_enum_subset_values, TSchemaItemParameter<Enum1::eType>()), TSchemaItemParameter<size_t>(10), TSchemaItemParameter<size_t>(42));

  // Struct member arrayOfEnum6.
  Tstd::shared_ptr<ISchemaItem> arrayOfEnum6_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Enum_new4::eType>::create(sub3_allowed_enum_subset_values, TSchemaItemParameter<Enum_new4::eType>()), TSchemaItemParameter<size_t>(10), TSchemaItemParameter<size_t>(43));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["intParam"] = CObjectSchemaItem::SMember(intParam_SchemaItem, true);
  schema_members["doubleParam"] = CObjectSchemaItem::SMember(doubleParam_SchemaItem, false);
  schema_members["boolParam"] = CObjectSchemaItem::SMember(boolParam_SchemaItem, true);
  schema_members["structParam"] = CObjectSchemaItem::SMember(structParam_SchemaItem, true);
  schema_members["enumParam"] = CObjectSchemaItem::SMember(enumParam_SchemaItem, true);
  schema_members["enumParam1"] = CObjectSchemaItem::SMember(enumParam1_SchemaItem, true);
  schema_members["enumSubset1"] = CObjectSchemaItem::SMember(enumSubset1_SchemaItem, false);
  schema_members["arrayOfInt"] = CObjectSchemaItem::SMember(arrayOfInt_SchemaItem, false);
  schema_members["arrayOfEnum1"] = CObjectSchemaItem::SMember(arrayOfEnum1_SchemaItem, false);
  schema_members["arrayOfEnum3"] = CObjectSchemaItem::SMember(arrayOfEnum3_SchemaItem, true);
  schema_members["arrayOfEnum4"] = CObjectSchemaItem::SMember(arrayOfEnum4_SchemaItem, true);
  schema_members["arrayOfEnum5"] = CObjectSchemaItem::SMember(arrayOfEnum5_SchemaItem, true);
  schema_members["arrayOfEnum6"] = CObjectSchemaItem::SMember(arrayOfEnum6_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

Tstd::shared_ptr<ISchemaItem> XXX::YYY::ZZZ::Test::InitStructSchemaItem_Struct2(
    const TStructsSchemaItems &struct_schema_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  return CObjectSchemaItem::create(schema_members);
}

//-------------- String to value enum mapping ----------------

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <>
const std::map<XXX::YYY::ZZZ::Enum1::eType, std::string> &TEnumSchemaItem<XXX::YYY::ZZZ::Enum1::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<XXX::YYY::ZZZ::Enum1::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum1::name1, "name1"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum1::internal_name2, "name2"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<XXX::YYY::ZZZ::E2::eType, std::string> &TEnumSchemaItem<XXX::YYY::ZZZ::E2::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<XXX::YYY::ZZZ::E2::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::E2::val_1, "xxx"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::E2::val_2, "yyy"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::E2::val_3, "val_3"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<XXX::YYY::ZZZ::Enum_new2::eType, std::string> &TEnumSchemaItem<XXX::YYY::ZZZ::Enum_new2::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<XXX::YYY::ZZZ::Enum_new2::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum_new2::_1, "xxx"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum_new2::_2, "xxx"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum_new2::_3, "xxx"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<XXX::YYY::ZZZ::Enum_new4::eType, std::string> &TEnumSchemaItem<XXX::YYY::ZZZ::Enum_new4::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<XXX::YYY::ZZZ::Enum_new4::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum_new4::_11, "xxx"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::Enum_new4::_22, "xxx"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<XXX::YYY::ZZZ::messageType::eType, std::string> &TEnumSchemaItem<XXX::YYY::ZZZ::messageType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<XXX::YYY::ZZZ::messageType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::messageType::request, "request"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::messageType::response, "response"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::messageType::notification, "notification"));
    enum_string_representation.insert(std::make_pair(XXX::YYY::ZZZ::messageType::error_response, "error_response"));

    is_initialized = true;
  }

  return enum_string_representation;
}

} // NsSmartObjects
} // NsSmartDeviceLink

