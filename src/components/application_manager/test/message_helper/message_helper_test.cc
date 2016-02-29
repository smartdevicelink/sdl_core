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

#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "utils/macro.h"
#include "utils/make_shared.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/test/resumption/include/application_mock.h"
#include "utils/custom_string.h"

namespace application_manager {
namespace test {

namespace HmiLanguage = hmi_apis::Common_Language;
namespace HmiResults = hmi_apis::Common_Result;
namespace MobileResults = mobile_apis::Result;

typedef ::test::components::resumption_test::MockApplication AppMock;
typedef utils::SharedPtr<AppMock> MockApplicationSharedPtr;
typedef std::vector<std::string> StringArray;
typedef ::application_manager::Application App;
typedef utils::SharedPtr<App> ApplicationSharedPtr;

using testing::AtLeast;
using testing::ReturnRefOfCopy;
using testing::ReturnRef;
using testing::Return;

TEST(MessageHelperTestCreate,
   CreateBlockedByPoliciesResponse_SmartObject_Equal) {
  mobile_apis::FunctionID::eType function_id =
      mobile_apis::FunctionID::eType::AddCommandID;
  mobile_apis::Result::eType result = mobile_apis::Result::eType::ABORTED;
  uint32_t correlation_id = 0;
  uint32_t connection_key = 0;
  bool success = false;
  
  smart_objects::SmartObjectSPtr ptr =
      MessageHelper::CreateBlockedByPoliciesResponse(
              function_id, result, correlation_id, connection_key);

  EXPECT_TRUE(ptr);

  smart_objects::SmartObject& obj = *ptr;
  
  EXPECT_EQ(function_id,
      obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kResponse,
      obj[strings::params][strings::message_type].asInt());
  EXPECT_EQ(success,
      obj[strings::msg_params][strings::success].asBool());
  EXPECT_EQ(result,
      obj[strings::msg_params][strings::result_code].asInt());
  EXPECT_EQ(correlation_id,
      obj[strings::params][strings::correlation_id].asUInt());
  EXPECT_EQ(connection_key,
      obj[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(kV2,
      obj[strings::params][strings::protocol_version].asInt());
}

TEST(MessageHelperTestCreate,
   CreateSetAppIcon_SendNullPathImagetype_Equal) {
  std::string path_to_icon = "";
  uint32_t app_id = 0;
  smart_objects::SmartObjectSPtr ptr =
      MessageHelper::CreateSetAppIcon(path_to_icon, app_id);

  EXPECT_TRUE(ptr);

  smart_objects::SmartObject& obj = *ptr;
  
  int image_type = static_cast<int>(mobile_api::ImageType::DYNAMIC);
  
  EXPECT_EQ(path_to_icon,
      obj[strings::sync_file_name][strings::value].asString());
  EXPECT_EQ(image_type,
      obj[strings::sync_file_name][strings::image_type].asInt());
  EXPECT_EQ(app_id, obj[strings::app_id].asUInt());
}

TEST(MessageHelperTestCreate,
   CreateSetAppIcon_SendPathImagetype_Equal) {
  std::string path_to_icon = "/qwe/qwe/";
  uint32_t app_id = 10;
  smart_objects::SmartObjectSPtr ptr =
      MessageHelper::CreateSetAppIcon(path_to_icon, app_id);

  EXPECT_TRUE(ptr);

  smart_objects::SmartObject& obj = *ptr;

  int image_type = static_cast<int>(mobile_api::ImageType::DYNAMIC);

  EXPECT_EQ(path_to_icon,
      obj[strings::sync_file_name][strings::value].asString());
  EXPECT_EQ(image_type,
      obj[strings::sync_file_name][strings::image_type].asInt());
  EXPECT_EQ(app_id, obj[strings::app_id].asUInt());
}

TEST(MessageHelperTestCreate,
   CreateGlobalPropertiesRequestsToHMI_SmartObject_EmptyList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  
  EXPECT_CALL(*appSharedMock, vr_help_title()).Times(AtLeast(1));
  EXPECT_CALL(*appSharedMock, vr_help()).Times(AtLeast(1));
  EXPECT_CALL(*appSharedMock, help_prompt()).Times(AtLeast(1));
  EXPECT_CALL(*appSharedMock, timeout_prompt()).Times(AtLeast(1));
  
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateGlobalPropertiesRequestsToHMI(appSharedMock);
  
  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate,
   CreateGlobalPropertiesRequestsToHMI_SmartObject_NotEmpty) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  smart_objects::SmartObjectSPtr objPtr =
      MakeShared<smart_objects::SmartObject>();

  (*objPtr)[0][strings::vr_help_title] = "111";
  (*objPtr)[1][strings::vr_help] = "222";
  (*objPtr)[2][strings::keyboard_properties] = "333";
  (*objPtr)[3][strings::menu_title] = "444";
  (*objPtr)[4][strings::menu_icon] = "555";
  (*objPtr)[5][strings::help_prompt] = "666";
  (*objPtr)[6][strings::timeout_prompt] = "777";
  
  EXPECT_CALL(*appSharedMock,
      vr_help_title()).Times(AtLeast(3)).WillRepeatedly(Return(&(*objPtr)[0]));
  EXPECT_CALL(*appSharedMock,
      vr_help()).Times(AtLeast(2)).WillRepeatedly(Return(&(*objPtr)[1]));
  EXPECT_CALL(*appSharedMock,
      help_prompt()).Times(AtLeast(3)).WillRepeatedly(Return(&(*objPtr)[5]));
  EXPECT_CALL(*appSharedMock,
      timeout_prompt()).Times(AtLeast(2)).WillRepeatedly(Return(&(*objPtr)[6]));
  EXPECT_CALL(*appSharedMock,
      keyboard_props()).Times(AtLeast(2)).WillRepeatedly(Return(&(*objPtr)[2]));
  EXPECT_CALL(*appSharedMock,
      menu_title()).Times(AtLeast(2)).WillRepeatedly(Return(&(*objPtr)[3]));
  EXPECT_CALL(*appSharedMock,
      menu_icon()).Times(AtLeast(2)).WillRepeatedly(Return(&(*objPtr)[4]));
  EXPECT_CALL(*appSharedMock, app_id()).WillRepeatedly(Return(0));
  
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateGlobalPropertiesRequestsToHMI(appSharedMock);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& first = *ptr[0];
  smart_objects::SmartObject& second = *ptr[1];
  
  EXPECT_EQ((*objPtr)[0], first[strings::msg_params][strings::vr_help_title]);
  EXPECT_EQ((*objPtr)[1], first[strings::msg_params][strings::vr_help]);
  EXPECT_EQ((*objPtr)[2], first[strings::msg_params][strings::keyboard_properties]);
  EXPECT_EQ((*objPtr)[3], first[strings::msg_params][strings::menu_title]);
  EXPECT_EQ((*objPtr)[4], first[strings::msg_params][strings::menu_icon]);
  EXPECT_EQ((*objPtr)[5], second[strings::msg_params][strings::help_prompt]);
  EXPECT_EQ((*objPtr)[6], second[strings::msg_params][strings::timeout_prompt]);
}

TEST(MessageHelperTestCreate,
   CreateShowRequestToHMI_SendSmartObject_Equal) {
  MockApplicationSharedPtr appSharedMock =
          utils::MakeShared<AppMock>();
  
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  const smart_objects::SmartObject& object = *smartObjectPtr;
  
  EXPECT_CALL(*appSharedMock,
      show_command()).Times(AtLeast(2)).WillRepeatedly(Return(&object));
  
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateShowRequestToHMI(appSharedMock);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& obj = *ptr[0];
  
  int function_id = static_cast<int>(hmi_apis::FunctionID::UI_Show);
  
  EXPECT_EQ(function_id, obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(*smartObjectPtr, obj[strings::msg_params]);
}

TEST(MessageHelperTestCreate,
   CreateAddCommandRequestToHMI_SendSmartObject_Empty) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  CommandsMap vis;
  DataAccessor< CommandsMap> data_accessor(vis, true);
  
  EXPECT_CALL(*appSharedMock,
      commands_map()).WillOnce(Return(data_accessor));
  
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddCommandRequestToHMI(appSharedMock);
  
  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate,
    CreateAddCommandRequestToHMI_SendSmartObject_Equal) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  CommandsMap vis;
  DataAccessor< CommandsMap> data_accessor(vis, true);
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  smart_objects::SmartObject& object = *smartObjectPtr;

  object[strings::menu_params] = 1;
  object[strings::cmd_icon] = 1;
  object[strings::cmd_icon][strings::value] = "10";

  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(5, &object));

