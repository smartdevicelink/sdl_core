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

#include "application_manager/application_impl.h"

#include <stdint.h>
#include <iostream>

#include "gtest/gtest.h"
#include "application_manager/hmi_state.h"
#include "utils/file_system.h"

#include "utils/make_shared.h"
#include "application_manager/mock_message_helper.h"
#include "utils/custom_string.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_request_controller_settings.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/state_controller.h"
#include "resumption/last_state.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "policy/mock_statistics_manager.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace application_manager_test {

using namespace application_manager;

using namespace mobile_apis;
namespace custom_str = utils::custom_string;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
using usage_statistics_test::MockStatisticsManager;

typedef void (ApplicationImpl::*AddSet)(HmiStatePtr args);

class ApplicationImplTest : public ::testing::Test {
 protected:
  virtual void SetUp() OVERRIDE {
    app_id = 10;
    policy_app_id = "policy_app_id";
    app_name = "app_name";
    mac_address = "mac_address";
    test_lvl = HMILevel::INVALID_ENUM;
    state_id = HmiState::STATE_ID_REGULAR;
    audiostate = AudioStreamingState::NOT_AUDIBLE;
    syst_context = SystemContext::SYSCTXT_MAIN;

    testHmiState = CreateTestHmiState();
    EXPECT_CALL(mock_application_manager_, CreateRegularState(app_id, _, _, _))
        .WillOnce(Return(testHmiState));
    EXPECT_CALL(mock_application_manager_, get_settings())
        .WillRepeatedly(ReturnRef(mock_application_manager_settings_));
    EXPECT_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillRepeatedly(ReturnRef(directory_name));
    EXPECT_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillRepeatedly(ReturnRef(directory_name));
    EXPECT_CALL(mock_application_manager_settings_,
                audio_data_stopped_timeout()).WillOnce(Return(0));
    EXPECT_CALL(mock_application_manager_settings_,
                video_data_stopped_timeout()).WillOnce(Return(0));
    app_impl = new ApplicationImpl(app_id,
                                   policy_app_id,
                                   mac_address,
                                   app_name,
                                   utils::MakeShared<MockStatisticsManager>(),
                                   mock_application_manager_);
  }
  void TearDown() OVERRIDE {
    delete app_impl;
  }
  HmiStatePtr CreateTestHmiState();

  HmiStatePtr TestAddHmiState(HMILevel::eType hmi_lvl,
                              HmiState::StateID id_state,
                              AddSet hmi_action);

  void CheckCurrentHMIState();
  MockApplicationManagerSettings mock_application_manager_settings_;
  MockApplicationManager mock_application_manager_;
  ApplicationImpl* app_impl;
  uint32_t app_id;
  std::string policy_app_id;
  std::string mac_address;
  custom_str::CustomString app_name;
  const std::string directory_name = "./test_storage";
  HmiState::StateID state_id;
  HmiStatePtr testHmiState;
  HMILevel::eType test_lvl;
  AudioStreamingState::eType audiostate;
  SystemContext::eType syst_context;
};

HmiStatePtr ApplicationImplTest::CreateTestHmiState() {
  HmiStatePtr testState =
      utils::MakeShared<HmiState>(app_id, mock_application_manager_, state_id);
  testState->set_hmi_level(test_lvl);
  testState->set_audio_streaming_state(audiostate);
  testState->set_system_context(syst_context);
  return testState;
}

HmiStatePtr ApplicationImplTest::TestAddHmiState(HMILevel::eType hmi_lvl,
                                                 HmiState::StateID id_state,
                                                 AddSet hmi_action) {
  test_lvl = hmi_lvl;
  state_id = id_state;
  HmiStatePtr state = CreateTestHmiState();
  (app_impl->*hmi_action)(state);
  return state;
}

void ApplicationImplTest::CheckCurrentHMIState() {
  HmiStatePtr current_state = app_impl->CurrentHmiState();
  EXPECT_EQ(test_lvl, current_state->hmi_level());
  EXPECT_EQ(state_id, current_state->state_id());
}

TEST_F(ApplicationImplTest, AddHmiState_GetCurrentState) {
  TestAddHmiState(HMILevel::HMI_FULL,
                  HmiState::STATE_ID_PHONE_CALL,
                  &ApplicationImpl::AddHMIState);

  CheckCurrentHMIState();
  EXPECT_EQ(app_id, app_impl->app_id());
}

