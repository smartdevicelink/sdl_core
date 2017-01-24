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
#include "utils/push_log.h"
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
#include "application_manager/test/include/application_manager/mock_message_helper.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace am = application_manager;
namespace policy_test = test::components::policy_handler_test;
namespace con_test = connection_handler_test;

using testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::SetArgReferee;
using ::testing::SetArgPointee;
using ::testing::Truly;
using ::testing::AtLeast;
using ::testing::DoAll;
using usage_statistics_test::MockStatisticsManager;
using namespace application_manager;

namespace {
const std::string kHmiCapabilitiesFile("hmi_capabilities.json");
const uint32_t kThreadPoolSize = 1;
const bool kHmiLaunch = false;
const uint32_t kStopStreamingTimeout = 10000;
const uint32_t kWaitFreeQueue = 2000;
const std::string kDirectoryName = "./test_storage";
const uint32_t kTimeout = 10000u;
const std::string kFolderName = "app";
const std::string kTestPath =
    file_system::ConcatPath(kDirectoryName, kFolderName);
const std::string kTestFileName =
    file_system::ConcatPath(kTestPath, "test_file2.bmp");
const std::string kMacAddr_("kMacAddr_ess");

bool is_called_ = false;
uint32_t hmi_app_id_ = 0u;
sync_primitives::Lock state_lock_;
sync_primitives::ConditionalVariable state_condition_;
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 public:
  ApplicationManagerImplTest()
      : mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock())
  // message_helper_(new MessageHelper::MessageHelper())
  {}

 protected:
  void SetUp() OVERRIDE {
    app_id_ = 10;

    Mock::VerifyAndClearExpectations(&mock_message_helper_);

    CreateAppManager();

    ON_CALL(mock_session_observer_, GetDataOnDeviceID(_, _, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(kMacAddr_), Return(0)));
    ON_CALL(mock_connection_handler_, GetDataOnSessionKey(_, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(app_id_), Return(0)));
    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer_));

    mock_storage_ =
        ::utils::MakeShared<NiceMock<resumption_test::MockResumptionData> >(
            app_mngr_);
    // app_mock = utils::MakeShared<NiceMock<MockApplication> >();
    // res_ctrl = utils::MakeShared<ResumeCtrlImpl>(app_mngr_);
    // res_ctrl->set_resumption_storage(mock_storage);

    logger::create_log_message_loop_thread();

    app_manager_impl_->resume_controller().set_resumption_storage(
        mock_storage_);
    app_manager_impl_->set_connection_handler(&mock_connection_handler_);
    app_manager_impl_->set_hmi_message_handler(&mock_hmi_msg_handler_);
  }

  void CreateAppManager() {
    ASSERT_TRUE(file_system::CreateDirectoryRecursively(kTestPath));
    ASSERT_TRUE(file_system::CreateFile(kTestFileName));

    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(2u));
    ON_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(true));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(1u));
    ON_CALL(mock_application_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(kTimeout));
    app_manager_impl_.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings_, mock_policy_settings_));

    ASSERT_TRUE(app_manager_impl_.get());
  }

  uint32_t app_id_;
  NiceMock<policy_test::MockPolicySettings> mock_policy_settings_;
  utils::SharedPtr<NiceMock<resumption_test::MockResumptionData> >
      mock_storage_;
  NiceMock<con_test::MockConnectionHandler> mock_connection_handler_;
  NiceMock<protocol_handler_test::MockSessionObserver> mock_session_observer_;
  NiceMock<hmi_message_handler_test::MockHMIMessageHandler>
      mock_hmi_msg_handler_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  application_manager_test::MockApplicationManager app_mngr_;
  std::auto_ptr<am::ApplicationManagerImpl> app_manager_impl_;
  application_manager::MockMessageHelper* mock_message_helper_;
  application_manager::MessageHelper* message_helper_;
};

struct AppIdExtractor {
  AppIdExtractor() {
    hmi_app_id_ = 0u;
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

TEST_F(ApplicationManagerImplTest, ProcessQueryApp_ExpectSuccess) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject app_data;
  const uint32_t connection_key = 65537u;
  const uint32_t app_id = 0u;
  app_data[am::json::name] = "application_manager_test";
  app_data[am::json::appId] = app_id;
  app_data[am::json::android] = "bucket";
  app_data[am::json::android][am::json::packageName] = "com.android.test";
  smart_objects::SmartObject sm_object(SmartType_Map);
  sm_object[am::json::response][0] = app_data;

  EXPECT_CALL(mock_hmi_msg_handler_, SendMessageToHMI(Truly(AppIdExtractor())))
      .Times(AtLeast(1u));

  smart_objects::SmartObjectSPtr module_info =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *module_info;
  object[strings::params][strings::message_type] = static_cast<int>(kRequest);
  object[strings::params][strings::function_id] = static_cast<int>(1);
  object[strings::params][strings::correlation_id] =
      app_manager_impl_
          ->GetNextHMICorrelationID();  // GetNextHMICorrelationID();
  object[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  ON_CALL(*mock_message_helper_, CreateModuleInfoSO(_, _))
      .WillByDefault(Return(module_info));

  ON_CALL(*mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillByDefault(Return(module_info));

  app_manager_impl_->ProcessQueryApp(sm_object, connection_key);
  // Can't test that functionality ofr the moment.
  // EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(AtLeast(1));
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
