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
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/test/include/application_manager/mock_message_helper.h"
#include "connection_handler/mock_connection_handler.h"
#include "hmi_message_handler/mock_hmi_message_handler.h"
#include "policy/mock_policy_settings.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol/bson_object_keys.h"
#include "protocol_handler/mock_session_observer.h"
#include "utils/custom_string.h"
#include "utils/file_system.h"
#include "utils/lock.h"
#include "utils/make_shared.h"
#include "utils/push_log.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace am = application_manager;
namespace policy_test = test::components::policy_handler_test;
namespace con_test = connection_handler_test;

using testing::_;
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
const std::vector<std::string> kTimeoutPrompt{"timeoutPrompt"};
sync_primitives::Lock state_lock_;
sync_primitives::ConditionalVariable state_condition_;
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 public:
  ApplicationManagerImplTest()
      : mock_storage_(
            ::utils::MakeShared<NiceMock<resumption_test::MockResumptionData> >(
                app_mngr_))
      , mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock())
      , app_id_(0u) {
    logger::create_log_message_loop_thread();
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }
  ~ApplicationManagerImplTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

 protected:
  void SetUp() OVERRIDE {
    CreateAppManager();

    ON_CALL(mock_connection_handler_, GetDataOnSessionKey(_, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3u>(app_id_), Return(0)));
    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer_));

    app_manager_impl_->resume_controller().set_resumption_storage(
        mock_storage_);
    app_manager_impl_->set_connection_handler(&mock_connection_handler_);
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
        .WillByDefault(Return(true));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(stop_streaming_timeout));
    ON_CALL(mock_application_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(kTimeout));
    ON_CALL(mock_application_manager_settings_, time_out_promt())
        .WillByDefault(ReturnRef(kTimeoutPrompt));

    app_manager_impl_.reset(new am::ApplicationManagerImpl(
        mock_application_manager_settings_, mock_policy_settings_));
    mock_app_ptr_ = utils::SharedPtr<MockApplication>(new MockApplication());

    ASSERT_TRUE(app_manager_impl_.get());
    ASSERT_TRUE(mock_app_ptr_.get());
  }

  void AddMockApplication() {
    app_manager_impl_->AddMockApplication(mock_app_ptr_);
  }

  NiceMock<policy_test::MockPolicySettings> mock_policy_settings_;
  utils::SharedPtr<NiceMock<resumption_test::MockResumptionData> >
      mock_storage_;
  NiceMock<con_test::MockConnectionHandler> mock_connection_handler_;
  NiceMock<protocol_handler_test::MockSessionObserver> mock_session_observer_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  application_manager_test::MockApplicationManager app_mngr_;
  std::auto_ptr<am::ApplicationManagerImpl> app_manager_impl_;
  application_manager::MockMessageHelper* mock_message_helper_;
  uint32_t app_id_;
  utils::SharedPtr<MockApplication> mock_app_ptr_;
};

TEST_F(ApplicationManagerImplTest, ProcessQueryApp_ExpectSuccess) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject app_data;
  const uint32_t connection_key = 65537u;

  app_data[am::json::name] = "application_manager_test";
  app_data[am::json::appId] = app_id_;
  app_data[am::json::android] = "bucket";
  app_data[am::json::android][am::json::packageName] = "com.android.test";
  smart_objects::SmartObject sm_object(SmartType_Map);
  sm_object[am::json::response][0] = app_data;
  SmartObjectSPtr sptr = MakeShared<SmartObject>(sm_object);

  ON_CALL(*mock_message_helper_, CreateModuleInfoSO(_, _))
      .WillByDefault(Return(sptr));
  ON_CALL(*mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillByDefault(Return(sptr));
  app_manager_impl_->ProcessQueryApp(sm_object, connection_key);
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
       OnDeviceSwitchingStart_ExpectPutAppsInWaitList) {
  utils::SharedPtr<MockApplication> switching_app_ptr =
      utils::MakeShared<MockApplication>();

  const std::string switching_device_id = "switching";
  app_manager_impl_->AddMockApplication(switching_app_ptr);
  EXPECT_CALL(*switching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(switching_device_id));

  const std::string policy_app_id_switch = "abc";
  EXPECT_CALL(*switching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_switch));

  utils::SharedPtr<MockApplication> nonswitching_app_ptr =
      utils::MakeShared<MockApplication>();

  const std::string nonswitching_device_id = "nonswitching";
  app_manager_impl_->AddMockApplication(nonswitching_app_ptr);
  EXPECT_CALL(*nonswitching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(nonswitching_device_id));

  const std::string policy_app_id_nonswitch = "efg";
  EXPECT_CALL(*nonswitching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_nonswitch));

  // Act
  app_manager_impl_->OnDeviceSwitchingStart(switching_device_id);
  EXPECT_TRUE(app_manager_impl_->IsAppInReconnectMode(policy_app_id_switch));
  EXPECT_FALSE(
      app_manager_impl_->IsAppInReconnectMode(policy_app_id_nonswitch));
}

