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
#include "application_manager/mock_application.h"
#include "utils/custom_string.h"
#include "policy/mock_policy_settings.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/state_controller.h"
#include "application_manager/resumption/resume_ctrl.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace HmiLanguage = hmi_apis::Common_Language;
namespace HmiResults = hmi_apis::Common_Result;
namespace MobileResults = mobile_apis::Result;

using namespace application_manager;

typedef utils::SharedPtr<MockApplication> MockApplicationSharedPtr;
typedef std::vector<std::string> StringArray;
typedef utils::SharedPtr<application_manager::Application> ApplicationSharedPtr;

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
      application_manager::MessageHelper::CreateBlockedByPoliciesResponse(
          function_id, result, correlation_id, connection_key);

  EXPECT_TRUE(ptr);

  smart_objects::SmartObject& obj = *ptr;

  EXPECT_EQ(function_id, obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kResponse, obj[strings::params][strings::message_type].asInt());
  EXPECT_EQ(success, obj[strings::msg_params][strings::success].asBool());
  EXPECT_EQ(result, obj[strings::msg_params][strings::result_code].asInt());
  EXPECT_EQ(correlation_id,
            obj[strings::params][strings::correlation_id].asUInt());
  EXPECT_EQ(connection_key,
            obj[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(kV2, obj[strings::params][strings::protocol_version].asInt());
}

TEST(MessageHelperTestCreate, CreateSetAppIcon_SendNullPathImagetype_Equal) {
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

TEST(MessageHelperTestCreate, CreateSetAppIcon_SendPathImagetype_Equal) {
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
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  EXPECT_CALL(*appSharedMock, vr_help_title()).Times(AtLeast(1));
  EXPECT_CALL(*appSharedMock, vr_help()).Times(AtLeast(1));
  EXPECT_CALL(*appSharedMock, help_prompt()).Times(AtLeast(1));
  EXPECT_CALL(*appSharedMock, timeout_prompt()).Times(AtLeast(1));

  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateGlobalPropertiesRequestsToHMI(appSharedMock, 0u);

  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate,
     CreateGlobalPropertiesRequestsToHMI_SmartObject_NotEmpty) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  smart_objects::SmartObjectSPtr objPtr =
      MakeShared<smart_objects::SmartObject>();

  (*objPtr)[0][strings::vr_help_title] = "111";
  (*objPtr)[1][strings::vr_help] = "222";
  (*objPtr)[2][strings::keyboard_properties] = "333";
  (*objPtr)[3][strings::menu_title] = "444";
  (*objPtr)[4][strings::menu_icon] = "555";
  (*objPtr)[5][strings::help_prompt] = "666";
  (*objPtr)[6][strings::timeout_prompt] = "777";

  EXPECT_CALL(*appSharedMock, vr_help_title())
      .Times(AtLeast(3))
      .WillRepeatedly(Return(&(*objPtr)[0]));
  EXPECT_CALL(*appSharedMock, vr_help())
      .Times(AtLeast(2))
      .WillRepeatedly(Return(&(*objPtr)[1]));
  EXPECT_CALL(*appSharedMock, help_prompt())
      .Times(AtLeast(3))
      .WillRepeatedly(Return(&(*objPtr)[5]));
  EXPECT_CALL(*appSharedMock, timeout_prompt())
      .Times(AtLeast(2))
      .WillRepeatedly(Return(&(*objPtr)[6]));
  EXPECT_CALL(*appSharedMock, keyboard_props())
      .Times(AtLeast(2))
      .WillRepeatedly(Return(&(*objPtr)[2]));
  EXPECT_CALL(*appSharedMock, menu_title())
      .Times(AtLeast(2))
      .WillRepeatedly(Return(&(*objPtr)[3]));
  EXPECT_CALL(*appSharedMock, menu_icon())
      .Times(AtLeast(2))
      .WillRepeatedly(Return(&(*objPtr)[4]));
  EXPECT_CALL(*appSharedMock, app_id()).WillRepeatedly(Return(0));

  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateGlobalPropertiesRequestsToHMI(appSharedMock, 0u);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& first = *ptr[0];
  smart_objects::SmartObject& second = *ptr[1];

  EXPECT_EQ((*objPtr)[0], first[strings::msg_params][strings::vr_help_title]);
  EXPECT_EQ((*objPtr)[1], first[strings::msg_params][strings::vr_help]);
  EXPECT_EQ((*objPtr)[2],
            first[strings::msg_params][strings::keyboard_properties]);
  EXPECT_EQ((*objPtr)[3], first[strings::msg_params][strings::menu_title]);
  EXPECT_EQ((*objPtr)[4], first[strings::msg_params][strings::menu_icon]);
  EXPECT_EQ((*objPtr)[5], second[strings::msg_params][strings::help_prompt]);
  EXPECT_EQ((*objPtr)[6], second[strings::msg_params][strings::timeout_prompt]);
}

TEST(MessageHelperTestCreate, CreateShowRequestToHMI_SendSmartObject_Equal) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();

  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  const smart_objects::SmartObject& object = *smartObjectPtr;

  EXPECT_CALL(*appSharedMock, show_command())
      .Times(AtLeast(2))
      .WillRepeatedly(Return(&object));

  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateShowRequestToHMI(appSharedMock, 0u);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& obj = *ptr[0];

  int function_id = static_cast<int>(hmi_apis::FunctionID::UI_Show);

  EXPECT_EQ(function_id, obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(*smartObjectPtr, obj[strings::msg_params]);
}

TEST(MessageHelperTestCreate,
     CreateAddCommandRequestToHMI_SendSmartObject_Empty) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  ::application_manager::CommandsMap vis;
  DataAccessor<application_manager::CommandsMap> data_accessor(vis, true);

  EXPECT_CALL(*appSharedMock, commands_map()).WillOnce(Return(data_accessor));
  application_manager_test::MockApplicationManager mock_application_manager;
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddCommandRequestToHMI(appSharedMock,
                                                  mock_application_manager);

  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate,
     CreateAddCommandRequestToHMI_SendSmartObject_Equal) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  CommandsMap vis;
  DataAccessor<CommandsMap> data_accessor(vis, true);
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  smart_objects::SmartObject& object = *smartObjectPtr;

  object[strings::menu_params] = 1;
  object[strings::cmd_icon] = 1;
  object[strings::cmd_icon][strings::value] = "10";

  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(5, &object));

  EXPECT_CALL(*appSharedMock, commands_map()).WillOnce(Return(data_accessor));
  EXPECT_CALL(*appSharedMock, app_id()).WillOnce(Return(1u));
  application_manager_test::MockApplicationManager mock_application_manager;
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddCommandRequestToHMI(appSharedMock,
                                                  mock_application_manager);

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
  EXPECT_EQ(
      "10",
      obj[strings::msg_params][strings::cmd_icon][strings::value].asString());
}

TEST(MessageHelperTestCreate,
     CreateAddVRCommandRequestFromChoiceToHMI_SendEmptyData_EmptyList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  application_manager::ChoiceSetMap vis;
  DataAccessor< ::application_manager::ChoiceSetMap> data_accessor(vis, true);

  EXPECT_CALL(*appSharedMock, choice_set_map()).WillOnce(Return(data_accessor));
  application_manager_test::MockApplicationManager mock_application_manager;
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
          appSharedMock, mock_application_manager);

  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate,
     CreateAddVRCommandRequestFromChoiceToHMI_SendObject_EqualList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  application_manager::ChoiceSetMap vis;
  DataAccessor< ::application_manager::ChoiceSetMap> data_accessor(vis, true);
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  smart_objects::SmartObject& object = *smartObjectPtr;

  object[strings::choice_set] = "10";
  object[strings::grammar_id] = 111;
  object[strings::choice_set][0][strings::choice_id] = 1;
  object[strings::choice_set][0][strings::vr_commands] = 2;

  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(5, &object));
  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(6, &object));
  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(7, &object));
  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(8, &object));
  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(9, &object));

  EXPECT_CALL(*appSharedMock, choice_set_map()).WillOnce(Return(data_accessor));
  EXPECT_CALL(*appSharedMock, app_id())
      .Times(AtLeast(5))
      .WillRepeatedly(Return(1u));
  application_manager_test::MockApplicationManager mock_application_manager;
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
          appSharedMock, mock_application_manager);

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
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  application_manager::SubMenuMap vis;
  DataAccessor< ::application_manager::SubMenuMap> data_accessor(vis, true);
  smart_objects::SmartObjectSPtr smartObjectPtr =
      utils::MakeShared<smart_objects::SmartObject>();

  smart_objects::SmartObject& object = *smartObjectPtr;

  object[strings::position] = 1;
  object[strings::menu_name] = 1;

  vis.insert(std::pair<uint32_t, smart_objects::SmartObject*>(5, &object));

  EXPECT_CALL(*appSharedMock, sub_menu_map()).WillOnce(Return(data_accessor));
  EXPECT_CALL(*appSharedMock, app_id()).Times(AtLeast(1)).WillOnce(Return(1u));

  const uint32_t cor_id = 0u;
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddSubMenuRequestToHMI(appSharedMock, cor_id);

  EXPECT_FALSE(ptr.empty());

  smart_objects::SmartObject& obj = *ptr[0];

  int function_id = static_cast<int>(hmi_apis::FunctionID::UI_AddSubMenu);

  EXPECT_EQ(function_id, obj[strings::params][strings::function_id].asInt());
  EXPECT_EQ(5, obj[strings::msg_params][strings::menu_id].asInt());
  EXPECT_EQ(1,
            obj[strings::msg_params][strings::menu_params][strings::position]
                .asInt());
  EXPECT_EQ(1,
            obj[strings::msg_params][strings::menu_params][strings::menu_name]
                .asInt());
  EXPECT_EQ(1u, obj[strings::msg_params][strings::app_id].asUInt());
}

