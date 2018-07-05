/*
 * Copyright (c) 2017, Ford Motor Company
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
#include <algorithm>

#include "gtest/gtest.h"
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/usage_statistics.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_app_extension.h"
#include "application_manager/mock_resumption_data.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application.h"
#include "utils/data_accessor.h"
#include "utils/make_shared.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_state_controller.h"

namespace test {
namespace components {
namespace resumption_test {

using ::testing::_;
using ::testing::A;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;
using ::testing::ReturnPointee;
using ::testing::SaveArg;
using namespace application_manager_test;

using namespace resumption;
using namespace mobile_apis::HMILevel;

class ResumeCtrlTest : public ::testing::Test {
 protected:
  ResumeCtrlTest()
      : kTestAppId_(10u)
      , kTestPolicyAppId_("test_policy_app_id")
      , kMacAddress_("12345")
      , kDefaultTestLevel_(eType::HMI_NONE)
      , kCorId_(7u)
      , kTestDevId_(5u)
      , kTestGrammarId_(10)
      , kHash_("saved_hash")
      , kAppResumingTimeout_(30000u)  // miliseconds
      , kTestTimeStamp_(1452074434u)
      , app_set_lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , kDefaultDeferredTestLevel_(eType::INVALID_ENUM)
      , kNaviLowbandwidthLevel_("LIMITED")
      , kProjectionLowbandwidthLevel_("NONE")
      , kMediaLowbandwidthLevel_("NONE") {}

  virtual void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_app_mngr_);
    ON_CALL(mock_app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(mock_event_dispatcher_));
    mock_storage_ =
        ::utils::MakeShared<NiceMock<resumption_test::MockResumptionData> >(
            mock_app_mngr_);
    mock_app_ = utils::MakeShared<NiceMock<MockApplication> >();
    mock_app_extension_ = utils::MakeShared<
        NiceMock<application_manager_test::MockAppExtension> >();
    const_app_ =
        static_cast<application_manager::ApplicationConstSharedPtr>(mock_app_);
    res_ctrl_ = utils::MakeShared<ResumeCtrlImpl>(mock_app_mngr_);
    res_ctrl_->set_resumption_storage(mock_storage_);

    ON_CALL(mock_app_mngr_, state_controller())
        .WillByDefault(ReturnRef(mock_state_controller_));
    ON_CALL(mock_app_mngr_, get_settings())
        .WillByDefault(ReturnRef(mock_application_manager_settings_));
    EXPECT_CALL(app_mngr_, CheckResumptionRequiredTransportAvailable(_))
        .Times(AtLeast(0))
        .WillRepeatedly(Return(true));

    ON_CALL(mock_application_manager_settings_, use_db_for_resumption())
        .WillByDefault(Return(false));
    ON_CALL(mock_application_manager_settings_, app_resuming_timeout())
        .WillByDefault(ReturnRef(kAppResumingTimeout_));
    // use EXPECTED_CALL().Times(AtLeast(0)) instead of ON_CALL to remove
    // warning messages
    EXPECT_CALL(mock_application_manager_settings_,
                navigation_lowbandwidth_resumption_level())
        .Times(AtLeast(0))
        .WillRepeatedly(ReturnRef(kNaviLowbandwidthLevel_));
    EXPECT_CALL(mock_application_manager_settings_,
                projection_lowbandwidth_resumption_level())
        .Times(AtLeast(0))
        .WillRepeatedly(ReturnRef(kProjectionLowbandwidthLevel_));
    EXPECT_CALL(mock_application_manager_settings_,
                media_lowbandwidth_resumption_level())
        .Times(AtLeast(0))
        .WillRepeatedly(ReturnRef(kMediaLowbandwidthLevel_));

    EXPECT_CALL(*app_mock_, deferred_resumption_hmi_level())
        .Times(AtLeast(0))
        .WillRepeatedly(Return(kDefaultDeferredTestLevel_));
  }
  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_app_mngr_);
  }
  void GetInfoFromApp() {
    ON_CALL(*mock_app_, policy_app_id())
        .WillByDefault(Return(kTestPolicyAppId_));
    ON_CALL(*mock_app_, mac_address()).WillByDefault(ReturnRef(kMacAddress_));
    ON_CALL(*mock_app_, device()).WillByDefault(Return(kTestDevId_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kTestAppId_));
  }

  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
  application_manager_test::MockApplicationManagerSettings
      mock_application_manager_settings_;
  application_manager_test::MockApplicationManager mock_app_mngr_;
  utils::SharedPtr<NiceMock<application_manager_test::MockAppExtension> >
      mock_app_extension_;
  MockStateController mock_state_controller_;
  utils::SharedPtr<ResumeCtrl> res_ctrl_;
  utils::SharedPtr<NiceMock<resumption_test::MockResumptionData> >
      mock_storage_;
  utils::SharedPtr<NiceMock<MockApplication> > mock_app_;
  application_manager::ApplicationConstSharedPtr const_app_;
  const uint32_t kTestAppId_;
  const std::string kTestPolicyAppId_;
  const std::string kMacAddress_;
  const mobile_apis::HMILevel::eType kDefaultTestLevel_;
  const uint32_t kCorId_;
  const uint32_t kTestDevId_;
  const uint32_t kTestGrammarId_;
  const std::string kHash_;
  const uint32_t kAppResumingTimeout_;
  const uint32_t kTestTimeStamp_;
  std::shared_ptr<sync_primitives::Lock> app_set_lock_ptr_;
  sync_primitives::Lock app_set_lock_;
  const mobile_apis::HMILevel::eType kDefaultDeferredTestLevel_;
  const std::string kNaviLowbandwidthLevel_;
  const std::string kProjectionLowbandwidthLevel_;
  const std::string kMediaLowbandwidthLevel_;
};

/**
 * @brief  Group of tests which check starting resumption with different data
 */

