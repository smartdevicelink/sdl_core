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

#include <stdint.h>

#include "gtest/gtest.h"
#include "application_manager/request_controller.h"
#include "application_manager/request_info.h"
#include "application_manager/mock_request.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/application_impl.h"
#include "utils/make_shared.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "resumption/last_state.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/state_controller.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/mock_request_controller_settings.h"
#include "application_manager/mock_application_manager.h"

namespace test {
namespace components {
namespace request_controller_test {

using ::application_manager::request_controller::RequestController;
using ::application_manager::request_controller::RequestInfo;
using ::application_manager::request_controller::HMIRequestInfo;
using ::application_manager::request_controller::MobileRequestInfo;

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::application_manager::request_controller::RequestInfoPtr;
using ::application_manager::request_controller::RequestInfoSet;

typedef NiceMock<application_manager_test::MockRequest> MRequest;
typedef utils::SharedPtr<MRequest> RequestPtr;
typedef utils::SharedPtr<RequestController> RequestControllerSPtr;
typedef std::list<RequestInfoPtr> RequestInfoPtrList;
typedef std::list< ::application_manager::request_controller::RequestPtr>
    RequestPtrList;

namespace {
const size_t kNumberOfRequests = 10u;
const uint32_t kTimeScale = 5000u;  // 5 seconds
const uint32_t kMaxRequestAmount = 2u;
const uint32_t kDefaultCorrelationID = 1u;
const uint32_t kDefaultConnectionKey = 0u;
const uint32_t kDefaultTimeout = 100u;
const uint32_t kThreadPoolSize = 1u;
}  // namespace

class RequestControllerTestClass : public ::testing::Test {
 public:
  struct TestSettings {
    uint32_t app_hmi_level_none_requests_time_scale_;
    uint32_t app_hmi_level_none_time_scale_max_requests_;
    uint32_t app_time_scale_max_requests_;
    uint32_t app_requests_time_scale_;
    uint32_t pending_requests_amount_;

    TestSettings(const uint32_t app_hmi_level_none_requests_time_scale = 0u,
                 const uint32_t app_hmi_level_none_time_scale_max_requests = 0u,
                 const uint32_t app_time_scale_max_requests = 0u,
                 const uint32_t app_requests_time_scale = 0u,
                 const uint32_t pending_requests_amount = 0u)
        : app_hmi_level_none_requests_time_scale_(
              app_hmi_level_none_requests_time_scale)
        , app_hmi_level_none_time_scale_max_requests_(
              app_hmi_level_none_time_scale_max_requests)
        , app_time_scale_max_requests_(app_time_scale_max_requests)
        , app_requests_time_scale_(app_requests_time_scale)
        , pending_requests_amount_(pending_requests_amount) {}
  };

  RequestControllerTestClass() {
    ON_CALL(mock_request_controller_settings_, thread_pool_size())
        .WillByDefault(Return(kThreadPoolSize));
    request_ctrl_ =
        utils::MakeShared<RequestController>(mock_request_controller_settings_);
  }

  RequestPtr GetMockRequest(
      const uint32_t correlation_id = kDefaultCorrelationID,
      const uint32_t kConnectionKey = kDefaultConnectionKey,
      const uint32_t default_timeout = kDefaultTimeout) {
    RequestPtr output =
        utils::MakeShared<MRequest>(kConnectionKey, correlation_id);
    ON_CALL(*output, default_timeout()).WillByDefault(Return(default_timeout));
    return output;
  }

  RequestInfoPtr GetRequestInfo(RequestPtr request,
                                const RequestInfo::RequestType request_type =
                                    RequestInfo::RequestType::HMIRequest,
                                const mobile_apis::HMILevel::eType& hmi_level =
                                    mobile_apis::HMILevel::INVALID_ENUM) {
    RequestInfoPtr request_info_ptr;
    if (request_type == RequestInfo::RequestType::HMIRequest) {
      request_info_ptr = utils::MakeShared<HMIRequestInfo>(
          request, request->default_timeout());
    } else {
      request_info_ptr = utils::MakeShared<MobileRequestInfo>(
          request, request->default_timeout());
    }
    request_info_ptr->set_hmi_level(hmi_level);

    return request_info_ptr;
  }

