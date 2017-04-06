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

#include <stdint.h>
#include "gtest/gtest.h"

#include "application_manager/application_manager_impl.h"
#include "application_manager/mock_resumption_data.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/test/include/application_manager/mock_message_helper.h"
#include "policy/mock_policy_settings.h"
#include "protocol_handler/mock_session_observer.h"
#include "connection_handler/mock_connection_handler.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace am = application_manager;
namespace am_test = application_manager_test;
namespace con_test = connection_handler_test;
namespace policy_test = test::components::policy_handler_test;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgPointee;

using ::test::components::application_manager_test::MockApplication;
using ::test::components::resumption_test::MockResumptionData;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kThreadPoolSize = 1u;
const uint32_t kDefaultTimeout = 100u;
const uint32_t kStopStreamingTimeout = 1000u;
const std::string kDirectoryName = "./test_storage";
const std::string kConnectionType = "BLUETOOTH";
const int32_t kInvalidTransportType =
    static_cast<int32_t>(hmi_apis::Common_TransportType::INVALID_ENUM);
const int32_t kValidTransportType =
    static_cast<int32_t>(hmi_apis::Common_TransportType::BLUETOOTH);
typedef utils::SharedPtr<NiceMock<MockApplication> > MockAppPtr;
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 public:
  ApplicationManagerImplTest()
      : mock_storage_(
            utils::MakeShared<NiceMock<resumption_test::MockResumptionData> >(
                app_mngr_))
      , mock_message_helper_(am::MockMessageHelper::message_helper_mock()) {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    CreateAppManager();

    ON_CALL(mock_connection_handler_, GetDataOnSessionKey(_, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(kAppId), Return(0)));
    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer_));

    app_manager_impl_->resume_controller().set_resumption_storage(
        mock_storage_);
    app_manager_impl_->set_connection_handler(&mock_connection_handler_);
  }
  ~ApplicationManagerImplTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

 protected:
  void CreateAppManager() {
    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(kThreadPoolSize));
    ON_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(true));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(kStopStreamingTimeout));
    ON_CALL(mock_application_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(kDefaultTimeout));
    app_manager_impl_.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings_, mock_policy_settings_));

    ASSERT_TRUE(app_manager_impl_.get());
  }

  MockAppPtr InsertBasicApp(const uint32_t app_id) {
    MockAppPtr mock_app = utils::MakeShared<NiceMock<MockApplication> >();
    ON_CALL(*mock_app, app_id()).WillByDefault(Return(app_id));

    return mock_app;
  }

  MockAppPtr InsertNaviApp(const uint32_t app_id) {
    MockAppPtr mock_app = InsertBasicApp(app_id);
    ON_CALL(*mock_app, is_navi()).WillByDefault(Return(true));

    return mock_app;
  }

  NiceMock<policy_test::MockPolicySettings> mock_policy_settings_;
  utils::SharedPtr<NiceMock<resumption_test::MockResumptionData> >
      mock_storage_;
  NiceMock<con_test::MockConnectionHandler> mock_connection_handler_;
  NiceMock<protocol_handler_test::MockSessionObserver> mock_session_observer_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  am_test::MockApplicationManager app_mngr_;
  std::auto_ptr<am::ApplicationManagerImpl> app_manager_impl_;
  am::MockMessageHelper* mock_message_helper_;
};

TEST_F(ApplicationManagerImplTest, ProcessQueryApp_ExpectSuccess) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject app_data;
  const uint32_t connection_key = 65537u;

  app_data[am::json::name] = "application_manager_test";
  app_data[am::json::appId] = kAppId;
  app_data[am::json::android] = "bucket";
  app_data[am::json::android][am::json::packageName] = "com.android.test";
  smart_objects::SmartObject sm_object(SmartType_Map);
  sm_object[am::json::response][0] = app_data;
  SmartObjectSPtr sptr = utils::MakeShared<SmartObject>(sm_object);

  ON_CALL(*mock_message_helper_, CreateModuleInfoSO(_, _))
      .WillByDefault(Return(sptr));
  ON_CALL(*mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillByDefault(Return(sptr));
  app_manager_impl_->ProcessQueryApp(sm_object, connection_key);
}

TEST_F(ApplicationManagerImplTest,
       SubscribeAppForWayPoints_ExpectSubscriptionApp) {
  app_manager_impl_->SubscribeAppForWayPoints(kAppId);
  EXPECT_TRUE(app_manager_impl_->IsAppSubscribedForWayPoints(kAppId));
}