TEST_F(ResumeCtrlTest, StartResumption_AppWithGrammarId) {
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;

  // Check RestoreApplicationData
  GetInfoFromApp();

  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*mock_app_, UpdateHash());
  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
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
  saved_app[application_manager::strings::hash_id] = kHash_;

  GetInfoFromApp();
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  // Check RestoreApplicationData
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*mock_app_, UpdateHash());
  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_)).Times(0);

  bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_FALSE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithFiles) {
  GetInfoFromApp();
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
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_files] =
      test_application_files;

  // Check RestoreApplicationData
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*mock_app_, UpdateHash());
  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));
  for (uint32_t i = 0; i < count_of_files; ++i) {
    EXPECT_CALL(*mock_app_,
                AddFile(CheckAppFile(
                    true,
                    true,
                    file_names[i],
                    static_cast<mobile_apis::FileType::eType>(file_types[i]))));
  }

  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubmenues) {
  GetInfoFromApp();
  smart_objects::SmartObject test_application_submenues;
  smart_objects::SmartObject test_submenu;

  const uint32_t count_of_submenues = 20;
  for (uint32_t i = 0; i < count_of_submenues; ++i) {
    test_submenu[application_manager::strings::menu_id] = i;
    test_application_submenues[i] = test_submenu;
  }

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_submenus] =
      test_application_submenues;

  // Check RestoreApplicationData
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  for (uint32_t i = 0; i < count_of_submenues; ++i) {
    EXPECT_CALL(*mock_app_, AddSubMenu(i, test_application_submenues[i]));
  }
  smart_objects::SmartObjectList requests;

  EXPECT_CALL(mock_app_mngr_, GetNextHMICorrelationID())
      .WillRepeatedly(Return(kCorId_));
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              CreateAddSubMenuRequestToHMI(_, kCorId_))
      .WillRepeatedly(Return(requests));

  EXPECT_CALL(*mock_app_, UpdateHash());
  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithCommands) {
  GetInfoFromApp();
  smart_objects::SmartObject test_application_commands;
  smart_objects::SmartObject test_commands;
  const uint32_t count_of_commands = 20;

  for (uint32_t i = 0; i < count_of_commands; ++i) {
    test_commands[application_manager::strings::cmd_id] = i;
    test_application_commands[i] = test_commands;
  }

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_commands] =
      test_application_commands;

  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  // Check RestoreApplicationData
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*mock_app_, UpdateHash());
  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  for (uint32_t i = 0; i < count_of_commands; ++i) {
    EXPECT_CALL(*mock_app_, AddCommand(i, test_application_commands[i]));
  }

  smart_objects::SmartObjectList requests;
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              CreateAddCommandRequestToHMI(_, _))
      .WillRepeatedly(Return(requests));

  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
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
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_choice_sets] =
      application_choice_sets;

  // Check RestoreApplicationData
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*mock_app_, UpdateHash());
  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  for (uint32_t i = 0; i < count_of_choice_sets; ++i) {
    EXPECT_CALL(*mock_app_, AddChoiceSet(i, application_choice_sets[i]));
  }

  smart_objects::SmartObjectList requests;
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              CreateAddVRCommandRequestFromChoiceToHMI(_))
      .WillRepeatedly(Return(requests));

  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithGlobalProperties) {
  // Prepare Data
  smart_objects::SmartObject test_global_properties;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_global_properties] =
      test_global_properties;

  // Check RestoreApplicationData
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              SendGlobalPropertiesToHMI(_));

  EXPECT_CALL(*mock_app_, load_global_properties(test_global_properties));

  EXPECT_CALL(*mock_app_, UpdateHash());
  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubscribeOnButtons) {
  // Prepare Data
  smart_objects::SmartObject test_subscriptions;
  smart_objects::SmartObject app_buttons;

  const uint32_t count_of_buttons = 17;
  for (uint32_t i = 0; i < count_of_buttons; ++i) {
    app_buttons[i] = i;
  }

  test_subscriptions[application_manager::strings::application_buttons] =
      app_buttons;

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_subscriptions] =
      test_subscriptions;

  // Check RestoreApplicationData
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  for (uint32_t i = 0; i < count_of_buttons; ++i) {
    EXPECT_CALL(
        *mock_app_,
        SubscribeToButton(static_cast<mobile_apis::ButtonName::eType>(i)));
  }
  EXPECT_CALL(*mock_app_, UpdateHash());

  std::list<application_manager::AppExtensionPtr> extensions;
  extensions.insert(extensions.begin(), mock_app_extension_);

  EXPECT_CALL(*mock_app_, Extensions()).WillOnce(ReturnRef(extensions));

  EXPECT_CALL(*mock_app_extension_, ProcessResumption(test_subscriptions));

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              SendAllOnButtonSubscriptionNotificationsForApp(_, _)).Times(2);

  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubscriptionToIVI) {
  // Prepare Data
  smart_objects::SmartObject test_subscriptions;
  smart_objects::SmartObject app_vi;

  int vtype = mobile_apis::VehicleDataType::VEHICLEDATA_GPS;
  uint i = 0;
  for (; vtype < mobile_apis::VehicleDataType::VEHICLEDATA_STEERINGWHEEL;
       ++i, ++vtype) {
    app_vi[i] = vtype;
  }

  test_subscriptions[application_manager::strings::application_vehicle_info] =
      app_vi;

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::application_subscriptions] =
      test_subscriptions;

  // Check RestoreApplicationData
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));

  smart_objects::SmartObjectList requests;

  EXPECT_CALL(*mock_app_, UpdateHash());
  std::list<application_manager::AppExtensionPtr> extensions;

  extensions.insert(extensions.begin(), mock_app_extension_);

  EXPECT_CALL(*mock_app_, Extensions()).WillOnce(ReturnRef(extensions));

  EXPECT_CALL(*mock_app_extension_, ProcessResumption(test_subscriptions));
  const bool res = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, StartResumption_AppWithSubscriptionToWayPoints) {
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::subscribed_for_way_points] = true;
  saved_app[application_manager::strings::time_stamp] = kTestTimeStamp_;

  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_CALL(*mock_app_, set_grammar_id(kTestGrammarId_));
  EXPECT_CALL(
      mock_app_mngr_,
      SubscribeAppForWayPoints(A<application_manager::ApplicationSharedPtr>()));
  const mobile_apis::HMILevel::eType hmi_test_level =
      mobile_apis::HMILevel::HMI_FULL;
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(hmi_test_level));
  EXPECT_CALL(mock_state_controller_, SetRegularState(_, hmi_test_level));

  const bool result = res_ctrl_->StartResumption(mock_app_, kHash_);
  EXPECT_TRUE(result);
}