TEST_F(ApplicationImplTest, AddRegularHmiState_GetCurrentState) {
  TestAddHmiState(HMILevel::HMI_LIMITED,
                  HmiState::STATE_ID_REGULAR,
                  &ApplicationImpl::SetRegularState);

  CheckCurrentHMIState();
  EXPECT_EQ(app_id, app_impl->app_id());
}

TEST_F(ApplicationImplTest, AddStateAddRegularState_GetCurrentState) {
  TestAddHmiState(HMILevel::HMI_FULL,
                  HmiState::STATE_ID_REGULAR,
                  &ApplicationImpl::SetRegularState);
  TestAddHmiState(HMILevel::HMI_LIMITED,
                  HmiState::STATE_ID_NAVI_STREAMING,
                  &ApplicationImpl::AddHMIState);

  CheckCurrentHMIState();
  EXPECT_EQ(app_id, app_impl->app_id());
}

TEST_F(ApplicationImplTest, AddStateAddRegularState_GetRegularState) {
  TestAddHmiState(HMILevel::HMI_FULL,
                  HmiState::STATE_ID_REGULAR,
                  &ApplicationImpl::SetRegularState);
  TestAddHmiState(HMILevel::HMI_LIMITED,
                  HmiState::STATE_ID_NAVI_STREAMING,
                  &ApplicationImpl::AddHMIState);

  HmiStatePtr current_state = app_impl->RegularHmiState();
  EXPECT_EQ(HMILevel::HMI_FULL, current_state->hmi_level());
  EXPECT_EQ(HmiState::STATE_ID_REGULAR, current_state->state_id());
  EXPECT_EQ(app_id, app_impl->app_id());
}

TEST_F(ApplicationImplTest, AddStates_RemoveLastState) {
  // First state
  TestAddHmiState(HMILevel::HMI_FULL,
                  HmiState::STATE_ID_PHONE_CALL,
                  &ApplicationImpl::AddHMIState);
  HmiStatePtr state2 = TestAddHmiState(HMILevel::HMI_NONE,
                                       HmiState::STATE_ID_NAVI_STREAMING,
                                       &ApplicationImpl::AddHMIState);
  HmiStatePtr state3 = TestAddHmiState(HMILevel::HMI_LIMITED,
                                       HmiState::STATE_ID_TTS_SESSION,
                                       &ApplicationImpl::AddHMIState);

  CheckCurrentHMIState();

  // Remove last state
  app_impl->RemoveHMIState(state3->state_id());
  HmiStatePtr current_state = app_impl->CurrentHmiState();
  EXPECT_EQ(state2, current_state);
  EXPECT_EQ(HMILevel::HMI_NONE, current_state->hmi_level());
  EXPECT_EQ(HmiState::STATE_ID_NAVI_STREAMING, current_state->state_id());
}

TEST_F(ApplicationImplTest, AddStates_RemoveNotLastNotFirstState) {
  HmiStatePtr state1 = TestAddHmiState(HMILevel::HMI_FULL,
                                       HmiState::STATE_ID_PHONE_CALL,
                                       &ApplicationImpl::AddHMIState);
  HmiStatePtr state2 = TestAddHmiState(HMILevel::HMI_NONE,
                                       HmiState::STATE_ID_NAVI_STREAMING,
                                       &ApplicationImpl::AddHMIState);
  HmiStatePtr state3 = TestAddHmiState(HMILevel::HMI_LIMITED,
                                       HmiState::STATE_ID_TTS_SESSION,
                                       &ApplicationImpl::AddHMIState);

  CheckCurrentHMIState();

  // Remove not last state
  app_impl->RemoveHMIState(state2->state_id());
  HmiStatePtr current_state = app_impl->CurrentHmiState();
  EXPECT_EQ(state3, current_state);
  // HMI level is equal to parent hmi_level
  EXPECT_EQ(HMILevel::HMI_FULL, current_state->hmi_level());
  EXPECT_EQ(HmiState::STATE_ID_TTS_SESSION, current_state->state_id());
  EXPECT_EQ(state1, current_state->parent());
}

