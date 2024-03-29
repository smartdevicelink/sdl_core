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

#include <algorithm>
#include <string>
#include "gtest/gtest.h"

#include "application_manager/mock_application.h"
#include "application_manager/mock_resumption_data.h"
#include "application_manager/usage_statistics.h"
#include "interfaces/MOBILE_API.h"
#include "resumption/last_state_impl.h"
#include "resumption/last_state_wrapper_impl.h"

#include "application_manager/resumption_data_test.h"
#include "formatters/CFormatterJsonBase.h"

#include "utils/file_system.h"

#include "application_manager/resumption/resumption_data_json.h"
namespace test {
namespace components {
namespace resumption_test {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace am = application_manager;
using namespace Json;
using namespace file_system;

using namespace resumption;
using namespace mobile_apis;
namespace formatters = ns_smart_device_link::ns_json_handler::formatters;

class ResumptionDataJsonTest : public ResumptionDataTest {
 protected:
  ResumptionDataJsonTest()
      : last_state_wrapper_(std::make_shared<resumption::LastStateWrapperImpl>(
            std::make_shared<resumption::LastStateImpl>("app_storage_folder",
                                                        "app_info_storage")))
      , res_json(last_state_wrapper_, mock_application_manager_) {}
  virtual void SetUp() {
    app_mock = std::make_shared<
        NiceMock<application_manager_test::MockApplication> >();

    policy_app_id_ = "test_policy_app_id";
    app_id_ = 10;
    is_audio_ = true;
    grammar_id_ = 20;
    hash_ = "saved_hash";
    hmi_level_ = HMILevel::eType::HMI_FULL;
    hmi_app_id_ = 8;
    ign_off_count_ = 0;
  }

  void CheckSavedJson() {
    resumption::LastStateAccessor accessor =
        last_state_wrapper_->get_accessor();
    Value dictionary = accessor.GetData().dictionary();
    ASSERT_TRUE(dictionary[am::strings::resumption].isObject());
    ASSERT_TRUE(
        dictionary[am::strings::resumption][am::strings::resume_app_list]
            .isArray());
    const Value& resume_app_list =
        dictionary[am::strings::resumption][am::strings::resume_app_list];
    sm::SmartObject res_app_list;
    for (uint32_t i = 0; i < resume_app_list.size(); i++) {
      formatters::CFormatterJsonBase::jsonValueToObj(resume_app_list[i],
                                                     res_app_list);
      CheckSavedApp(res_app_list);
    }
  }

  void SetZeroIgnOff() {
    resumption::LastStateAccessor accessor =
        last_state_wrapper_->get_accessor();
    Value dictionary = accessor.GetData().dictionary();
    Value& res = dictionary[am::strings::resumption];
    res[am::strings::last_ign_off_time] = 0;
    accessor.GetMutableData().set_dictionary(dictionary);
    accessor.GetMutableData().SaveToFileSystem();
  }

  std::shared_ptr<resumption::LastStateWrapperImpl> last_state_wrapper_;
  ResumptionDataJson res_json;
};

TEST_F(ResumptionDataJsonTest, SaveApplication) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, SavedApplicationTwice) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_)).Times(2);
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, SavedApplicationTwice_UpdateApp) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_)).Times(2);
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  (*vr_help_)[0][am::strings::position] = 2;

  res_json.SaveApplication(app_mock);
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, RemoveApplicationFromSaved) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  EXPECT_TRUE(
      res_json.RemoveApplicationFromSaved(policy_app_id_, kMacAddress_));

  // Check that application was deleted
  smart_objects::SmartObject remove_app;
  EXPECT_FALSE(
      res_json.GetSavedApplication(policy_app_id_, kMacAddress_, remove_app));
  EXPECT_TRUE(remove_app.empty());
}

TEST_F(ResumptionDataJsonTest, RemoveApplicationFromSaved_AppNotSaved) {
  EXPECT_FALSE(res_json.RemoveApplicationFromSaved(policy_app_id_, "54321"));
}

TEST_F(ResumptionDataJsonTest, IsApplicationSaved_ApplicationSaved) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  ssize_t result = res_json.IsApplicationSaved(policy_app_id_, kMacAddress_);
  EXPECT_EQ(0, result);
}

TEST_F(ResumptionDataJsonTest, IsApplicationSaved_ApplicationRemoved) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  EXPECT_TRUE(
      res_json.RemoveApplicationFromSaved(policy_app_id_, kMacAddress_));
  ssize_t result = res_json.IsApplicationSaved(policy_app_id_, kMacAddress_);
  EXPECT_EQ(-1, result);
}

TEST_F(ResumptionDataJsonTest, GetSavedApplication) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  smart_objects::SmartObject saved_app;
  EXPECT_TRUE(
      res_json.GetSavedApplication(policy_app_id_, kMacAddress_, saved_app));
  CheckSavedApp(saved_app);
}

TEST_F(ResumptionDataJsonTest, GetSavedApplication_AppNotSaved) {
  smart_objects::SmartObject saved_app;
  EXPECT_FALSE(
      res_json.GetSavedApplication(policy_app_id_, "54321", saved_app));
  EXPECT_TRUE(saved_app.empty());
}