TEST_F(ResumeCtrlTest, StartResumptionOnlyHMILevel) {
  smart_objects::SmartObject saved_app;

  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  const bool res = res_ctrl_->StartResumptionOnlyHMILevel(mock_app_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, RetryResumption) {
  const uint32_t app_id = 1;

  EXPECT_CALL(app_mngr_, application(app_id)).WillOnce(Return(app_mock_));
  EXPECT_CALL(*app_mock_, deferred_resumption_hmi_level())
      .WillOnce(Return(eType::HMI_FULL));

  res_ctrl_->RetryResumption(app_id);

  bool is_resumption_active = res_ctrl_->get_resumption_active();
  EXPECT_TRUE(is_resumption_active);
}

TEST_F(ResumeCtrlTest, RetryResumption_NotDeferred) {
  const uint32_t app_id = 1;

  EXPECT_CALL(app_mngr_, application(app_id)).WillOnce(Return(app_mock_));
  EXPECT_CALL(*app_mock_, deferred_resumption_hmi_level())
      .WillOnce(Return(eType::INVALID_ENUM));

  res_ctrl_->RetryResumption(app_id);

  bool is_resumption_active = res_ctrl_->get_resumption_active();
  EXPECT_FALSE(is_resumption_active);
}

TEST_F(ResumeCtrlTest, StartAppHmiStateResumption_AppInFull) {
  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_FULL;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  EXPECT_CALL(mock_state_controller_, SetRegularState(_, restored_test_type))
      .Times(AtLeast(1));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*mock_storage_,
              RemoveApplicationFromSaved(kTestPolicyAppId_, kMacAddress_))
      .WillOnce(Return(true));

  ON_CALL(mock_app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceAllowed));
  res_ctrl_->StartAppHmiStateResumption(mock_app_);
}

