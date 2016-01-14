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

#ifndef SRC_COMPONENTS_FORMATTERS_TEST_INCLUDE_SMARTFACTORYTESTHELPER_H_
#define SRC_COMPONENTS_FORMATTERS_TEST_INCLUDE_SMARTFACTORYTESTHELPER_H_

#include <map>
#include <set>

#include "formatters/CSmartFactory.h"
#include "HMI_API_schema.h"
#include "smart_objects/always_true_schema_item.h"
#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/schema_item_parameter.h"

namespace test {
namespace components {
namespace formatters {

using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace hmi_apis;

namespace TestType {
enum eType {
  INVALID_ENUM = -1,
  APPLICATION_NOT_REGISTERED = 0,
  SUCCESS,
  TOO_MANY_PENDING_REQUESTS,
  REJECTED,
  INVALID_DATA,
  OUT_OF_MEMORY,
  ABORTED,
  USER_DISALLOWED,
  GENERIC_ERROR,
  DISALLOWED
};
}  // namespace TestType

namespace FunctionIdTest {
enum eType {
  INVALID_ENUM = -1,
  Function1,
  Function2,
  Function3
};
}  // namespace FunctionIdTest

namespace MessageTypeTest {
enum eType {
  INVALID_ENUM = -1,
  request,
  response,
  notification,
  error_response
};
}  // namespace MessageTypeTest

namespace StructIdentifiersTest {
enum eType {
  INVALID_ENUM = -1,
  Common_1,
  Common_2,
  Common_3
};
}  // namespace StructIdentifiersTest

class CSmartFactoryTest : public CSmartFactory<FunctionIdTest::eType,
    MessageTypeTest::eType, StructIdentifiersTest::eType> {
 public:
  CSmartFactoryTest();
  std::map<SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType>,
      CSmartSchema> function_schemes() {
    return functions_schemes_;
  }
  std::map<StructIdentifiersTest::eType, CSmartSchema> structs_schemes() {
    return structs_schemes_;
  }
 protected:
  typedef std::map<const StructIdentifiersTest::eType,
      utils::SharedPtr<ISchemaItem> > TStructsSchemaItems;

  static utils::SharedPtr<ISchemaItem> ProvideObjectSchemaItemForStruct(
      TStructsSchemaItems &struct_schema_items,
      const StructIdentifiersTest::eType struct_id);

  void InitStructSchemes(TStructsSchemaItems &struct_schema_items);

  void InitFunctionSchemes(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static CSmartSchema InitFunction_Function1_request(
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static CSmartSchema InitFunction_Function1_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static CSmartSchema InitFunction_Function2_request(
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static CSmartSchema InitFunction_Function2_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static CSmartSchema InitFunction_Function3_request(
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static CSmartSchema InitFunction_Function3_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionIdTest::eType> &function_id_items,
      const std::set<MessageTypeTest::eType> &message_type_items);

  static utils::SharedPtr<ISchemaItem> InitStructSchemaItem_Common_1(
      TStructsSchemaItems &struct_schema_items);

  static utils::SharedPtr<ISchemaItem> InitStructSchemaItem_Common_2();
};

}  // namespace formatters
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_FORMATTERS_TEST_INCLUDE_SMARTFACTORYTESTHELPER_H_
