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

#include "policy/update_status_manager.h"
#include "gtest/gtest.h"
#include "policy/mock_policy_listener.h"
#include "policy/mock_ptu_retry_handler.h"
#include "policy/policy_manager_impl.h"

#include "utils/conditional_variable.h"
#include "utils/test_async_waiter.h"

namespace test {
namespace components {
namespace policy_test {

using namespace ::policy;
using ::testing::_;
using testing::NiceMock;
using ::testing::Return;

class UpdateStatusManagerTest : public ::testing::Test {
 protected:
  std::shared_ptr<UpdateStatusManager> manager_;
  PolicyTableStatus status_;
  const uint32_t k_timeout_;
  NiceMock<MockPolicyListener> listener_;
  NiceMock<MockPTURetryHandler> mock_ptu_retry_handler_;
  const std::string up_to_date_status_;
  const std::string update_needed_status_;
  const std::string updating_status_;

 public:
  UpdateStatusManagerTest()
      : manager_(std::make_shared<UpdateStatusManager>())
      , k_timeout_(1000)
      , listener_()
      , mock_ptu_retry_handler_()
      , up_to_date_status_("UP_TO_DATE")
      , update_needed_status_("UPDATE_NEEDED")
      , updating_status_("UPDATING") {}

  void SetUp() OVERRIDE {
    manager_->set_listener(&listener_);
    ON_CALL(listener_, OnUpdateStatusChanged(_)).WillByDefault(Return());
    ON_CALL(listener_, ptu_retry_handler())
        .WillByDefault(ReturnRef(mock_ptu_retry_handler_));
  }

  void TearDown() OVERRIDE {}
};

ACTION_P2(RetryFailed, manager, listener) {
  manager->OnResetRetrySequence();
  listener->OnPTUFinished(false);
}

TEST_F(UpdateStatusManagerTest,
       OnUpdateSentOut_WaitForTimeoutExpired_ExpectStatusUpdateNeeded) {
  // Arrange
  const uint32_t count = 3u;
  const uint32_t timeout = 2u * k_timeout_;
  auto waiter = TestAsyncWaiter::createInstance();

  EXPECT_CALL(listener_, OnUpdateStatusChanged(_))
      .WillRepeatedly(NotifyTestAsyncWaiter(waiter));
  manager_->ScheduleUpdate();
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  EXPECT_EQ(StatusUpdatePending, status_);
  EXPECT_TRUE(waiter->WaitFor(count, timeout));
  status_ = manager_->GetLastUpdateStatus();
  // Check
  EXPECT_EQ(StatusUpdateRequired, status_);
}

TEST_F(
    UpdateStatusManagerTest,
    OnUpdateSentOut_WaitForTimeoutExpired_ExpectStatusUpdateNeeded_RetryExceeded) {
  sync_primitives::Lock lock;
  sync_primitives::AutoLock auto_lock(lock);
  const uint32_t count = 3u;
  const uint32_t timeout = 2u * k_timeout_;
  auto waiter = TestAsyncWaiter::createInstance();
  EXPECT_CALL(listener_, OnUpdateStatusChanged(_))
      .WillRepeatedly(NotifyTestAsyncWaiter(waiter));
  EXPECT_CALL(mock_ptu_retry_handler_, RetrySequenceFailed())
      .WillOnce(RetryFailed(manager_, &listener_));
  manager_->ScheduleUpdate();
  manager_->OnUpdateSentOut(k_timeout_);
  status_ = manager_->GetLastUpdateStatus();
  {
    ::testing::InSequence s;
    EXPECT_CALL(mock_ptu_retry_handler_, IsAllowedRetryCountExceeded())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(mock_ptu_retry_handler_, IsAllowedRetryCountExceeded())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(mock_ptu_retry_handler_, IsAllowedRetryCountExceeded())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(listener_, OnPTUFinished(false));
  }
  EXPECT_EQ(StatusUpdatePending, status_);
  EXPECT_TRUE(waiter->WaitFor(count, timeout));
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
  status_ = manager_->GetLastUpdateStatus();
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
  EXPECT_EQ(StatusUpdateRequired, status_);
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

TEST_F(UpdateStatusManagerTest, OnAppRegistered) {
  manager_->ScheduleUpdate();
  ASSERT_EQ(policy::kUpdateNeeded, manager_->StringifiedUpdateStatus());
  manager_->ProcessEvent(policy::UpdateEvent::kOnNewAppRegistered);
  EXPECT_EQ(policy::kUpdateNeeded, manager_->StringifiedUpdateStatus());
}
}  // namespace policy_test
}  // namespace components
}  // namespace test
