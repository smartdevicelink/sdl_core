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

#include "application_manager/resumption/resume_ctrl.h"
#include <string>
#include <algorithm>
#include "gtest/gtest.h"
#include "application_manager/usage_statistics.h"
#include "include/application_mock.h"
#include "include/resumption_data_mock.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application.h"
#include "config_profile/profile.h"
#include "utils/data_accessor.h"
#include "application_manager/test/mock_message_helper.h"

namespace test {
namespace components {
namespace resumption_test {

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::AtLeast;

using namespace resumption;
using namespace mobile_apis::HMILevel;

class ResumeCtrlTest : public ::testing::Test {
 public:
  virtual void SetUp() OVERRIDE {
    app_mngr = application_manager::ApplicationManagerImpl::instance();
    // Singleton should not be destroyed
    Mock::AllowLeak(app_mngr);
    mock_storage = new NiceMock<ResumptionDataMock>();
    app_mock = new NiceMock<ApplicationMock>();
    res_ctrl.set_resumption_storage(mock_storage);
    test_audio_state = mobile_apis::AudioStreamingState::NOT_AUDIBLE;
    test_app_id = 10;
    default_testType = eType::HMI_NONE;
    test_dev_id = 5;
    test_policy_app_id = "test_policy_app_id";
    test_grammar_id = 10;
    hash = "saved_hash";
  }

 protected:
  application_manager::ApplicationManagerImpl* app_mngr;
  ResumeCtrl res_ctrl;
  NiceMock<ResumptionDataMock>* mock_storage;
  NiceMock<ApplicationMock>* app_mock;
  mobile_apis::AudioStreamingState::eType test_audio_state;
  // app_mock.app_id() will return this value
  uint32_t test_app_id;
  std::string test_policy_app_id;
  mobile_apis::HMILevel::eType default_testType;