  RequestController::TResult AddRequest(
      const TestSettings& settings,
      RequestPtr request,
      const RequestInfo::RequestType request_type =
          RequestInfo::RequestType::HMIRequest,
      const mobile_apis::HMILevel::eType& hmi_level =
          mobile_apis::HMILevel::INVALID_ENUM) {
    if (RequestInfo::RequestType::HMIRequest == request_type) {
      return request_ctrl_->addHMIRequest(request);
    }
    CallSettings(settings);
    return request_ctrl_->addMobileRequest(request, hmi_level);
  }

  void CallSettings(const TestSettings& settings) const {
    ON_CALL(mock_request_controller_settings_, app_hmi_level_none_time_scale())
        .WillByDefault(
            ReturnRef(settings.app_hmi_level_none_requests_time_scale_));

    ON_CALL(mock_request_controller_settings_,
            app_hmi_level_none_time_scale_max_requests())
        .WillByDefault(
            ReturnRef(settings.app_hmi_level_none_time_scale_max_requests_));

    ON_CALL(mock_request_controller_settings_, app_time_scale())
        .WillByDefault(ReturnRef(settings.app_requests_time_scale_));

    ON_CALL(mock_request_controller_settings_, app_time_scale_max_requests())
        .WillByDefault(ReturnRef(settings.app_time_scale_max_requests_));

    ON_CALL(mock_request_controller_settings_, pending_requests_amount())
        .WillByDefault(ReturnRef(settings.pending_requests_amount_));
  }

  NiceMock<application_manager_test::MockRequestControlerSettings>
      mock_request_controller_settings_;
  RequestControllerSPtr request_ctrl_;
  RequestPtr empty_mock_request_;
  const TestSettings kDefaultSettings_;
};

TEST_F(RequestControllerTestClass,
       CheckPosibilitytoAdd_ZeroValueLimiters_SUCCESS) {
  // Test case than pending_requests_amount,
  // app_time_scale_max_requests_ and
  // app_hmi_level_none_time_scale_max_requests_ equals 0
  // (in the default settings they setted to 0)
  for (size_t i = 0; i < kMaxRequestAmount; ++i) {
    EXPECT_EQ(RequestController::SUCCESS,
              AddRequest(kDefaultSettings_,
                         GetMockRequest(i),
                         RequestInfo::RequestType::MobileRequest,
                         mobile_apis::HMILevel::HMI_FULL));
  }
}

TEST_F(
    RequestControllerTestClass,
    CheckPosibilitytoAdd_ExcessPendingRequestsAmount_TooManyPendingRequests) {
  TestSettings settings;
  settings.pending_requests_amount_ = kNumberOfRequests;

  request_ctrl_->DestroyThreadpool();

  // Adding requests to fit in pending_requests_amount_
  for (size_t i = 0; i < kNumberOfRequests; ++i) {
    EXPECT_EQ(RequestController::TResult::SUCCESS,
              AddRequest(settings,
                         GetMockRequest(),
                         RequestInfo::RequestType::MobileRequest,
                         mobile_apis::HMILevel::HMI_FULL));
  }

  // Trying to add one more extra request
  // Expect overflow and TOO_MANY_PENDING_REQUESTS result
  EXPECT_EQ(RequestController::TResult::TOO_MANY_PENDING_REQUESTS,
            AddRequest(settings,
                       GetMockRequest(),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_FULL));
}

TEST_F(
    RequestControllerTestClass,
    CheckPosibilitytoAdd_ExcessHMILevelNoneTimeScaleMaxRequests_NoneHMILevelManyRequests) {
  TestSettings settings;
  // Setting time scale and max requests amount per it
  // for none HMILevel type of requests
  settings.app_hmi_level_none_requests_time_scale_ = kTimeScale;
  settings.app_hmi_level_none_time_scale_max_requests_ = kNumberOfRequests;

  request_ctrl_->DestroyThreadpool();

  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();

  // Artificially adding requests to waiting_for_response list
  size_t i = 0;
  for (; i < kNumberOfRequests; ++i) {
    waiting_for_response.Add(
        GetRequestInfo(GetMockRequest(i),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_NONE));
  }
  // Trying to add one more extra request
  // Expect overflow and NONE_HMI_LEVEL_MANY_REQUESTS result
  EXPECT_EQ(RequestController::TResult::NONE_HMI_LEVEL_MANY_REQUESTS,
            AddRequest(settings,
                       GetMockRequest(i),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_NONE));
}

TEST_F(RequestControllerTestClass,
       CheckPosibilitytoAdd_ExcessTimeScaleMaxRequest_ToManyRequests) {
  TestSettings settings;
  // Setting time scale and max requests amount per it
  settings.app_requests_time_scale_ = kTimeScale;
  settings.app_time_scale_max_requests_ = kNumberOfRequests;

  request_ctrl_->DestroyThreadpool();

  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();

  // Artificially adding requests to waiting_for_response list
  size_t i = 0u;
  for (; i < kNumberOfRequests; ++i) {
    waiting_for_response.Add(
        GetRequestInfo(GetMockRequest(i),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::INVALID_ENUM));
  }
  // Trying to add one more extra request
  // Expect overflow and TOO_MANY_REQUESTS result
  EXPECT_EQ(RequestController::TResult::TOO_MANY_REQUESTS,
            AddRequest(settings,
                       GetMockRequest(i),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::INVALID_ENUM));
}

TEST_F(RequestControllerTestClass, IsLowVoltage_SetOnLowVoltage_TRUE) {
  request_ctrl_->OnLowVoltage();
  const bool result = true;
  EXPECT_EQ(result, request_ctrl_->IsLowVoltage());
}

TEST_F(RequestControllerTestClass, IsLowVoltage_SetOnWakeUp_FALSE) {
  request_ctrl_->OnWakeUp();
  const bool result = false;
  EXPECT_EQ(result, request_ctrl_->IsLowVoltage());
}

TEST_F(RequestControllerTestClass, AddMobileRequest_SetValidData_SUCCESS) {
  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       GetMockRequest(),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_FULL));
}

TEST_F(RequestControllerTestClass,
       AddMobileRequest_SetInvalidData_INVALID_DATA) {
  EXPECT_EQ(RequestController::INVALID_DATA,
            AddRequest(kDefaultSettings_,
                       empty_mock_request_,
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_NONE));
}

TEST_F(RequestControllerTestClass, AddHMIRequest_AddRequest_SUCCESS) {
  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       GetMockRequest(),
                       RequestInfo::RequestType::HMIRequest));
}

