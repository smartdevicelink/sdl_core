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
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <bson_object.h>

#include "gtest/gtest.h"
#include "application_manager/application.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_resumption_data.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/mock_rpc_plugin_manager.h"
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/test/include/application_manager/mock_message_helper.h"
#include "connection_handler/mock_connection_handler.h"
#include "hmi_message_handler/mock_hmi_message_handler.h"
#include "media_manager/mock_media_manager.h"
#include "policy/mock_policy_settings.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol/bson_object_keys.h"
#include "protocol_handler/mock_session_observer.h"
#include "protocol_handler/mock_protocol_handler.h"
#include "utils/custom_string.h"
#include "utils/file_system.h"
#include "utils/lock.h"

#include "utils/push_log.h"
#include "encryption/hashing.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace am = application_manager;
namespace policy_test = test::components::policy_handler_test;
namespace con_test = connection_handler_test;

using testing::_;
using ::testing::An;
using ::testing::Matcher;
using ::testing::ByRef;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::SaveArg;
using ::testing::SetArgPointee;

using namespace application_manager;

// custom action to call a member function with 4 arguments
ACTION_P6(InvokeMemberFuncWithArg4, ptr, memberFunc, a, b, c, d) {
  (ptr->*memberFunc)(a, b, c, d);
}

namespace {
const std::string kDirectoryName = "./test_storage";
const uint32_t kTimeout = 10000u;
connection_handler::DeviceHandle kDeviceId = 12345u;
const std::string kAppId = "someID";
const uint32_t kConnectionKey = 1232u;
const std::string kAppName = "appName";

// Cloud application params
const std::string kEndpoint = "endpoint";
const std::string kAuthToken = "auth_token";
const std::string kCertificate = "cert";
const std::string kTransportType = "WS";
const mobile_api::HybridAppPreference::eType kHybridAppPreference =
    mobile_api::HybridAppPreference::CLOUD;
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 public:
  ApplicationManagerImplTest()
      : app_id_(0u)
      , mock_storage_(
            std::make_shared<NiceMock<resumption_test::MockResumptionData> >(
                mock_app_mngr_))
      , mock_rpc_service_(new MockRPCService)
      , mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock())

  {
#ifdef ENABLE_LOG
    logger::create_log_message_loop_thread();
#endif
    Mock::VerifyAndClearExpectations(mock_message_helper_);
  }
  ~ApplicationManagerImplTest() {
    Mock::VerifyAndClearExpectations(mock_message_helper_);
  }

 protected:
  void SetUp() OVERRIDE {
    CreateAppManager();
    ON_CALL(mock_session_observer_, GetDataOnSessionKey(_, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(kDeviceId), Return(0)));
    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer_));
    app_manager_impl_->SetMockRPCService(mock_rpc_service_);
    app_manager_impl_->resume_controller().set_resumption_storage(
        mock_storage_);
    app_manager_impl_->set_connection_handler(&mock_connection_handler_);
    std::unique_ptr<AppServiceManager> app_service_manager_ptr(
        new AppServiceManager(*app_manager_impl_));
    app_manager_impl_->SetAppServiceManager(app_service_manager_ptr);
  }

  void CreateAppManager() {
    const uint8_t expected_tread_pool_size = 2u;
    const uint8_t stop_streaming_timeout = 1u;

    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(expected_tread_pool_size));
    ON_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(false));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(stop_streaming_timeout));
    ON_CALL(mock_application_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(kTimeout));
    ON_CALL(mock_application_manager_settings_,
            application_list_update_timeout()).WillByDefault(Return(kTimeout));

    app_manager_impl_.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings_, mock_policy_settings_));
    mock_app_ptr_ = std::shared_ptr<MockApplication>(new MockApplication());
    app_manager_impl_->set_protocol_handler(&mock_protocol_handler_);
    ASSERT_TRUE(app_manager_impl_.get());
    ASSERT_TRUE(mock_app_ptr_.get());
  }

  void AddMockApplication() {
    app_manager_impl_->AddMockApplication(mock_app_ptr_);
  }

  void SetCommonExpectationOnAppReconnection(
      const connection_handler::DeviceHandle new_device_id,
      const uint32_t new_application_id,
      const std::string& mac_address) {
    EXPECT_CALL(
        mock_session_observer_,
        GetDataOnSessionKey(new_application_id,
                            _,
                            _,
                            testing::An<connection_handler::DeviceHandle*>()))
        .WillOnce(DoAll(SetArgPointee<3u>(new_device_id), Return(0)));

    const std::string connection_type = "MyConnectionType";
    EXPECT_CALL(
        mock_session_observer_,
        GetDataOnDeviceID(
            ::testing::Matcher<connection_handler::DeviceHandle>(new_device_id),
            _,
            _,
            _,
            _))
        .WillOnce(DoAll(SetArgPointee<3u>(mac_address),
                        SetArgPointee<4u>(connection_type),
                        Return(0)));
  }

  bool CheckResumptionRequiredTransportAvailableTest(
      smart_objects::SmartObject* app_types_array,
      connection_handler::DeviceHandle primary_device_handle,
      std::string primary_transport_device_string,
      connection_handler::DeviceHandle secondary_device_handle,
      std::string secondary_transport_device_string);

  uint32_t app_id_;
  NiceMock<policy_test::MockPolicySettings> mock_policy_settings_;
  std::shared_ptr<NiceMock<resumption_test::MockResumptionData> > mock_storage_;

  MockRPCService* mock_rpc_service_;
  NiceMock<con_test::MockConnectionHandler> mock_connection_handler_;
  NiceMock<protocol_handler_test::MockSessionObserver> mock_session_observer_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  application_manager_test::MockApplicationManager mock_app_mngr_;
  std::unique_ptr<am::ApplicationManagerImpl> app_manager_impl_;
  application_manager::MockMessageHelper* mock_message_helper_;

  std::shared_ptr<MockApplication> mock_app_ptr_;
  NiceMock<protocol_handler_test::MockProtocolHandler> mock_protocol_handler_;
};