TEST_F(ResumeCtrlTest, StartAppHmiStateResumption_AppInBackground) {
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;

  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_BACKGROUND;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  EXPECT_CALL(mock_app_mngr_, state_controller()).Times(0);
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  res_ctrl_->StartAppHmiStateResumption(mock_app_);
}

TEST_F(ResumeCtrlTest, StartAppHmiStateResumption_AppHasDeferredResumption) {
  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_NONE;
  mobile_apis::HMILevel::eType deferred_level = eType::HMI_FULL;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  // resume into deferred level instead of restored level
  EXPECT_CALL(state_controller_, SetRegularState(_, deferred_level))
      .Times(AtLeast(1));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  mobile_apis::HMILevel::eType app_deferred_level = deferred_level;
  EXPECT_CALL(*app_mock_, deferred_resumption_hmi_level())
      .WillRepeatedly(ReturnPointee(&app_deferred_level));
  EXPECT_CALL(*app_mock_,
              set_deferred_resumption_hmi_level(eType::INVALID_ENUM))
      .WillOnce(SaveArg<0>(&app_deferred_level));

  EXPECT_CALL(*mock_storage_,
              RemoveApplicationFromSaved(kTestPolicyAppId_, kMacAddress_))
      .WillOnce(Return(true));

  ON_CALL(app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceAllowed));
  res_ctrl_->StartAppHmiStateResumption(app_mock_);
}

TEST_F(ResumeCtrlTest,
       StartAppHmiStateResumption_HighBandwidthTransportNotAvailable) {
  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_FULL;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  EXPECT_CALL(state_controller_, SetRegularState(_, eType::HMI_LIMITED))
      .Times(AtLeast(1));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(app_mngr_, CheckResumptionRequiredTransportAvailable(_))
      .WillOnce(Return(false));

  EXPECT_CALL(*app_mock_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*app_mock_, mobile_projection_enabled())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(*app_mock_, is_media_application()).WillRepeatedly(Return(false));

  // if resumption is deferred ...
  EXPECT_CALL(*app_mock_, deferred_resumption_hmi_level())
      .WillRepeatedly(Return(restored_test_type));

  // ... then RemoveApplicationFromSaved() will not be called
  EXPECT_CALL(*mock_storage_,
              RemoveApplicationFromSaved(kTestPolicyAppId_, kMacAddress_))
      .Times(0);

  ON_CALL(app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceAllowed));
  res_ctrl_->StartAppHmiStateResumption(app_mock_);
}