  // app_mock.Device() will return this value
  uint32_t test_dev_id;
  uint32_t test_grammar_id;
  std::string hash;
};

/**
 * @brief  Group of tests which check starting resumption with different data
 */

TEST_F(ResumeCtrlTest, StartResumption_AppWithGrammarId) {
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*app_mock, UpdateHash());
  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

MATCHER_P4(CheckAppFile, is_persistent, is_download, file_name, file_type, "") {
  application_manager::AppFile app_file = arg;
  return app_file.is_persistent == is_persistent &&
         app_file.is_download_complete == is_download &&
         app_file.file_name == file_name && app_file.file_type == file_type;
}

TEST_F(ResumeCtrlTest, StartResumption_WithoutGrammarId) {
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*app_mock, UpdateHash());
  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id)).Times(0);

  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_FALSE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithFiles) {
  smart_objects::SmartObject test_application_files;
  smart_objects::SmartObject test_file;
  const uint32_t count_of_files = 8;

  int file_types[count_of_files];
  std::string file_names[count_of_files];
  const size_t max_size = 12;
  char numb[max_size];
  for (uint32_t i = 0; i < count_of_files; i++) {
    file_types[i] = i;
    std::snprintf(numb, max_size, "%d", i);
    file_names[i] = "test_file" + std::string(numb);
  }

  // Should not been added
  test_file[application_manager::strings::persistent_file] = false;
  test_application_files[0] = test_file;

  for (uint32_t i = 0; i < count_of_files; ++i) {
    test_file[application_manager::strings::persistent_file] = true;
    test_file[application_manager::strings::is_download_complete] = true;
    test_file[application_manager::strings::file_type] = file_types[i];
    test_file[application_manager::strings::sync_file_name] = file_names[i];
    test_application_files[i + 1] = test_file;
  }

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_files] =
      test_application_files;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*app_mock, UpdateHash());
  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));
  for (uint32_t i = 0; i < count_of_files; ++i) {
    EXPECT_CALL(*app_mock,
                AddFile(CheckAppFile(
                    true, true, file_names[i],
                    static_cast<mobile_apis::FileType::eType>(file_types[i]))));
  }

  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubmenues) {
  smart_objects::SmartObject test_application_submenues;
  smart_objects::SmartObject test_submenu;

  const uint32_t count_of_submenues = 20;
  for (uint32_t i = 0; i < count_of_submenues; ++i) {
    test_submenu[application_manager::strings::menu_id] = i;
    test_application_submenues[i] = test_submenu;
  }

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_submenus] =
      test_application_submenues;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  for (uint32_t i = 0; i < count_of_submenues; ++i) {
    EXPECT_CALL(*app_mock, AddSubMenu(i, test_application_submenues[i]));
  }
  smart_objects::SmartObjectList requests;
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              CreateAddSubMenuRequestToHMI(_)).WillRepeatedly(Return(requests));

  EXPECT_CALL(*app_mock, UpdateHash());
  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithCommands) {
  smart_objects::SmartObject test_application_commands;
  smart_objects::SmartObject test_commands;
  const uint32_t count_of_commands = 20;

  for (uint32_t i = 0; i < count_of_commands; ++i) {
    test_commands[application_manager::strings::cmd_id] = i;
    test_application_commands[i] = test_commands;
  }

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_commands] =
      test_application_commands;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*app_mock, UpdateHash());
  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  for (uint32_t i = 0; i < count_of_commands; ++i) {
    EXPECT_CALL(*app_mock, AddCommand(i, test_application_commands[i]));
  }

  smart_objects::SmartObjectList requests;
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              CreateAddCommandRequestToHMI(_)).WillRepeatedly(Return(requests));

  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithChoiceSet) {
  smart_objects::SmartObject application_choice_sets;
  smart_objects::SmartObject app_choice_set;

  const uint32_t count_of_choice = 10;
  smart_objects::SmartObject choice_vector;
  smart_objects::SmartObject choice;
  const size_t max_size = 12;
  char numb[max_size];
  for (uint32_t i = 0; i < count_of_choice; ++i) {
    std::snprintf(numb, max_size, "%d", i);
    choice[application_manager::strings::vr_commands] =
        "VrCommand" + std::string(numb);
    choice[application_manager::strings::choice_id] = i;
    choice_vector[i] = choice;
  }
  const uint32_t count_of_choice_sets = 5;
  for (uint32_t i = 0; i < count_of_choice_sets; ++i) {
    app_choice_set[application_manager::strings::interaction_choice_set_id] = i;
    app_choice_set[application_manager::strings::choice_set] = choice_vector;
    application_choice_sets[i] = app_choice_set;
  }

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_choice_sets] =
      application_choice_sets;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*app_mock, UpdateHash());
  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  for (uint32_t i = 0; i < count_of_choice_sets; ++i) {
    EXPECT_CALL(*app_mock, AddChoiceSet(i, application_choice_sets[i]));
  }

  smart_objects::SmartObjectList requests;
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              CreateAddVRCommandRequestFromChoiceToHMI(_))
      .WillRepeatedly(Return(requests));

  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithGlobalProperties) {
  // Prepare Data
  smart_objects::SmartObject test_global_properties;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_global_properties] =
      test_global_properties;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              SendGlobalPropertiesToHMI(_));

  EXPECT_CALL(*app_mock, load_global_properties(test_global_properties));

  EXPECT_CALL(*app_mock, UpdateHash());
  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubscribeOnButtons) {
  // Prepare Data
  smart_objects::SmartObject test_subscriptions;
  smart_objects::SmartObject app_buttons;

  uint32_t count_of_buttons = 17;
  for (uint32_t i = 0; i < count_of_buttons; ++i) {
    app_buttons[i] = i;
  }

  test_subscriptions[application_manager::strings::application_buttons] =
      app_buttons;

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_subscribtions] =
      test_subscriptions;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  for (uint32_t i = 0; i < count_of_buttons; ++i) {
    EXPECT_CALL(*app_mock, SubscribeToButton(
                               static_cast<mobile_apis::ButtonName::eType>(i)));
  }
  EXPECT_CALL(*app_mock, UpdateHash());

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              SendAllOnButtonSubscriptionNotificationsForApp(_)).Times(2);

  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubscriptionToIVI) {
  // Prepare Data
  smart_objects::SmartObject test_subscriptions;
  smart_objects::SmartObject app_vi;

  int vtype = application_manager::VehicleDataType::GPS;
  uint i = 0;
  for (; vtype < application_manager::VehicleDataType::STEERINGWHEEL;
       ++i, ++vtype) {
    app_vi[i] = vtype;
  }

  test_subscriptions[application_manager::strings::application_vehicle_info] =
      app_vi;

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::application_subscribtions] =
      test_subscriptions;

  // Check RestoreApplicationData
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .Times(3)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mock, set_grammar_id(test_grammar_id));

  for (size_t i = 0; i < app_vi.length(); ++i) {
    EXPECT_CALL(
        *app_mock,
        SubscribeToIVI(static_cast<application_manager::VehicleDataType>(i)));
  }

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  smart_objects::SmartObjectList requests;
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              GetIVISubscriptionRequests(_)).WillRepeatedly(Return(requests));

  EXPECT_CALL(*app_mock, UpdateHash());
  bool res = res_ctrl.StartResumption(app_mock, hash);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumptionOnlyHMILevel) {
  smart_objects::SmartObject saved_app;
  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));

  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  EXPECT_CALL(*mock_storage, GetSavedApplication(test_policy_app_id, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .Times(3)
      .WillRepeatedly(Return(default_testType));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr, SetState(test_app_id, default_testType,
                                  test_audio_state)).Times(2);
  bool res = res_ctrl.StartResumptionOnlyHMILevel(app_mock);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartAppHmiStateResumption_AppInFull) {
  mobile_apis::HMILevel::eType restored_testType = eType::HMI_FULL;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_testType;

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(true));

  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr,
              SetHmiState(test_app_id, restored_testType)).Times(AtLeast(1));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*mock_storage, GetSavedApplication(test_policy_app_id, _, _))
      .Times(2)
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));

  EXPECT_CALL(*mock_storage, RemoveApplicationFromSaved(_, _))
      .WillOnce(Return(true));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("12345"))
      .WillRepeatedly(Return(policy::kDeviceAllowed));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(true));

  res_ctrl.StartAppHmiStateResumption(app_mock);
}