  EXPECT_CALL(*appSharedMock,
      commands_map()).WillOnce(Return(data_accessor));
  EXPECT_CALL(*appSharedMock,
      app_id()).WillOnce(Return(1u));

  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddCommandRequestToHMI(appSharedMock);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& obj = *ptr[0];

  int function_id = static_cast<int>(hmi_apis::FunctionID::UI_AddCommand);

  EXPECT_EQ(function_id, obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(1u, obj[strings::msg_params][strings::app_id].asUInt());
  EXPECT_EQ(5, obj[strings::msg_params][strings::cmd_id].asInt());
  EXPECT_EQ(object[strings::menu_params],
      obj[strings::msg_params][strings::menu_params]);
  EXPECT_EQ(object[strings::cmd_icon],
      obj[strings::msg_params][strings::cmd_icon]);
  EXPECT_EQ("10", obj[strings::msg_params]
      [strings::cmd_icon][strings::value].asString());
}

TEST(MessageHelperTestCreate,
   CreateAddVRCommandRequestFromChoiceToHMI_SendEmptyData_EmptyList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  application_manager::ChoiceSetMap vis;
  DataAccessor< ::application_manager::ChoiceSetMap> data_accessor(vis, true);
  
  EXPECT_CALL(*appSharedMock,
      choice_set_map()).WillOnce(Return(data_accessor));
  
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(appSharedMock);
  
  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate,
  CreateAddVRCommandRequestFromChoiceToHMI_SendObject_EqualList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  application_manager::ChoiceSetMap vis;
  DataAccessor< ::application_manager::ChoiceSetMap> data_accessor(vis, true);
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  smart_objects::SmartObject& object = *smartObjectPtr;
  
  object[strings::choice_set] = "10";
  object[strings::grammar_id] = 111;
  object[strings::choice_set][0][strings::choice_id] = 1;
  object[strings::choice_set][0][strings::vr_commands] = 2;

  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(5, &object));
  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(6, &object));
  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(7, &object));
  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(8, &object));
  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(9, &object));

  EXPECT_CALL(*appSharedMock,
      choice_set_map()).WillOnce(Return(data_accessor));
  EXPECT_CALL(*appSharedMock,
      app_id()).Times(AtLeast(5)).WillRepeatedly(Return(1u));
  
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(appSharedMock);

  EXPECT_FALSE(ptr.empty());

  int function_id = static_cast<int>(hmi_apis::FunctionID::VR_AddCommand);
  int type = static_cast<int>(hmi_apis::Common_VRCommandType::Choice);

  smart_objects::SmartObject& obj = *ptr[0];

  EXPECT_EQ(function_id, obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(1u, obj[strings::msg_params][strings::app_id].asUInt());
  EXPECT_EQ(111u, obj[strings::msg_params][strings::grammar_id].asUInt());
  EXPECT_EQ(object[strings::choice_set][0][strings::choice_id],
      obj[strings::msg_params][strings::cmd_id]);
  EXPECT_EQ(object[strings::choice_set][0][strings::vr_commands],
      obj[strings::msg_params][strings::vr_commands]);
  EXPECT_EQ(type, obj[strings::msg_params][strings::type].asInt());
}