TEST_F(
    ResumeCtrlTest,
    StartAppHmiStateResumption_HighBandwidthTransportNotAvailable_NaviAndMedia) {
  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_LIMITED;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  // in this test, it is expected that the app will resume into LIMITED, which
  // is the higher level among NONE and LIMITED
  EXPECT_CALL(state_controller_, SetRegularState(_, eType::HMI_LIMITED))
      .Times(AtLeast(1));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(app_mngr_, CheckResumptionRequiredTransportAvailable(_))
      .WillOnce(Return(false));

  EXPECT_CALL(*app_mock_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*app_mock_, mobile_projection_enabled())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(*app_mock_, is_media_application()).WillRepeatedly(Return(true));

  std::string navi_lowbandwidth_level("NONE");
  std::string projection_lowbandwidth_level("BACKGROUND");
  std::string media_lowbandwidth_level("LIMITED");
  EXPECT_CALL(mock_application_manager_settings_,
              navigation_lowbandwidth_resumption_level())
      .WillRepeatedly(ReturnRef(navi_lowbandwidth_level));
  EXPECT_CALL(mock_application_manager_settings_,
              projection_lowbandwidth_resumption_level())
      .WillRepeatedly(ReturnRef(projection_lowbandwidth_level));
  EXPECT_CALL(mock_application_manager_settings_,
              media_lowbandwidth_resumption_level())
      .WillRepeatedly(ReturnRef(media_lowbandwidth_level));

  EXPECT_CALL(*app_mock_, deferred_resumption_hmi_level())
      .WillRepeatedly(Return(restored_test_type));

  EXPECT_CALL(*mock_storage_,
              RemoveApplicationFromSaved(kTestPolicyAppId_, kMacAddress_))
      .Times(0);

  ON_CALL(app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceAllowed));
  res_ctrl_->StartAppHmiStateResumption(app_mock_);
}

/**
 * @brief  Group of tests which check restoring resumption with different data
 */

TEST_F(ResumeCtrlTest, RestoreAppHMIState_RestoreHMILevelFull) {
  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_FULL;

  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;
  saved_app[application_manager::strings::grammar_id] = kTestGrammarId_;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  EXPECT_CALL(mock_state_controller_, SetRegularState(_, restored_test_type))
      .Times(AtLeast(1));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  ON_CALL(mock_app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceAllowed));
  EXPECT_CALL(*mock_app_, set_is_resuming(true));

  const bool res = res_ctrl_->RestoreAppHMIState(mock_app_);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetupDefaultHMILevel) {
  smart_objects::SmartObject saved_app;

  saved_app[application_manager::strings::hmi_level] = kDefaultTestLevel_;

  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  EXPECT_CALL(mock_app_mngr_, GetUserConsentForDevice("12345")).Times(0);

  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));

  EXPECT_CALL(mock_state_controller_, SetRegularState(_, kDefaultTestLevel_))
      .Times(AtLeast(1));

  res_ctrl_->SetupDefaultHMILevel(mock_app_);
}

TEST_F(ResumeCtrlTest,
       SetupDefaultHMILevel_HighBandwidthTransportNotAvailable) {
  smart_objects::SmartObject saved_app;

  saved_app[application_manager::strings::hmi_level] = kDefaultTestLevel_;

  ON_CALL(app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  GetInfoFromApp();
  EXPECT_CALL(app_mngr_, GetUserConsentForDevice("12345")).Times(0);

  ON_CALL(app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));

  EXPECT_CALL(app_mngr_, CheckResumptionRequiredTransportAvailable(_))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(*app_mock_, is_navi()).WillRepeatedly(Return(false));
  EXPECT_CALL(*app_mock_, mobile_projection_enabled())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*app_mock_, is_media_application()).WillRepeatedly(Return(false));

  // SetRegularState() should be called with kProjectionLowbandwidthLevel_
  EXPECT_CALL(state_controller_, SetRegularState(_, eType::HMI_NONE))
      .Times(AtLeast(1));

  res_ctrl_->SetupDefaultHMILevel(app_mock_);
}

TEST_F(ResumeCtrlTest, ApplicationResumptiOnTimer_AppInFull) {
  ON_CALL(mock_app_mngr_, application(kTestAppId_))
      .WillByDefault(Return(mock_app_));

  mobile_apis::HMILevel::eType restored_test_type = eType::HMI_FULL;
  const uint32_t ign_off_count = 0u;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = restored_test_type;

  MockStateController state_controller;
  EXPECT_CALL(mock_app_mngr_, state_controller())
      .WillOnce(ReturnRef(state_controller));
  EXPECT_CALL(state_controller, SetRegularState(_, restored_test_type))
      .Times(AtLeast(1));
  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*mock_storage_,
              RemoveApplicationFromSaved(kTestPolicyAppId_, kMacAddress_))
      .WillOnce(Return(true));

  ON_CALL(mock_app_mngr_, GetUserConsentForDevice(kMacAddress_))
      .WillByDefault(Return(policy::kDeviceAllowed));
  res_ctrl_->StartAppHmiStateResumption(mock_app_);
}

