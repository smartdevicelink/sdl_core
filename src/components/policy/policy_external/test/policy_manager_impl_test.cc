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

#include <fstream>

#include "json/reader.h"
#include "gtest/gtest.h"

#include "policy/policy_manager_impl_test_base.h"
#include "utils/make_shared.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AtLeast;

namespace test {
namespace components {
namespace policy_test {

TEST_F(
    PolicyManagerImplTest,
    RefreshRetrySequence_SetSecondsBetweenRetries_ExpectRetryTimeoutSequenceWithSameSeconds) {
  // Arrange
  std::vector<int> seconds;
  seconds.push_back(50);
  seconds.push_back(100);
  seconds.push_back(200);

  // Assert
  EXPECT_CALL(*cache_manager_, TimeoutResponse()).WillOnce(Return(60));
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_))
      .WillOnce(DoAll(SetArgReferee<0>(seconds), Return(true)));

  // Act
  policy_manager_->RefreshRetrySequence();

  // Assert
  EXPECT_EQ(50, policy_manager_->NextRetryTimeout());
  EXPECT_EQ(100, policy_manager_->NextRetryTimeout());
  EXPECT_EQ(200, policy_manager_->NextRetryTimeout());
  EXPECT_EQ(0, policy_manager_->NextRetryTimeout());
}

TEST_F(PolicyManagerImplTest, GetNotificationsNumber) {
  std::string priority = "EMERGENCY";
  uint32_t notif_number = 100;
  EXPECT_CALL(*cache_manager_, GetNotificationsNumber(priority))
      .WillOnce(Return(notif_number));

  EXPECT_EQ(notif_number, policy_manager_->GetNotificationsNumber(priority));
}

TEST_F(PolicyManagerImplTest, IncrementGlobalCounter) {
  // Assert
  EXPECT_CALL(*cache_manager_, Increment(usage_statistics::SYNC_REBOOTS));
  policy_manager_->Increment(usage_statistics::SYNC_REBOOTS);
}

TEST_F(PolicyManagerImplTest, IncrementAppCounter) {
  // Assert
  EXPECT_CALL(*cache_manager_,
              Increment("12345", usage_statistics::USER_SELECTIONS));
  policy_manager_->Increment("12345", usage_statistics::USER_SELECTIONS);
}

TEST_F(PolicyManagerImplTest, SetAppInfo) {
  // Assert
  EXPECT_CALL(*cache_manager_,
              Set("12345", usage_statistics::LANGUAGE_GUI, "de-de"));
  policy_manager_->Set("12345", usage_statistics::LANGUAGE_GUI, "de-de");
}

TEST_F(PolicyManagerImplTest, AddAppStopwatch) {
  // Assert
  EXPECT_CALL(*cache_manager_,
              Add("12345", usage_statistics::SECONDS_HMI_FULL, 30));
  policy_manager_->Add("12345", usage_statistics::SECONDS_HMI_FULL, 30);
}

TEST_F(PolicyManagerImplTest, ResetPT) {
  EXPECT_CALL(*cache_manager_, ResetPT("filename"))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, IsPTPreloaded())
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions()).Times(AtLeast(1));
  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  EXPECT_TRUE(policy_manager_->ResetPT("filename"));
  EXPECT_TRUE(cache->IsPTPreloaded());
  EXPECT_FALSE(policy_manager_->ResetPT("filename"));
  EXPECT_FALSE(cache->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest, LoadPT_SetPT_PTIsLoaded) {
  // Arrange
  policy_manager_->ForcePTExchange();
  policy_manager_->OnUpdateStarted();
  Json::Value table = createPTforLoad();

  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert
  ASSERT_TRUE(IsValid(update));

  EXPECT_CALL(*cache_manager_, GetHMIAppTypeAfterUpdate(_)).Times(AtLeast(1));

  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  utils::SharedPtr<policy_table::Table> snapshot =
      new policy_table::Table(update.policy_table);
  // Assert
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(*cache_manager_, ApplyUpdate(_)).WillOnce(Return(true));
  EXPECT_CALL(listener_, GetAppName("1234"))
      .WillOnce(Return(custom_str::CustomString("")));
  EXPECT_CALL(listener_, OnUpdateStatusChanged(_));
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(false));
  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_CALL(*cache_manager_, IsPTPreloaded());
  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest2,
       KmsChanged_SetExceededKms_ExpectCorrectSchedule) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  ::policy::Counters counter = ::policy::Counters::KILOMETERS;
  policy_manager_->PTUpdatedAt(counter, 50000);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  // Set kms changed but not exceed limit
  policy_manager_->KmsChanged(51500);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  // Set kms changed and exceed limit
  policy_manager_->KmsChanged(52500);
  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, ForcePTExchange_ExpectUpdateNeeded) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  // Force OT Exchange
  policy_manager_->ForcePTExchange();
  // Check update required
  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, OnSystemReady) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  // Check
  EXPECT_CALL(listener_, OnSystemInfoUpdateRequired());
  policy_manager_->OnSystemReady();
}