TEST_F(ApplicationImplTest, AddStates_RemoveFirstState) {
  HmiStatePtr state1 = TestAddHmiState(HMILevel::HMI_FULL,
                                       HmiState::STATE_ID_PHONE_CALL,
                                       &ApplicationImpl::AddHMIState);
  // Second state
  TestAddHmiState(HMILevel::HMI_NONE,
                  HmiState::STATE_ID_NAVI_STREAMING,
                  &ApplicationImpl::AddHMIState);
  HmiStatePtr state3 = TestAddHmiState(HMILevel::HMI_LIMITED,
                                       HmiState::STATE_ID_TTS_SESSION,
                                       &ApplicationImpl::AddHMIState);
  CheckCurrentHMIState();

  // Remove first added state
  app_impl->RemoveHMIState(state1->state_id());
  HmiStatePtr current_state = app_impl->CurrentHmiState();
  EXPECT_EQ(state3, current_state);
  // Last state does not have a parent
  EXPECT_EQ(HMILevel::HMI_LIMITED, current_state->hmi_level());
  EXPECT_EQ(HmiState::STATE_ID_TTS_SESSION, current_state->state_id());
  EXPECT_EQ(NULL, current_state->parent());
}

TEST_F(ApplicationImplTest, SetRegularState_RemoveFirstState) {
  HmiStatePtr state1 = TestAddHmiState(HMILevel::HMI_NONE,
                                       HmiState::STATE_ID_NAVI_STREAMING,
                                       &ApplicationImpl::AddHMIState);
  // Set regular state
  HmiStatePtr state2 = TestAddHmiState(HMILevel::HMI_FULL,
                                       HmiState::STATE_ID_REGULAR,
                                       &ApplicationImpl::SetRegularState);
  HmiStatePtr state3 = TestAddHmiState(HMILevel::HMI_LIMITED,
                                       HmiState::STATE_ID_TTS_SESSION,
                                       &ApplicationImpl::AddHMIState);
  CheckCurrentHMIState();

  // Remove first state
  app_impl->RemoveHMIState(state1->state_id());
  HmiStatePtr current_state = app_impl->CurrentHmiState();
  EXPECT_EQ(state3, current_state);
  // Last state has a parent
  EXPECT_EQ(HMILevel::HMI_FULL, current_state->hmi_level());
  EXPECT_EQ(HmiState::STATE_ID_TTS_SESSION, current_state->state_id());
  EXPECT_EQ(state2, current_state->parent());
}

TEST_F(ApplicationImplTest, SetPostponedState_RemovePostponedState) {
  // Set postponed hmi state
  HmiStatePtr state1 = TestAddHmiState(HMILevel::HMI_NONE,
                                       HmiState::STATE_ID_POSTPONED,
                                       &ApplicationImpl::SetPostponedState);

  // Check that state was setted correctly
  HmiStatePtr state2 = app_impl->PostponedHmiState();
  EXPECT_EQ(state1, state2);

  // Check that state was correctly removed
  app_impl->RemovePostponedState();
  state2 = app_impl->PostponedHmiState();
  EXPECT_EQ(NULL, state2);
}

TEST_F(ApplicationImplTest, AddStateAddRegularState_GetHmiLvlAudioSystemState) {
  audiostate = AudioStreamingState::ATTENUATED;
  syst_context = SystemContext::SYSCTXT_ALERT;
  TestAddHmiState(HMILevel::HMI_FULL,
                  HmiState::STATE_ID_REGULAR,
                  &ApplicationImpl::SetRegularState);

  EXPECT_EQ(test_lvl, app_impl->hmi_level());
  EXPECT_EQ(audiostate, app_impl->audio_streaming_state());
  EXPECT_EQ(syst_context, app_impl->system_context());

  audiostate = AudioStreamingState::AUDIBLE;
  syst_context = SystemContext::SYSCTXT_MENU;
  TestAddHmiState(HMILevel::HMI_LIMITED,
                  HmiState::STATE_ID_NAVI_STREAMING,
                  &ApplicationImpl::AddHMIState);

  EXPECT_EQ(test_lvl, app_impl->hmi_level());
  EXPECT_EQ(audiostate, app_impl->audio_streaming_state());
  EXPECT_EQ(syst_context, app_impl->system_context());
}