TEST_F(ResumeCtrlTest, StartAppHmiStateResumption_AppInBackground) {
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;

  mobile_apis::HMILevel::eType restored_testType = eType::HMI_BACKGROUND;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_testType;

  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*mock_storage, GetSavedApplication(test_policy_app_id, _, _))
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("12345"))
      .WillOnce(Return(policy::kDeviceAllowed));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(true));

  res_ctrl.StartAppHmiStateResumption(app_mock);
}

/**
 * @brief  Group of tests which check restoring resumption with different data
 */

TEST_F(ResumeCtrlTest, RestoreAppHMIState_RestoreHMILevelFull) {
  mobile_apis::HMILevel::eType restored_testType = eType::HMI_FULL;

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = hash;
  saved_app[application_manager::strings::grammar_id] = test_grammar_id;
  saved_app[application_manager::strings::hmi_level] = restored_testType;

  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice(""))
      .WillOnce(Return(policy::kDeviceAllowed));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillRepeatedly(Return(false));

  utils::SharedPtr<application_manager::Application> null_app;
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(null_app));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, restored_testType, test_audio_state));

  bool res = res_ctrl.RestoreAppHMIState(app_mock);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetupDefaultHMILevel) {
  smart_objects::SmartObject saved_app;

  saved_app[application_manager::strings::hmi_level] = default_testType;

  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillRepeatedly(Return(default_testType));

  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("")).Times(0);
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(true));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));
  EXPECT_CALL(*app_mngr,
              SetHmiState(test_app_id, default_testType)).Times(AtLeast(1));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr, SetState(test_app_id, default_testType,
                                  test_audio_state)).Times(2);

  res_ctrl.SetupDefaultHMILevel(app_mock);
}

/**
 * @brief group of tests which check correct SetAppHMIState
*/

TEST_F(ResumeCtrlTest, SetAppHMIState_HMINone_NotMedia_WithoutCheckPolicy) {
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("")).Times(0);
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(true));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr, SetState(test_app_id, default_testType,
                                  test_audio_state)).Times(2);

  bool res = res_ctrl.SetAppHMIState(app_mock, default_testType, false);
  EXPECT_TRUE(res);
}