TEST(MessageHelperTestCreate, CreateAddSubMenuRequestToHMI_SendObject_Equal) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  application_manager::SubMenuMap vis;
  DataAccessor< ::application_manager::SubMenuMap> data_accessor(vis, true);
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  smart_objects::SmartObject& object = *smartObjectPtr;

  object[strings::position] = 1;
  object[strings::menu_name] = 1;

  vis.insert(std::pair<uint32_t,
      smart_objects::SmartObject*>(5, &object));

  EXPECT_CALL(*appSharedMock,
      sub_menu_map() ).WillOnce(Return(data_accessor));
  EXPECT_CALL(*appSharedMock,
      app_id()).Times(AtLeast(1)).WillOnce(Return(1u));

  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddSubMenuRequestToHMI(appSharedMock);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& obj = *ptr[0];

  int function_id = static_cast<int>(hmi_apis::FunctionID::UI_AddSubMenu);

  EXPECT_EQ(function_id,
      obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(5,
      obj[strings::msg_params][strings::menu_id].asInt());
  EXPECT_EQ(1,
      obj[strings::msg_params]
          [strings::menu_params][strings::position].asInt());
  EXPECT_EQ(1, obj[strings::msg_params]
      [strings::menu_params][strings::menu_name].asInt());
  EXPECT_EQ(1u,
      obj[strings::msg_params][strings::app_id].asUInt());
}