TEST_F(ApplicationImplTest, IsAudioApplication) {
  EXPECT_FALSE(app_impl->IsAudioApplication());
  app_impl->set_is_navi(true);
  app_impl->set_is_media_application(false);
  app_impl->set_voice_communication_supported(false);
  EXPECT_TRUE(app_impl->IsAudioApplication());
  app_impl->set_is_navi(false);
  app_impl->set_is_media_application(true);
  app_impl->set_voice_communication_supported(false);
  EXPECT_TRUE(app_impl->IsAudioApplication());
  app_impl->set_is_navi(false);
  app_impl->set_is_media_application(false);
  app_impl->set_voice_communication_supported(true);
  EXPECT_TRUE(app_impl->IsAudioApplication());
  app_impl->set_is_media_application(false);
  app_impl->set_voice_communication_supported(false);
  app_impl->set_is_navi(false);
  EXPECT_FALSE(app_impl->IsAudioApplication());
}

TEST_F(ApplicationImplTest, AddFile) {
  AppFile test_file;

  test_file.is_persistent = true;
  test_file.is_download_complete = false;
  test_file.file_type = FileType::AUDIO_AAC;
  test_file.file_name = "test_file 1";

  EXPECT_TRUE(app_impl->AddFile(test_file));
  test_file.is_download_complete = true;
  EXPECT_FALSE(app_impl->AddFile(test_file));
  test_file.file_name = "test_file 2";
  EXPECT_TRUE(app_impl->AddFile(test_file));
}

TEST_F(ApplicationImplTest, UpdateFile) {
  AppFile test_file;

  test_file.is_persistent = true;
  test_file.is_download_complete = false;
  test_file.file_type = FileType::AUDIO_AAC;
  test_file.file_name = "test_file 1";

  EXPECT_FALSE(app_impl->UpdateFile(test_file));
  EXPECT_TRUE(app_impl->AddFile(test_file));
  test_file.is_download_complete = true;
  EXPECT_FALSE(app_impl->AddFile(test_file));
  EXPECT_TRUE(app_impl->UpdateFile(test_file));
}

TEST_F(ApplicationImplTest, DeleteFile) {
  AppFile test_file;
  test_file.is_persistent = true;
  test_file.is_download_complete = false;
  test_file.file_type = FileType::AUDIO_AAC;
  test_file.file_name = "test_file 1";

  EXPECT_FALSE(app_impl->DeleteFile(test_file.file_name));
  EXPECT_TRUE(app_impl->AddFile(test_file));
  EXPECT_TRUE(app_impl->DeleteFile(test_file.file_name));
  EXPECT_FALSE(app_impl->DeleteFile(test_file.file_name));
  EXPECT_TRUE(app_impl->AddFile(test_file));
}

TEST_F(ApplicationImplTest, GetFile) {
  AppFile test_file;
  test_file.is_persistent = true;
  test_file.is_download_complete = true;
  test_file.file_type = FileType::GRAPHIC_JPEG;
  test_file.file_name = "test_file 1";

  EXPECT_EQ(NULL, app_impl->GetFile(test_file.file_name));
  EXPECT_TRUE(app_impl->AddFile(test_file));
  const AppFile* app_file = app_impl->GetFile(test_file.file_name);
  EXPECT_EQ(test_file.is_persistent, app_file->is_persistent);
  EXPECT_EQ(test_file.is_download_complete, app_file->is_download_complete);
  EXPECT_EQ(test_file.file_type, app_file->file_type);
}

TEST_F(ApplicationImplTest, SetIconPath) {
  AppFile test_file;
  test_file.is_persistent = true;
  test_file.is_download_complete = false;
  test_file.file_type = FileType::GRAPHIC_BMP;
  test_file.file_name = "test_file 1";

  EXPECT_TRUE(app_impl->AddFile(test_file));
  test_file.file_name = "test_file 2";
  test_file.file_type = FileType::GRAPHIC_PNG;
  EXPECT_TRUE(app_impl->AddFile(test_file));

  EXPECT_TRUE(app_impl->set_app_icon_path(test_file.file_name));
  EXPECT_EQ(test_file.file_name, app_impl->app_icon_path());
}