TEST_F(ApplicationManagerImplTest, ProcessQueryApp_ExpectSuccess) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject app_data;
  const uint32_t connection_key = 65537u;
  app_data[am::json::name] = "application_manager_test";
  app_data[am::json::appId] = app_id_;
  app_data[am::json::android] = "bucket";
  app_data[am::json::android][am::json::packageName] = "com.android.test";
  smart_objects::SmartObject sm_object(SmartType_Map);
  sm_object[am::json::response][0] = app_data;
  SmartObjectSPtr sptr = std::make_shared<SmartObject>(sm_object);

  ON_CALL(*mock_message_helper_, CreateModuleInfoSO(_, _))
      .WillByDefault(Return(sptr));
  ON_CALL(*mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillByDefault(Return(sptr));
  app_manager_impl_->ProcessQueryApp(sm_object, connection_key);
}

TEST_F(ApplicationManagerImplTest,
       SubscribeAppForWayPoints_ExpectSubscriptionApp) {
  auto app_ptr = std::static_pointer_cast<am::Application>(mock_app_ptr_);
  app_manager_impl_->SubscribeAppForWayPoints(app_ptr);
  EXPECT_TRUE(app_manager_impl_->IsAppSubscribedForWayPoints(app_ptr));
}

TEST_F(ApplicationManagerImplTest,
       UnsubscribeAppForWayPoints_ExpectUnsubscriptionApp) {
  auto app_ptr = std::static_pointer_cast<am::Application>(mock_app_ptr_);
  app_manager_impl_->SubscribeAppForWayPoints(app_ptr);
  EXPECT_TRUE(app_manager_impl_->IsAppSubscribedForWayPoints(app_ptr));
  app_manager_impl_->UnsubscribeAppFromWayPoints(app_ptr);
  EXPECT_FALSE(app_manager_impl_->IsAppSubscribedForWayPoints(app_ptr));
  const std::set<uint32_t> result =
      app_manager_impl_->GetAppsSubscribedForWayPoints();
  EXPECT_TRUE(result.empty());
}

TEST_F(
    ApplicationManagerImplTest,
    IsAnyAppSubscribedForWayPoints_SubcribeAppForWayPoints_ExpectCorrectResult) {
  EXPECT_FALSE(app_manager_impl_->IsAnyAppSubscribedForWayPoints());
  auto app_ptr = std::static_pointer_cast<am::Application>(mock_app_ptr_);
  app_manager_impl_->SubscribeAppForWayPoints(app_ptr);
  EXPECT_TRUE(app_manager_impl_->IsAnyAppSubscribedForWayPoints());
}

TEST_F(
    ApplicationManagerImplTest,
    GetAppsSubscribedForWayPoints_SubcribeAppForWayPoints_ExpectCorrectResult) {
  auto app_ptr = std::static_pointer_cast<am::Application>(mock_app_ptr_);
  app_manager_impl_->SubscribeAppForWayPoints(app_ptr);
  std::set<uint32_t> result =
      app_manager_impl_->GetAppsSubscribedForWayPoints();
  EXPECT_EQ(1u, result.size());
  EXPECT_TRUE(result.find(app_ptr->app_id()) != result.end());
}

TEST_F(ApplicationManagerImplTest, OnServiceStartedCallback_RpcService) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kRpc;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is true and list is empty
  EXPECT_TRUE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest, OnServiceStartedCallback_UnknownApp) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kInvalidServiceType;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(456));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is false and list is empty
  EXPECT_FALSE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest, OnServiceStartedCallback_UnknownService) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kInvalidServiceType;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is false and list is empty
  EXPECT_FALSE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest, OnServiceStartedCallback_VideoServiceStart) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kMobileNav;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  // check: SetVideoConfig() should not be called, StartStreaming() is called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(_, _)).Times(0);
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).WillOnce(Return());

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is true and list is empty
  EXPECT_TRUE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest,
       OnServiceStartedCallback_VideoServiceNotStart1) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kMobileNav;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  // is_navi() is false
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(false));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  // check: SetVideoConfig() and StartStreaming() should not be called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(_, _)).Times(0);
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).Times(0);

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is false and list is empty
  EXPECT_FALSE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest,
       OnServiceStartedCallback_VideoServiceNotStart2) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kMobileNav;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  // HMI level is not FULL nor LIMITED
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_BACKGROUND));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  // check: SetVideoConfig() and StartStreaming() should not be called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(_, _)).Times(0);
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).Times(0);

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is false and list is empty
  EXPECT_FALSE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest,
       OnServiceStartedCallback_VideoSetConfig_SUCCESS) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kMobileNav;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_LIMITED));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  BsonObject input_params;
  bson_object_initialize_default(&input_params);
  char protocol_version[] = "1.0.0";
  bson_object_put_string(&input_params,
                         protocol_handler::strings::protocol_version,
                         protocol_version);
  bson_object_put_int64(&input_params, protocol_handler::strings::mtu, 100);
  char protocol_name[] = "RTP";
  bson_object_put_string(
      &input_params, protocol_handler::strings::video_protocol, protocol_name);
  char codec_name[] = "VP9";
  bson_object_put_string(
      &input_params, protocol_handler::strings::video_codec, codec_name);
  bson_object_put_int32(&input_params, protocol_handler::strings::height, 640);
  bson_object_put_int32(&input_params, protocol_handler::strings::width, 480);

  smart_objects::SmartObject converted_params(smart_objects::SmartType_Map);
  converted_params[strings::protocol] =
      hmi_apis::Common_VideoStreamingProtocol::RTP;
  converted_params[strings::codec] = hmi_apis::Common_VideoStreamingCodec::VP9;
  converted_params[strings::height] = 640;
  converted_params[strings::width] = 480;

  std::vector<std::string> empty;

  // check: SetVideoConfig() and StartStreaming() are called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(service_type, converted_params))
      .WillOnce(DoAll(InvokeMemberFuncWithArg4(
                          app_manager_impl_.get(),
                          &ApplicationManagerImpl::OnStreamingConfigured,
                          session_key,
                          service_type,
                          true,
                          ByRef(empty)),
                      Return(true)));
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).WillOnce(Return());

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, &input_params);

  // check: return value is true and list is empty
  EXPECT_TRUE(result);
  EXPECT_TRUE(rejected_params.empty());
}