TEST_F(PolicyManagerImplTest2, ResetRetrySequence) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  policy_manager_->ResetRetrySequence();
  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
  policy_manager_->OnUpdateStarted();
  EXPECT_EQ("UPDATING", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, NextRetryTimeout_ExpectTimeoutsFromPT) {
  // Arrange
  std::ifstream ifile(preloadet_pt_filename_);
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT(preloadet_pt_filename_);
    for (uint32_t i = 0; i < size; ++i) {
      EXPECT_EQ(seconds_between_retries[i],
                policy_manager_->NextRetryTimeout());
    }
  }
}

TEST_F(PolicyManagerImplTest2, TimeOutExchange) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  // Check value taken from PT
  EXPECT_EQ(70000u, policy_manager_->TimeoutExchangeMSec());
}

TEST_F(PolicyManagerImplTest,
       RequestPTUpdate_SetPT_GeneratedSnapshotAndPTUpdate) {
  Json::Value table = createPTforLoad();
  utils::SharedPtr<policy_table::Table> p_table =
      utils::MakeShared<policy_table::Table>(&table);
  ASSERT_TRUE(p_table);
  p_table->SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  EXPECT_TRUE(IsValid(*p_table));

  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(p_table));

  policy_manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_InvalidPT_PTUpdateFail) {
  utils::SharedPtr<policy_table::Table> p_table =
      utils::MakeShared<policy_table::Table>();
  ASSERT_TRUE(p_table);
  EXPECT_FALSE(IsValid(*p_table));

  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _)).Times(0);
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(p_table));

  policy_manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_InvalidSnapshot_PTUpdateFail) {
  utils::SharedPtr<policy_table::Table> p_table;
  EXPECT_FALSE(p_table);

  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _)).Times(0);
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(p_table));

  policy_manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, ResetUserConsent_ResetOnlyOnce) {
  EXPECT_CALL(*cache_manager_, ResetUserConsent())
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_TRUE(policy_manager_->ResetUserConsent());
  EXPECT_FALSE(policy_manager_->ResetUserConsent());
}

TEST_F(PolicyManagerImplTest2, GetPolicyTableStatus_ExpectUpToDate) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  // Check
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest,
       SetUpdateStarted_GetPolicyTableStatus_Expect_Updating) {
  // Arrange
  policy_manager_->ForcePTExchange();
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(true));
  policy_manager_->OnUpdateStarted();
  // Check
  EXPECT_EQ("UPDATING", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       RetrySequenceDelaysSeconds_Expect_CorrectValues) {
  // Arrange
  std::ifstream ifile(preloadet_pt_filename_);
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT(preloadet_pt_filename_);
    std::vector<int> delaySecs = policy_manager_->RetrySequenceDelaysSeconds();
    // Check
    ASSERT_EQ(size, delaySecs.size());
    for (uint32_t i = 0; i < size; ++i) {
      EXPECT_EQ(seconds_between_retries[i], delaySecs[i]);
    }
  }
}

TEST_F(PolicyManagerImplTest2,
       OnExceededTimeout_GetPolicyTableStatus_ExpectUpdateNeeded) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  policy_manager_->ForcePTExchange();
  policy_manager_->OnExceededTimeout();
  // Check
  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest, MarkUnpairedDevice) {
  // Assert
  EXPECT_CALL(*cache_manager_, SetUnpairedDevice(unpaired_device_id_, true))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, SetDeviceConsent(unpaired_device_id_, false));
  EXPECT_CALL(*cache_manager_,
              HasDeviceSpecifiedConsent(unpaired_device_id_, false))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*cache_manager_, IgnitionCyclesBeforeExchange());
  EXPECT_CALL(*cache_manager_, DaysBeforeExchange(_));
  // Act
  policy_manager_->MarkUnpairedDevice(unpaired_device_id_);
}

TEST_F(PolicyManagerImplTest2, GetCurrentDeviceId) {
  // Arrange
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_));
  EXPECT_EQ("", policy_manager_->GetCurrentDeviceId(app_id_2_));
}

}  // namespace policy
}  // namespace components
}  // namespace test