TEST_F(ApplicationImplTest, LoadPersistentFiles) {
  // Precondition
  // Create test folder with diff files

  app_impl->MarkRegistered();
  std::string folder_name = "";
  app_impl->set_folder_name(folder_name);

  file_system::CreateDirectory(directory_name);

  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file1.json"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file2.bmp"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file3.jpeg"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file4.png"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file5"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file6.wave"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file7.mp4"));
  EXPECT_TRUE(file_system::CreateFile(directory_name + "/test_file8.mp3"));

  app_impl->LoadPersistentFiles();

  AppFilesMap files_map = app_impl->getAppFiles();

  EXPECT_EQ(8u, files_map.size());
  const AppFile* test_file1 =
      app_impl->GetFile(directory_name + "//test_file1.json");
  ASSERT_TRUE(test_file1);
  EXPECT_EQ(FileType::JSON, test_file1->file_type);
  const AppFile* test_file2 =
      app_impl->GetFile(directory_name + "//test_file2.bmp");
  ASSERT_TRUE(test_file2);
  EXPECT_EQ(FileType::GRAPHIC_BMP, test_file2->file_type);
  const AppFile* test_file3 =
      app_impl->GetFile(directory_name + "//test_file3.jpeg");
  ASSERT_TRUE(test_file3);
  EXPECT_EQ(FileType::GRAPHIC_JPEG, test_file3->file_type);
  const AppFile* test_file4 =
      app_impl->GetFile(directory_name + "//test_file4.png");
  ASSERT_TRUE(test_file4);
  EXPECT_EQ(FileType::GRAPHIC_PNG, test_file4->file_type);
  const AppFile* test_file5 =
      app_impl->GetFile(directory_name + "//test_file5");
  ASSERT_TRUE(test_file5);
  EXPECT_EQ(FileType::BINARY, test_file5->file_type);
  const AppFile* test_file6 =
      app_impl->GetFile(directory_name + "//test_file6.wave");
  ASSERT_TRUE(test_file6);
  EXPECT_EQ(FileType::AUDIO_WAVE, test_file6->file_type);
  const AppFile* test_file7 =
      app_impl->GetFile(directory_name + "//test_file7.mp4");
  ASSERT_TRUE(test_file7);
  EXPECT_EQ(FileType::AUDIO_AAC, test_file7->file_type);
  const AppFile* test_file8 =
      app_impl->GetFile(directory_name + "//test_file8.mp3");
  ASSERT_TRUE(test_file8);
  EXPECT_EQ(FileType::AUDIO_MP3, test_file8->file_type);
}

TEST_F(ApplicationImplTest, AreCommandLimitsExceeded_SetLimitFromConfig) {
  std::pair<uint32_t, int32_t> get_frequency;
  get_frequency.first = 5;
  get_frequency.second = 1;
  EXPECT_CALL(mock_application_manager_settings_, read_did_frequency())
      .WillRepeatedly(ReturnRef(get_frequency));
  EXPECT_CALL(mock_application_manager_settings_, get_vehicle_data_frequency())
      .WillRepeatedly(ReturnRef(get_frequency));
  for (uint32_t i = 0; i < get_frequency.first; i++) {
    EXPECT_FALSE(app_impl->AreCommandLimitsExceeded(FunctionID::ReadDIDID,
                                                    TLimitSource::CONFIG_FILE));
  }
  EXPECT_TRUE(app_impl->AreCommandLimitsExceeded(FunctionID::ReadDIDID,
                                                 TLimitSource::CONFIG_FILE));

  for (uint32_t i = 0; i < get_frequency.first; i++) {
    EXPECT_FALSE(app_impl->AreCommandLimitsExceeded(
        FunctionID::GetVehicleDataID, TLimitSource::CONFIG_FILE));
  }
  EXPECT_TRUE(app_impl->AreCommandLimitsExceeded(FunctionID::GetVehicleDataID,
                                                 TLimitSource::CONFIG_FILE));
}

TEST_F(ApplicationImplTest, AreCommandLimitsExceeded_LimitFromPT) {
  const uint32_t cmd_limit = 100u;
  policy_test::MockPolicyHandlerInterface policy_interface;
  EXPECT_CALL(mock_application_manager_, GetPolicyHandler())
      .WillRepeatedly(ReturnRef(policy_interface));
  EXPECT_CALL(policy_interface, GetPriority(policy_app_id, _))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(policy_interface, GetNotificationsNumber(_))
      .WillOnce(Return(cmd_limit));
  EXPECT_FALSE(app_impl->AreCommandLimitsExceeded(FunctionID::ReadDIDID,
                                                  TLimitSource::POLICY_TABLE));

  EXPECT_CALL(policy_interface, GetNotificationsNumber(_))
      .WillOnce(Return(cmd_limit));
  EXPECT_FALSE(app_impl->AreCommandLimitsExceeded(FunctionID::GetVehicleDataID,
                                                  TLimitSource::POLICY_TABLE));
  EXPECT_CALL(policy_interface, GetNotificationsNumber(_))
      .WillRepeatedly(Return(0));
  EXPECT_TRUE(app_impl->AreCommandLimitsExceeded(FunctionID::ReadDIDID,
                                                 TLimitSource::POLICY_TABLE));
  EXPECT_TRUE(app_impl->AreCommandLimitsExceeded(FunctionID::GetVehicleDataID,
                                                 TLimitSource::POLICY_TABLE));
}

