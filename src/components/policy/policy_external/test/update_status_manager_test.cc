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

#include "gtest/gtest.h"
#include "policy/mock_policy_listener.h"
#include "policy/policy_manager_impl.h"
#include "policy/update_status_manager.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace policy_test {

using namespace ::policy;
using ::testing::_;
using ::testing::Return;
using testing::NiceMock;

class UpdateStatusManagerTest : public ::testing::Test {
 protected:
  utils::SharedPtr<UpdateStatusManager> manager_;
  PolicyTableStatus status_;
  const uint32_t k_timeout_;
  NiceMock<MockPolicyListener> listener_;
  const std::string up_to_date_status_;
  const std::string update_needed_status_;
  const std::string updating_status_;

 public:
  UpdateStatusManagerTest()
      : manager_(utils::MakeShared<UpdateStatusManager>())
      , k_timeout_(1)
      , listener_()
      , up_to_date_status_("UP_TO_DATE")
      , update_needed_status_("UPDATE_NEEDED")
      , updating_status_("UPDATING") {}

  void SetUp() OVERRIDE {
    manager_->set_listener(&listener_);
    ON_CALL(listener_, OnUpdateStatusChanged(_)).WillByDefault(Return());
  }

  void TearDown() OVERRIDE {}
};

// TODO(AKutsan) : APPLINK-31222 use miliseconds as parameter of OnUpdateSentOut
// function
TEST_F(
    UpdateStatusManagerTest,
    DISABLED_OnUpdateSentOut_WaitForTimeoutExpired_ExpectStatusUpdateNeeded) {
  // Arrange
  manager_->ScheduleUpdate();
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  // Wait until timeout expired
  sleep(k_timeout_ + 1);
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnUpdateTimeOutOccurs_ExpectStatusUpdateNeeded) {
  // Arrange
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpToDate, status_);
  manager_->ScheduleUpdate();
  manager_->OnUpdateTimeoutOccurs();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnValidUpdateReceived_SetValidUpdateReceived_ExpectStatusUpToDate) {
  // Arrange
  EXPECT_CALL(listener_, OnUpdateStatusChanged(update_needed_status_));
  manager_->ScheduleUpdate();
  EXPECT_EQ(StatusUpdateRequired, status_);
  EXPECT_CALL(listener_, OnUpdateStatusChanged(updating_status_));
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);

  EXPECT_CALL(listener_, OnUpdateStatusChanged(up_to_date_status_));
  manager_->OnValidUpdateReceived();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpToDate, status_);
}

TEST_F(
    UpdateStatusManagerTest,
    SheduledUpdate_OnValidUpdateReceived_ExpectStatusUpToDateThanUpdateNeeded) {
  using ::testing::InSequence;
  // Arrange
  EXPECT_CALL(listener_, OnUpdateStatusChanged(update_needed_status_));
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  EXPECT_CALL(listener_, OnUpdateStatusChanged(updating_status_));
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->ScheduleUpdate();

  InSequence s;
  EXPECT_CALL(listener_, OnUpdateStatusChanged(up_to_date_status_));
  EXPECT_CALL(listener_, OnUpdateStatusChanged(update_needed_status_));

  manager_->OnValidUpdateReceived();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnWrongUpdateReceived_SetWrongUpdateReceived_ExpectStatusUpdateNeeded) {
  // Arrange
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->OnWrongUpdateReceived();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnResetDefaulPT_ResetPTtoDefaultState_ExpectPTinDefaultState) {
  // Arrange
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  // Reset PT to default state with flag update required
  manager_->OnResetDefaultPT(true);
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnResetDefaulPT2_ResetPTtoDefaultState_ExpectPTinDefaultState) {
  // Arrange
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  // Reset PT to default state with flag update not needed
  manager_->OnResetDefaultPT(false);
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpToDate, status_);
}

TEST_F(UpdateStatusManagerTest, OnResetRetrySequence_ExpectStatusUpToDate) {
  // Arrange
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->OnResetRetrySequence();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdatePending, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnNewApplicationAdded_ExpectStatusUpdateNeeded) {
  // Arrange
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->OnNewApplicationAdded(kDeviceAllowed);
  status_ = manager_->GetLastUpdateStatus();
  // Checks
  EXPECT_EQ(StatusUpdatePending, status_);
  EXPECT_TRUE(manager_->IsUpdatePending());
  EXPECT_TRUE(manager_->IsUpdateRequired());
}

TEST_F(UpdateStatusManagerTest, ScheduleUpdate_ExpectStatusUpdateNeeded) {
  // Arrange
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdateRequired, status_);
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->OnValidUpdateReceived();
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpToDate, status_);
  EXPECT_FALSE(manager_->IsUpdatePending());
  EXPECT_FALSE(manager_->IsUpdateRequired());
  manager_->ScheduleUpdate();
  status_ = manager_->GetLastUpdateStatus();
  // Checks
  EXPECT_EQ(StatusUpdateRequired, status_);
  EXPECT_FALSE(manager_->IsUpdatePending());
  EXPECT_TRUE(manager_->IsUpdateRequired());
}

TEST_F(UpdateStatusManagerTest,
       OnPolicyInit_SetUpdateRequired_ExpectStatusUpdateNeeded) {
  // Arrange
  manager_->OnPolicyInit(true);
  status_ = manager_->GetLastUpdateStatus();
  // Checks
  EXPECT_EQ(StatusUpdateRequired, status_);
  EXPECT_FALSE(manager_->IsUpdatePending());
  EXPECT_TRUE(manager_->IsUpdateRequired());
}

TEST_F(UpdateStatusManagerTest,
       OnPolicyInit_SetUpdateNotRequired_ExpectStatusUpToDate) {
  // Arrange
  manager_->OnPolicyInit(false);
  status_ = manager_->GetLastUpdateStatus();
  // Checks
  EXPECT_EQ(StatusUpToDate, status_);
  EXPECT_FALSE(manager_->IsUpdatePending());
  EXPECT_FALSE(manager_->IsUpdateRequired());
}

TEST_F(UpdateStatusManagerTest,
       StringifiedUpdateStatus_SetStatuses_ExpectCorrectStringifiedStatuses) {
  // Arrange
  manager_->OnPolicyInit(false);
  // Check
  EXPECT_EQ("UP_TO_DATE", manager_->StringifiedUpdateStatus());
  manager_->OnPolicyInit(true);
  // Check
  EXPECT_EQ("UPDATE_NEEDED", manager_->StringifiedUpdateStatus());
  manager_->OnUpdateSentOut(k_timeout_);
  // Check
  EXPECT_EQ("UPDATING", manager_->StringifiedUpdateStatus());
}

TEST_F(UpdateStatusManagerTest,
       OnAppSearchStartedCompleted_ExpectAppSearchCorrectStatus) {
  // Arrange
  manager_->OnAppsSearchStarted();
  // Check
  EXPECT_TRUE(manager_->IsAppsSearchInProgress());
  // Arrange
  manager_->OnAppsSearchCompleted();
  // Check
  EXPECT_FALSE(manager_->IsAppsSearchInProgress());
}

}  // namespace policy
}  // namespace components
}  // namespace test
