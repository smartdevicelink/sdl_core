/**
 * @file Test.h
 * @brief Generated class Test header file.
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

#ifndef __CSMARTFACTORY_TEST_12345678123456781234567812345678_H__
#define __CSMARTFACTORY_TEST_12345678123456781234567812345678_H__

#include "JSONHandler/CSmartFactory.hpp"
#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/ISchemaItem.hpp"
#include "SmartObjects/TSharedPtr.hpp"

namespace XXX {
namespace YYY {
namespace ZZZ {

namespace StructIdentifiers {
/**
 * @brief Enumeration StructIdentifiers.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief Struct1.
   */
  Struct1,

  /**
   * @brief Struct2.
   */
  Struct2
};
} // StructIdentifiers

namespace Enum1 {
/**
 * @brief Enumeration Enum1.
 *
 * @todo Do1
 * @todo Do2
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief name1.
   *
   * Design Line1
   *
   * @todo Do1
   * @todo Do2
   */
  name1 = 1,

  /**
   * @brief internal_name2.
   *
   * Description Line1
   * Description Line2
   *
   * @note Issue1
   * @note Issue2
   * @note Issue3
   */
  internal_name2
};
} // Enum1

namespace E2 {
/**
 * @brief Enumeration E2.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief val_1.
   */
  val_1,

  /**
   * @brief val_2.
   */
  val_2 = 100,

  /**
   * @brief val_3.
   */
  val_3
};
} // E2

namespace Enum_new2 {
/**
 * @brief Enumeration Enum_new2.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief _1.
   */
  _1,

  /**
   * @brief _2.
   */
  _2,

  /**
   * @brief _3.
   */
  _3
};
} // Enum_new2

namespace Enum_new4 {
/**
 * @brief Enumeration Enum_new4.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief _11.
   */
  _11,

  /**
   * @brief _22.
   */
  _22
};
} // Enum_new4

namespace messageType {
/**
 * @brief Enumeration messageType.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief request.
   */
  request,

  /**
   * @brief response.
   */
  response,

  /**
   * @brief notification.
   */
  notification,

  /**
   * @brief error_response.
   */
  error_response
};
} // messageType


/**
 * @brief Class Test.
 *
 * Params:
 *     param2 - value2
 *     param1 - value1
 */
class Test : public NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType> {
 public:
  /**
   * @brief Constructor.
   */
  Test();

 protected:
  /**
   * @brief Type that maps of struct IDs to schema items.
   */
  typedef std::map<const StructIdentifiers::eType, NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> > TStructsSchemaItems;

  /**
   * @brief Helper that allows to make reference to struct
   *
   * @param struct_schema_items Struct schema items.
   * @param struct_id ID of structure to provide.
   *
   * @return TSharedPtr of strucute
   */
  static NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> ProvideObjectSchemaItemForStruct(
        const TStructsSchemaItems &struct_schema_items,
        const StructIdentifiers::eType struct_id);

  /**
   * @brief Initializes all struct schemes.
   */
  void InitStructSchemes(TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Initializes all function schemes.
   *
   * @param struct_schema_items Struct schema items.
   * @param function_id_items Set of all elements of FunctionID enum.
   * @param message_type_items Set of all elements of messageType enum.
   */
  void InitFunctionSchemes(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Function1.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_name1_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Function2.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_val_1_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Function2.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_val_2_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema item for structure Struct1.
   *
   * Design Line1
   *
   * @note Issue1
   * @note Issue2
   * @note Issue3
   */
  static NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Struct1(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Struct2.
   *
   * @note Issue1
   * @note Issue2
   * @note Issue3
   */
  static NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Struct2(
      const TStructsSchemaItems &struct_schema_items);
};

} // XXX
} // YYY
} // ZZZ

#endif //__CSMARTFACTORY_TEST_12345678123456781234567812345678_H__