TEST_F(ApplicationImplTest, SubscribeToButton_UnsubscribeFromButton) {
  EXPECT_TRUE(app_impl->SubscribeToButton(ButtonName::PRESET_0));
  // It is impossible to subsribe to the same button twice
  EXPECT_FALSE(app_impl->SubscribeToButton(ButtonName::PRESET_0));
  EXPECT_TRUE(app_impl->IsSubscribedToButton(ButtonName::PRESET_0));
  EXPECT_TRUE(app_impl->UnsubscribeFromButton(ButtonName::PRESET_0));
  EXPECT_FALSE(app_impl->IsSubscribedToButton(ButtonName::PRESET_0));
}

TEST_F(ApplicationImplTest, SubscribeToDefaultButton_UnsubscribeFromButton) {
  EXPECT_TRUE(app_impl->IsSubscribedToButton(ButtonName::CUSTOM_BUTTON));
  EXPECT_FALSE(app_impl->SubscribeToButton(ButtonName::CUSTOM_BUTTON));
}

TEST_F(ApplicationImplTest, SubscribeToSoftButton_UnsubscribeFromSoftButton) {
  const uint btn_count = 10;
  for (uint i = 0; i < btn_count; i++) {
    EXPECT_FALSE(app_impl->IsSubscribedToSoftButton(i));
  }

  SoftButtonID test_button;
  for (uint i = 0; i < btn_count; i++) {
    test_button.insert(i);
  }
  app_impl->SubscribeToSoftButtons(FunctionID::ScrollableMessageID,
                                   test_button);

  for (uint i = 0; i < btn_count; i++) {
    EXPECT_TRUE(app_impl->IsSubscribedToSoftButton(i));
  }
  app_impl->UnsubscribeFromSoftButtons(FunctionID::ScrollableMessageID);

  for (uint i = 0; i < btn_count; i++) {
    EXPECT_FALSE(app_impl->IsSubscribedToSoftButton(i));
  }
}

TEST_F(ApplicationImplTest, ChangeSupportingAppHMIType_TypeNotNaviNotVoice) {
  smart_objects::SmartObject type_media;
  type_media[0] = AppHMIType::MEDIA;

  EXPECT_FALSE(app_impl->is_navi());
  EXPECT_FALSE(app_impl->is_voice_communication_supported());

  app_impl->set_app_types(type_media);
  app_impl->ChangeSupportingAppHMIType();

  EXPECT_FALSE(app_impl->is_navi());
  EXPECT_FALSE(app_impl->is_voice_communication_supported());
}

TEST_F(ApplicationImplTest, ChangeSupportingAppHMIType_TypeIsVoice) {
  smart_objects::SmartObject type_comm;
  type_comm[0] = AppHMIType::COMMUNICATION;

  EXPECT_FALSE(app_impl->is_navi());
  EXPECT_FALSE(app_impl->is_voice_communication_supported());

  app_impl->set_app_types(type_comm);
  app_impl->ChangeSupportingAppHMIType();

  EXPECT_FALSE(app_impl->is_navi());
  EXPECT_TRUE(app_impl->is_voice_communication_supported());
}

TEST_F(ApplicationImplTest, ChangeSupportingAppHMIType_TypeIsNavi) {
  smart_objects::SmartObject type_navi;
  type_navi[0] = AppHMIType::NAVIGATION;

  EXPECT_FALSE(app_impl->is_navi());
  EXPECT_FALSE(app_impl->is_voice_communication_supported());

  app_impl->set_app_types(type_navi);
  app_impl->ChangeSupportingAppHMIType();

  EXPECT_TRUE(app_impl->is_navi());
  EXPECT_FALSE(app_impl->is_voice_communication_supported());
}