TEST_F(ResumptionDataJsonTest, GetDataForLoadResumeData) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  smart_objects::SmartObject saved_app;
  res_json.GetDataForLoadResumeData(saved_app);

  EXPECT_EQ(policy_app_id_, saved_app[0][am::strings::app_id].asString());
  EXPECT_EQ(kMacAddress_, saved_app[0][am::strings::device_id].asString());
  EXPECT_EQ(hmi_level_,
            static_cast<HMILevel::eType>(
                saved_app[0][am::strings::hmi_level].asInt()));
  EXPECT_EQ(ign_off_count_, saved_app[0][am::strings::ign_off_count].asUInt());
}

TEST_F(ResumptionDataJsonTest, GetDataForLoadResumeData_AppRemove) {
  smart_objects::SmartObject saved_app;

  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  EXPECT_TRUE(
      res_json.RemoveApplicationFromSaved(policy_app_id_, kMacAddress_));
  res_json.GetDataForLoadResumeData(saved_app);
  EXPECT_TRUE(saved_app.empty());
}

TEST_F(ResumptionDataJsonTest, UpdateHmiLevel) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  HMILevel::eType new_hmi_level = HMILevel::HMI_LIMITED;
  res_json.UpdateHmiLevel(policy_app_id_, kMacAddress_, new_hmi_level);
  hmi_level_ = new_hmi_level;

  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, IsHMIApplicationIdExist_AppIsSaved) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  EXPECT_TRUE(res_json.IsHMIApplicationIdExist(hmi_app_id_));
}

TEST_F(ResumptionDataJsonTest, IsHMIApplicationIdExist_AppNotSaved) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);

  CheckSavedJson();
  uint32_t new_hmi_app_id_ = hmi_app_id_ + 10;
  EXPECT_FALSE(res_json.IsHMIApplicationIdExist(new_hmi_app_id_));
}

TEST_F(ResumptionDataJsonTest, GetHMIApplicationID) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  EXPECT_EQ(hmi_app_id_,
            res_json.GetHMIApplicationID(policy_app_id_, kMacAddress_));
}

TEST_F(ResumptionDataJsonTest, GetHMIApplicationID_AppNotSaved) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  EXPECT_EQ(0u, res_json.GetHMIApplicationID(policy_app_id_, "other_dev_id"));
}

TEST_F(ResumptionDataJsonTest, OnSuspend) {
  SetZeroIgnOff();
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  res_json.IncrementIgnOffCount();
  ign_off_count_++;
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, OnSuspendFourTimes) {
  PrepareData();
  SetZeroIgnOff();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  res_json.IncrementIgnOffCount();
  ign_off_count_++;
  CheckSavedJson();

  res_json.IncrementIgnOffCount();
  res_json.IncrementIgnOffCount();
  res_json.IncrementIgnOffCount();

  EXPECT_TRUE(-1 != res_json.IsApplicationSaved(policy_app_id_, kMacAddress_));
}

TEST_F(ResumptionDataJsonTest, OnSuspendOnAwake) {
  PrepareData();
  SetZeroIgnOff();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  res_json.IncrementIgnOffCount();
  ign_off_count_++;
  CheckSavedJson();

  res_json.DecrementIgnOffCount();
  ign_off_count_ = 0;
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, Awake_AppNotSuspended) {
  SetZeroIgnOff();
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  res_json.DecrementIgnOffCount();
  ign_off_count_ = 0;
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, TwiceAwake_AppNotSuspended) {
  SetZeroIgnOff();
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  res_json.IncrementIgnOffCount();
  res_json.DecrementIgnOffCount();
  ign_off_count_ = 0;
  CheckSavedJson();

  res_json.DecrementIgnOffCount();
  CheckSavedJson();
}

TEST_F(ResumptionDataJsonTest, GetHashId) {
  PrepareData();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  std::string test_hash;
  EXPECT_TRUE(res_json.GetHashId(policy_app_id_, kMacAddress_, test_hash));
  EXPECT_EQ(hash_, test_hash);
}

TEST_F(ResumptionDataJsonTest, GetIgnOffTime_AfterSuspendAndAwake) {
  int64_t last_ign_off_time;
  PrepareData();
  SetZeroIgnOff();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();
  last_ign_off_time = res_json.GetIgnOffTime();
  EXPECT_EQ(0u, last_ign_off_time);

  res_json.IncrementIgnOffCount();

  int64_t after_suspend;
  after_suspend = res_json.GetIgnOffTime();
  EXPECT_LE(last_ign_off_time, after_suspend);

  int64_t after_awake;
  res_json.DecrementIgnOffCount();

  after_awake = res_json.GetIgnOffTime();
  EXPECT_LE(after_suspend, after_awake);
}

TEST_F(ResumptionDataJsonTest, DropAppDataResumption) {
  PrepareData();
  SetZeroIgnOff();
  EXPECT_CALL(*mock_app_extension_, SaveResumptionData(_));
  res_json.SaveApplication(app_mock);
  CheckSavedJson();

  EXPECT_TRUE(res_json.DropAppDataResumption(kMacAddress_, policy_app_id_));

  smart_objects::SmartObject app;
  EXPECT_TRUE(res_json.GetSavedApplication(policy_app_id_, kMacAddress_, app));

  EXPECT_TRUE(app.keyExists(am::strings::application_commands) &&
              app[am::strings::application_commands].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_submenus) &&
              app[am::strings::application_submenus].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_choice_sets) &&
              app[am::strings::application_choice_sets].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_global_properties) &&
              app[am::strings::application_global_properties].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_subscriptions) &&
              app[am::strings::application_subscriptions].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_files) &&
              app[am::strings::application_files].empty());

  EXPECT_FALSE(app.keyExists(am::strings::grammar_id));
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test
