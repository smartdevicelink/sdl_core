/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "gtest/gtest.h"

#include "policy/policy_manager_impl_test_base.h"
#include "utils/date_time.h"

namespace test {
namespace components {
namespace policy_test {

using ::testing::_;
using ::testing::Return;

// Tests that use device without Consent
TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddNewApplicationFromDeviceWithConsent_ExpectUpdateRequired) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillRepeatedly(Return(device_id_1_));

  policy_manager_->SetUserConsentForDevice(device_id_1_, true);

  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));

  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddNewApplicationFromDeviceWithoutConsent_ExpectUpToDate) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  // To set UP_TO_DATE before registration
  GetPTU(kValidSdlPtUpdateJson);

  const TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  const int days_after_epoch = current_time.tv_sec / kSecondsInDay;

  policy_manager_->PTUpdatedAt(DAYS_AFTER_EPOCH, days_after_epoch);
  policy_manager_->PTUpdatedAt(KILOMETERS, 1000);

  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillRepeatedly(Return(device_id_1_));

  policy_manager_->SetUserConsentForDevice(device_id_1_, false);

  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));

  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithoutConsent_ExpectPreDataConsent) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));
  ASSERT_TRUE(policy_manager_->IsPredataPolicy(app_id_1_));
  policy_manager_->ReactOnUserDevConsentForApp(app_id_1_, false);
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_1_));
}

// Related to manual test APPLINK-18768
TEST_F(PolicyManagerImplTest2,
       CheckPreDataConsent_AppRegistered_ExpectReceivedRpcPermissionCorrect) {
  // Arrange
  CreateLocalPT(kSdlPreloadedPtJson2);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::DEFAULT));

  // Expect RPCs from pre_dataConsent group are allowed
  // Next checks are equal to BaseBeforeDataConsent_APIs.xml checks from task
  CheckRpcPermissions("ListFiles", ::policy::kRpcAllowed);
  CheckRpcPermissions("PutFile", ::policy::kRpcAllowed);
  CheckRpcPermissions("SetAppIcon", ::policy::kRpcAllowed);
  CheckRpcPermissions("SetGlobalProperties", ::policy::kRpcAllowed);
  CheckRpcPermissions("ResetGlobalProperties", ::policy::kRpcAllowed);
  CheckRpcPermissions("SetDisplayLayout", ::policy::kRpcAllowed);
  CheckRpcPermissions("ListFiles", ::policy::kRpcAllowed);
  CheckRpcPermissions("DeleteFile", ::policy::kRpcAllowed);

  // Next checks are equal to NonBaseBeforeDataConsent_APIs.xml checks from task
  CheckRpcPermissions("AddSubMenu", ::policy::kRpcDisallowed);
  CheckRpcPermissions("AddCommand", ::policy::kRpcDisallowed);
  CheckRpcPermissions("DeleteCommand", ::policy::kRpcDisallowed);
  CheckRpcPermissions("DeleteSubMenu", ::policy::kRpcDisallowed);
  CheckRpcPermissions("CreateInteractionChoiceSet", ::policy::kRpcDisallowed);
  CheckRpcPermissions("PerformInteraction", ::policy::kRpcDisallowed);
  CheckRpcPermissions("DeleteInteractionChoiceSet", ::policy::kRpcDisallowed);
  CheckRpcPermissions("Alert", ::policy::kRpcDisallowed);
  CheckRpcPermissions("Show", ::policy::kRpcDisallowed);
  CheckRpcPermissions("Speak", ::policy::kRpcDisallowed);
  CheckRpcPermissions("SetMediaClockTimer", ::policy::kRpcDisallowed);
  CheckRpcPermissions("SubscribeButton", ::policy::kRpcDisallowed);
  CheckRpcPermissions("UnsubscribeButton", ::policy::kRpcDisallowed);
  CheckRpcPermissions("PerformAudioPassThru", ::policy::kRpcDisallowed);
  CheckRpcPermissions("EndAudioPassThru", ::policy::kRpcDisallowed);
  CheckRpcPermissions("SubscribeVehicleData", ::policy::kRpcDisallowed);
  CheckRpcPermissions("UnsubscribeVehicleData", ::policy::kRpcDisallowed);
  CheckRpcPermissions("GetVehicleData", ::policy::kRpcDisallowed);
  CheckRpcPermissions("ReadDID", ::policy::kRpcDisallowed);
  CheckRpcPermissions("GetDTCs", ::policy::kRpcDisallowed);
  CheckRpcPermissions("ScrollableMessage", ::policy::kRpcDisallowed);
  CheckRpcPermissions("Slider", ::policy::kRpcDisallowed);
  CheckRpcPermissions("ShowConstantTBT", ::policy::kRpcDisallowed);
  CheckRpcPermissions("AlertManeuver", ::policy::kRpcDisallowed);
  CheckRpcPermissions("UpdateTurnList", ::policy::kRpcDisallowed);
  CheckRpcPermissions("SendLocation", ::policy::kRpcDisallowed);
  CheckRpcPermissions("GenericRequest", ::policy::kRpcDisallowed);
  CheckRpcPermissions("DialNumber", ::policy::kRpcDisallowed);
}