TEST_F(RequestControllerTestClass, AddHMIRequest_AddInvalidData_INVALID_DATA) {
  EXPECT_EQ(RequestController::INVALID_DATA,
            AddRequest(kDefaultSettings_,
                       empty_mock_request_,
                       RequestInfo::RequestType::HMIRequest));
}

TEST_F(RequestControllerTestClass,
       TerminateRequest_TerminateExistingRequest_SUCCESS) {
  const bool force_terminate = false;
  RequestPtr mock_request =
      GetMockRequest(kDefaultCorrelationID, kDefaultConnectionKey);

  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_, mock_request));

  // Check if request is added to waiting_for_response list
  EXPECT_EQ(1u, waiting_for_response.Size());

  // Expect call of 'AllowedToTerminate method' cause 'force_terminate' is
  // 'false'
  EXPECT_CALL(*mock_request, AllowedToTerminate()).WillOnce(Return(true));

  request_ctrl_->terminateRequest(
      kDefaultCorrelationID, kDefaultConnectionKey, force_terminate);

  // Check if request is terminated
  EXPECT_EQ(0u, waiting_for_response.Size());
}

TEST_F(RequestControllerTestClass,
       TerminateRequest_ForceToTerminateExistingRequest_SUCCESS) {
  const bool force_terminate = true;
  RequestPtr mock_request =
      GetMockRequest(kDefaultCorrelationID, kDefaultConnectionKey);

  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_, mock_request));

  // Check if request is added to waiting_for_response list
  EXPECT_EQ(1u, waiting_for_response.Size());

  // Call is not expected because request termination will be forced
  EXPECT_CALL(*mock_request, AllowedToTerminate()).Times(0);

  request_ctrl_->terminateRequest(
      kDefaultCorrelationID, kDefaultConnectionKey, force_terminate);

  // Check if request is terminated
  EXPECT_EQ(0u, waiting_for_response.Size());
}