static bool ValidateList(std::vector<std::string>& expected,
                         std::vector<std::string>& actual) {
  if (expected.size() != actual.size()) {
    return false;
  }
  for (unsigned int i = 0; i < expected.size(); i++) {
    std::string& param = expected[i];
    unsigned int j;
    for (j = 0; j < actual.size(); j++) {
      if (param == actual[j]) {
        break;
      }
    }
    if (j == actual.size()) {
      // not found
      return false;
    }
  }
  return true;
}

TEST_F(ApplicationManagerImplTest,
       OnServiceStartedCallback_VideoSetConfig_FAILURE) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kMobileNav;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  BsonObject input_params;
  bson_object_initialize_default(&input_params);
  char protocol_version[] = "1.0.0";
  bson_object_put_string(&input_params,
                         protocol_handler::strings::protocol_version,
                         protocol_version);
  bson_object_put_int64(&input_params, protocol_handler::strings::mtu, 100);
  char protocol_name[] = "RTP";
  bson_object_put_string(
      &input_params, protocol_handler::strings::video_protocol, protocol_name);
  char codec_name[] = "VP9";
  bson_object_put_string(
      &input_params, protocol_handler::strings::video_codec, codec_name);
  bson_object_put_int32(&input_params, protocol_handler::strings::height, 640);
  bson_object_put_int32(&input_params, protocol_handler::strings::width, 480);

  smart_objects::SmartObject converted_params(smart_objects::SmartType_Map);
  converted_params[strings::protocol] =
      hmi_apis::Common_VideoStreamingProtocol::RTP;
  converted_params[strings::codec] = hmi_apis::Common_VideoStreamingCodec::VP9;
  converted_params[strings::height] = 640;
  converted_params[strings::width] = 480;

  std::vector<std::string> rejected_list;
  rejected_list.push_back(std::string("protocol"));
  rejected_list.push_back(std::string("codec"));

  // simulate HMI returning negative response
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(service_type, converted_params))
      .WillOnce(DoAll(InvokeMemberFuncWithArg4(
                          app_manager_impl_.get(),
                          &ApplicationManagerImpl::OnStreamingConfigured,
                          session_key,
                          service_type,
                          false,
                          ByRef(rejected_list)),
                      Return(true)));

  // check: StartStreaming() should not be called
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).Times(0);

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, &input_params);

  // check: return value is false
  EXPECT_FALSE(result);

  // check: rejected param list contains "videoProtocol" and "videoCodec"
  ASSERT_EQ(2u, rejected_params.size());
  std::vector<std::string> expected_list;
  expected_list.push_back(
      std::string(protocol_handler::strings::video_protocol));
  expected_list.push_back(std::string(protocol_handler::strings::video_codec));
  ASSERT_TRUE(ValidateList(expected_list, rejected_params));
}

TEST_F(ApplicationManagerImplTest,
       OnServiceStartedCallback_VideoServiceWithoutVideoParams) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kMobileNav;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  BsonObject input_params;
  bson_object_initialize_default(&input_params);
  char protocol_version[] = "1.0.0";
  bson_object_put_string(&input_params, "protocolVersion", protocol_version);
  bson_object_put_int64(&input_params, "mtu", 100);

  // check: SetVideoConfig() should not be called, StartStreaming() is called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(_, _)).Times(0);
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).WillOnce(Return());

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, &input_params);

  // check: return value is true and list is empty
  EXPECT_TRUE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest, OnServiceStartedCallback_AudioServiceStart) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kAudio;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  // check: SetVideoConfig() should not be called, StartStreaming() is called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(_, _)).Times(0);
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).WillOnce(Return());

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, NULL);

  // check: return value is true and list is empty
  EXPECT_TRUE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest,
       OnServiceStartedCallback_AudioServiceWithParams) {
  AddMockApplication();

  const connection_handler::DeviceHandle device_handle = 0;
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceType::kAudio;
  const int32_t session_key = 123;
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(session_key));
  EXPECT_CALL(*mock_app_ptr_, is_navi()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app_ptr_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  bool result = false;
  std::vector<std::string> rejected_params;
  EXPECT_CALL(mock_connection_handler_, NotifyServiceStartedResult(_, _, _))
      .WillOnce(DoAll(SaveArg<1>(&result), SaveArg<2>(&rejected_params)));

  BsonObject input_params;
  bson_object_initialize_default(&input_params);
  char protocol_version[] = "1.0.0";
  bson_object_put_string(&input_params,
                         protocol_handler::strings::protocol_version,
                         protocol_version);
  bson_object_put_int64(&input_params, protocol_handler::strings::mtu, 100);
  char protocol_name[] = "RTP";
  bson_object_put_string(
      &input_params, protocol_handler::strings::video_protocol, protocol_name);
  char codec_name[] = "VP9";
  bson_object_put_string(
      &input_params, protocol_handler::strings::video_codec, codec_name);
  bson_object_put_int32(&input_params, protocol_handler::strings::height, 640);
  bson_object_put_int32(&input_params, protocol_handler::strings::width, 480);

  // check: SetVideoConfig() should not be called, StartStreaming() is called
  EXPECT_CALL(*mock_app_ptr_, SetVideoConfig(_, _)).Times(0);
  EXPECT_CALL(*mock_app_ptr_, StartStreaming(service_type)).WillOnce(Return());

  app_manager_impl_->OnServiceStartedCallback(
      device_handle, session_key, service_type, &input_params);

  // check: return value is true and list is empty
  EXPECT_TRUE(result);
  EXPECT_TRUE(rejected_params.empty());
}

TEST_F(ApplicationManagerImplTest,
       OnSecondaryTransportStartedCallback_BeforeAppRegistration) {
  const connection_handler::DeviceHandle device_handle = 1;
  const int32_t session_key = 123;

  // make sure that BC.UpdateAppList is not invoked
  EXPECT_CALL(*mock_message_helper_,
              CreateModuleInfoSO(
                  hmi_apis::FunctionID::BasicCommunication_UpdateAppList, _))
      .Times(0);

  app_manager_impl_->OnSecondaryTransportStartedCallback(device_handle,
                                                         session_key);
}

