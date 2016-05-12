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

#include "utils/lock.h"
#include "utils/file_system.h"
#include "utils/make_shared.h"
#include "utils/custom_string.h"
#include "policy/mock_policy_settings.h"
#include "application_manager/application.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/mock_resumption_data.h"
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/mock_application_manager_settings.h"
#include "hmi_message_handler/mock_hmi_message_handler.h"
#include "connection_handler/mock_connection_handler.h"
#include "protocol_handler/mock_session_observer.h"
#include "policy/usage_statistics/mock_statistics_manager.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace am = application_manager;
namespace policy_test = test::components::policy_handler_test;
namespace con_test = connection_handler_test;

using testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::SetArgReferee;
using ::testing::SetArgPointee;
using ::testing::Truly;
using ::testing::AtLeast;
using ::testing::DoAll;
using usage_statistics_test::MockStatisticsManager;

namespace {
const std::string kHmiCapabilitiesFile("hmi_capabilities.json");
const uint32_t kThreadPoolSize = 1;
const bool kHmiLaunch = false;
const uint32_t kStopStreamingTimeout = 10000;
const uint32_t kWaitFreeQueue = 2000;
const std::string directory_name = "./test_storage";

bool is_called_ = false;
uint32_t hmi_app_id_ = 0u;
sync_primitives::Lock state_lock_;
sync_primitives::ConditionalVariable state_condition_;
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    app_id_ = 10;

    CreateAppManager();

    const std::string kMacAddr_("kMacAddr_ess");

    ON_CALL(mock_session_observer, GetDataOnDeviceID(_, _, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(kMacAddr_), Return(0)));
    ON_CALL(mock_connection_handler, GetDataOnSessionKey(_, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(app_id_), Return(0)));
    ON_CALL(mock_connection_handler, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer));

    mock_storage =
        ::utils::MakeShared<NiceMock<resumption_test::MockResumptionData>>(
            mock_application_manager_settings_);

    app_manager_impl_->resume_controller().set_resumption_storage(mock_storage);
    app_manager_impl_->set_connection_handler(&mock_connection_handler);
    app_manager_impl_->set_hmi_message_handler(&mock_hmi_mess_handler);
  }

  void CreateAppManager() {
    const uint32_t timeout = 10000u;
    const std::string folder_name = "app";
    const std::string test_path =
        file_system::ConcatPath(directory_name, folder_name);
    const std::string file1 =
        file_system::ConcatPath(test_path, "test_file2.bmp");

    ASSERT_TRUE(file_system::CreateDirectoryRecursively(test_path));
    ASSERT_TRUE(file_system::CreateFile(file1));

    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(2u));
    ON_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(directory_name));
    ON_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(directory_name));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(true));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(1u));
    ON_CALL(mock_application_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(timeout));
    app_manager_impl_.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings_, mock_policy_settings_));

    ASSERT_TRUE(app_manager_impl_.get());
  }

  uint32_t app_id_;
  NiceMock<policy_test::MockPolicySettings> mock_policy_settings_;
  utils::SharedPtr<NiceMock<resumption_test::MockResumptionData>> mock_storage;
  NiceMock<con_test::MockConnectionHandler> mock_connection_handler;
  NiceMock<protocol_handler_test::MockSessionObserver> mock_session_observer;
  NiceMock<hmi_message_handler_test::MockHMIMessageHandler>
      mock_hmi_mess_handler;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  std::auto_ptr<am::ApplicationManagerImpl> app_manager_impl_;
};

struct AppIdExtractor {
  AppIdExtractor() {
    hmi_app_id_ = 5u;
    is_called_ = false;
  }

  bool operator()(
      const hmi_message_handler::MessageSharedPointer& message) const {
    using namespace application_manager::strings;
    sync_primitives::AutoLock auto_lock(state_lock_);
    bool retVal = false;
    Json::Reader reader;
    Json::Value json_value;
    const std::string str = message->json_message();
    if (reader.parse(str, json_value, false)) {
      hmi_app_id_ = json_value[params][applications][0][app_id].asUInt();
      retVal = true;
    }
    is_called_ = true;
    state_condition_.Broadcast();
    return retVal;
  }
};