TEST_F(ApplicationManagerImplTest,
       OnDeviceSwitchingFinish_ExpectUnregisterAppsInWaitList) {
  utils::SharedPtr<MockApplication> switching_app_ptr =
      utils::MakeShared<MockApplication>();

  const std::string switching_device_id = "switching";
  app_manager_impl_->AddMockApplication(switching_app_ptr);
  EXPECT_CALL(*switching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(switching_device_id));

  const std::string policy_app_id_switch = "abc";
  EXPECT_CALL(*switching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_switch));

  utils::SharedPtr<MockApplication> nonswitching_app_ptr =
      utils::MakeShared<MockApplication>();

  const std::string nonswitching_device_id = "nonswitching";
  app_manager_impl_->AddMockApplication(nonswitching_app_ptr);
  EXPECT_CALL(*nonswitching_app_ptr, mac_address())
      .WillRepeatedly(ReturnRef(nonswitching_device_id));

  const std::string policy_app_id_nonswitch = "efg";
  EXPECT_CALL(*nonswitching_app_ptr, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_nonswitch));

  // Act
  app_manager_impl_->OnDeviceSwitchingStart(switching_device_id);
  EXPECT_TRUE(app_manager_impl_->IsAppInReconnectMode(policy_app_id_switch));

  app_manager_impl_->OnDeviceSwitchFinish(switching_device_id);
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

  utils::SharedPtr<ApplicationImpl> app_impl = new ApplicationImpl(
      application_id,
      policy_app_id,
      mac_address,
      device_id,
      app_name,
      utils::SharedPtr<usage_statistics::StatisticsManager>(
          new usage_statistics_test::MockStatisticsManager()),
      *app_manager_impl_);

  app_manager_impl_->AddMockApplication(app_impl);

  const connection_handler::DeviceHandle new_device_id = 2;
  const uint32_t new_application_id = 2;
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

  // Act
  app_manager_impl_->ProcessReconnection(app_impl, new_application_id);
  EXPECT_EQ(new_device_id, app_impl->device());
  EXPECT_EQ(new_application_id, app_impl->app_id());
}

TEST_F(ApplicationManagerImplTest, RecallApplicationData_ExpectAppDataReset) {
  const uint32_t application_id = 1;
  const std::string policy_app_id = "p_app_id";
  const std::string mac_address = "MA:CA:DD:RE:SS";
  const connection_handler::DeviceHandle device_id = 1;
  const custom_str::CustomString app_name("");

  ApplicationSharedPtr app_impl = new ApplicationImpl(
      application_id,
      policy_app_id,
      mac_address,
      device_id,
      app_name,
      utils::SharedPtr<usage_statistics::StatisticsManager>(
          new usage_statistics_test::MockStatisticsManager()),
      *app_manager_impl_);

  app_manager_impl_->AddMockApplication(app_impl);

  const uint32_t cmd_id = 1;
  const uint32_t menu_id = 2;
  const uint32_t choice_set_id = 3;
  const VehicleDataType vi = VehicleDataType::ACCPEDAL;
  const mobile_apis::ButtonName::eType button = mobile_apis::ButtonName::AC;

  smart_objects::SmartObject cmd;
  cmd[strings::msg_params][strings::cmd_id] = cmd_id;
  cmd[strings::msg_params][strings::vr_commands][0] = "vrCmd";
  cmd[strings::msg_params][strings::menu_id] = menu_id;
  cmd[strings::msg_params][strings::interaction_choice_set_id] = choice_set_id;

  app_impl->AddCommand(cmd_id, cmd[strings::msg_params]);
  app_impl->AddSubMenu(menu_id, cmd[strings::menu_params]);
  app_impl->AddChoiceSet(choice_set_id, cmd[strings::msg_params]);
  EXPECT_TRUE(app_impl->SubscribeToIVI(static_cast<uint32_t>(vi)));
  EXPECT_TRUE(app_impl->SubscribeToButton(button));

  const std::string some_string = "some_string";
  smart_objects::SmartObject dummy_data =
      smart_objects::SmartObject(smart_objects::SmartType_String);
  dummy_data = some_string;
  app_impl->set_help_prompt(dummy_data);
  app_impl->set_timeout_prompt(dummy_data);
  app_impl->set_vr_help(dummy_data);
  app_impl->set_vr_help_title(dummy_data);
  app_impl->set_keyboard_props(dummy_data);
  app_impl->set_menu_title(dummy_data);
  app_impl->set_menu_icon(dummy_data);

  const bool persistent = false;
  const bool downloaded = true;
  const std::string filename = "filename";
  AppFile file(filename, persistent, downloaded, mobile_apis::FileType::BINARY);

  app_impl->AddFile(file);

  EXPECT_TRUE(NULL != app_impl->FindCommand(cmd_id));
  EXPECT_TRUE(NULL != app_impl->FindSubMenu(menu_id));
  EXPECT_TRUE(NULL != app_impl->FindChoiceSet(choice_set_id));
  EXPECT_TRUE(app_impl->IsSubscribedToButton(button));
  EXPECT_TRUE(app_impl->IsSubscribedToIVI(static_cast<uint32_t>(vi)));
  auto help_prompt = app_impl->help_prompt();
  EXPECT_TRUE(help_prompt->asString() == some_string);
  auto timeout_prompt = app_impl->timeout_prompt();
  EXPECT_TRUE(timeout_prompt->asString() == some_string);
  auto vr_help = app_impl->vr_help();
  EXPECT_TRUE(vr_help->asString() == some_string);
  auto vr_help_title = app_impl->vr_help_title();
  EXPECT_TRUE(vr_help_title->asString() == some_string);
  auto kb_properties = app_impl->keyboard_props();
  EXPECT_TRUE(kb_properties->asString() == some_string);
  auto menu_title = app_impl->menu_title();
  EXPECT_TRUE(menu_title->asString() == some_string);
  auto menu_icon = app_impl->menu_icon();
  EXPECT_TRUE(menu_icon->asString() == some_string);
  auto file_ptr = app_impl->GetFile(filename);
  EXPECT_TRUE(NULL != file_ptr);
  EXPECT_TRUE(file_ptr->file_name == filename);

  // Act
  app_manager_impl_->RecallApplicationData(app_impl);
  EXPECT_FALSE(NULL != app_impl->FindCommand(cmd_id));
  EXPECT_FALSE(NULL != app_impl->FindSubMenu(menu_id));
  EXPECT_FALSE(NULL != app_impl->FindChoiceSet(choice_set_id));
  EXPECT_FALSE(app_impl->IsSubscribedToButton(button));
  EXPECT_FALSE(app_impl->IsSubscribedToIVI(static_cast<uint32_t>(vi)));
  help_prompt = app_impl->help_prompt();
  EXPECT_FALSE(help_prompt->asString() == some_string);
  timeout_prompt = app_impl->timeout_prompt();
  EXPECT_FALSE(timeout_prompt->asString() == some_string);
  vr_help = app_impl->vr_help();
  EXPECT_TRUE(vr_help == NULL);
  vr_help_title = app_impl->vr_help_title();
  EXPECT_TRUE(vr_help_title == NULL);
  kb_properties = app_impl->keyboard_props();
  EXPECT_FALSE(kb_properties->asString() == some_string);
  menu_title = app_impl->menu_title();
  EXPECT_FALSE(menu_title->asString() == some_string);
  menu_icon = app_impl->menu_icon();
  EXPECT_FALSE(menu_icon->asString() == some_string);
  file_ptr = app_impl->GetFile(filename);
  EXPECT_TRUE(NULL == file_ptr);
}