TEST(MessageHelperTestCreate,
     CreateAddSubMenuRequestToHMI_SendEmptyMap_EmptySmartObjectList) {
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  application_manager::SubMenuMap vis;
  DataAccessor< ::application_manager::SubMenuMap> data_accessor(vis, true);

  EXPECT_CALL(*appSharedMock, sub_menu_map()).WillOnce(Return(data_accessor));

  const uint32_t cor_id = 0u;
  smart_objects::SmartObjectList ptr =
      MessageHelper::CreateAddSubMenuRequestToHMI(appSharedMock, cor_id);

  EXPECT_TRUE(ptr.empty());
}

TEST(MessageHelperTestCreate, CreateNegativeResponse_SendSmartObject_Equal) {
  uint32_t connection_key = 111;
  int32_t function_id = 222;
  uint32_t correlation_id = 333u;
  int32_t result_code = 0;

  smart_objects::SmartObjectSPtr ptr = MessageHelper::CreateNegativeResponse(
      connection_key, function_id, correlation_id, result_code);

  EXPECT_TRUE(ptr);

  smart_objects::SmartObject& obj = *ptr;

  int objFunction_id = obj[strings::params][strings::function_id].asInt();
  uint32_t objCorrelation_id =
      obj[strings::params][strings::correlation_id].asUInt();
  int objResult_code = obj[strings::msg_params][strings::result_code].asInt();
  uint32_t objConnection_key =
      obj[strings::params][strings::connection_key].asUInt();

  int message_type = static_cast<int>(mobile_apis::messageType::response);
  int protocol_type =
      static_cast<int>(commands::CommandImpl::mobile_protocol_type_);
  int protocol_version =
      static_cast<int>(commands::CommandImpl::protocol_version_);
  bool success = false;

  EXPECT_EQ(function_id, objFunction_id);
  EXPECT_EQ(message_type, obj[strings::params][strings::message_type].asInt());
  EXPECT_EQ(protocol_type,
            obj[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(protocol_version,
            obj[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(correlation_id, objCorrelation_id);
  EXPECT_EQ(result_code, objResult_code);
  EXPECT_EQ(success, obj[strings::msg_params][strings::success].asBool());
  EXPECT_EQ(connection_key, objConnection_key);
}

class MessageHelperTest : public ::testing::Test {
 public:
  MessageHelperTest()
      : language_strings{"EN-US", "ES-MX", "FR-CA", "DE-DE", "ES-ES", "EN-GB",
                         "RU-RU", "TR-TR", "PL-PL", "FR-FR", "IT-IT", "SV-SE",
                         "PT-PT", "NL-NL", "EN-AU", "ZH-CN", "ZH-TW", "JA-JP",
                         "AR-SA", "KO-KR", "PT-BR", "CS-CZ", "DA-DK", "NO-NO",
                         "NL-BE", "EL-GR", "HU-HU", "FI-FI", "SK-SK"}
      , hmi_result_strings{"SUCCESS",
                           "UNSUPPORTED_REQUEST",
                           "UNSUPPORTED_RESOURCE",
                           "DISALLOWED",
                           "REJECTED",
                           "ABORTED",
                           "IGNORED",
                           "RETRY",
                           "IN_USE",
                           "DATA_NOT_AVAILABLE",
                           "TIMED_OUT",
                           "INVALID_DATA",
                           "CHAR_LIMIT_EXCEEDED",
                           "INVALID_ID",
                           "DUPLICATE_NAME",
                           "APPLICATION_NOT_REGISTERED",
                           "WRONG_LANGUAGE",
                           "OUT_OF_MEMORY",
                           "TOO_MANY_PENDING_REQUESTS",
                           "NO_APPS_REGISTERED",
                           "NO_DEVICES_CONNECTED",
                           "WARNINGS",
                           "GENERIC_ERROR",
                           "USER_DISALLOWED",
                           "TRUNCATED_DATA"}
      , mobile_result_strings{"SUCCESS",
                              "UNSUPPORTED_REQUEST",
                              "UNSUPPORTED_RESOURCE",
                              "DISALLOWED",
                              "REJECTED",
                              "ABORTED",
                              "IGNORED",
                              "RETRY",
                              "IN_USE",
                              "VEHICLE_DATA_NOT_AVAILABLE",
                              "TIMED_OUT",
                              "INVALID_DATA",
                              "CHAR_LIMIT_EXCEEDED",
                              "INVALID_ID",
                              "DUPLICATE_NAME",
                              "APPLICATION_NOT_REGISTERED",
                              "WRONG_LANGUAGE",
                              "OUT_OF_MEMORY",
                              "TOO_MANY_PENDING_REQUESTS",
                              "TOO_MANY_APPLICATIONS",
                              "APPLICATION_REGISTERED_ALREADY",
                              "WARNINGS",
                              "GENERIC_ERROR",
                              "USER_DISALLOWED",
                              "UNSUPPORTED_VERSION",
                              "VEHICLE_DATA_NOT_ALLOWED",
                              "FILE_NOT_FOUND",
                              "CANCEL_ROUTE",
                              "TRUNCATED_DATA",
                              "SAVED",
                              "INVALID_CERT",
                              "EXPIRED_CERT",
                              "RESUME_FAILED"}
      , function_id_strings{"RESERVED",
                            "RegisterAppInterface",
                            "UnregisterAppInterface",
                            "SetGlobalProperties",
                            "ResetGlobalProperties",
                            "AddCommand",
                            "DeleteCommand",
                            "AddSubMenu",
                            "DeleteSubMenu",
                            "CreateInteractionChoiceSet",
                            "PerformInteraction",
                            "DeleteInteractionChoiceSet",
                            "Alert",
                            "Show",
                            "Speak",
                            "SetMediaClockTimer",
                            "PerformAudioPassThru",
                            "EndAudioPassThru",
                            "SubscribeButton",
                            "UnsubscribeButton",
                            "SubscribeVehicleData",
                            "UnsubscribeVehicleData",
                            "GetVehicleData",
                            "ReadDID",
                            "GetDTCs",
                            "ScrollableMessage",
                            "Slider",
                            "ShowConstantTBT",
                            "AlertManeuver",
                            "UpdateTurnList",
                            "ChangeRegistration",
                            "GenericResponse",
                            "PutFile",
                            "DeleteFile",
                            "ListFiles",
                            "SetAppIcon",
                            "SetDisplayLayout",
                            "DiagnosticMessage",
                            "SystemRequest",
                            "SendLocation",
                            "DialNumber",
                            "GetWayPoints",
                            "SubscribeWayPoints",
                            "UnsubscribeWayPoints"}
      , events_id_strings{"OnHMIStatus",
                          "OnAppInterfaceUnregistered",
                          "OnButtonEvent",
                          "OnButtonPress",
                          "OnVehicleData",
                          "OnCommand",
                          "OnTBTClientState",
                          "OnDriverDistraction",
                          "OnPermissionsChange",
                          "OnAudioPassThru",
                          "OnLanguageChange",
                          "OnKeyboardInput",
                          "OnTouchEvent",
                          "OnSystemRequest",
                          "OnHashChange"}
      , hmi_level_strings{"FULL", "LIMITED", "BACKGROUND", "NONE"}
      , delta_from_functions_id(32768) {}

 protected:
  application_manager_test::MockApplicationManager mock_application_manager;
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
  for (size_t array_index = 0; array_index < language_strings.size();
       ++array_index) {
    enum_value = static_cast<HmiLanguage::eType>(array_index);
    enum_from_string_value =
        MessageHelper::CommonLanguageFromString(language_strings[array_index]);
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
  for (size_t array_index = 0; array_index < language_strings.size();
       ++array_index) {
    casted_enum = static_cast<HmiLanguage::eType>(array_index);
    string_from_enum = MessageHelper::CommonLanguageToString(casted_enum);
    EXPECT_EQ(language_strings[array_index], string_from_enum);
  }
  // Check InvalidEnum == -1
  string_from_enum =
      MessageHelper::CommonLanguageToString(HmiLanguage::INVALID_ENUM);
  EXPECT_EQ("", string_from_enum);
}

TEST_F(MessageHelperTest, ConvertEnumAPINoCheck_AnyEnumType_AnotherEnumType) {
  hmi_apis::Common_LayoutMode::eType tested_enum_value =
      hmi_apis::Common_LayoutMode::ICON_ONLY;
  hmi_apis::Common_AppHMIType::eType converted =
      MessageHelper::ConvertEnumAPINoCheck<hmi_apis::Common_LayoutMode::eType,
                                           hmi_apis::Common_AppHMIType::eType>(
          tested_enum_value);
  EXPECT_EQ(hmi_apis::Common_AppHMIType::DEFAULT, converted);
}

TEST_F(MessageHelperTest, HMIResultFromString_StringValueOfEnum_CorrectEType) {
  HmiResults::eType enum_value;
  HmiResults::eType enum_from_string_value;
  // Check all results >= 0
  for (size_t array_index = 0; array_index < hmi_result_strings.size();
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

TEST_F(MessageHelperTest, HMIResultToString_ETypeValueOfEnum_CorrectString) {
  std::string string_from_enum;
  HmiResults::eType casted_enum;
  // Check all results >=0
  for (size_t array_index = 0; array_index < hmi_result_strings.size();
       ++array_index) {
    casted_enum = static_cast<HmiResults::eType>(array_index);
    string_from_enum = MessageHelper::HMIResultToString(casted_enum);
    EXPECT_EQ(hmi_result_strings[array_index], string_from_enum);
  }
  // Check InvalidEnum == -1
  string_from_enum = MessageHelper::HMIResultToString(HmiResults::INVALID_ENUM);
  EXPECT_EQ("", string_from_enum);
}

TEST_F(MessageHelperTest,
       HMIToMobileResult_HmiResultEType_GetCorrectMobileResultEType) {
  MobileResults::eType tested_enum;
  HmiResults::eType casted_hmi_enum;
  MobileResults::eType converted_enum;
  // Check enums >=0
  for (size_t enum_index = 0; enum_index < hmi_result_strings.size();
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
  const StringArray wrong_strings{"soft_button1\t\ntext",
                                  "soft_button1\\ntext",
                                  "soft_button1\\ttext",
                                  " ",
                                  "soft_button1\t\n",
                                  "soft_button1\\n",
                                  "soft_button1\\t"};
  for (size_t i = 0; i < wrong_strings.size(); ++i) {
    EXPECT_FALSE(MessageHelper::VerifySoftButtonString(wrong_strings[i]));
  }
}

TEST_F(MessageHelperTest, VerifySoftButtonString_CorrectStrings_True) {
  const StringArray wrong_strings{"soft_button1.text",
                                  "soft_button1?text",
                                  " asd asdasd    .././/",
                                  "soft_button1??....asd",
                                  "soft_button12313fcvzxc./.,"};
  for (size_t i = 0; i < wrong_strings.size(); ++i) {
    EXPECT_TRUE(MessageHelper::VerifySoftButtonString(wrong_strings[i]));
  }
}

TEST_F(MessageHelperTest,
       GetIVISubscriptionRequests_ValidApplication_HmiRequestNotEmpty) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating data acessor
  application_manager::VehicleInfoSubscriptions vis;
  DataAccessor<application_manager::VehicleInfoSubscriptions> data_accessor(
      vis, true);
  // Calls for ApplicationManager
  EXPECT_CALL(*appSharedMock, app_id()).WillOnce(Return(1u));
  EXPECT_CALL(*appSharedMock, SubscribedIVI()).WillOnce(Return(data_accessor));

  smart_objects::SmartObjectList outList =
      MessageHelper::GetIVISubscriptionRequests(appSharedMock,
                                                mock_application_manager);
  // Expect not empty request
  EXPECT_FALSE(outList.empty());
}

TEST_F(MessageHelperTest,
       ProcessSoftButtons_SmartObjectWithoutButtonsKey_Success) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject object;
  policy_handler_test::MockPolicySettings policy_settings_;
  const policy::PolicyHandler policy_handler(policy_settings_,
                                             mock_application_manager);
  // Method call
  mobile_apis::Result::eType result = MessageHelper::ProcessSoftButtons(
      object, appSharedMock, policy_handler, mock_application_manager);
  // Expect
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
       ProcessSoftButtons_IncorectSoftButonValue_InvalidData) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject object;
  smart_objects::SmartObject& buttons = object[strings::soft_buttons];
  // Setting invalid image string to button
  buttons[0][strings::image][strings::value] = "invalid\\nvalue";
  policy_handler_test::MockPolicySettings policy_settings_;
  const policy::PolicyHandler policy_handler(policy_settings_,
                                             mock_application_manager);
  // Method call
  mobile_apis::Result::eType result = MessageHelper::ProcessSoftButtons(
      object, appSharedMock, policy_handler, mock_application_manager);
  // Expect
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}

TEST_F(MessageHelperTest, VerifyImage_ImageTypeIsStatic_Success) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject image;
  image[strings::image_type] = mobile_apis::ImageType::STATIC;
  // Method call
  mobile_apis::Result::eType result = MessageHelper::VerifyImage(
      image, appSharedMock, mock_application_manager);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest, VerifyImage_ImageValueNotValid_InvalidData) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject image;
  image[strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  // Invalid value
  image[strings::value] = "   ";
  // Method call
  mobile_apis::Result::eType result = MessageHelper::VerifyImage(
      image, appSharedMock, mock_application_manager);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}

TEST_F(MessageHelperTest, VerifyImageFiles_SmartObjectWithValidData_Success) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject images;
  images[0][strings::image_type] = mobile_apis::ImageType::STATIC;
  images[1][strings::image_type] = mobile_apis::ImageType::STATIC;
  // Method call
  mobile_apis::Result::eType result = MessageHelper::VerifyImageFiles(
      images, appSharedMock, mock_application_manager);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
       VerifyImageFiles_SmartObjectWithInvalidData_NotSuccsess) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject images;
  images[0][strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  images[1][strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  // Invalid values
  images[0][strings::value] = "   ";
  images[1][strings::value] = "image\\n";
  // Method call
  mobile_apis::Result::eType result = MessageHelper::VerifyImageFiles(
      images, appSharedMock, mock_application_manager);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result);
}

TEST_F(MessageHelperTest,
       VerifyImageVrHelpItems_SmartObjectWithSeveralValidImages_Succsess) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
  // Creating input data for method
  smart_objects::SmartObject message;
  message[0][strings::image][strings::image_type] =
      mobile_apis::ImageType::STATIC;
  message[1][strings::image][strings::image_type] =
      mobile_apis::ImageType::STATIC;
  // Method call
  mobile_apis::Result::eType result = MessageHelper::VerifyImageVrHelpItems(
      message, appSharedMock, mock_application_manager);
  // EXPECT
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(MessageHelperTest,
       VerifyImageVrHelpItems_SmartObjWithSeveralInvalidImages_NotSuccsess) {
  // Creating sharedPtr to MockApplication
  MockApplicationSharedPtr appSharedMock = utils::MakeShared<MockApplication>();
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
  mobile_apis::Result::eType result = MessageHelper::VerifyImageVrHelpItems(
      message, appSharedMock, mock_application_manager);
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

TEST_F(MessageHelperTest, StringToHmiLevel_LevelString_EqEType) {
  mobile_apis::HMILevel::eType tested_enum;
  mobile_apis::HMILevel::eType converted_enum;
  for (size_t i = 0; i < hmi_level_strings.size(); ++i) {
    tested_enum = static_cast<mobile_apis::HMILevel::eType>(i);
    converted_enum = MessageHelper::StringToHMILevel(hmi_level_strings[i]);
    EXPECT_EQ(tested_enum, converted_enum);
  }
}

TEST_F(MessageHelperTest, SubscribeApplicationToSoftButton_CallFromApp) {
  // Create application mock
  MockApplicationSharedPtr appSharedPtr = utils::MakeShared<MockApplication>();
  // Prepare data for method
  smart_objects::SmartObject message_params;
  size_t function_id = 1;
  //
  EXPECT_CALL(*appSharedPtr,
              SubscribeToSoftButtons(function_id, SoftButtonID())).Times(1);
  MessageHelper::SubscribeApplicationToSoftButton(
      message_params, mock_app_, function_id);
}

TEST_F(MessageHelperTest, SendOnAppUnregNotificationToHMI) {
  const bool kIsUnexpectedDisconnect = false;

  const uint32_t kMockHmiAppId = 1u;
  EXPECT_CALL(*mock_app_, hmi_app_id()).WillOnce(Return(kMockHmiAppId));

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnAppUnregNotificationToHMI(
      mock_app_, kIsUnexpectedDisconnect, mock_application_manager_);

  EXPECT_EQ(
      hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      MessageType::kNotification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(kMockHmiAppId,
            (*test_notification)[strings::msg_params][strings::app_id].asInt());
  EXPECT_EQ(
      kIsUnexpectedDisconnect,
      (*test_notification)[strings::msg_params][strings::unexpected_disconnect]
          .asInt());
}

TEST_F(MessageHelperTest, SendOnAppUnregNotificationToHMI_InvalidApp) {
  const bool kIsUnexpectedDisconnect = false;

  MockApplicationSharedPtr mock_app;

  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  MessageHelper::SendOnAppUnregNotificationToHMI(
      mock_app, kIsUnexpectedDisconnect, mock_application_manager_);
}

TEST_F(MessageHelperTest, SendOnButtonSubscriptionNotification) {
  const hmi_apis::Common_ButtonName::eType kButton =
      hmi_apis::Common_ButtonName::OK;
  const bool kIsSubscribed = true;

  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = kAppId_;
  msg_params[strings::name] = kButton;
  msg_params[strings::is_suscribed] = kIsSubscribed;

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnButtonSubscriptionNotification(
      kAppId_, kButton, kIsSubscribed, mock_application_manager_);

  EXPECT_EQ(
      application_manager::MessageType::kNotification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::protocol_version_,
      (*test_notification)[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(
      commands::CommandImpl::hmi_protocol_type_,
      (*test_notification)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(
      hmi_apis::FunctionID::Buttons_OnButtonSubscription,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      hmi_apis::FunctionID::Buttons_OnButtonSubscription,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(msg_params, (*test_notification)[strings::msg_params]);
}

TEST_F(MessageHelperTest, SendOnDataStreaming_Audio) {
  const protocol_handler::ServiceType kService =
      protocol_handler::ServiceType::kAudio;

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnDataStreaming(
      kService, kAvailable_, mock_application_manager_);

  EXPECT_EQ(
      hmi_apis::FunctionID::Navigation_OnAudioDataStreaming,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      hmi_apis::messageType::notification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::protocol_version_,
      (*test_notification)[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(
      commands::CommandImpl::hmi_protocol_type_,
      (*test_notification)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(kAvailable_,
            (*test_notification)[strings::msg_params]["available"].asInt());
}

TEST_F(MessageHelperTest, SendOnDataStreaming_MobileNav) {
  const protocol_handler::ServiceType kService =
      protocol_handler::ServiceType::kMobileNav;

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnDataStreaming(
      kService, kAvailable_, mock_application_manager_);

  EXPECT_EQ(
      hmi_apis::FunctionID::Navigation_OnVideoDataStreaming,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      hmi_apis::messageType::notification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::protocol_version_,
      (*test_notification)[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(
      commands::CommandImpl::hmi_protocol_type_,
      (*test_notification)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(kAvailable_,
            (*test_notification)[strings::msg_params]["available"].asInt());
}

TEST_F(MessageHelperTest, SendOnDataStreaming_Fail) {
  const protocol_handler::ServiceType kService =
      protocol_handler::ServiceType::kInvalidServiceType;

  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  MessageHelper::SendOnDataStreaming(
      kService, kAvailable_, mock_application_manager_);
}

TEST_F(MessageHelperTest, SendOnPermissionsChangeNotification) {
  const policy::RpcName kRpcName1 = "rpc_name_1";
  const std::string kHMIPermissionLabel1 = "level_1";
  const std::string kHMIPermissionValue1 = "hmi_level_1";

  std::set<policy::HMILevel> hmi_levels_1;
  hmi_levels_1.insert(kHMIPermissionValue1);

  policy::HMIPermissions hmi_permissions_1;
  hmi_permissions_1[kHMIPermissionLabel1] = hmi_levels_1;

  std::set<policy::Parameter> parameters_1;
  parameters_1.insert("parameter_1");

  policy::ParameterPermissions parameter_permissions_1;
  parameter_permissions_1["1"] = parameters_1;

  const policy::RpcPermissions kRpcPermissions1 = {hmi_permissions_1,
                                                   parameter_permissions_1};

  policy::Permissions permissions;
  permissions[kRpcName1] = kRpcPermissions1;

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_,
              ManageMobileCommand(_, commands::Command::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnPermissionsChangeNotification(
      kConnectionKey_, permissions, mock_application_manager_);

  EXPECT_EQ(
      mobile_apis::FunctionID::OnPermissionsChangeID,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      mobile_apis::messageType::notification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::mobile_protocol_type_,
      (*test_notification)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(
      kConnectionKey_,
      (*test_notification)[strings::params][strings::connection_key].asUInt());

  // Permissions Data check
  policy::Permissions::const_iterator it_permissions = permissions.begin();
  policy::Permissions::const_iterator it_permissions_end = permissions.end();
  const smart_objects::SmartObject& kPermissionsItemArray =
      (*test_notification)[strings::msg_params]["permissionItem"];

  for (size_t index_pi = 0; it_permissions != it_permissions_end;
       ++it_permissions, ++index_pi) {
    const smart_objects::SmartObject& kPermissionItem =
        kPermissionsItemArray[index_pi];

    EXPECT_EQ((*it_permissions).first, kPermissionItem["rpcName"].asString());

    // Checking the hmiPermissions
    const smart_objects::SmartObject& kHmiPermissions =
        kPermissionItem["hmiPermissions"];
    const policy::RpcPermissions& rpc_permissions = (*it_permissions).second;
    policy::HMIPermissions::const_iterator it_hmi_permissions =
        rpc_permissions.hmi_permissions.begin();
    policy::HMIPermissions::const_iterator it_hmi_permissions_end =
        rpc_permissions.hmi_permissions.end();

    for (; it_hmi_permissions != it_hmi_permissions_end; ++it_hmi_permissions) {
      std::set<policy::HMILevel>::const_iterator it_hmi_levels =
          (*it_hmi_permissions).second.begin();
      std::set<policy::HMILevel>::const_iterator it_hmi_levels_end =
          (*it_hmi_permissions).second.end();
      const smart_objects::SmartObject& kHmiLevels =
          kHmiPermissions[(*it_hmi_permissions).first];

      for (size_t index_hmi_levels = 0; it_hmi_levels != it_hmi_levels_end;
           ++it_hmi_levels, ++index_hmi_levels) {
        EXPECT_EQ(kHMIPermissionValue1,
                  kHmiLevels[index_hmi_levels].asString());
      }
    }

    // Checking the parameterPermissions
    const smart_objects::SmartObject& kParameterPermissions =
        kPermissionItem["parameterPermissions"];
    policy::ParameterPermissions::const_iterator it_parameter_permissions =
        rpc_permissions.parameter_permissions.begin();
    policy::ParameterPermissions::const_iterator it_parameter_permissions_end =
        rpc_permissions.parameter_permissions.end();

    for (; it_parameter_permissions != it_parameter_permissions_end;
         ++it_parameter_permissions) {
      const smart_objects::SmartObject& kParameters =
          kParameterPermissions[(*it_parameter_permissions).first];

      std::set<policy::Parameter>::const_iterator it_parameters =
          (*it_parameter_permissions).second.begin();
      std::set<policy::Parameter>::const_iterator it_parameters_end =
          (*it_parameter_permissions).second.end();

      for (size_t index_parameters = 0; it_parameters != it_parameters_end;
           ++it_parameters, ++index_parameters) {
        EXPECT_EQ(*it_parameters, kParameters[index_parameters].asString());
      }
    }
  }
}

TEST_F(MessageHelperTest, SendOnResumeAudioSourceToHMI) {
  EXPECT_CALL(mock_application_manager_, application(kAppId_))
      .WillOnce(Return(mock_app_));
  EXPECT_EQ(mock_app_, true);

  const uint32_t kCorrelationId = 0;
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnResumeAudioSourceToHMI(kAppId_,
                                              mock_application_manager_);

  EXPECT_EQ(
      hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      MessageType::kNotification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      kCorrelationId,
      (*test_notification)[strings::params][strings::correlation_id].asUInt());
  EXPECT_EQ(kAppId_,
            (*test_notification)[strings::msg_params][strings::app_id].asInt());
}

TEST_F(MessageHelperTest, SendOnResumeAudioSourceToHMI_InvalidAppId) {
  MockApplicationSharedPtr mock_app;
  EXPECT_CALL(mock_application_manager_, application(kAppId_))
      .WillOnce(Return(MockApplicationSharedPtr()));
  EXPECT_EQ(mock_app, false);
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID()).Times(0);
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  MessageHelper::SendOnResumeAudioSourceToHMI(kAppId_,
                                              mock_application_manager_);
}

TEST_F(MessageHelperTest, SendOnStatusUpdate) {
  const std::string& kStatus = "test_status";

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendOnStatusUpdate(kStatus, mock_application_manager_);

  EXPECT_EQ(
      hmi_apis::FunctionID::SDL_OnStatusUpdate,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      MessageType::kNotification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(kStatus,
            (*test_notification)[strings::msg_params]["status"].asString());
}

TEST_F(MessageHelperTest, SendPolicySnapshotNotification) {
  const std::vector<uint8_t> kPolicyData;
  const std::string& kUrl = "test_url";

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_,
              ManageMobileCommand(_, commands::Command::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendPolicySnapshotNotification(
      kConnectionKey_, kPolicyData, kUrl, mock_application_manager_);

  EXPECT_EQ(
      mobile_apis::FunctionID::OnSystemRequestID,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      mobile_apis::messageType::notification,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::mobile_protocol_type_,
      (*test_notification)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::protocol_version_,
      (*test_notification)[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(
      kConnectionKey_,
      (*test_notification)[strings::params][strings::connection_key].asInt());
  EXPECT_EQ(
      kUrl,
      (*test_notification)[strings::msg_params][mobile_notification::syncp_url]
          .asString());
}

TEST_F(MessageHelperTest, SendUnsubscribedWayPoints) {
  const uint32_t kCorrelationId = 0;
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  const bool kRes =
      MessageHelper::SendUnsubscribedWayPoints(mock_application_manager_);
  EXPECT_TRUE(kRes);

  EXPECT_EQ(
      hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      hmi_apis::messageType::request,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      commands::CommandImpl::protocol_version_,
      (*test_notification)[strings::params][strings::protocol_version].asInt());
  EXPECT_EQ(
      commands::CommandImpl::hmi_protocol_type_,
      (*test_notification)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(
      kCorrelationId,
      (*test_notification)[strings::params][strings::correlation_id].asInt());
}

TEST_F(MessageHelperTest, SendUpdateSDLResponse) {
  const std::string& kResult = "test_result";
  const uint32_t kCorrelationId = 1u;

  smart_objects::SmartObjectSPtr test_notification;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&test_notification), Return(true)));

  MessageHelper::SendUpdateSDLResponse(
      kResult, kCorrelationId, mock_application_manager_);

  EXPECT_EQ(
      hmi_apis::FunctionID::SDL_UpdateSDL,
      (*test_notification)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      MessageType::kResponse,
      (*test_notification)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      kCorrelationId,
      (*test_notification)[strings::params][strings::correlation_id].asInt());
  EXPECT_EQ(0,
            (*test_notification)[strings::params][hmi_response::code].asInt());
  EXPECT_EQ(kResult,
            (*test_notification)[strings::msg_params]["result"].asString());
}

TEST_F(MessageHelperTest,
       SendUIChangeRegistrationRequestToHMI_AppNotValid_UNSUCCESS) {
  MockApplicationSharedPtr mock_app;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  MessageHelper::SendUIChangeRegistrationRequestToHMI(
      mock_app, mock_application_manager_);
}

TEST_F(MessageHelperTest,
       SendUIChangeRegistrationRequestToHMI_NoAppTypes_UNSUCCESS) {
  EXPECT_CALL(*mock_app_, app_hmi_types())
      .WillOnce(Return(static_cast<smart_objects::SmartObject*>(NULL)));
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);
  MessageHelper::SendUIChangeRegistrationRequestToHMI(
      mock_app_, mock_application_manager_);
}

TEST_F(MessageHelperTest, SendUIChangeRegistrationRequestToHMI_SUCCESS) {
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId_));

  smart_objects::SmartObject dummy_app_types;
  EXPECT_CALL(*mock_app_, app_hmi_types())
      .WillRepeatedly(Return(&dummy_app_types));

  const mobile_api::Language::eType kDummyLanguage =
      mobile_api::Language::INVALID_ENUM;
  EXPECT_CALL(*mock_app_, ui_language())
      .WillRepeatedly(ReturnRef(kDummyLanguage));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kNextHMICorrelationID));

  MessageHelper::SendUIChangeRegistrationRequestToHMI(
      mock_app_, mock_application_manager_);
  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::UI_ChangeRegistration,
            (*result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId_, (*result)[strings::msg_params][strings::app_id].asInt());
}

TEST_F(MessageHelperTest, CreateDeviceListSO_EmptyDeviceList_SUCCESS) {
  connection_handler::DeviceMap devices;

  smart_objects::SmartObjectSPtr result(MessageHelper::CreateDeviceListSO(
      devices,
      *static_cast<policy::PolicyHandlerInterface*>(NULL),
      mock_application_manager_));

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(smart_objects::SmartType_Array,
            (*result)[strings::device_list].getType());
  EXPECT_EQ(0u, (*result)[strings::device_list].length());
}

TEST_F(MessageHelperTest, CreateDeviceListSO_SUCCESS) {
  connection_handler::DeviceMap devices;
  policy_test::MockPolicyHandlerInterface policy_handler;

  const uint32_t kDeviceHandle = 0u;
  const std::string kDeviceName = "test_device";
  connection_handler::Device device(kDeviceHandle, kDeviceName);

  devices.insert(std::pair<uint32_t, connection_handler::Device>(0, device));

  EXPECT_CALL(policy_handler, GetUserConsentForDevice(_))
      .WillOnce(Return(policy::DeviceConsent::kDeviceAllowed));
  EXPECT_CALL(mock_application_manager_, GetDeviceTransportType(_))
      .WillOnce(Return(hmi_apis::Common_TransportType::INVALID_ENUM));

  smart_objects::SmartObjectSPtr result(MessageHelper::CreateDeviceListSO(
      devices, policy_handler, mock_application_manager_));

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(smart_objects::SmartType_Array,
            (*result)[strings::device_list].getType());
  EXPECT_TRUE(0 < (*result)[strings::device_list].length());
}

TEST_F(MessageHelperTest, CreateGetVehicleDataRequest_NoParams_SUCCESS) {
  const uint32_t kCorrelationId = 1u;
  const std::vector<std::string> kDummyMsgParams;

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageHelper::CreateGetVehicleDataRequest(
      kCorrelationId, kDummyMsgParams, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(kCorrelationId,
            (*result)[strings::params][strings::correlation_id].asUInt());
}

TEST_F(MessageHelperTest, CreateGetVehicleDataRequest_SUCCESS) {
  const uint32_t kCorrelationId = 1u;
  const std::string kTestParam = "test_param";
  std::vector<std::string> msg_params;
  msg_params.push_back(kTestParam);

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageHelper::CreateGetVehicleDataRequest(
      kCorrelationId, msg_params, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(kCorrelationId,
            (*result)[strings::params][strings::correlation_id].asUInt());
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(kTestParam));
}

TEST_F(MessageHelperTest, CreateHMIApplicationStruct_SUCCESS) {
  protocol_handler_test::MockSessionObserver session_observer;
  policy_test::MockPolicyHandlerInterface policy_handler;

  smart_objects::SmartObject dummy_so(smart_objects::SmartType_Map);
  dummy_so["dummy_param"] = 0;
  EXPECT_CALL(*mock_app_, tts_name()).WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, vr_synonyms()).WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, ngn_media_screen_name())
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, app_hmi_types()).WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, is_media()).WillRepeatedly(Return(false));
  EXPECT_CALL(*mock_app_, hmi_app_id()).WillRepeatedly(Return(0u));
  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(0u));
  EXPECT_CALL(*mock_app_, policy_app_id())
      .WillRepeatedly(Return(std::string()));
  const mobile_api::Language::eType kUiLanguage =
      mobile_api::Language::INVALID_ENUM;
  EXPECT_CALL(*mock_app_, ui_language()).WillRepeatedly(ReturnRef(kUiLanguage));

  EXPECT_CALL(policy_handler, GetUserConsentForDevice(_))
      .WillRepeatedly(Return(policy::DeviceConsent::kDeviceAllowed));
  EXPECT_CALL(policy_handler, GetDeviceUSBTransportStatus(_))
      .WillRepeatedly(Return(hmi_apis::Common_UserSetting::DISABLED));
  EXPECT_CALL(session_observer, GetDataOnDeviceHandle(_, _, _, _, _))
      .WillRepeatedly(Return(kNextHMICorrelationID));
  EXPECT_CALL(mock_application_manager_, GetDeviceTransportType(_))
      .WillRepeatedly(Return(hmi_apis::Common_TransportType::INVALID_ENUM));

  const ::utils::custom_string::CustomString kAppName("app_name");
  EXPECT_CALL(*mock_app_, name()).WillRepeatedly(ReturnRef(kAppName));
  const std::string kAppIconPath("AppIconPath");
  EXPECT_CALL(*mock_app_, app_icon_path())
      .WillRepeatedly(ReturnRef(kAppIconPath));

  smart_objects::SmartObject result;
  EXPECT_TRUE(
      MessageHelper::CreateHMIApplicationStruct(mock_app_,
                                                session_observer,
                                                policy_handler,
                                                &result,
                                                mock_application_manager_));

  EXPECT_EQ(smart_objects::SmartType_Map, result.getType());
  EXPECT_NE(result.length(), 0u);
}

TEST_F(MessageHelperTest, SendStopAudioPathThru_SUCCESS) {
  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kNextHMICorrelationID));

  MessageHelper::SendStopAudioPathThru(mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::UI_EndAudioPassThru,
            (*result)[strings::params][strings::function_id].asInt());
}

TEST_F(MessageHelperTest, SendSDLActivateAppResponse_SUCCESS) {
  const uint32_t kCorrelationId = 0u;
  const std::string kStrAppId("Application_ID");
  policy::AppPermissions permissions(kStrAppId);
  permissions.isSDLAllowed = true;
  permissions.isAppPermissionsRevoked = false;
  permissions.appRevoked = false;

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageHelper::SendSDLActivateAppResponse(
      permissions, kCorrelationId, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::SDL_ActivateApp,
            (*result)[strings::params][strings::function_id].asInt());
}

TEST_F(MessageHelperTest, SendPolicyUpdate_SUCCESS) {
  const std::string kFilePath("file_path");
  const int kTimeout = 0u;
  std::vector<int> dummy_retries;
  dummy_retries.push_back(5);

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kNextHMICorrelationID));

  MessageHelper::SendPolicyUpdate(
      kFilePath, kTimeout, dummy_retries, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::BasicCommunication_PolicyUpdate,
            (*result)[strings::params][strings::function_id].asInt());
}

TEST_F(MessageHelperTest, SendSystemRequestNotification_SUCCESS) {
  smart_objects::SmartObject result;
  EXPECT_CALL(mock_application_manager_, ManageMobileCommand(_, _))
      .WillOnce(Return(true));

  MessageHelper::SendSystemRequestNotification(
      kConnectionKey_, result, mock_application_manager_);

  EXPECT_EQ(mobile_apis::FunctionID::OnSystemRequestID,
            result[strings::params][strings::function_id].asInt());
}

TEST_F(MessageHelperTest, SendShowRequestToHMI_NoApp_UNSUCCESS) {
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  ApplicationSharedPtr dummy_app;
  MessageHelper::SendShowRequestToHMI(dummy_app, mock_application_manager_);
}

TEST_F(MessageHelperTest, SendShowRequestToHMI_SUCCESS) {
  smart_objects::SmartObject dummy_show_command(smart_objects::SmartType_Map);

  EXPECT_CALL(*mock_app_, show_command())
      .WillRepeatedly(Return(&dummy_show_command));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillRepeatedly(Return(kNextHMICorrelationID));

  MessageHelper::SendShowRequestToHMI(mock_app_, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::UI_Show,
            (*result)[strings::params][strings::function_id].asInt());
}

TEST_F(MessageHelperTest, SendAddVRCommandToHMI_SUCCESS) {
  const uint32_t kCmdId = 0u;
  const smart_objects::SmartObject kDummyVrCommands;

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId_));
  EXPECT_CALL(*mock_app_, get_grammar_id()).WillRepeatedly(Return(0u));

  EXPECT_CALL(mock_application_manager_, application(_))
      .WillRepeatedly(Return(mock_app_));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillRepeatedly(Return(kNextHMICorrelationID));

  MessageHelper::SendAddVRCommandToHMI(
      kCmdId, kDummyVrCommands, kAppId_, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::VR_AddCommand,
            (*result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId_, (*result)[strings::msg_params][strings::app_id].asUInt());
}

TEST_F(MessageHelperTest, SendAudioStopStream_SUCCESS) {
  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillRepeatedly(Return(kNextHMICorrelationID));

  MessageHelper::SendAudioStopStream(kAppId_, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::Navigation_StopAudioStream,
            (*result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId_, (*result)[strings::msg_params][strings::app_id].asUInt());
}

TEST_F(MessageHelperTest, SendGetStatusUpdateResponse_SUCCESS) {
  const uint32_t kCorrelationId = 0u;
  const std::string kStatus("test_status");

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageHelper::SendGetStatusUpdateResponse(
      kStatus, kCorrelationId, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::SDL_GetStatusUpdate,
            (*result)[strings::params][strings::function_id].asInt());
}

TEST_F(MessageHelperTest,
       SendChangeRegistrationRequestToHMI_NotValidAppPtr_UNSUCCESS) {
  ApplicationSharedPtr app;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  MessageHelper::SendChangeRegistrationRequestToHMI(app,
                                                    mock_application_manager_);
}

TEST_F(MessageHelperTest, SendChangeRegistrationRequestToHMI_SUCCESS) {
  const mobile_api::Language::eType kAppLanguage = mobile_api::Language::RU_RU;

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId_));
  EXPECT_CALL(*mock_app_, language()).WillRepeatedly(ReturnRef(kAppLanguage));
  EXPECT_CALL(*mock_app_, ui_language())
      .WillRepeatedly(ReturnRef(kAppLanguage));

  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillRepeatedly(Return(kNextHMICorrelationID));
  smart_objects::SmartObjectSPtr vr_command_result;
  smart_objects::SmartObjectSPtr tts_command_result;
  smart_objects::SmartObjectSPtr ui_command_result;
  {
    ::testing::InSequence sequence;
    EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));
    EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&tts_command_result), Return(true)));
    EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));
  }
  MessageHelper::SendChangeRegistrationRequestToHMI(mock_app_,
                                                    mock_application_manager_);

  ASSERT_TRUE(vr_command_result.valid());
  ASSERT_TRUE(tts_command_result.valid());
  ASSERT_TRUE(ui_command_result.valid());
  EXPECT_EQ(
      hmi_apis::FunctionID::VR_ChangeRegistration,
      (*vr_command_result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      hmi_apis::FunctionID::TTS_ChangeRegistration,
      (*tts_command_result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      hmi_apis::FunctionID::UI_ChangeRegistration,
      (*ui_command_result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(
      kAppId_,
      (*vr_command_result)[strings::msg_params][strings::app_id].asBool());
  EXPECT_EQ(
      kAppId_,
      (*tts_command_result)[strings::msg_params][strings::app_id].asBool());
  EXPECT_EQ(
      kAppId_,
      (*ui_command_result)[strings::msg_params][strings::app_id].asBool());
}

TEST_F(MessageHelperTest, SendAddSubMenuRequestToHMI_NoApp_UNSUCCESS) {
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  ApplicationSharedPtr dummy_app;
  MessageHelper::SendAddSubMenuRequestToHMI(dummy_app,
                                            mock_application_manager_);
}

TEST_F(MessageHelperTest, SendAddSubMenuRequestToHMI_SUCCESS) {
  SubMenuMap commands_map;
  smart_objects::SmartObject command(smart_objects::SmartType_Map);
  commands_map.insert(SubMenuMap::value_type(0u, &command));

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId_));
  EXPECT_CALL(*mock_app_, sub_menu_map())
      .WillRepeatedly(Return(DataAccessor<SubMenuMap>(commands_map, lock_)));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kNextHMICorrelationID));

  MessageHelper::SendAddSubMenuRequestToHMI(mock_app_,
                                            mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::UI_AddSubMenu,
            (*result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId_, (*result)[strings::msg_params][strings::app_id].asUInt());
}