TEST_F(ApplicationManagerImplTest,
       OnSecondaryTransportStartedCallback_AfterAppRegistration) {
  using namespace ns_smart_device_link::ns_smart_objects;

  AddMockApplication();
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(app_id_));

  const connection_handler::DeviceHandle device_handle = 1;
  const int32_t session_key = app_id_;

  EXPECT_CALL(*mock_app_ptr_, set_secondary_device(device_handle)).Times(1);
  // called by ResumeCtrlImpl::RetryResumption()
  EXPECT_CALL(*mock_app_ptr_, deferred_resumption_hmi_level())
      .WillOnce(Return(mobile_api::HMILevel::eType::INVALID_ENUM));

  smart_objects::SmartObject dummy_object(SmartType_Map);
  SmartObjectSPtr sptr = std::make_shared<SmartObject>(dummy_object);

  EXPECT_CALL(*mock_message_helper_,
              CreateModuleInfoSO(
                  hmi_apis::FunctionID::BasicCommunication_UpdateAppList, _))
      .WillOnce(Return(sptr));

  app_manager_impl_->OnSecondaryTransportStartedCallback(device_handle,
                                                         session_key);
}

TEST_F(ApplicationManagerImplTest,
       OnSecondaryTransportEndedCallback_AfterAppRegistration) {
  using namespace ns_smart_device_link::ns_smart_objects;

  const connection_handler::DeviceHandle device_handle = 1;
  const int32_t session_key = app_id_;

  AddMockApplication();
  EXPECT_CALL(*mock_app_ptr_, app_id()).WillRepeatedly(Return(app_id_));
  EXPECT_CALL(*mock_app_ptr_, secondary_device())
      .WillRepeatedly(Return(device_handle));

  EXPECT_CALL(*mock_app_ptr_, set_secondary_device(0)).Times(1);

  smart_objects::SmartObject dummy_object(SmartType_Map);
  SmartObjectSPtr sptr = std::make_shared<SmartObject>(dummy_object);

  EXPECT_CALL(*mock_message_helper_,
              CreateModuleInfoSO(
                  hmi_apis::FunctionID::BasicCommunication_UpdateAppList, _))
      .WillOnce(Return(sptr));

  app_manager_impl_->OnSecondaryTransportEndedCallback(session_key);
}

TEST_F(ApplicationManagerImplTest,
       OnSecondaryTransportEndedCallback_BeforeAppRegistration) {
  const int32_t session_key = app_id_;

  // make sure that BC.UpdateAppList is not invoked
  EXPECT_CALL(*mock_message_helper_,
              CreateModuleInfoSO(
                  hmi_apis::FunctionID::BasicCommunication_UpdateAppList, _))
      .Times(0);

  app_manager_impl_->OnSecondaryTransportEndedCallback(session_key);
}

TEST_F(ApplicationManagerImplTest,
       OnDeviceSwitchingStart_ExpectPutAppsInWaitList) {
  std::shared_ptr<MockApplication> switching_app_ptr =
      std::make_shared<MockApplication>();

  const std::string switching_device_id = "switching";
  const std::string switching_device_id_hash =
      encryption::MakeHash(switching_device_id);
  app_manager_impl_->AddMockApplication(switching_app_ptr);
  EXPECT_CALL(*switching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(switching_device_id_hash));

  const std::string policy_app_id_switch = "abc";
  EXPECT_CALL(*switching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_switch));

  const auto hmi_level_switching_app = mobile_apis::HMILevel::HMI_FULL;
  EXPECT_CALL(*switching_app_ptr, hmi_level())
      .WillRepeatedly(Return(hmi_level_switching_app));

  std::shared_ptr<MockApplication> nonswitching_app_ptr =
      std::make_shared<MockApplication>();

  const std::string nonswitching_device_id = "nonswitching";
  const std::string nonswitching_device_id_hash =
      encryption::MakeHash(nonswitching_device_id);
  app_manager_impl_->AddMockApplication(nonswitching_app_ptr);
  EXPECT_CALL(*nonswitching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(nonswitching_device_id_hash));

  const std::string policy_app_id_nonswitch = "efg";
  EXPECT_CALL(*nonswitching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_nonswitch));

  const auto hmi_level_nonswitching_app = mobile_apis::HMILevel::HMI_LIMITED;
  EXPECT_CALL(*nonswitching_app_ptr, hmi_level())
      .WillRepeatedly(Return(hmi_level_nonswitching_app));

  // Act
  const connection_handler::DeviceHandle switching_handle = 1;
  const connection_handler::Device switching_device(
      switching_handle, "switching_device", switching_device_id, "BLUETOOTH");

  const connection_handler::DeviceHandle non_switching_handle = 2;
  const connection_handler::Device non_switching_device(non_switching_handle,
                                                        "non_switching_device",
                                                        nonswitching_device_id,
                                                        "USB");

  EXPECT_CALL(*mock_message_helper_, CreateDeviceListSO(_, _, _))
      .WillOnce(Return(smart_objects::SmartObjectSPtr()));
  app_manager_impl_->OnDeviceSwitchingStart(switching_device,
                                            non_switching_device);
  EXPECT_TRUE(app_manager_impl_->IsAppInReconnectMode(policy_app_id_switch));
  EXPECT_FALSE(
      app_manager_impl_->IsAppInReconnectMode(policy_app_id_nonswitch));
}