TEST_F(ApplicationManagerImplTest,
       RecallApplicationData_ExpectHMICleanupRequests) {
  const uint32_t application_id = 1;
  const std::string policy_app_id = "p_app_id";
  const std::string mac_address = "MA:CA:DD:RE:SS";
  const connection_handler::DeviceHandle device_id = 1;
  const custom_str::CustomString app_name("");

  ApplicationSharedPtr app_impl = new ApplicationImpl(
      application_id,
      policy_app_id,
      mac_address,
      device_id,
      app_name,
      utils::SharedPtr<usage_statistics::StatisticsManager>(
          new usage_statistics_test::MockStatisticsManager()),
      *app_manager_impl_);

  app_manager_impl_->AddMockApplication(app_impl);

  const uint32_t cmd_id = 1;
  const uint32_t menu_id = 2;
  const uint32_t choice_set_id = 3;
  smart_objects::SmartObject cmd;
  cmd[strings::msg_params][strings::cmd_id] = cmd_id;
  cmd[strings::msg_params][strings::vr_commands][0] = "vrCmd";
  cmd[strings::msg_params][strings::menu_id] = menu_id;
  cmd[strings::msg_params][strings::interaction_choice_set_id] = choice_set_id;

  app_impl->AddCommand(cmd_id, cmd[strings::msg_params]);
  app_impl->AddSubMenu(menu_id, cmd[strings::menu_params]);
  app_impl->AddChoiceSet(choice_set_id, cmd[strings::msg_params]);
  app_impl->SubscribeToIVI(static_cast<uint32_t>(VehicleDataType::ACCPEDAL));
  app_impl->SubscribeToButton(mobile_apis::ButtonName::AC);

  EXPECT_CALL(*mock_message_helper_, SendUnsubscribedWayPoints(_));

  EXPECT_CALL(*mock_message_helper_, SendDeleteCommandRequest(_, _, _));

  EXPECT_CALL(*mock_message_helper_, SendDeleteSubmenuRequest(_, _, _));

  EXPECT_CALL(*mock_message_helper_, SendDeleteChoiceSetRequest(_, _, _));

  EXPECT_CALL(*mock_message_helper_, SendResetPropertiesRequest(_, _));

  EXPECT_CALL(*mock_message_helper_,
              SendUnsubscribeButtonNotification(_, _, _));

  EXPECT_CALL(*mock_message_helper_, SendUnsubscribeIVIRequest(_, _, _));

  // Act
  app_manager_impl_->RecallApplicationData(app_impl);
}

}  // application_manager_test
}  // namespace components
}  // namespace test