TEST_F(MessageHelperTest,
       SendAllOnButtonSubscriptionNotificationsForApp_NoApp_UNSUCCESS) {
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);

  ApplicationSharedPtr dummy_app;
  MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
      dummy_app, mock_application_manager_);
}

TEST_F(MessageHelperTest,
       SendAllOnButtonSubscriptionNotificationsForApp_SUCCESS) {
  EXPECT_CALL(*mock_app_, hmi_app_id()).WillRepeatedly(Return(kAppId_));

  ButtonSubscriptions subscribed_buttons;
  subscribed_buttons.insert(mobile_apis::ButtonName::CUSTOM_BUTTON);

  EXPECT_CALL(*mock_app_, SubscribedButtons())
      .WillRepeatedly(
          Return(DataAccessor<ButtonSubscriptions>(subscribed_buttons, lock_)));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
      mock_app_, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::Buttons_OnButtonSubscription,
            (*result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId_, (*result)[strings::msg_params][strings::app_id].asUInt());
}

TEST_F(MessageHelperTest, SendAudioStartStream_SUCCESS) {
  MockApplicationManagerSettings app_mngr_settings;

  EXPECT_CALL(mock_application_manager_, get_settings())
      .WillRepeatedly(ReturnRef(app_mngr_settings));

  const std::string kAudioServerType("test_audio_server_type");
  EXPECT_CALL(app_mngr_settings, audio_server_type())
      .WillRepeatedly(ReturnRef(kAudioServerType));

  const std::string kAudioStreamFile("test_audio_stream_file");
  EXPECT_CALL(app_mngr_settings, audio_stream_file())
      .WillRepeatedly(ReturnRef(kAudioStreamFile));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kNextHMICorrelationID));

  MessageHelper::SendAudioStartStream(kAppId_, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::Navigation_StartAudioStream,
            (*result)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId_, (*result)[strings::msg_params][strings::app_id].asUInt());
}