// Related to manual tests APPLINK-18763, APPLINK-18764
TEST_F(PolicyManagerImplTest2, CheckPreDataConsent_GetDefaultHmiLevel_NONE) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  std::string default_hmi;
  // Default HMI level is NONE
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi);
  EXPECT_EQ("NONE", default_hmi);

  // Default priority level is NONE
  std::string priority1;
  EXPECT_TRUE(policy_manager_->GetPriority(app_id_2_, &priority1));
  EXPECT_EQ("NONE", priority1);
}

// Related to manual tests APPLINK-18763, APPLINK-18764
TEST_F(PolicyManagerImplTest2,
       CheckPreDataConsent_GetDefaultHmiLevel_BACKGROUNG) {
  // Arrange
  CreateLocalPT(kSdlPreloadedPtJson2);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  std::string default_hmi;
  // Default HMI level is BACKGROUND
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi);
  EXPECT_EQ("BACKGROUND", default_hmi);
  // Default priority level is EMERGENCY
  std::string priority1;
  EXPECT_TRUE(policy_manager_->GetPriority(app_id_2_, &priority1));
  EXPECT_EQ("EMERGENCY", priority1);
}

TEST_F(
    PolicyManagerImplTest2,
    ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithConsent_ExpectDefault) {
  // Arrange
  // RequestTypes for default & preDataConsent are different
  CreateLocalPT("json/ptu_requestType.json");
  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));
  ASSERT_TRUE(policy_manager_->IsPredataPolicy(app_id_1_));
  policy_manager_->ReactOnUserDevConsentForApp(app_id_1_, true);
  EXPECT_FALSE(policy_manager_->IsPredataPolicy(app_id_1_));
  // Expect app_id_1_ has default policy
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_1_));
}

TEST_F(
    PolicyManagerImplTest2,
    ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithConsent_ExpectPreDataConsent) {
  // Arrange
  // RequestTypes for default & preDataConsent are the same
  CreateLocalPT(kPtu2RequestTypeJson);
  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));
  ASSERT_TRUE(policy_manager_->IsPredataPolicy(app_id_1_));
  EXPECT_CALL(listener_, OnPendingPermissionChange(app_id_1_)).Times(0);
  policy_manager_->ReactOnUserDevConsentForApp(app_id_1_, true);
  EXPECT_FALSE(policy_manager_->IsPredataPolicy(app_id_1_));
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_1_));
}

TEST_F(
    PolicyManagerImplTest2,
    GetUserConsentForDevice_SetDeviceWithoutConcent_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceHasNoConsent, consent);
}

TEST_F(PolicyManagerImplTest2,
       GetUserConsentForDevice_SetDeviceAllowed_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));

  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
}

TEST_F(
    PolicyManagerImplTest2,
    GetUserConsentForDevice_SetDeviceDisallowed_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  policy_manager_->SetUserConsentForDevice(device_id_2_, false);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceDisallowed, consent);
}

TEST_F(PolicyManagerImplTest2,
       GetDefaultHmi_SetDeviceDisallowed_ExpectReceivedHmiCorrect) {
  // Arrange
  CreateLocalPT(kPtu2RequestTypeJson);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  policy_manager_->SetUserConsentForDevice(device_id_2_, false);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceDisallowed, consent);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  std::string default_hmi;
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi);
  EXPECT_EQ("NONE", default_hmi);
}

TEST_F(PolicyManagerImplTest2,
       GetDefaultHmi_SetDeviceAllowed_ExpectReceivedHmiCorrect) {
  // Arrange
  CreateLocalPT(kPtu2RequestTypeJson);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check if app has preData policy
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_2_));
  std::string default_hmi1;
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi1);
  EXPECT_EQ("NONE", default_hmi1);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_2_));
  std::string default_hmi2;
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi2);
  EXPECT_EQ("LIMITED", default_hmi2);
}