TEST_F(ApplicationManagerImplTest,
       OnDeviceSwitchingFinish_ExpectUnregisterAppsInWaitList) {
  std::shared_ptr<MockApplication> switching_app_ptr =
      std::make_shared<MockApplication>();

  plugin_manager::MockRPCPluginManager* mock_rpc_plugin_manager =
      new plugin_manager::MockRPCPluginManager;
  std::unique_ptr<plugin_manager::RPCPluginManager> mock_rpc_plugin_manager_ptr(
      mock_rpc_plugin_manager);
  app_manager_impl_->SetPluginManager(mock_rpc_plugin_manager_ptr);

  const std::string switching_device_id = "switching";
  const std::string switching_device_id_hash =
      encryption::MakeHash(switching_device_id);
  app_manager_impl_->AddMockApplication(switching_app_ptr);
  EXPECT_CALL(*switching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(switching_device_id_hash));

  const std::string policy_app_id_switch = "abc";
  EXPECT_CALL(*switching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_switch));

  const auto hmi_level_switching_app = mobile_apis::HMILevel::HMI_FULL;
  EXPECT_CALL(*switching_app_ptr, hmi_level())
      .WillRepeatedly(Return(hmi_level_switching_app));

  std::shared_ptr<MockApplication> nonswitching_app_ptr =
      std::make_shared<MockApplication>();

  const std::string nonswitching_device_id = "nonswitching";
  const std::string nonswitching_device_id_hash =
      encryption::MakeHash(nonswitching_device_id);
  app_manager_impl_->AddMockApplication(nonswitching_app_ptr);
  EXPECT_CALL(*nonswitching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(nonswitching_device_id_hash));

  const std::string policy_app_id_nonswitch = "efg";
  EXPECT_CALL(*nonswitching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_nonswitch));

  const auto hmi_level_nonswitching_app = mobile_apis::HMILevel::HMI_LIMITED;
  EXPECT_CALL(*nonswitching_app_ptr, hmi_level())
      .WillRepeatedly(Return(hmi_level_nonswitching_app));

  // Act
  const connection_handler::DeviceHandle switching_handle = 1;
  const connection_handler::Device switching_device(
      switching_handle, "switching_device", switching_device_id, "BLUETOOTH");

  const connection_handler::DeviceHandle non_switching_handle = 2;
  const connection_handler::Device non_switching_device(non_switching_handle,
                                                        "non_switching_device",
                                                        nonswitching_device_id,
                                                        "USB");

  EXPECT_CALL(*mock_message_helper_, CreateDeviceListSO(_, _, _))
      .WillOnce(Return(smart_objects::SmartObjectSPtr()));
  app_manager_impl_->OnDeviceSwitchingStart(switching_device,
                                            non_switching_device);

  EXPECT_TRUE(app_manager_impl_->IsAppInReconnectMode(policy_app_id_switch));

  app_manager_impl_->OnDeviceSwitchingFinish(switching_device_id);
  EXPECT_FALSE(
      app_manager_impl_->application_by_policy_id(policy_app_id_switch));
}

TEST_F(ApplicationManagerImplTest,
       ProcessReconnection_ExpectChangeAppIdDeviceId) {
  const uint32_t application_id = 1;
  const std::string policy_app_id = "p_app_id";
  const std::string mac_address = "MA:CA:DD:RE:SS";
  const connection_handler::DeviceHandle device_id = 1;
  const custom_str::CustomString app_name("");

  std::shared_ptr<ApplicationImpl> app_impl = std::make_shared<ApplicationImpl>(
      application_id,
      policy_app_id,
      mac_address,
      device_id,
      app_name,
      std::shared_ptr<usage_statistics::StatisticsManager>(
          new usage_statistics_test::MockStatisticsManager()),
      *app_manager_impl_);

  app_manager_impl_->AddMockApplication(app_impl);

  const connection_handler::DeviceHandle new_device_id = 2;
  const uint32_t new_application_id = 2;
  SetCommonExpectationOnAppReconnection(
      new_device_id, new_application_id, mac_address);

  // Act
  app_manager_impl_->ProcessReconnection(app_impl, new_application_id);
  EXPECT_EQ(new_device_id, app_impl->device());
  EXPECT_EQ(new_application_id, app_impl->app_id());
}

TEST_F(ApplicationManagerImplTest, StartStopAudioPassThru) {
  std::string dummy_file_name;
  ON_CALL(mock_application_manager_settings_, recording_file_name())
      .WillByDefault(ReturnRef(dummy_file_name));

  NiceMock<test::components::media_manager_test::MockMediaManager>
      mock_media_manager;
  app_manager_impl_->SetMockMediaManager(&mock_media_manager);

  const uint32_t app_id = 65537;
  const int32_t max_duration = 1000;
  // below are not used
  const int32_t correlation_id = 0;
  const int32_t sampling_rate = 0;
  const int32_t bits_per_sample = 0;
  const int32_t audio_type = 0;

  EXPECT_CALL(mock_media_manager,
              StartMicrophoneRecording(app_id, _, max_duration))
      .WillOnce(Return());
  EXPECT_CALL(mock_media_manager, StopMicrophoneRecording(app_id))
      .WillOnce(Return());

  bool result = app_manager_impl_->BeginAudioPassThru(app_id);
  EXPECT_TRUE(result);
  if (result) {
    app_manager_impl_->StartAudioPassThruThread(app_id,
                                                correlation_id,
                                                max_duration,
                                                sampling_rate,
                                                bits_per_sample,
                                                audio_type);
  }

  result = app_manager_impl_->EndAudioPassThru(app_id);
  EXPECT_TRUE(result);
  if (result) {
    app_manager_impl_->StopAudioPassThru(app_id);
  }
}