TEST_F(MessageHelperTest, SendGetListOfPermissionsResponse_SUCCESS) {
  std::vector<policy::FunctionalGroupPermission> permissions;
  policy::CCSStatus ccs_status;
  policy::FunctionalGroupPermission permission;
  permission.state = policy::GroupConsent::kGroupAllowed;
  permissions.push_back(permission);

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  const uint32_t kCorrelationId = 0u;
  MessageHelper::SendGetListOfPermissionsResponse(
      permissions, ccs_status, kCorrelationId, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  EXPECT_EQ(hmi_apis::FunctionID::SDL_GetListOfPermissions,
            (*result)[strings::params][strings::function_id].asInt());

  smart_objects::SmartObject& msg_params = (*result)[strings::msg_params];
  const std::string ccs_status_key = "ccsStatus";
  EXPECT_TRUE(msg_params.keyExists(ccs_status_key));
  EXPECT_TRUE(msg_params[ccs_status_key].empty());
}

TEST_F(MessageHelperTest,
       SendGetListOfPermissionsResponse_CCSStatusNonEmpty_SUCCESS) {
  using namespace policy;
  using namespace smart_objects;

  std::vector<policy::FunctionalGroupPermission> permissions;

  CCSStatus ccs_status;
  const int32_t entity_type_1 = 1;
  const int32_t entity_id_1 = 2;
  const EntityStatus entity_status_1 = kStatusOn;
  const int32_t entity_type_2 = 3;
  const int32_t entity_id_2 = 4;
  const EntityStatus entity_status_2 = kStatusOff;
  ccs_status.insert(CCSStatusItem(entity_type_1, entity_id_1, entity_status_1));
  ccs_status.insert(CCSStatusItem(entity_type_2, entity_id_2, entity_status_2));

  SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  const uint32_t correlation_id = 0u;
  MessageHelper::SendGetListOfPermissionsResponse(
      permissions, ccs_status, correlation_id, mock_application_manager_);

  ASSERT_TRUE(result.valid());

  SmartObject& msg_params = (*result)[strings::msg_params];
  const std::string ccs_status_key = "ccsStatus";
  EXPECT_TRUE(msg_params.keyExists(ccs_status_key));

  SmartArray* status_array = msg_params[ccs_status_key].asArray();
  EXPECT_TRUE(ccs_status.size() == status_array->size());

  const std::string entityType = "entityType";
  const std::string entityID = "entityID";
  const std::string status = "status";
  SmartObject item_1_so = SmartObject(SmartType_Map);
  item_1_so[entityType] = entity_type_1;
  item_1_so[entityID] = entity_id_1;
  item_1_so[status] = hmi_apis::Common_EntityStatus::ON;

  SmartObject item_2_so = SmartObject(SmartType_Map);
  item_2_so[entityType] = entity_type_2;
  item_2_so[entityID] = entity_id_2;
  item_2_so[status] = hmi_apis::Common_EntityStatus::OFF;

  EXPECT_TRUE(status_array->end() !=
              std::find(status_array->begin(), status_array->end(), item_1_so));
  EXPECT_TRUE(status_array->end() !=
              std::find(status_array->begin(), status_array->end(), item_2_so));
}

TEST_F(MessageHelperTest,
       SendGetSystemInfoRequest_ExpectSendCorrectMessageToHMI) {
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageHelper::SendGetSystemInfoRequest(mock_application_manager_);

  ASSERT_TRUE(result.valid());
  EXPECT_EQ(hmi_apis::FunctionID::BasicCommunication_GetSystemInfo,
            (*result)[strings::params][strings::function_id].asInt());
  CheckParamsOfMessage(result, false, true);
}

TEST_F(
    MessageHelperTest,
    SendGetUserFriendlyMessageResponseWithEmptyMessages_ExpectSendCorrectMessageToHMI) {
  std::vector<policy::UserFriendlyMessage> msg;
  CheckSendingGetUserFriendlyMessage(msg);
}

TEST_F(
    MessageHelperTest,
    SendGetUserFriendlyMessageResponseWithMessages_ExpectSendCorrectMessageToHMI) {
  policy::UserFriendlyMessage arr[] = {
      {"first_code"}, {"second_code"}, {"third_code"}};
  std::vector<policy::UserFriendlyMessage> msg(arr, arr + 3);
  CheckSendingGetUserFriendlyMessage(msg);
}

TEST_F(MessageHelperTest,
       SendGlobalPropertiesToHMIApplicationIsNotValid_ExpectReturnFromMethod) {
  MockApplicationSharedPtr mock_app;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_)).Times(0);
  MessageHelper::SendGlobalPropertiesToHMI(mock_app, mock_application_manager_);
}

