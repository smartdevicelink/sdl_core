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
#include "mock_policy_listener.h"
#include "policy/policy_manager_impl.h"
#include "policy/update_status_manager.h"

using ::policy::MockPolicyListener;

namespace test {
namespace components {
namespace policy {

using namespace ::policy;

class UpdateStatusManagerTest : public ::testing::Test {
 protected:
  UpdateStatusManager* manager_;
  PolicyTableStatus status_;
  const uint32_t k_timeout_;

 public:
  UpdateStatusManagerTest() : k_timeout_(1) {}

  void SetUp() { manager_ = new UpdateStatusManager(); }

  void TearDown() { delete manager_; }
};

TEST_F(UpdateStatusManagerTest,
       OnUpdateSentOut_WaitForTimeoutExpired_ExpectStatusUpdateNeeded) {
  // Arrange
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
  manager_->OnUpdateTimeoutOccurs();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnValidUpdateReceived_SetValidUpdateReceived_ExpectStatusUpToDate) {
  // Arrange
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->OnValidUpdateReceived();
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpToDate, status_);
}

TEST_F(UpdateStatusManagerTest,
       OnWrongUpdateReceived_SetWrongUpdateReceived_ExpectStatusUpdateNeeded) {
  // Arrange
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
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  manager_->OnNewApplicationAdded();
  status_ = manager_->GetLastUpdateStatus();
  // Checks
  EXPECT_EQ(StatusUpdatePending, status_);
  EXPECT_TRUE(manager_->IsUpdatePending());
  EXPECT_TRUE(manager_->IsUpdateRequired());
}

TEST_F(UpdateStatusManagerTest, ScheduleUpdate_ExpectStatusUpdateNeeded) {
  // Arrange
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
       ResetUpdateSchedule_SetUpdateScheduleThenReset_ExpectStatusUpToDate) {
  // Arrange
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpToDate, status_);
  EXPECT_FALSE(manager_->IsUpdatePending());
  EXPECT_FALSE(manager_->IsUpdateRequired());
  manager_->ScheduleUpdate();
  // Check
  EXPECT_TRUE(manager_->IsUpdateRequired());
  // Act
  manager_->OnPolicyInit(false);
  // Check
  EXPECT_TRUE(manager_->IsUpdateRequired());
  // Act
  manager_->ResetUpdateSchedule();
  // Check
  EXPECT_FALSE(manager_->IsUpdateRequired());
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpToDate, status_);
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