TEST_F(ApplicationManagerImplTest,
       UnsubscribeAppForWayPoints_ExpectUnsubscriptionApp) {
  app_manager_impl_->SubscribeAppForWayPoints(kAppId);
  EXPECT_TRUE(app_manager_impl_->IsAppSubscribedForWayPoints(kAppId));
  app_manager_impl_->UnsubscribeAppFromWayPoints(kAppId);
  EXPECT_FALSE(app_manager_impl_->IsAppSubscribedForWayPoints(kAppId));
  const std::set<int32_t> result =
      app_manager_impl_->GetAppsSubscribedForWayPoints();
  EXPECT_TRUE(result.empty());
}

TEST_F(
    ApplicationManagerImplTest,
    IsAnyAppSubscribedForWayPoints_SubcribeAppForWayPoints_ExpectCorrectResult) {
  EXPECT_FALSE(app_manager_impl_->IsAnyAppSubscribedForWayPoints());
  app_manager_impl_->SubscribeAppForWayPoints(kAppId);
  EXPECT_TRUE(app_manager_impl_->IsAnyAppSubscribedForWayPoints());
}

TEST_F(
    ApplicationManagerImplTest,
    GetAppsSubscribedForWayPoints_SubcribeAppForWayPoints_ExpectCorrectResult) {
  app_manager_impl_->SubscribeAppForWayPoints(kAppId);
  std::set<int32_t> result = app_manager_impl_->GetAppsSubscribedForWayPoints();
  EXPECT_EQ(1u, result.size());
  EXPECT_TRUE(result.find(kAppId) != result.end());
}

TEST_F(ApplicationManagerImplTest,
       GetDeviceTransportType_ValidConnectionType_ValidTransportType) {
  const std::string connection_type = kConnectionType;
  const int32_t transport_type = kValidTransportType;
  EXPECT_EQ(transport_type,
            app_manager_impl_->GetDeviceTransportType(connection_type));
}

TEST_F(ApplicationManagerImplTest,
       GetDeviceTransportType_InvalidConnectionType_InvalidEnum) {
  const std::string connection_type = "Invalid";
  const int32_t transport_type = kInvalidTransportType;
  EXPECT_EQ(transport_type,
            app_manager_impl_->GetDeviceTransportType(connection_type));
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_Full_NaviApp_StreamingAllowed_Success) {
  InsertNaviApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(
      kAppId, mobile_apis::HMILevel::HMI_NONE, mobile_apis::HMILevel::HMI_FULL);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_Limited_NaviApp_StreamingAllowed_Success) {
  InsertNaviApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(kAppId,
                                       mobile_apis::HMILevel::HMI_NONE,
                                       mobile_apis::HMILevel::HMI_LIMITED);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_Background_NaviApp_StreamingDisallowed_Success) {
  InsertNaviApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(kAppId,
                                       mobile_apis::HMILevel::HMI_NONE,
                                       mobile_apis::HMILevel::HMI_BACKGROUND);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_None_NaviApp_StreamingForbidden_Success) {
  InsertNaviApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(
      kAppId, mobile_apis::HMILevel::HMI_FULL, mobile_apis::HMILevel::HMI_NONE);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_Full_NotNaviApp_StreamingAllowed_Fail) {
  InsertBasicApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(
      kAppId, mobile_apis::HMILevel::HMI_NONE, mobile_apis::HMILevel::HMI_FULL);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_Limited_NotNaviApp_StreamingAllowed_Fail) {
  InsertBasicApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(kAppId,
                                       mobile_apis::HMILevel::HMI_NONE,
                                       mobile_apis::HMILevel::HMI_LIMITED);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_Background_NotNaviApp_StreamingDisallowed_Fail) {
  InsertBasicApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(kAppId,
                                       mobile_apis::HMILevel::HMI_NONE,
                                       mobile_apis::HMILevel::HMI_BACKGROUND);
}

TEST_F(ApplicationManagerImplTest,
       OnHmiLevelChanged_None_NotNaviApp_StreamingForbidden_Fail) {
  InsertBasicApp(kAppId);
  app_manager_impl_->OnHMILevelChanged(
      kAppId, mobile_apis::HMILevel::HMI_FULL, mobile_apis::HMILevel::HMI_NONE);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