TEST_F(
    MessageHelperTest,
    SendGlobalPropertiesToHMIWithUIGlobalProperties_ExpectSendCorrectMessageToHMI) {
  const uint8_t number_msg_params = 6;
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  smart_objects::SmartObject dummy_so(smart_objects::SmartType_Map);
  dummy_so["dummy_param"] = "dummy_param";
  EXPECT_CALL(*mock_app_, vr_help_title())
      .Times(3)
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, vr_help()).Times(2).WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .Times(2)
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, menu_title())
      .Times(2)
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, menu_icon())
      .Times(2)
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(*mock_app_, help_prompt())
      .WillOnce(Return(static_cast<smart_objects::SmartObject*>(NULL)));
  EXPECT_CALL(*mock_app_, timeout_prompt())
      .WillOnce(Return(static_cast<smart_objects::SmartObject*>(NULL)));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  MessageHelper::SendGlobalPropertiesToHMI(mock_app_,
                                           mock_application_manager_);

  ASSERT_TRUE(result.valid());
  smart_objects::SmartObject& global_properties_msg_params =
      (*result)[strings::msg_params];

  EXPECT_EQ((*result)[strings::params][strings::function_id].asInt(),
            hmi_apis::FunctionID::UI_SetGlobalProperties);
  CheckParamsOfMessage(result, true, true);

  EXPECT_EQ(global_properties_msg_params.length(), number_msg_params);
  EXPECT_TRUE(global_properties_msg_params.keyExists(strings::vr_help_title));
  EXPECT_TRUE(global_properties_msg_params.keyExists(strings::vr_help));
  EXPECT_TRUE(
      global_properties_msg_params.keyExists(strings::keyboard_properties));
  EXPECT_TRUE(global_properties_msg_params.keyExists(strings::menu_title));
  EXPECT_TRUE(global_properties_msg_params.keyExists(strings::menu_icon));
  std::set<std::string> keys = global_properties_msg_params.enumerate();
  keys.erase(strings::app_id);
  for (std::set<std::string>::iterator it = keys.begin(); it != keys.end();
       ++it) {
    EXPECT_TRUE(global_properties_msg_params[(*it)] == dummy_so);
  }
}