TEST_F(ApplicationImplTest, ChangeSupportingAppHMIType_TypeIsNaviAndVoice) {
  smart_objects::SmartObject app_types;
  app_types[0] = AppHMIType::NAVIGATION;
  app_types[1] = AppHMIType::COMMUNICATION;
  app_types[2] = AppHMIType::MEDIA;

  EXPECT_FALSE(app_impl->is_navi());
  EXPECT_FALSE(app_impl->is_voice_communication_supported());

  app_impl->set_app_types(app_types);
  app_impl->ChangeSupportingAppHMIType();

  EXPECT_TRUE(app_impl->is_navi());
  EXPECT_TRUE(app_impl->is_voice_communication_supported());
}

TEST_F(ApplicationImplTest, UpdateHash_AppMngrNotSuspended) {
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendHashUpdateNotification(app_id, _)).Times(1);
  app_impl->UpdateHash();

  EXPECT_TRUE(app_impl->is_application_data_changed());
}

TEST_F(ApplicationImplTest, StartStreaming_MobileNavi_StreamingNotApproved) {
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendNaviStartStream(app_id, _));
  app_impl->StartStreaming(protocol_handler::ServiceType::kMobileNav);

  EXPECT_EQ(0u, app_impl->video_stream_retry_number());
}

TEST_F(ApplicationImplTest, StartStreaming_Audio_StreamingNotApproved) {
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendAudioStartStream(app_id, _));
  app_impl->StartStreaming(protocol_handler::ServiceType::kAudio);

  EXPECT_EQ(0u, app_impl->video_stream_retry_number());
}

TEST_F(ApplicationImplTest, StartStreaming_StreamingApproved) {
  app_impl->set_video_streaming_approved(true);
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendNaviStartStream(app_id, _)).Times(0);
  app_impl->StartStreaming(protocol_handler::ServiceType::kMobileNav);

  app_impl->set_audio_streaming_approved(true);

  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendAudioStartStream(app_id, _)).Times(0);
  app_impl->StartStreaming(protocol_handler::ServiceType::kAudio);
}

TEST_F(ApplicationImplTest, SuspendNaviStreaming) {
  protocol_handler::ServiceType type =
      protocol_handler::ServiceType::kMobileNav;
  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, false));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, false, _));
  app_impl->SuspendStreaming(type);
}

TEST_F(ApplicationImplTest, SuspendAudioStreaming) {
  protocol_handler::ServiceType type = protocol_handler::ServiceType::kAudio;
  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, false));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, false, _));
  app_impl->SuspendStreaming(type);
}

// TODO {AKozoriz} : Fix tests with streaming (APPLINK-19289)
TEST_F(ApplicationImplTest, DISABLED_Suspend_WakeUpAudioStreaming) {
  protocol_handler::ServiceType type = protocol_handler::ServiceType::kAudio;
  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, false));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, false, _));
  app_impl->SuspendStreaming(type);

  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, true));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, true, _));
  app_impl->WakeUpStreaming(type);
}

TEST_F(ApplicationImplTest, DISABLED_Suspend_WakeUpNaviStreaming) {
  protocol_handler::ServiceType type =
      protocol_handler::ServiceType::kMobileNav;
  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, false));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, false, _));
  app_impl->SuspendStreaming(type);

  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, true));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, true, _));
  app_impl->WakeUpStreaming(type);
}

TEST_F(ApplicationImplTest, StopStreaming_StreamingApproved) {
  // Stop navigation streaming
  protocol_handler::ServiceType type =
      protocol_handler::ServiceType::kMobileNav;
  app_impl->set_video_streaming_approved(true);

  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, false));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, false, _));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendNaviStopStream(app_id, _));

  app_impl->StopStreaming(type);
  EXPECT_FALSE(app_impl->video_streaming_approved());

  // Stop audio streaming
  app_impl->set_audio_streaming_approved(true);
  type = protocol_handler::ServiceType::kAudio;
  EXPECT_CALL(mock_application_manager_, OnAppStreaming(app_id, type, false));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnDataStreaming(type, false, _));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendAudioStopStream(app_id, _));

  app_impl->StopStreaming(type);
  EXPECT_FALSE(app_impl->audio_streaming_approved());
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