/**
 * @brief group of tests which check correct SetAppHMIState
 */

TEST_F(ResumeCtrlTest, SetAppHMIState_HMINone_WithoutCheckPolicy) {
  GetInfoFromApp();

  EXPECT_CALL(mock_app_mngr_, GetUserConsentForDevice("12345")).Times(0);

  EXPECT_CALL(*mock_app_, set_is_resuming(true));
  EXPECT_CALL(mock_state_controller_, SetRegularState(_, kDefaultTestLevel_))
      .Times(AtLeast(1));
  const bool res =
      res_ctrl_->SetAppHMIState(mock_app_, kDefaultTestLevel_, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMILimited_WithoutCheckPolicy) {
  mobile_apis::HMILevel::eType test_type = eType::HMI_LIMITED;
  GetInfoFromApp();
  EXPECT_CALL(mock_app_mngr_, GetUserConsentForDevice("12345")).Times(0);

  EXPECT_CALL(*mock_app_, set_is_resuming(true));
  EXPECT_CALL(mock_state_controller_, SetRegularState(_, test_type))
      .Times(AtLeast(1));
  const bool res = res_ctrl_->SetAppHMIState(mock_app_, test_type, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_WithoutCheckPolicy) {
  mobile_apis::HMILevel::eType test_type = eType::HMI_FULL;
  GetInfoFromApp();
  // GetDefaultHmiLevel should not be called
  EXPECT_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_)).Times(0);
  EXPECT_CALL(mock_app_mngr_, GetUserConsentForDevice("12345")).Times(0);

  EXPECT_CALL(*mock_app_, set_is_resuming(true));
  EXPECT_CALL(mock_state_controller_, SetRegularState(_, test_type))
      .Times(AtLeast(1));

  const bool res = res_ctrl_->SetAppHMIState(mock_app_, test_type, false);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_WithPolicy_DevAllowed) {
  mobile_apis::HMILevel::eType test_type = eType::HMI_FULL;

  GetInfoFromApp();
  ON_CALL(mock_app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceAllowed));

  EXPECT_CALL(*mock_app_, set_is_resuming(true));
  EXPECT_CALL(mock_state_controller_, SetRegularState(_, test_type))
      .Times(AtLeast(1));

  const bool res = res_ctrl_->SetAppHMIState(mock_app_, test_type, true);
  EXPECT_TRUE(res);
}

TEST_F(ResumeCtrlTest, SetAppHMIState_HMIFull_WithPolicy_DevDisallowed) {
  mobile_apis::HMILevel::eType test_type = eType::HMI_FULL;

  GetInfoFromApp();
  ON_CALL(mock_app_mngr_, GetUserConsentForDevice("12345"))
      .WillByDefault(Return(policy::kDeviceDisallowed));

  EXPECT_CALL(*mock_app_, set_is_resuming(true));
  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  EXPECT_CALL(mock_state_controller_, SetRegularState(_, kDefaultTestLevel_))
      .Times(AtLeast(1));
  const bool res = res_ctrl_->SetAppHMIState(mock_app_, test_type, true);
  EXPECT_FALSE(res);
}

TEST_F(ResumeCtrlTest, SaveAllApplications) {
  utils::SharedPtr<application_manager_test::MockApplication> test_app =
      ::utils::MakeShared<application_manager_test::MockApplication>();
  EXPECT_CALL(*test_app, app_id()).WillRepeatedly(Return(kTestAppId_));

  application_manager::ApplicationSet app_set;
  app_set.insert(test_app);

  DataAccessor<application_manager::ApplicationSet> accessor(app_set,
                                                             app_set_lock_ptr_);

  ON_CALL(mock_app_mngr_, applications()).WillByDefault(Return(accessor));
  EXPECT_CALL(*mock_storage_, SaveApplication(Eq(test_app)));
  res_ctrl_->SaveAllApplications();
}