TEST_F(
    MessageHelperTest,
    SendGlobalPropertiesToHMIWithTTSGlobalProperties_ExpectSendCorrectMessageToHMI) {
  const uint8_t number_msg_params = 3;
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  smart_objects::SmartObject dummy_so(smart_objects::SmartType_Map);
  dummy_so["dummy_param"] = "dummy_param";
  EXPECT_CALL(*mock_app_, vr_help_title())
      .WillOnce(Return(static_cast<smart_objects::SmartObject*>(NULL)));
  EXPECT_CALL(*mock_app_, vr_help())
      .WillOnce(Return(static_cast<smart_objects::SmartObject*>(NULL)));
  EXPECT_CALL(*mock_app_, help_prompt())
      .Times(3)
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, timeout_prompt())
      .Times(2)
      .WillRepeatedly(Return(&dummy_so));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  MessageHelper::SendGlobalPropertiesToHMI(mock_app_,
                                           mock_application_manager_);

  ASSERT_TRUE(result.valid());
  smart_objects::SmartObject& global_properties_msg_params =
      (*result)[strings::msg_params];

  EXPECT_EQ((*result)[strings::params][strings::function_id].asInt(),
            hmi_apis::FunctionID::TTS_SetGlobalProperties);
  CheckParamsOfMessage(result, true, true);
  EXPECT_EQ(global_properties_msg_params.length(), number_msg_params);
  EXPECT_TRUE(global_properties_msg_params.keyExists(strings::help_prompt));
  EXPECT_TRUE(global_properties_msg_params.keyExists(strings::timeout_prompt));
  std::set<std::string> keys = global_properties_msg_params.enumerate();
  keys.erase(strings::app_id);
  for (std::set<std::string>::iterator it = keys.begin(); it != keys.end();
       ++it) {
    EXPECT_TRUE(global_properties_msg_params[(*it)] == dummy_so);
  }
}