TEST_F(ApplicationManagerImplTest, UnregisterAnotherAppDuringAudioPassThru) {
  std::string dummy_file_name;
  ON_CALL(mock_application_manager_settings_, recording_file_name())
      .WillByDefault(ReturnRef(dummy_file_name));

  std::unique_ptr<plugin_manager::RPCPluginManager> mock_rpc_plugin_manager_ptr(
      new plugin_manager::MockRPCPluginManager);
  app_manager_impl_->SetPluginManager(mock_rpc_plugin_manager_ptr);

  const uint32_t app_id_1 = 65537;
  const uint32_t app_id_2 = 65538;

  std::string dummy_mac_address;
  std::shared_ptr<MockApplication> mock_app_1 =
      std::shared_ptr<MockApplication>(new MockApplication());
  EXPECT_CALL(*mock_app_1, app_id()).WillRepeatedly(Return(app_id_1));
  EXPECT_CALL(*mock_app_1, device()).WillRepeatedly(Return(0));
  EXPECT_CALL(*mock_app_1, mac_address())
      .WillRepeatedly(ReturnRef(dummy_mac_address));
  EXPECT_CALL(*mock_app_1, policy_app_id()).WillRepeatedly(Return(""));
  EXPECT_CALL(*mock_app_1, protocol_version())
      .WillRepeatedly(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4));

  std::shared_ptr<MockApplication> mock_app_2 =
      std::shared_ptr<MockApplication>(new MockApplication());
  EXPECT_CALL(*mock_app_2, app_id()).WillRepeatedly(Return(app_id_2));
  EXPECT_CALL(*mock_app_2, device()).WillRepeatedly(Return(0));
  EXPECT_CALL(*mock_app_2, mac_address())
      .WillRepeatedly(ReturnRef(dummy_mac_address));
  EXPECT_CALL(*mock_app_2, policy_app_id()).WillRepeatedly(Return(""));
  EXPECT_CALL(*mock_app_2, protocol_version())
      .WillRepeatedly(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4));

  NiceMock<test::components::media_manager_test::MockMediaManager>
      mock_media_manager;
  app_manager_impl_->SetMockMediaManager(&mock_media_manager);

  app_manager_impl_->AddMockApplication(mock_app_1);
  app_manager_impl_->AddMockApplication(mock_app_2);

  const int32_t max_duration = 1000;
  // below are not used
  const int32_t correlation_id = 0;
  const int32_t sampling_rate = 0;
  const int32_t bits_per_sample = 0;
  const int32_t audio_type = 0;

  EXPECT_CALL(mock_media_manager,
              StartMicrophoneRecording(app_id_2, _, max_duration))
      .WillOnce(Return());
  EXPECT_CALL(mock_media_manager, StopMicrophoneRecording(app_id_2))
      .WillOnce(Return());

  // app 2 starts Audio Pass Thru
  bool result = app_manager_impl_->BeginAudioPassThru(app_id_2);
  EXPECT_TRUE(result);
  if (result) {
    app_manager_impl_->StartAudioPassThruThread(app_id_2,
                                                correlation_id,
                                                max_duration,
                                                sampling_rate,
                                                bits_per_sample,
                                                audio_type);
  }

  // while running APT, app 1 is unregistered
  app_manager_impl_->UnregisterApplication(
      app_id_1, mobile_apis::Result::SUCCESS, false, true);

  // confirm that APT is still running
  result = app_manager_impl_->EndAudioPassThru(app_id_2);
  EXPECT_TRUE(result);
  if (result) {
    app_manager_impl_->StopAudioPassThru(app_id_2);
  }
}

static std::map<std::string, std::vector<std::string> > CreateTransportMap() {
  /*
   * DefaultTransportRequiredForResumption = TCP_WIFI, IAP_USB, SPP_BLUETOOTH
   * MediaTransportRequiredForResumption = TCP_WIFI, AOA_USB
   * NavigationTransportRequiredForResumption = AOA_USB, SPP_BLUETOOTH
   * TestingTransportRequiredForResumption =
   * EmptyAppTransportRequiredForResumption = TCP_WIFI
   */
  std::string TCP_WIFI("TCP_WIFI");
  std::string IAP_USB("IAP_USB");
  std::string SPP_BLUETOOTH("SPP_BLUETOOTH");
  std::string AOA_USB("AOA_USB");

  std::vector<std::string> default_transports;
  default_transports.push_back(TCP_WIFI);
  default_transports.push_back(IAP_USB);
  default_transports.push_back(SPP_BLUETOOTH);
  std::vector<std::string> media_transports;
  media_transports.push_back(TCP_WIFI);
  media_transports.push_back(AOA_USB);
  std::vector<std::string> navi_transports;
  navi_transports.push_back(AOA_USB);
  navi_transports.push_back(SPP_BLUETOOTH);
  std::vector<std::string> testing_transports;
  std::vector<std::string> empty_transports;
  empty_transports.push_back(TCP_WIFI);

  std::map<std::string, std::vector<std::string> > transport_map;
  transport_map[std::string("DEFAULT")] = default_transports;
  transport_map[std::string("MEDIA")] = media_transports;
  transport_map[std::string("NAVIGATION")] = navi_transports;
  transport_map[std::string("TESTING")] = testing_transports;
  transport_map[std::string("EMPTY_APP")] = empty_transports;

  return transport_map;
}