TEST_F(ResumeCtrlTest, SaveAllApplications_EmptyApplicationlist) {
  utils::SharedPtr<application_manager::Application> mock_app =
      ::utils::MakeShared<application_manager_test::MockApplication>();

  application_manager::ApplicationSet app_set;

  DataAccessor<application_manager::ApplicationSet> accessor(app_set,
                                                             app_set_lock_ptr_);

  ON_CALL(mock_app_mngr_, applications()).WillByDefault(Return(accessor));
  EXPECT_CALL(*mock_storage_, SaveApplication(mock_app)).Times(0);
  res_ctrl_->SaveAllApplications();
}

TEST_F(ResumeCtrlTest, SaveApplication) {
  utils::SharedPtr<application_manager::Application> app_sh_mock =
      ::utils::MakeShared<application_manager_test::MockApplication>();

  EXPECT_CALL(*mock_storage_, SaveApplication(app_sh_mock));
  res_ctrl_->SaveApplication(app_sh_mock);
}

TEST_F(ResumeCtrlTest, OnAppActivated_ResumptionHasStarted) {
  smart_objects::SmartObject saved_app;
  GetInfoFromApp();

  ON_CALL(mock_app_mngr_, GetDefaultHmiLevel(const_app_))
      .WillByDefault(Return(kDefaultTestLevel_));
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kTestAppId_));

  const bool res = res_ctrl_->StartResumptionOnlyHMILevel(mock_app_);
  EXPECT_TRUE(res);

  utils::SharedPtr<application_manager_test::MockApplication> app_sh_mock =
      ::utils::MakeShared<application_manager_test::MockApplication>();

  EXPECT_CALL(*app_sh_mock, app_id()).WillOnce(Return(kTestAppId_));
  res_ctrl_->OnAppActivated(app_sh_mock);
}

TEST_F(ResumeCtrlTest, OnAppActivated_ResumptionNotActive) {
  utils::SharedPtr<application_manager_test::MockApplication> app_sh_mock =
      ::utils::MakeShared<application_manager_test::MockApplication>();
  EXPECT_CALL(*app_sh_mock, app_id()).Times(0);
  res_ctrl_->OnAppActivated(app_sh_mock);
}

TEST_F(ResumeCtrlTest, IsHMIApplicationIdExist) {
  uint32_t hmi_app_id = 10;

  EXPECT_CALL(*mock_storage_, IsHMIApplicationIdExist(hmi_app_id))
      .WillOnce(Return(true));
  EXPECT_TRUE(res_ctrl_->IsHMIApplicationIdExist(hmi_app_id));
}

TEST_F(ResumeCtrlTest, GetHMIApplicationID) {
  uint32_t hmi_app_id = 10;
  std::string device_id = "test_device_id";

  EXPECT_CALL(*mock_storage_, GetHMIApplicationID(kTestPolicyAppId_, device_id))
      .WillOnce(Return(hmi_app_id));
  EXPECT_EQ(hmi_app_id,
            res_ctrl_->GetHMIApplicationID(kTestPolicyAppId_, device_id));
}

TEST_F(ResumeCtrlTest, IsApplicationSaved) {
  const bool is_app_saved = true;
  EXPECT_CALL(*mock_storage_,
              IsApplicationSaved(kTestPolicyAppId_, kMacAddress_))
      .WillOnce(Return(is_app_saved));
  EXPECT_TRUE(res_ctrl_->IsApplicationSaved(kTestPolicyAppId_, kMacAddress_));
}

TEST_F(ResumeCtrlTest, OnAppRegistrationStart) {
  const bool is_app_saved = false;
  EXPECT_CALL(*mock_storage_,
              IsApplicationSaved(kTestPolicyAppId_, kMacAddress_))
      .WillOnce(Return(is_app_saved));
  res_ctrl_->OnAppRegistrationStart(kTestPolicyAppId_, kMacAddress_);
}

TEST_F(ResumeCtrlTest, OnAppRegistrationEnd) {
  const uint32_t timeout = 1000u;
  EXPECT_CALL(mock_application_manager_settings_,
              app_resumption_save_persistent_data_timeout())
      .WillOnce(ReturnRef(timeout));
  res_ctrl_->OnAppRegistrationEnd();
}

TEST_F(ResumeCtrlTest, CheckPersistenceFiles_WithoutCommandAndChoiceSets) {
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = HMI_FULL;

  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_TRUE(res_ctrl_->CheckPersistenceFilesForResumption(mock_app_));
}