// TODO(VVeremjova) APPLINK-16718
TEST_F(ResumeCtrlTest,
       DISABLED_SetAppHMIState_HMILimited_NotMedia_WithoutCheckPolicy) {
  mobile_apis::HMILevel::eType testType = eType::HMI_LIMITED;

  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice(""))
      .WillRepeatedly(Return(policy::DeviceConsent::kDeviceAllowed));
  EXPECT_CALL(*app_mngr, SetState(test_app_id, testType, test_audio_state));

  bool res = res_ctrl.SetAppHMIState(app_mock, testType, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_NotMedia_WithoutCheckPolicy) {
  mobile_apis::HMILevel::eType testType = eType::HMI_FULL;
  ::testing::InSequence seq;
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(false));

  // Only mocked application is exist
  utils::SharedPtr<application_manager::Application> null_app;
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(null_app));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));

  // GetDefaultHmiLevel should not be called
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_)).Times(0);
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  EXPECT_CALL(*app_mngr, SetState(test_app_id, testType, test_audio_state));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("")).Times(0);
  bool res = res_ctrl.SetAppHMIState(app_mock, testType, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest,
       SetAppHMIState_HMIFull_NotMedia_WithoutPolicy_AnotherFullActiveExists) {
  mobile_apis::HMILevel::eType testType = eType::HMI_FULL;
  ::testing::InSequence seq;
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(false));

  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  //  Expect set default HMI state
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("")).Times(0);
  bool res = res_ctrl.SetAppHMIState(app_mock, testType, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest,
       SetAppHMIState_HMIFull_MediaAudio_WithoutPolicy_AnotherActiveAppExists) {
  mobile_apis::HMILevel::eType testType = eType::HMI_FULL;

  // In case application is media, audio state will be AUDIBLE
  test_audio_state = mobile_apis::AudioStreamingState::AUDIBLE;
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  // Only our app in FULL
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(false));

  // Active app exists
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));

  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_)).Times(0);
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(true));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  mobile_apis::HMILevel::eType expected_testType = eType::HMI_LIMITED;
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, expected_testType, test_audio_state));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("")).Times(0);
  bool res = res_ctrl.SetAppHMIState(app_mock, testType, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_Media_WithoutPolicy_FullExists) {
  default_testType = eType::HMI_FULL;

  // In case application is media, audio state will be AUDIBLE
  test_audio_state = mobile_apis::AudioStreamingState::AUDIBLE;
  ::testing::InSequence seq;
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(true));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(true));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("")).Times(0);
  bool res = res_ctrl.SetAppHMIState(app_mock, default_testType, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_NotMedia_WithPolicy_DevAllowed) {
  mobile_apis::HMILevel::eType testType = eType::HMI_FULL;

  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("12345"))
      .WillOnce(Return(policy::kDeviceAllowed));

  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(true));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));

  bool res = res_ctrl.SetAppHMIState(app_mock, testType, true);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_Media_WithCheckPolicy) {
  mobile_apis::HMILevel::eType testType = eType::HMI_FULL;

  // In case application is media, audio state will be AUDIBLE
  test_audio_state = mobile_apis::AudioStreamingState::AUDIBLE;
  ::testing::InSequence seq;
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  // App is allowed
  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("12345"))
      .WillOnce(Return(policy::kDeviceAllowed));
  EXPECT_CALL(*app_mngr, IsAppTypeExistsInFullOrLimited(_))
      .WillOnce(Return(false));
  EXPECT_CALL(*app_mngr, active_application()).WillOnce(Return(app_mock));
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_)).Times(0);
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(true));

  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  mobile_apis::HMILevel::eType expected_testType = eType::HMI_LIMITED;
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, expected_testType, test_audio_state));

  bool res = res_ctrl.SetAppHMIState(app_mock, testType, true);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_Media_WithPolicy_DevDisallowed) {
  mobile_apis::HMILevel::eType testType = eType::HMI_FULL;

  ::testing::InSequence seq;
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  EXPECT_CALL(*app_mngr, GetUserConsentForDevice("12345"))
      .WillOnce(Return(policy::kDeviceDisallowed));

  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(true));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));

  bool res = res_ctrl.SetAppHMIState(app_mock, testType, true);
  EXPECT_FALSE(res);
}

TEST_F(ResumeCtrlTest, SaveApplication) {
  utils::SharedPtr<application_manager::Application> app_sh_mock =
      new ApplicationMock();

  EXPECT_CALL(*mock_storage, SaveApplication(app_sh_mock));
  res_ctrl.SaveApplication(app_sh_mock);
}

TEST_F(ResumeCtrlTest, OnAppActivated_ResumptionHasStarted) {
  ::profile::Profile::instance()->config_file_name("smartDeviceLink_test.ini");

  smart_objects::SmartObject saved_app;
  EXPECT_CALL(*app_mngr, GetDefaultHmiLevel(_))
      .WillOnce(Return(default_testType));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  ON_CALL(*app_mock, is_media_application()).WillByDefault(Return(false));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));
  EXPECT_CALL(*app_mngr,
              SetState(test_app_id, default_testType, test_audio_state));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  EXPECT_CALL(*mock_storage, GetSavedApplication(test_policy_app_id, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(test_app_id));

  bool res = res_ctrl.StartResumptionOnlyHMILevel(app_mock);
  EXPECT_TRUE(res);

  utils::SharedPtr<ApplicationMock> app_sh_mock = new ApplicationMock();

  EXPECT_CALL(*app_sh_mock, app_id()).WillOnce(Return(test_app_id));
  res_ctrl.OnAppActivated(app_sh_mock);
}