TEST(MessageHelperTestCreate,
  CreateAddSubMenuRequestToHMI_SendEmptyMap_EmptySmartObjectList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  application_manager::SubMenuMap vis;
  DataAccessor< ::application_manager::SubMenuMap> data_accessor(vis, true);

  EXPECT_CALL(*appSharedMock,
      sub_menu_map() ).WillOnce(Return(data_accessor));

  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddSubMenuRequestToHMI(appSharedMock);

  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate, CreateNegativeResponse_SendSmartObject_Equal) {
  uint32_t connection_key = 111;
  int32_t function_id = 222;
  uint32_t correlation_id = 333u;
  int32_t result_code = 0;

  smart_objects::SmartObjectSPtr ptr =
      MessageHelper::CreateNegativeResponse(connection_key, function_id,
          correlation_id, result_code);

  EXPECT_TRUE(ptr);

  smart_objects::SmartObject& obj = *ptr;

  int objFunction_id =
      obj[strings::params][strings::function_id].asInt();
  uint32_t objCorrelation_id =
      obj[strings::params][strings::correlation_id].asUInt();
  int objResult_code =
      obj[strings::msg_params][strings::result_code].asInt();
  uint32_t objConnection_key =
     obj[strings::params][strings::connection_key].asUInt();

  int message_type =
      static_cast<int>(mobile_apis::messageType::response);
  int protocol_type =
      static_cast<int>(commands::CommandImpl::mobile_protocol_type_);
  int protocol_version =
      static_cast<int>(commands::CommandImpl::protocol_version_);
  bool success = false;

  EXPECT_EQ(function_id, objFunction_id);
  EXPECT_EQ(message_type,
      obj[strings::params][strings::message_type].asInt());
  EXPECT_EQ(protocol_type,
      obj[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(protocol_version,
      obj[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(correlation_id, objCorrelation_id);
  EXPECT_EQ(result_code, objResult_code);
  EXPECT_EQ(success,
      obj[strings::msg_params][strings::success].asBool());
  EXPECT_EQ(connection_key, objConnection_key);
}

class MessageHelperTest : public ::testing::Test {
  public:
    MessageHelperTest() :
        language_strings {
          "EN-US", "ES-MX", "FR-CA", "DE-DE", "ES-ES", "EN-GB", "RU-RU",
          "TR-TR", "PL-PL", "FR-FR", "IT-IT", "SV-SE", "PT-PT", "NL-NL",
          "EN-AU", "ZH-CN", "ZH-TW", "JA-JP", "AR-SA", "KO-KR", "PT-BR",
          "CS-CZ", "DA-DK", "NO-NO", "NL-BE", "EL-GR", "HU-HU", "FI-FI",
          "SK-SK"
        },
        hmi_result_strings {
          "SUCCESS", "UNSUPPORTED_REQUEST", "UNSUPPORTED_RESOURCE",
          "DISALLOWED", "REJECTED", "ABORTED",
          "IGNORED", "RETRY", "IN_USE",
          "DATA_NOT_AVAILABLE", "TIMED_OUT", "INVALID_DATA",
          "CHAR_LIMIT_EXCEEDED", "INVALID_ID", "DUPLICATE_NAME",
          "APPLICATION_NOT_REGISTERED", "WRONG_LANGUAGE", "OUT_OF_MEMORY",
          "TOO_MANY_PENDING_REQUESTS", "NO_APPS_REGISTERED",
          "NO_DEVICES_CONNECTED", "WARNINGS", "GENERIC_ERROR",
          "USER_DISALLOWED", "TRUNCATED_DATA"
        },
        mobile_result_strings {
          "SUCCESS", "UNSUPPORTED_REQUEST", "UNSUPPORTED_RESOURCE",
          "DISALLOWED", "REJECTED", "ABORTED",
          "IGNORED", "RETRY", "IN_USE",
          "VEHICLE_DATA_NOT_AVAILABLE", "TIMED_OUT", "INVALID_DATA",
          "CHAR_LIMIT_EXCEEDED", "INVALID_ID", "DUPLICATE_NAME",
          "APPLICATION_NOT_REGISTERED", "WRONG_LANGUAGE", "OUT_OF_MEMORY",
          "TOO_MANY_PENDING_REQUESTS", "TOO_MANY_APPLICATIONS",
          "APPLICATION_REGISTERED_ALREADY", "WARNINGS", "GENERIC_ERROR",
          "USER_DISALLOWED", "UNSUPPORTED_VERSION", "VEHICLE_DATA_NOT_ALLOWED",
          "FILE_NOT_FOUND", "CANCEL_ROUTE", "TRUNCATED_DATA",
          "SAVED", "INVALID_CERT", "EXPIRED_CERT", "RESUME_FAILED"
        },
        function_id_strings {
          "RESERVED", "RegisterAppInterface", "UnregisterAppInterface",
          "SetGlobalProperties", "ResetGlobalProperties", "AddCommand",
          "DeleteCommand", "AddSubMenu", "DeleteSubMenu",
          "CreateInteractionChoiceSet", "PerformInteraction",
          "DeleteInteractionChoiceSet", "Alert", "Show", "Speak",
          "SetMediaClockTimer", "PerformAudioPassThru", "EndAudioPassThru",
          "SubscribeButton", "UnsubscribeButton", "SubscribeVehicleData",
          "UnsubscribeVehicleData", "GetVehicleData", "ReadDID",
          "GetDTCs", "ScrollableMessage", "Slider",
          "ShowConstantTBT", "AlertManeuver", "UpdateTurnList",
          "ChangeRegistration", "GenericResponse", "PutFile",
          "DeleteFile", "ListFiles", "SetAppIcon",
          "SetDisplayLayout", "DiagnosticMessage", "SystemRequest",
          "SendLocation", "DialNumber"
        },
        events_id_strings {
          "OnHMIStatus", "OnAppInterfaceUnregistered", "OnButtonEvent",
          "OnButtonPress", "OnVehicleData", "OnCommand",
          "OnTBTClientState", "OnDriverDistraction", "OnPermissionsChange",
          "OnAudioPassThru", "OnLanguageChange", "OnKeyboardInput",
          "OnTouchEvent", "OnSystemRequest", "OnHashChange"
        },
        hmi_level_strings {
          "FULL", "LIMITED",
          "BACKGROUND", "NONE"
        },
        delta_from_functions_id(32768) {}

  protected:
    const StringArray language_strings;
    const StringArray hmi_result_strings;
    const StringArray mobile_result_strings;
    const StringArray function_id_strings;
    const StringArray events_id_strings;
    const StringArray hmi_level_strings;

    const size_t delta_from_functions_id;
};

TEST_F(MessageHelperTest,
    CommonLanguageFromString_StringValueOfEnum_CorrectEType) {
  HmiLanguage::eType enum_value;
  HmiLanguage::eType enum_from_string_value;
  // Check all languages >= 0
  for (size_t array_index = 0;
      array_index < language_strings.size();
      ++array_index) {
    enum_value = static_cast<HmiLanguage::eType>(array_index);
    enum_from_string_value = MessageHelper::CommonLanguageFromString(
      language_strings[array_index]);
    EXPECT_EQ(enum_value, enum_from_string_value);
  }
  // Check InvalidEnum == -1
  enum_value = HmiLanguage::INVALID_ENUM;
  enum_from_string_value = MessageHelper::CommonLanguageFromString("");
  EXPECT_EQ(enum_value, enum_from_string_value);
}

TEST_F(MessageHelperTest,
    CommonLanguageToString_ETypeValueOfEnum_CorrectString) {
  std::string string_from_enum;
  HmiLanguage::eType casted_enum;
  // Check all languages >=0
  for (size_t array_index = 0;
      array_index < language_strings.size();
      ++array_index) {
    casted_enum = static_cast<HmiLanguage::eType>(array_index);
    string_from_enum = MessageHelper::CommonLanguageToString(casted_enum);
    EXPECT_EQ(language_strings[array_index], string_from_enum);
  }
  // Check InvalidEnum == -1
  string_from_enum = MessageHelper::CommonLanguageToString(
        HmiLanguage::INVALID_ENUM);
  EXPECT_EQ("", string_from_enum);
}

TEST_F(MessageHelperTest,
    ConvertEnumAPINoCheck_AnyEnumType_AnotherEnumType) {
  hmi_apis::Common_LayoutMode::eType tested_enum_value =
      hmi_apis::Common_LayoutMode::ICON_ONLY;
  hmi_apis::Common_AppHMIType::eType converted =
      MessageHelper::ConvertEnumAPINoCheck <hmi_apis::Common_LayoutMode::eType,
          hmi_apis::Common_AppHMIType::eType>(tested_enum_value);
  EXPECT_EQ(hmi_apis::Common_AppHMIType::DEFAULT, converted);
}

TEST_F(MessageHelperTest,
    HMIResultFromString_StringValueOfEnum_CorrectEType) {
  HmiResults::eType enum_value;
  HmiResults::eType enum_from_string_value;
  // Check all results >= 0
  for (size_t array_index = 0;
      array_index < hmi_result_strings.size();
      ++array_index) {
    enum_value = static_cast<HmiResults::eType>(array_index);
    enum_from_string_value =
        MessageHelper::HMIResultFromString(hmi_result_strings[array_index]);
    EXPECT_EQ(enum_value, enum_from_string_value);
  }
  // Check InvalidEnum == -1
  enum_value = HmiResults::INVALID_ENUM;
  enum_from_string_value = MessageHelper::HMIResultFromString("");
  EXPECT_EQ(enum_value, enum_from_string_value);
}

TEST_F(MessageHelperTest,
    HMIResultToString_ETypeValueOfEnum_CorrectString) {
  std::string string_from_enum;
  HmiResults::eType casted_enum;
  // Check all results >=0
  for (size_t array_index = 0;
      array_index < hmi_result_strings.size();
      ++array_index) {
    casted_enum = static_cast<HmiResults::eType>(array_index);
    string_from_enum = MessageHelper::HMIResultToString(casted_enum);
    EXPECT_EQ(hmi_result_strings[array_index], string_from_enum);
  }
  // Check InvalidEnum == -1
  string_from_enum = MessageHelper::HMIResultToString(
        HmiResults::INVALID_ENUM);
  EXPECT_EQ("", string_from_enum);
}

TEST_F(MessageHelperTest,
    HMIToMobileResult_HmiResultEType_GetCorrectMobileResultEType) {
  MobileResults::eType tested_enum;
  HmiResults::eType casted_hmi_enum;
  MobileResults::eType converted_enum;
  // Check enums >=0
  for (size_t enum_index = 0;
      enum_index < hmi_result_strings.size();
      ++enum_index) {
    tested_enum =
        MessageHelper::MobileResultFromString(hmi_result_strings[enum_index]);
    casted_hmi_enum = static_cast<HmiResults::eType>(enum_index);
    converted_enum = MessageHelper::HMIToMobileResult(casted_hmi_enum);
    EXPECT_EQ(tested_enum, converted_enum);
  }
  // Check invalid enums == -1
  tested_enum = MobileResults::INVALID_ENUM;
  converted_enum = MessageHelper::HMIToMobileResult(HmiResults::INVALID_ENUM);
  EXPECT_EQ(tested_enum, converted_enum);
  // Check when out of range (true == result.empty())
  casted_hmi_enum = static_cast<HmiResults::eType>(INT_MAX);
  converted_enum = MessageHelper::HMIToMobileResult(casted_hmi_enum);
  EXPECT_EQ(tested_enum, converted_enum);
}

TEST_F(MessageHelperTest, VerifySoftButtonString_WrongStrings_False) {
  const StringArray wrong_strings {
    "soft_button1\t\ntext",
    "soft_button1\\ntext",
    "soft_button1\\ttext",
    " ",
    "soft_button1\t\n",
    "soft_button1\\n",
    "soft_button1\\t"
  };
  for (size_t i = 0; i < wrong_strings.size(); ++i) {
    EXPECT_FALSE(MessageHelper::VerifySoftButtonString(wrong_strings[i]));
  }
}

TEST_F(MessageHelperTest, VerifySoftButtonString_CorrectStrings_True) {
  const StringArray wrong_strings {
    "soft_button1.text",
    "soft_button1?text",
    " asd asdasd    .././/",
    "soft_button1??....asd",
    "soft_button12313fcvzxc./.,"
  };
  for (size_t i = 0; i < wrong_strings.size(); ++i) {
    EXPECT_TRUE(MessageHelper::VerifySoftButtonString(wrong_strings[i]));
  }
}

TEST_F(MessageHelperTest,
    GetIVISubscriptionRequests_ValidApplication_HmiRequestNotEmpty) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating data acessor
  application_manager::VehicleInfoSubscriptions vis;
  DataAccessor<application_manager::VehicleInfoSubscriptions>
      data_accessor(vis, true);
  // Calls for ApplicationManager
  EXPECT_CALL(*appSharedMock, app_id() )
      .WillOnce(Return(1u));
  EXPECT_CALL(*appSharedMock, SubscribedIVI())
      .WillOnce(Return(data_accessor));
  smart_objects::SmartObjectList outList =
      MessageHelper::GetIVISubscriptionRequests(appSharedMock);
  // Expect not empty request
  EXPECT_FALSE(outList.empty());
}

TEST_F(MessageHelperTest,
    ProcessSoftButtons_SmartObjectWithoutButtonsKey_Success) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject object;
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::ProcessSoftButtons(object, appSharedMock);
  // Expect
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
    ProcessSoftButtons_IncorectSoftButonValue_InvalidData) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject object;
  smart_objects::SmartObject& buttons = object[strings::soft_buttons];
  // Setting invalid image string to button
  buttons[0][strings::image][strings::value] = "invalid\\nvalue";
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::ProcessSoftButtons(object, appSharedMock);
  // Expect
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}

TEST_F(MessageHelperTest,
    VerifyImage_ImageTypeIsStatic_Success) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject image;
  image[strings::image_type] = mobile_apis::ImageType::STATIC;
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::VerifyImage(image, appSharedMock);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
    VerifyImage_ImageValueNotValid_InvalidData) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject image;
  image[strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  // Invalid value
  image[strings::value] = "   ";
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::VerifyImage(image, appSharedMock);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}