TEST_F(MessageHelperTest,
       SendHashUpdateNotificationApplicationIsNotValid_ExpectReturnFromMethod) {
  MockApplicationSharedPtr mock_app;
  EXPECT_CALL(mock_application_manager_, application(_))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(mock_application_manager_, ManageMobileCommand(_, _)).Times(0);
  MessageHelper::SendHashUpdateNotification(kAppId, mock_application_manager_);
}

TEST_F(MessageHelperTest,
       SendHashUpdateNotification_ExpectSendCorrectMessageToMobile) {
  resumption::MockResumeCtrl mock_resume_ctrl;
  EXPECT_CALL(mock_application_manager_, application(_))
      .WillOnce(Return(mock_app_));
  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  EXPECT_CALL(mock_application_manager_, resume_controller())
      .WillOnce(ReturnRef(mock_resume_ctrl));
  EXPECT_CALL(mock_resume_ctrl, ApplicationsDataUpdated()).Times(1);
  MessageHelper::SendHashUpdateNotification(kAppId, mock_application_manager_);
  ASSERT_TRUE(result.valid());
  EXPECT_EQ((*result)[strings::params][strings::function_id].asInt(),
            mobile_apis::FunctionID::OnHashChangeID);
  EXPECT_EQ((*result)[strings::params][strings::connection_key].asInt(),
            kAppId);
}