TEST_F(ResumeCtrlTest, OnAppActivated_ResumptionNotActive) {
  utils::SharedPtr<ApplicationMock> app_sh_mock = new ApplicationMock();
  EXPECT_CALL(*app_sh_mock, app_id()).Times(0);
  res_ctrl.OnAppActivated(app_sh_mock);
}

TEST_F(ResumeCtrlTest, IsHMIApplicationIdExist) {
  uint32_t hmi_app_id = 10;

  EXPECT_CALL(*mock_storage, IsHMIApplicationIdExist(hmi_app_id))
      .WillOnce(Return(true));
  EXPECT_TRUE(res_ctrl.IsHMIApplicationIdExist(hmi_app_id));
}

TEST_F(ResumeCtrlTest, GetHMIApplicationID) {
  uint32_t hmi_app_id = 10;
  std::string device_id = "test_device_id";

  EXPECT_CALL(*mock_storage, GetHMIApplicationID(test_policy_app_id, device_id))
      .WillOnce(Return(hmi_app_id));
  EXPECT_EQ(hmi_app_id,
            res_ctrl.GetHMIApplicationID(test_policy_app_id, device_id));
}

TEST_F(ResumeCtrlTest, IsApplicationSaved) {
  std::string policy_app_id = "policy_app_id";
  std::string device_id = "device_id";

  EXPECT_CALL(*mock_storage, IsApplicationSaved(policy_app_id, device_id))
      .WillOnce(Return(true));
  EXPECT_TRUE(res_ctrl.IsApplicationSaved(policy_app_id, device_id));
}

TEST_F(ResumeCtrlTest, CheckPersistenceFiles_WithoutCommandAndChoiceSets) {
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = HMI_FULL;

  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_TRUE(res_ctrl.CheckPersistenceFilesForResumption(app_mock));
}

TEST_F(ResumeCtrlTest, CheckPersistenceFilesForResumption_WithCommands) {
  smart_objects::SmartObject test_application_commands;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = HMI_FULL;
  saved_app[application_manager::strings::application_commands] =
      test_application_commands;

  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              VerifyImageFiles(_, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  EXPECT_TRUE(res_ctrl.CheckPersistenceFilesForResumption(app_mock));
}

TEST_F(ResumeCtrlTest, CheckPersistenceFilesForResumption_WithChoiceSet) {
  smart_objects::SmartObject test_choice_sets;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = HMI_FULL;
  saved_app[application_manager::strings::application_choice_sets] =
      test_choice_sets;

  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));

  EXPECT_CALL(*mock_storage, GetSavedApplication(_, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_TRUE(res_ctrl.CheckPersistenceFilesForResumption(app_mock));
}

// TODO (VVeremjova) APPLINK-16718
TEST_F(ResumeCtrlTest, DISABLED_OnSuspend) {
  EXPECT_CALL(*mock_storage, OnSuspend());
  res_ctrl.OnSuspend();
}

TEST_F(ResumeCtrlTest, OnAwake) {
  EXPECT_CALL(*mock_storage, OnAwake());
  res_ctrl.OnAwake();
}

TEST_F(ResumeCtrlTest, RemoveApplicationFromSaved) {
  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  EXPECT_CALL(*mock_storage, RemoveApplicationFromSaved(_, _))
      .WillOnce(Return(true));
  EXPECT_TRUE(res_ctrl.RemoveApplicationFromSaved(app_mock));
}

TEST_F(ResumeCtrlTest, CheckApplicationHash) {
  smart_objects::SmartObject saved_app;

  std::string test_hash = "saved_hash";
  saved_app[application_manager::strings::hash_id] = test_hash;

  ON_CALL(*app_mock, device()).WillByDefault(Return(test_dev_id));
  ON_CALL(*app_mock, policy_app_id()).WillByDefault(Return(test_policy_app_id));
  EXPECT_CALL(*mock_storage, GetSavedApplication(test_policy_app_id, _, _))
      .WillRepeatedly(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_TRUE(res_ctrl.CheckApplicationHash(app_mock, test_hash));
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test