TEST_F(ApplicationManagerImplTest,
       FindAppToRegister_SearchValidHmiId_ExpectSuccess) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject app_data;
  const uint32_t connection_key = 65537u;
  const uint32_t app_id = 8975689u;
  app_data[am::json::name] = "application_manager_test";
  app_data[am::json::appId] = app_id;
  app_data[am::json::android] = "bucket";
  app_data[am::json::android][am::json::packageName] = "com.android.test";
  smart_objects::SmartObject sm_object(SmartType_Map);
  sm_object[am::json::response][0] = app_data;

  EXPECT_CALL(mock_hmi_mess_handler, SendMessageToHMI(Truly(AppIdExtractor())))
      .Times(AtLeast(1u));

  app_manager_impl_->ProcessQueryApp(sm_object, connection_key);

  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    while (!is_called_) {
      ASSERT_NE(sync_primitives::ConditionalVariable::kTimeout,
                state_condition_.WaitFor(auto_lock, kWaitFreeQueue));
    }
  }

  EXPECT_CALL(mock_hmi_mess_handler, SendMessageToHMI(_)).Times(AtLeast(0));
  EXPECT_TRUE(app_manager_impl_->FindAppToRegister(hmi_app_id_).valid());
}

TEST_F(ApplicationManagerImplTest,
       FindAppToRegister_SearchInvalidHmiId_ExpectFail) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject app_data;
  const uint32_t connection_key = 65537u;
  const uint32_t app_id = 8975689u;
  app_data[am::json::name] = "application_manager_test";
  app_data[am::json::appId] = app_id;
  app_data[am::json::android] = "bucket";
  app_data[am::json::android][am::json::packageName] = "com.android.test";
  smart_objects::SmartObject sm_object(SmartType_Map);
  sm_object[am::json::response][0] = app_data;

  EXPECT_CALL(mock_hmi_mess_handler, SendMessageToHMI(Truly(AppIdExtractor())))
      .Times(AtLeast(1u));

  app_manager_impl_->ProcessQueryApp(sm_object, connection_key);

  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    while (!is_called_) {
      ASSERT_NE(sync_primitives::ConditionalVariable::kTimeout,
                state_condition_.WaitFor(auto_lock, kWaitFreeQueue));
    }
  }

  EXPECT_CALL(mock_hmi_mess_handler, SendMessageToHMI(_)).Times(AtLeast(0));
  hmi_app_id_ += 100;  // change hmi_id
  EXPECT_FALSE(app_manager_impl_->FindAppToRegister(hmi_app_id_).valid());
}

TEST_F(ApplicationManagerImplTest,
       SubscribeAppForWayPoints_ExpectSubscriptionApp) {
  app_manager_impl_->SubscribeAppForWayPoints(app_id_);
  EXPECT_TRUE(app_manager_impl_->IsAppSubscribedForWayPoints(app_id_));
}

TEST_F(ApplicationManagerImplTest,
       UnsubscribeAppForWayPoints_ExpectUnsubscriptionApp) {
  app_manager_impl_->SubscribeAppForWayPoints(app_id_);
  EXPECT_TRUE(app_manager_impl_->IsAppSubscribedForWayPoints(app_id_));
  app_manager_impl_->UnsubscribeAppFromWayPoints(app_id_);
  EXPECT_FALSE(app_manager_impl_->IsAppSubscribedForWayPoints(app_id_));
  const std::set<int32_t> result =
      app_manager_impl_->GetAppsSubscribedForWayPoints();
  EXPECT_TRUE(result.empty());
}

TEST_F(
    ApplicationManagerImplTest,
    IsAnyAppSubscribedForWayPoints_SubcribeAppForWayPoints_ExpectCorrectResult) {
  EXPECT_FALSE(app_manager_impl_->IsAnyAppSubscribedForWayPoints());
  app_manager_impl_->SubscribeAppForWayPoints(app_id_);
  EXPECT_TRUE(app_manager_impl_->IsAnyAppSubscribedForWayPoints());
}

TEST_F(
    ApplicationManagerImplTest,
    GetAppsSubscribedForWayPoints_SubcribeAppForWayPoints_ExpectCorrectResult) {
  app_manager_impl_->SubscribeAppForWayPoints(app_id_);
  std::set<int32_t> result = app_manager_impl_->GetAppsSubscribedForWayPoints();
  EXPECT_EQ(1u, result.size());
  EXPECT_TRUE(result.find(app_id_) != result.end());
}

}  // application_manager_test
}  // namespace components
}  // namespace test