TEST_F(ResumeCtrlTest, CheckPersistenceFilesForResumption_WithCommands) {
  smart_objects::SmartObject test_application_commands;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = HMI_FULL;
  saved_app[application_manager::strings::application_commands] =
      test_application_commands;

  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              VerifyImageFiles(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  EXPECT_TRUE(res_ctrl_->CheckPersistenceFilesForResumption(mock_app_));
}

TEST_F(ResumeCtrlTest, CheckPersistenceFilesForResumption_WithChoiceSet) {
  smart_objects::SmartObject test_choice_sets;
  uint32_t ign_off_count = 0;
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::ign_off_count] = ign_off_count;
  saved_app[application_manager::strings::hmi_level] = HMI_FULL;
  saved_app[application_manager::strings::application_choice_sets] =
      test_choice_sets;

  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));

  EXPECT_TRUE(res_ctrl_->CheckPersistenceFilesForResumption(mock_app_));
}

// TODO (VVeremjova) APPLINK-16718
TEST_F(ResumeCtrlTest, DISABLED_OnSuspend) {
  EXPECT_CALL(*mock_storage_, IncrementIgnOffCount());
  res_ctrl_->OnSuspend();
}

TEST_F(ResumeCtrlTest, OnSuspend_EmptyApplicationlist) {
  utils::SharedPtr<application_manager::Application> mock_app =
      ::utils::MakeShared<application_manager_test::MockApplication>();

  application_manager::ApplicationSet app_set;

  DataAccessor<application_manager::ApplicationSet> accessor(app_set,
                                                             app_set_lock_ptr_);

  ON_CALL(mock_app_mngr_, applications()).WillByDefault(Return(accessor));
  EXPECT_CALL(*mock_storage_, SaveApplication(mock_app)).Times(0);

  EXPECT_CALL(*mock_storage_, IncrementIgnOffCount()).Times(0);
  EXPECT_CALL(*mock_storage_, Persist());
  res_ctrl_->OnSuspend();
}

TEST_F(ResumeCtrlTest, OnAwake) {
  uint32_t timeout = 10u;
  EXPECT_CALL(mock_application_manager_settings_,
              app_resumption_save_persistent_data_timeout())
      .WillOnce(ReturnRef(timeout));
  EXPECT_CALL(*mock_storage_, DecrementIgnOffCount()).Times(0);
  res_ctrl_->OnAwake();
}

TEST_F(ResumeCtrlTest, RemoveApplicationFromSaved) {
  GetInfoFromApp();
  EXPECT_CALL(*mock_storage_,
              RemoveApplicationFromSaved(kTestPolicyAppId_, kMacAddress_))
      .WillOnce(Return(true));
  EXPECT_TRUE(res_ctrl_->RemoveApplicationFromSaved(mock_app_));
}

TEST_F(ResumeCtrlTest, CheckApplicationkHash_) {
  smart_objects::SmartObject saved_app;
  saved_app[application_manager::strings::hash_id] = kHash_;

  GetInfoFromApp();
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetArgReferee<2>(saved_app), Return(true)));
  EXPECT_TRUE(res_ctrl_->CheckApplicationHash(mock_app_, kHash_));
}

TEST_F(ResumeCtrlTest, GetSavedAppHmiLevel_NoAskedApp_INVALID_ENUM) {
  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(Return(false));
  EXPECT_EQ(mobile_apis::HMILevel::INVALID_ENUM,
            res_ctrl_->GetSavedAppHmiLevel(kTestPolicyAppId_, kMacAddress_));
}

ACTION_P(SetHmiLevel, hmi_level) {
  arg2[am::strings::hmi_level] = hmi_level;
}

TEST_F(ResumeCtrlTest, GetSavedAppHmiLevel_AskedAppFound_INVALID_ENUM) {
  const mobile_apis::HMILevel::eType hmi_level =
      mobile_apis::HMILevel::HMI_FULL;

  ON_CALL(*mock_storage_,
          GetSavedApplication(kTestPolicyAppId_, kMacAddress_, _))
      .WillByDefault(DoAll(SetHmiLevel(hmi_level), Return(true)));
  EXPECT_EQ(hmi_level,
            res_ctrl_->GetSavedAppHmiLevel(kTestPolicyAppId_, kMacAddress_));
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test