TEST_F(RequestControllerTestClass,
       TerminateRequest_TerminateNotExistingRequest_UNSUCCESS) {
  const bool force_terminate = false;
  RequestPtr mock_request =
      GetMockRequest(kDefaultCorrelationID, kDefaultConnectionKey);

  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_, mock_request));

  // Check if request is added to waiting_for_response list
  EXPECT_EQ(1u, waiting_for_response.Size());

  EXPECT_CALL(*mock_request, AllowedToTerminate()).Times(0u);

  // Trying to terminate request from not existing connection key
  const uint32_t correlation_id = 2u;
  const uint32_t kConnectionKey = 1u;
  request_ctrl_->terminateRequest(
      correlation_id, kConnectionKey, force_terminate);

  // Check if request is still in waiting_for_response list
  EXPECT_EQ(1u, waiting_for_response.Size());
}

TEST_F(RequestControllerTestClass,
       UpdateRequestTimeout_UpdateTimeoutToExistingRequest_SUCCESS) {
  const uint32_t default_timeout = 15u;
  const uint32_t updated_timeout = 20u;
  RequestPtr mock_request = GetMockRequest(
      kDefaultCorrelationID, kDefaultConnectionKey, default_timeout);

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_, mock_request));

  request_ctrl_->updateRequestTimeout(
      kDefaultConnectionKey, kDefaultCorrelationID, updated_timeout);

  // Getting updated request
  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();
  const RequestInfoPtr& request =
      waiting_for_response.Find(kDefaultConnectionKey, kDefaultCorrelationID);

  EXPECT_EQ(updated_timeout, request->timeout_msec());
}

TEST_F(RequestControllerTestClass, AddNotification_RemoveNotification_SUCCESS) {
  RequestPtr mock_requests[kNumberOfRequests];

  for (size_t i = 0u; i < kNumberOfRequests; ++i) {
    mock_requests[i] = GetMockRequest(i);
    request_ctrl_->addNotification(mock_requests[i]);
  }

  const RequestPtrList& notification_list =
      request_ctrl_->get_notification_list();

  EXPECT_EQ(kNumberOfRequests, notification_list.size());

  for (size_t i = 0u; i < kNumberOfRequests; ++i) {
    request_ctrl_->removeNotification(mock_requests[i].get());
  }

  EXPECT_EQ(0u, notification_list.size());
}

TEST_F(RequestControllerTestClass,
       TerminateWaitingForResponseAppRequests_SUCCESS) {
  const uint32_t kConnectionKey = RequestInfo::HmiConnectoinKey;

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       GetMockRequest(1u, kConnectionKey),
                       RequestInfo::RequestType::HMIRequest));

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       GetMockRequest(2u, kConnectionKey),
                       RequestInfo::RequestType::HMIRequest));

  RequestInfoSet& waiting_for_response =
      request_ctrl_->get_waiting_for_response();

  // Checking size of waiting_for_response list before terminating all HMI
  // requests
  EXPECT_EQ(2u, waiting_for_response.Size());

  request_ctrl_->terminateAllHMIRequests();

  // Expect empty waiting_for_response list
  EXPECT_EQ(0u, waiting_for_response.Size());
}

TEST_F(RequestControllerTestClass,
       TerminateWaitingForExecutionAppRequests_SUCCESS) {
  const uint32_t kConnectionKey = 3u;

  request_ctrl_->DestroyThreadpool();

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       GetMockRequest(1u, kConnectionKey),
                       RequestInfo::RequestType::MobileRequest));

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       GetMockRequest(2u, kConnectionKey),
                       RequestInfo::RequestType::MobileRequest));

  const RequestInfoPtrList& mobile_request_info_list =
      request_ctrl_->get_mobile_request_info_list();

  // Checking size of waiting_for_response list before terminating all requests
  EXPECT_EQ(2u, mobile_request_info_list.size());

  request_ctrl_->terminateAppRequests(kConnectionKey);

  // Expect empty mobile_request_info_list list
  EXPECT_EQ(0u, mobile_request_info_list.size());
}

TEST_F(RequestControllerTestClass, OnTimer_SUCCESS) {
  const uint32_t request_timeout = 1u;
  RequestPtr mock_request = GetMockRequest(
      kDefaultCorrelationID, kDefaultConnectionKey, request_timeout);

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(kDefaultSettings_,
                       mock_request,
                       RequestInfo::RequestType::MobileRequest));

  EXPECT_CALL(*mock_request, onTimeOut());

  // Waiting for call of `onTimeOut` for `kTimeScale` seconds
  testing::Mock::AsyncVerifyAndClearExpectations(kTimeScale);
}

}  // namespace request_controller_test
}  // namespace components
}  // namespace test