TEST_F(PolicyManagerImplTest2,
       GetDefaultPriority_SetDeviceAllowed_ExpectReceivedPriorityCorrect) {
  // Arrange
  CreateLocalPT(kPtu2RequestTypeJson);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check if app has preData policy
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_2_));
  std::string priority1;
  EXPECT_TRUE(policy_manager_->GetPriority(app_id_2_, &priority1));
  EXPECT_EQ("NONE", priority1);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_2_));
  std::string priority2;
  EXPECT_TRUE(policy_manager_->GetPriority(app_id_2_, &priority2));
  EXPECT_EQ("EMERGENCY", priority2);
}

TEST_F(PolicyManagerImplTest2,
       GetUserFirendlyMessages_ExpectReceivedCorrectMessages) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ::policy::StringArray message_code;
  message_code.push_back("SettingEnableUpdates");
  message_code.push_back("AppPermissions");
  const std::string language = "en-us";
  const std::string active_hmi_language = language;
  std::vector< ::policy::UserFriendlyMessage> result =
      policy_manager_->GetUserFriendlyMessages(
          message_code, language, active_hmi_language);
  uint32_t size = result.size();
  EXPECT_GT(size, 0u);
  std::vector< ::policy::UserFriendlyMessage>::iterator result_iter;
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();

  policy_table::ConsumerFriendlyMessages& consumer_friendly_messages =
      *(pt->policy_table.consumer_friendly_messages);
  policy_table::Messages& Messages = *(consumer_friendly_messages.messages);

  policy_table::Messages::const_iterator messages_iter = Messages.begin();
  // Loop until end of messages
  for (; messages_iter != Messages.end(); ++messages_iter) {
    if (messages_iter->first == "AppPermissions") {
      for (result_iter = result.begin(); result_iter != result.end();
           ++result_iter) {
        if (result_iter->message_code == "AppPermissions") {
          const policy_table::MessageLanguages& MessageLanguages =
              messages_iter->second;
          const policy_table::Languages& Languages = MessageLanguages.languages;
          policy_table::Languages::const_iterator languages_iter =
              Languages.find("en-us");
          // If necessary language found
          if (languages_iter != Languages.end()) {
            const policy_table::MessageString& MessageString =
                languages_iter->second;
            EXPECT_EQ(static_cast<std::string>(*(MessageString.line1)),
                      result_iter->line1);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.line2)),
                      result_iter->line2);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.tts)),
                      result_iter->tts);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.label)),
                      result_iter->label);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.textBody)),
                      result_iter->text_body);
          }
        }
      }
    } else if (messages_iter->first == "SettingEnableUpdates") {
      for (result_iter = result.begin(); result_iter != result.end();
           ++result_iter) {
        if (result_iter->message_code == "SettingEnableUpdates") {
          const policy_table::MessageLanguages& MessageLanguages =
              messages_iter->second;
          const policy_table::Languages& Languages = MessageLanguages.languages;
          policy_table::Languages::const_iterator languages_iter =
              Languages.find("en-us");
          // If necessary language found
          if (languages_iter != Languages.end()) {
            const policy_table::MessageString& MessageString2 =
                languages_iter->second;
            EXPECT_EQ(static_cast<std::string>(*(MessageString2.line1)),
                      result_iter->line1);
          }
        }
      }
    }
  }
}

TEST_F(PolicyManagerImplTest2, SetDeviceInfo_ExpectDevInfoAddedToPT) {
  // Arrange
  ::policy::DeviceInfo dev_info;
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  dev_info.hardware = "hardware IPX";
  dev_info.firmware_rev = "v.8.0.1";
  dev_info.os = "Android";
  dev_info.os_ver = "4.4.2";
  dev_info.carrier = "Life";
  dev_info.max_number_rfcom_ports = 2;
  dev_info.connection_type = "Bluetooth";
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  policy_manager_->SetDeviceInfo(device_id_1_, dev_info);
  // Find device in PT
  policy_table::DeviceData::const_iterator iter =
      (*(pt->policy_table.device_data)).find(device_id_1_);
  // Checks
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.hardware),
            dev_info.hardware);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.firmware_rev),
            dev_info.firmware_rev);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.os), dev_info.os);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.os_version),
            dev_info.os_ver);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.carrier),
            dev_info.carrier);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.connection_type),
            dev_info.connection_type);
  EXPECT_EQ(static_cast<uint8_t>(*(*iter).second.max_number_rfcom_ports),
            dev_info.max_number_rfcom_ports);
}

