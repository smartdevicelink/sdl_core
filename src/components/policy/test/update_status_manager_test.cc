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

  void SetUp() {
    manager_ = new UpdateStatusManager();
  }

  void TearDown() OVERRIDE {
    delete manager_;
  }
};

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
