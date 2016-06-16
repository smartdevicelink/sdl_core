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
#include <stdint.h>
#include <set>
#include <memory>

#include "gtest/gtest.h"

#include "application_manager/application_manager_impl.h"
#include "application_manager/mock_application_manager_settings.h"
#include "policy/mock_policy_settings.h"

namespace am = application_manager;
namespace policy_test = test::components::policy_handler_test;
using ::testing::Return;
using ::testing::ReturnRef;

namespace test {
namespace components {
namespace application_manager_test {

namespace {
const std::string kHmiCapabilitiesFile("hmi_capabilities.json");
const uint32_t kThreadPoolSize = 1;
const bool kHmiLaunch = false;
const uint32_t kStopStreamingTimeout = 10000;
static uint32_t app_id;
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 public:
  virtual void SetUp() OVERRIDE {
    app_manager_impl.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings, mock_policy_settings));

    app_id = 10;
  }

  virtual void TearDown() OVERRIDE {}

  MockApplicationManagerSettings mock_application_manager_settings;
  policy_test::MockPolicySettings mock_policy_settings;

  am::ApplicationManagerImpl* get_app_manager() {
    return app_manager_impl.get();
  }

 private:
  std::auto_ptr<am::ApplicationManagerImpl> app_manager_impl;
};

TEST_F(ApplicationManagerImplTest,
       SubscribeAppForWayPoints_ExpectSubscriptionApp) {
  get_app_manager()->SubscribeAppForWayPoints(app_id);
  EXPECT_TRUE(get_app_manager()->IsAppSubscribedForWayPoints(app_id));
}

TEST_F(ApplicationManagerImplTest,
       UnsubscribeAppForWayPoints_ExpectUnsubscriptionApp) {
  get_app_manager()->SubscribeAppForWayPoints(app_id);
  EXPECT_TRUE(get_app_manager()->IsAppSubscribedForWayPoints(app_id));
  get_app_manager()->UnsubscribeAppFromWayPoints(app_id);
  EXPECT_FALSE(get_app_manager()->IsAppSubscribedForWayPoints(app_id));
  std::set<int32_t> result = get_app_manager()->GetAppsSubscribedForWayPoints();
  EXPECT_TRUE(result.empty());
}

TEST_F(
    ApplicationManagerImplTest,
    UnsubscribeSubscribeApp_ExpectCorrectResultIsAnyAppSubscribedForWayPoints) {
  EXPECT_FALSE(get_app_manager()->IsAnyAppSubscribedForWayPoints());
  get_app_manager()->SubscribeAppForWayPoints(app_id);
  EXPECT_TRUE(get_app_manager()->IsAnyAppSubscribedForWayPoints());
}

TEST_F(ApplicationManagerImplTest,
       SubscribeApp_ExpectCorrectResultGetAppsSubscribedForWayPoints) {
  get_app_manager()->SubscribeAppForWayPoints(app_id);
  std::set<int32_t> result = get_app_manager()->GetAppsSubscribedForWayPoints();
  EXPECT_TRUE(result.size() == 1);
  EXPECT_TRUE(result.find(app_id) != result.end());
}

}  // application_manager_test
}  // namespace components
}  // namespace test