TEST_F(PolicyManagerImplTest2, GetInitialAppData_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  ::policy::StringArray app_nicknames;
  ::policy::StringArray app_hmi_types;
  policy_manager_->GetInitialAppData(app_id_2_, &app_nicknames, &app_hmi_types);
  // Expect Empty nicknames and AppHmiTypes
  EXPECT_EQ(0u, app_nicknames.size());
  EXPECT_EQ(0u, app_hmi_types.size());

  Json::Value root = GetPTU(kValidSdlPtUpdateJson);

  Json::Value appHmiTypes = Json::Value(Json::arrayValue);
  appHmiTypes = root["policy_table"]["app_policies"][app_id_2_]["AppHMIType"];
  const uint32_t appHmiType_size = appHmiTypes.size();

  Json::Value appNicknames = Json::Value(Json::arrayValue);
  appNicknames = root["policy_table"]["app_policies"][app_id_2_]["nicknames"];
  const uint32_t appNicknames_size = appNicknames.size();

  ::policy::StringArray app_nicknames1;
  ::policy::StringArray app_hmi_types1;
  policy_manager_->GetInitialAppData(
      app_id_2_, &app_nicknames1, &app_hmi_types1);
  const uint32_t nick_names_size = app_nicknames1.size();
  const uint32_t app_hmi_types_size = app_hmi_types1.size();
  ASSERT_EQ(appHmiType_size, app_hmi_types_size);
  ASSERT_EQ(appNicknames_size, nick_names_size);
  ASSERT_GT(nick_names_size, 0u);
  ASSERT_GT(app_hmi_types_size, 0u);
  // Check nicknames match
  for (uint32_t i = 0; i < nick_names_size; ++i) {
    EXPECT_EQ(app_nicknames1[i], appNicknames[i].asString());
  }
  // Check AppHmiTypes match
  for (uint32_t i = 0; i < app_hmi_types_size; ++i) {
    EXPECT_EQ(app_hmi_types1[i], appHmiTypes[i].asString());
  }
}

TEST_F(
    PolicyManagerImplTest2,
    CanAppKeepContext_AddAppFromUnconsentedDevice_ExpectAppCannotKeepContext) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check if app has preData policy
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_2_));
  // Check keep context in preData policy
  EXPECT_FALSE(policy_manager_->CanAppKeepContext(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       CanAppKeepContext_AddAppFromConsentedDevice_ExpectAppCannotKeepContext) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_2_));
  // Check keep context in default policy
  EXPECT_FALSE(policy_manager_->CanAppKeepContext(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       CanAppStealFocus_AddAppFromUnconsentedDevice_ExpectAppCannotStealFocus) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check if app has preData policy
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_2_));
  // Check keep context in preData policy
  EXPECT_FALSE(policy_manager_->CanAppStealFocus(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       CanAppStealFocus_AddAppFromConsentedDevice_ExpectAppCannotStealFocus) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_2_));
  // Check keep context in default policy
  EXPECT_FALSE(policy_manager_->CanAppStealFocus(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       IsPredataPolicy_SetAppWIthPredataPolicy_ExpectPredataPolicy) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check if app has preData policy
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_2_));
}

TEST_F(
    PolicyManagerImplTest2,
    SendNotificationOnPermissionsUpdated_SetDeviceAllowed_ExpectNotificationSent) {
  // Arrange
  CreateLocalPT(kPtu2RequestTypeJson);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check if app has preData policy
  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_2_));
  std::string default_hmi1;
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi1);
  EXPECT_EQ("NONE", default_hmi1);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillOnce(Return(""));
  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_2_, _, default_hmi1))
      .Times(0);
  policy_manager_->SendNotificationOnPermissionsUpdated(app_id_2_);

  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  ::policy::DeviceConsent consent =
      policy_manager_->GetUserConsentForDevice(device_id_2_);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_TRUE((policy_manager_->GetCache())->IsDefaultPolicy(app_id_2_));
  std::string default_hmi2;
  policy_manager_->GetDefaultHmi(app_id_2_, &default_hmi2);
  EXPECT_EQ("LIMITED", default_hmi2);
  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_2_, _, default_hmi2));
  policy_manager_->SendNotificationOnPermissionsUpdated(app_id_2_);
}

}  // namespace policy
}  // namespace components
}  // namespace test