TEST_F(MessageHelperTest, SendNaviStartStream_ExpectSendCorrectMessageToHMI) {
  uint16_t video_streaming_port = 8080;
  CheckParametersNaviStartStreamMessage(
      "http://127.0.0.1:8080", "socket", "127.0.0.1", video_streaming_port);
  CheckParametersNaviStartStreamMessage("pipe_name", "pipe", "pipe_name");
  CheckParametersNaviStartStreamMessage("file_name", "file", "file_name");
}

TEST_F(MessageHelperTest, SendNaviStopStream_ExpectSendCorrectMessageToHMI) {
  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  MessageHelper::SendNaviStopStream(kAppId, mock_application_manager_);

  ASSERT_TRUE(result.valid());
  EXPECT_TRUE((*result)[strings::params].keyExists(strings::function_id));
  EXPECT_EQ((*result)[strings::params][strings::function_id].asInt(),
            hmi_apis::FunctionID::Navigation_StopStream);
  CheckParamsOfMessage(result, true, true);
}

TEST_F(MessageHelperTest,
       SendOnAppPermissionsChangedNotification_ExpectSendCorrectMessageToHMI) {
  std::string policy_app_id("policyappid");
  policy::AppPermissions dummy_permissions(policy_app_id);
  dummy_permissions.appRevoked = true;
  dummy_permissions.isAppPermissionsRevoked = true;
  std::vector<policy::FunctionalGroupPermission> functional_group_permissions;
  policy::FunctionalGroupPermission obj;
  FillFunctionalGroupPermissionObj(obj,
                                   "group_alias1",
                                   "group_name1",
                                   1,
                                   policy::GroupConsent::kGroupAllowed);
  functional_group_permissions.push_back(obj);
  FillFunctionalGroupPermissionObj(obj,
                                   "group_alias2",
                                   "group_name2",
                                   2,
                                   policy::GroupConsent::kGroupDisallowed);
  functional_group_permissions.push_back(obj);
  FillFunctionalGroupPermissionObj(obj,
                                   "group_alias3",
                                   "group_name3",
                                   3,
                                   policy::GroupConsent::kGroupUndefined);
  dummy_permissions.appRevokedPermissions = functional_group_permissions;
  dummy_permissions.appPermissionsConsentNeeded = true;
  dummy_permissions.appUnauthorized = true;
  dummy_permissions.priority = "NORMAL";
  dummy_permissions.requestTypeChanged = true;
  std::vector<std::string> request_type(3, "dummy");
  dummy_permissions.requestType = request_type;
  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_application_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));
  MessageHelper::SendOnAppPermissionsChangedNotification(
      kAppId, dummy_permissions, mock_application_manager_);
  ASSERT_TRUE(result.valid());
  EXPECT_TRUE((*result)[strings::params].keyExists(strings::function_id));
  EXPECT_EQ((*result)[strings::params][strings::function_id].asInt(),
            hmi_apis::FunctionID::SDL_OnAppPermissionChanged);
  CheckParamsOfMessage(result, true, false);
  uint16_t number_msg_params = 8;
  std::string app_revoked("appRevoked");
  std::string is_app_permissions_revoked("isAppPermissionsRevoked");
  std::string app_revoked_permissions("appRevokedPermissions");
  std::string allowed("allowed");
  std::string name("name");
  std::string id("id");

  EXPECT_EQ((*result)[strings::msg_params].length(), number_msg_params);
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(app_revoked));
  EXPECT_TRUE((*result)[strings::msg_params][app_revoked].asBool());
  EXPECT_TRUE(
      (*result)[strings::msg_params].keyExists(is_app_permissions_revoked));
  EXPECT_TRUE(
      (*result)[strings::msg_params][is_app_permissions_revoked].asBool());
  EXPECT_TRUE(
      (*result)[strings::msg_params].keyExists(app_revoked_permissions));
  EXPECT_TRUE(
      ((*result)[strings::msg_params][app_revoked_permissions].length() ==
       functional_group_permissions.size()));
  uint16_t number_revoked_params = 3;
  uint16_t number_revoked_params_without_allowed = 2;
  smart_objects::SmartObject& revoked_items =
      (*result)[strings::msg_params][app_revoked_permissions];
  for (uint16_t i = 0; i < revoked_items.length(); ++i) {
    if (policy::GroupConsent::kGroupUndefined ==
        functional_group_permissions[i].state) {
      EXPECT_TRUE(revoked_items[i].length() ==
                  number_revoked_params_without_allowed);
      EXPECT_FALSE(revoked_items[i].keyExists(allowed));
    } else {
      EXPECT_TRUE(revoked_items[i].length() == number_revoked_params);
      EXPECT_TRUE(revoked_items[i].keyExists(allowed));
      if (policy::kGroupAllowed == functional_group_permissions[i].state) {
        EXPECT_TRUE(revoked_items[i][allowed].asBool());
      } else {
        EXPECT_FALSE(revoked_items[i][allowed].asBool());
      }
    }
    EXPECT_TRUE(revoked_items[i].keyExists(name));
    EXPECT_TRUE(revoked_items[i][name].asString() ==
                functional_group_permissions[i].group_alias);
    EXPECT_TRUE(revoked_items[i].keyExists(id));
    EXPECT_TRUE(revoked_items[i][id].asInt() ==
                functional_group_permissions[i].group_id);
  }

  std::string app_permissions_consent_needed("appPermissionsConsentNeeded");
  std::string app_unauthorized("appUnauthorized");
  std::string priority("priority");
  EXPECT_TRUE(
      (*result)[strings::msg_params].keyExists(app_permissions_consent_needed));
  EXPECT_TRUE(
      (*result)[strings::msg_params][app_permissions_consent_needed].asBool());
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(app_unauthorized));
  EXPECT_TRUE((*result)[strings::msg_params][app_unauthorized].asBool());
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(priority));
  EXPECT_EQ((*result)[strings::msg_params][priority].asInt(),
            hmi_apis::Common_AppPriority::NORMAL);
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(strings::request_type));
  smart_objects::SmartObject& request_types_array =
      (*result)[strings::msg_params][strings::request_type];
  EXPECT_TRUE(request_types_array.length() ==
              dummy_permissions.requestType.size());
  for (uint16_t i = 0; i < request_types_array.length(); ++i) {
    EXPECT_TRUE(request_types_array[i].asString() ==
                dummy_permissions.requestType[i]);
  }
>>>>>>> d63b94c... Changes CCS item implementation in order to support status as boolean
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