bool ApplicationManagerImplTest::CheckResumptionRequiredTransportAvailableTest(
    smart_objects::SmartObject* app_types_array,
    connection_handler::DeviceHandle primary_device_handle,
    std::string primary_transport_device_string,
    connection_handler::DeviceHandle secondary_device_handle,
    std::string secondary_transport_device_string) {
  EXPECT_CALL(*mock_app_ptr_, app_types())
      .WillRepeatedly(Return(app_types_array));

  std::map<std::string, std::vector<std::string> > transport_map =
      CreateTransportMap();

  EXPECT_CALL(mock_application_manager_settings_,
              transport_required_for_resumption_map())
      .WillRepeatedly(ReturnRef(transport_map));

  EXPECT_CALL(*mock_app_ptr_, device())
      .WillRepeatedly(Return(primary_device_handle));
  EXPECT_CALL(*mock_app_ptr_, secondary_device())
      .WillRepeatedly(Return(secondary_device_handle));

  EXPECT_CALL(mock_session_observer_,
              TransportTypeProfileStringFromDeviceHandle(primary_device_handle))
      .WillOnce(Return(primary_transport_device_string));

  if (secondary_device_handle != 0) {
    EXPECT_CALL(
        mock_session_observer_,
        TransportTypeProfileStringFromDeviceHandle(secondary_device_handle))
        .WillOnce(Return(secondary_transport_device_string));
  } else {
    EXPECT_CALL(mock_session_observer_,
                TransportTypeProfileStringFromDeviceHandle(
                    secondary_device_handle)).WillOnce(Return(std::string("")));
  }

  return app_manager_impl_->CheckResumptionRequiredTransportAvailable(
      mock_app_ptr_);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_PrimaryOnly_Success) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::DEFAULT;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 0;

  // refer to transport_adapter_impl.cc
  std::string primary_transport_device_string("SPP_BLUETOOTH");
  std::string secondary_transport_device_string("");

  // - The app is DEFAULT.
  // - A DEFAULT app is allowed for resumption if either primary or secondary
  //   transport is TCP_WIFI, IAP_USB or SPP_BLUETOOTH.
  // - We have SPP_BLUETOOTH for primary transport.
  //   -> Conclusion: the app has required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_TRUE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_PrimaryOnly_NotListed) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::SOCIAL;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 0;

  std::string primary_transport_device_string("SPP_BLUETOOTH");
  std::string secondary_transport_device_string("");

  // - The app is SOCIAL.
  // - We do not have an entry in .ini file for SOCIAL apps.
  //   -> In this case, resumption is always enabled for backward compatibility.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_TRUE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_PrimaryOnly_Disabled) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::TESTING;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 0;

  std::string primary_transport_device_string("SPP_BLUETOOTH");
  std::string secondary_transport_device_string("");

  // - The app is TESTING.
  // - We do not have any transports allowed for TESTING apps.
  //   -> In this case, resumption is always disabled.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_FALSE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_PrimaryOnly_NoAppTypes) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  // we don't specify any app type

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 0;

  std::string primary_transport_device_string("SPP_BLUETOOTH");
  std::string secondary_transport_device_string("");

  // - The app doesn't specify AppHMIType.
  // - .ini file specifies TCP_WIFI for EMPTY_APP entry.
  //   -> The app does not have required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_FALSE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_PrimaryOnly_NoAppTypes2) {
  using namespace ns_smart_device_link::ns_smart_objects;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 0;

  std::string primary_transport_device_string("SPP_BLUETOOTH");
  std::string secondary_transport_device_string("");

  // - The app doesn't specify AppHMIType.
  // - .ini file specifies TCP_WIFI for EMPTY_APP entry.
  //   -> The app does not have required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      NULL,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_FALSE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_TwoTransports_Success) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::MEDIA;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 2;

  // refer to transport_adapter_impl.cc
  std::string primary_transport_device_string("SPP_BLUETOOTH");
  std::string secondary_transport_device_string("TCP_WIFI");

  // - The app is MEDIA.
  // - A MEDIA app is allowed for resumption if either primary or secondary
  //   transport is TCP_WIFI or AOA_USB.
  // - We have TCP_WIFI for secondary transport.
  //   -> Conclusion: the app has required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_TRUE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_TwoTransports_Failure) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::NAVIGATION;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 2;

  // refer to transport_adapter_impl.cc
  std::string primary_transport_device_string("IAP_USB");
  std::string secondary_transport_device_string("TCP_WIFI");

  // - The app is NAVIGATION.
  // - A NAVIGATION app is allowed for resumption if either primary or secondary
  //   transport is AOA_USB or SPP_BLUETOOTH.
  // - We have IAP_USB for primary and TCP_WIFI for secondary transport.
  //   -> Conclusion: the app does not have required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_FALSE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_MultipleAppTypes_Failure) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::MEDIA;
  app_types_array[1] = mobile_apis::AppHMIType::eType::NAVIGATION;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 2;

  std::string primary_transport_device_string("IAP_USB");
  std::string secondary_transport_device_string("TCP_WIFI");

  // - The app is MEDIA and NAVIGATION.
  // - A MEDIA app is allowed for resumption if either primary or secondary
  //   transport is TCP_WIFI or AOA_USB.
  // - A NAVIGATION app is allowed for resumption if either primary or secondary
  //   transport is AOA_USB or SPP_BLUETOOTH.
  // - We have IAP_USB for primary and TCP_WIFI is secondary
  //   -> Conclusion: the app does NOT have required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_FALSE(result);
}

TEST_F(ApplicationManagerImplTest,
       CheckResumptionRequiredTransportAvailableTest_MultipleAppTypes_Empty) {
  using namespace ns_smart_device_link::ns_smart_objects;

  smart_objects::SmartObject app_types_array(SmartType_Array);
  app_types_array[0] = mobile_apis::AppHMIType::eType::NAVIGATION;
  app_types_array[1] = mobile_apis::AppHMIType::eType::SYSTEM;

  const connection_handler::DeviceHandle primary_device_handle = 1;
  const connection_handler::DeviceHandle secondary_device_handle = 2;

  std::string primary_transport_device_string("IAP_USB");
  std::string secondary_transport_device_string("TCP_WIFI");

  // - The app is NAVIGATION and SYSTEM.
  // - A NAVIGATION app is allowed for resumption if either primary or secondary
  //   transport is AOA_USB or SPP_BLUETOOTH.
  // - .ini file does not have an entry for SYSTEM apps. So any transport is
  //   allowed.
  // - We have SPP_BLUETOOTH for primary and TCP_WIFI is secondary
  //   -> Conclusion: the app does NOT have required transport.
  bool result = CheckResumptionRequiredTransportAvailableTest(
      &app_types_array,
      primary_device_handle,
      primary_transport_device_string,
      secondary_device_handle,
      secondary_transport_device_string);
  EXPECT_FALSE(result);
}

TEST_F(ApplicationManagerImplTest,
       RegisterApplication_PathToTheIconExists_IconWasSet) {
  file_system::CreateDirectory(kDirectoryName);
  const std::string full_icon_path = kDirectoryName + "/" + kAppId;
  ASSERT_TRUE(file_system::CreateFile(full_icon_path));

  smart_objects::SmartObject request_for_registration(
      smart_objects::SmartType_Map);

  smart_objects::SmartObject& params =
      request_for_registration[strings::msg_params];
  params[strings::app_id] = kAppId;
  params[strings::language_desired] = mobile_api::Language::EN_US;
  params[strings::hmi_display_language_desired] = mobile_api::Language::EN_US;

  request_for_registration[strings::params][strings::connection_key] =
      kConnectionKey;
  request_for_registration[strings::msg_params][strings::app_name] = kAppName;
  request_for_registration[strings::msg_params][strings::sync_msg_version]
                          [strings::minor_version] = APIVersion::kAPIV2;
  request_for_registration[strings::msg_params][strings::sync_msg_version]
                          [strings::major_version] = APIVersion::kAPIV3;

  request_for_registration[strings::params][strings::protocol_version] =
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2;

  smart_objects::SmartObjectSPtr request_for_registration_ptr =
      std::make_shared<smart_objects::SmartObject>(request_for_registration);

  ApplicationSharedPtr application =
      app_manager_impl_->RegisterApplication(request_for_registration_ptr);
  EXPECT_STREQ(kAppName.c_str(), application->name().c_str());
  EXPECT_STREQ(full_icon_path.c_str(), application->app_icon_path().c_str());
  EXPECT_EQ(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2,
            application->protocol_version());
  EXPECT_EQ(APIVersion::kAPIV2,
            application->version().min_supported_api_version);
  EXPECT_EQ(APIVersion::kAPIV3,
            application->version().max_supported_api_version);

  EXPECT_TRUE(file_system::RemoveDirectory(kDirectoryName, true));
}