TEST_F(MessageHelperTest,
    VerifyImageFiles_SmartObjectWithValidData_Success) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject images;
  images[0][strings::image_type] = mobile_apis::ImageType::STATIC;
  images[1][strings::image_type] = mobile_apis::ImageType::STATIC;
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::VerifyImageFiles(images, appSharedMock);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
    VerifyImageFiles_SmartObjectWithInvalidData_NotSuccsess) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject images;
  images[0][strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  images[1][strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  // Invalid values
  images[0][strings::value] = "   ";
  images[1][strings::value] = "image\\n";
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::VerifyImageFiles(images, appSharedMock);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}

TEST_F(MessageHelperTest,
    VerifyImageVrHelpItems_SmartObjectWithSeveralValidImages_Succsess) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject message;
  message[0][strings::image][strings::image_type] =
      mobile_apis::ImageType::STATIC;
  message[1][strings::image][strings::image_type] =
      mobile_apis::ImageType::STATIC;
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::VerifyImageVrHelpItems(message, appSharedMock);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
    VerifyImageVrHelpItems_SmartObjWithSeveralInvalidImages_NotSuccsess) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<AppMock>();
  // Creating input data for method
  smart_objects::SmartObject message;
  message[0][strings::image][strings::image_type] =
      mobile_apis::ImageType::DYNAMIC;
  message[1][strings::image][strings::image_type] =
      mobile_apis::ImageType::DYNAMIC;
  // Invalid values
  message[0][strings::image][strings::value] = "   ";
  message[1][strings::image][strings::value] = "image\\n";
  // Method call
  mobile_apis::Result::eType result =
      MessageHelper::VerifyImageVrHelpItems(message, appSharedMock);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}