TEST_F(ApplicationManagerImplTest,
       RegisterApplication_CloudAppRegisterSuccess) {
  std::shared_ptr<MockApplication> waiting_app =
      std::make_shared<MockApplication>();
  ON_CALL(*waiting_app, device()).WillByDefault(Return(kDeviceId));
  EXPECT_CALL(*waiting_app, cloud_app_endpoint())
      .WillOnce(ReturnRef(kEndpoint));
  EXPECT_CALL(*waiting_app, cloud_app_auth_token())
      .WillOnce(ReturnRef(kAuthToken));
  EXPECT_CALL(*waiting_app, cloud_app_certificate())
      .WillOnce(ReturnRef(kCertificate));
  EXPECT_CALL(*waiting_app, cloud_app_transport_type())
      .WillOnce(ReturnRef(kTransportType));
  EXPECT_CALL(*waiting_app, hybrid_app_preference())
      .WillOnce(ReturnRef(kHybridAppPreference));
  ON_CALL(*waiting_app, is_cloud_app()).WillByDefault(Return(true));
  EXPECT_CALL(*waiting_app, policy_app_id()).WillRepeatedly(Return(kAppId));
  app_manager_impl_->AddMockPendingApplication(waiting_app);

  EXPECT_CALL(
      mock_session_observer_,
      GetDataOnSessionKey(kConnectionKey,
                          _,
                          _,
                          testing::An<connection_handler::DeviceHandle*>()))
      .WillOnce(DoAll(SetArgPointee<3u>(kDeviceId), Return(0)));
  EXPECT_CALL(*mock_rpc_service_,
              ManageMobileCommand(_, commands::Command::SOURCE_SDL)).Times(0);
  smart_objects::SmartObject request_for_registration(
      smart_objects::SmartType_Map);

  smart_objects::SmartObject& params =
      request_for_registration[strings::msg_params];
  params[strings::app_id] = kAppId;
  params[strings::language_desired] = mobile_api::Language::EN_US;
  params[strings::hmi_display_language_desired] = mobile_api::Language::EN_US;

  request_for_registration[strings::params][strings::connection_key] =
      kConnectionKey;
  request_for_registration[strings::msg_params][strings::app_name] = kAppName;
  request_for_registration[strings::msg_params][strings::sync_msg_version]
                          [strings::minor_version] = APIVersion::kAPIV2;
  request_for_registration[strings::msg_params][strings::sync_msg_version]
                          [strings::major_version] = APIVersion::kAPIV3;

  request_for_registration[strings::params][strings::protocol_version] =
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2;

  smart_objects::SmartObjectSPtr request_for_registration_ptr =
      std::make_shared<smart_objects::SmartObject>(request_for_registration);

  ApplicationSharedPtr application =
      app_manager_impl_->RegisterApplication(request_for_registration_ptr);

  EXPECT_EQ(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2,
            application->protocol_version());
  EXPECT_EQ(APIVersion::kAPIV2,
            application->version().min_supported_api_version);
  EXPECT_EQ(APIVersion::kAPIV3,
            application->version().max_supported_api_version);
  EXPECT_EQ(kEndpoint, application->cloud_app_endpoint());
  EXPECT_EQ(kAuthToken, application->cloud_app_auth_token());
  EXPECT_EQ(kCertificate, application->cloud_app_certificate());
  EXPECT_EQ(kTransportType, application->cloud_app_transport_type());
  EXPECT_EQ(kHybridAppPreference, application->hybrid_app_preference());
}

TEST_F(ApplicationManagerImplTest,
       RegisterApplication_CloudAppRegisterFailed_DISALLOWED) {
  std::shared_ptr<MockApplication> waiting_app =
      std::make_shared<MockApplication>();
  EXPECT_CALL(*waiting_app, device()).WillRepeatedly(Return(kDeviceId));
  EXPECT_CALL(*waiting_app, is_cloud_app()).WillRepeatedly(Return(true));
  EXPECT_CALL(*waiting_app, policy_app_id())
      .WillRepeatedly(Return("Fake" + kAppId));
  app_manager_impl_->AddMockPendingApplication(waiting_app);

  EXPECT_CALL(
      mock_session_observer_,
      GetDataOnSessionKey(kConnectionKey,
                          _,
                          _,
                          testing::An<connection_handler::DeviceHandle*>()))
      .WillOnce(DoAll(SetArgPointee<3u>(kDeviceId), Return(0)));
  EXPECT_CALL(*mock_rpc_service_,
              ManageMobileCommand(_, commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));
  smart_objects::SmartObject request_for_registration(
      smart_objects::SmartType_Map);

  smart_objects::SmartObject& params =
      request_for_registration[strings::msg_params];
  params[strings::app_id] = kAppId;
  params[strings::language_desired] = mobile_api::Language::EN_US;
  params[strings::hmi_display_language_desired] = mobile_api::Language::EN_US;

  request_for_registration[strings::params][strings::connection_key] =
      kConnectionKey;
  request_for_registration[strings::msg_params][strings::app_name] = kAppName;
  request_for_registration[strings::msg_params][strings::sync_msg_version]
                          [strings::minor_version] = APIVersion::kAPIV2;
  request_for_registration[strings::msg_params][strings::sync_msg_version]
                          [strings::major_version] = APIVersion::kAPIV3;

  request_for_registration[strings::params][strings::protocol_version] =
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2;

  smart_objects::SmartObjectSPtr request_for_registration_ptr =
      std::make_shared<smart_objects::SmartObject>(request_for_registration);

  ApplicationSharedPtr application =
      app_manager_impl_->RegisterApplication(request_for_registration_ptr);
  EXPECT_EQ(0, application.use_count());
}

}  // application_manager_test
}  // namespace components
}  // namespace test