TEST_F(MessageHelperTest,
    StringifiedFunctionID_FinctionId_EqualsWithStringsInArray) {
  // Start from 1 because 1 == RESERVED and haven`t ID in last 2 characters
  // if FUNCTION ID == 1 inner DCHECK is false
  mobile_apis::FunctionID::eType casted_enum;
  std::string converted;
  for (size_t i = 1; i < function_id_strings.size(); ++i) {
    casted_enum = static_cast<mobile_apis::FunctionID::eType>(i);
    converted = MessageHelper::StringifiedFunctionID(casted_enum);
    EXPECT_EQ(function_id_strings[i], converted);
  }
  // EventIDs emum strarts from delta_from_functions_id = 32768
  for (size_t i = delta_from_functions_id;
      i < events_id_strings.size() + delta_from_functions_id;
      ++i) {
    casted_enum = static_cast<mobile_apis::FunctionID::eType>(i);
    converted = MessageHelper::StringifiedFunctionID(casted_enum);
    EXPECT_EQ(events_id_strings[i - delta_from_functions_id], converted);
  }
}

TEST_F(MessageHelperTest,
    StringifiedHmiLevel_LevelEnum_EqualsWithStringsInArray) {
  mobile_apis::HMILevel::eType casted_enum;
  std::string converted_value;
  for (size_t i = 0; i < hmi_level_strings.size(); ++i) {
    casted_enum = static_cast<mobile_apis::HMILevel::eType>(i);
    converted_value = MessageHelper::StringifiedHMILevel(casted_enum);
    EXPECT_EQ(hmi_level_strings[i], converted_value);
  }
}

TEST_F(MessageHelperTest,
    StringToHmiLevel_LevelString_EqEType) {
  mobile_apis::HMILevel::eType tested_enum;
  mobile_apis::HMILevel::eType converted_enum;
  for (size_t i = 0; i < hmi_level_strings.size(); ++i) {
    tested_enum = static_cast<mobile_apis::HMILevel::eType>(i);
    converted_enum = MessageHelper::StringToHMILevel(hmi_level_strings[i]);
    EXPECT_EQ(tested_enum, converted_enum);
  }
}

TEST_F(MessageHelperTest,
    SubscribeApplicationToSoftButton_CallFromApp) {
  // Create application mock
  MockApplicationSharedPtr appSharedPtr = utils::MakeShared<AppMock>();
  // Prepare data for method
  smart_objects::SmartObject message_params;
  size_t function_id = 1;
  //
  EXPECT_CALL(*appSharedPtr,
      SubscribeToSoftButtons(function_id, SoftButtonID())).Times(1);
  MessageHelper::SubscribeApplicationToSoftButton(
      message_params, appSharedPtr, function_id);
}

}  // namespace test
}  // namespace application_manager
