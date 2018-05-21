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
#include "utils/test_async_waiter.h"

namespace test {
namespace components {
namespace request_controller_test {

using ::application_manager::request_controller::RequestController;
using ::application_manager::request_controller::RequestInfo;

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::_;

typedef NiceMock<application_manager_test::MockRequest> MRequest;
typedef utils::SharedPtr<MRequest> RequestPtr;
typedef utils::SharedPtr<RequestController> RequestControllerSPtr;

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
      const uint32_t connection_key = kDefaultConnectionKey,
      const uint32_t default_timeout = kDefaultTimeout) {
    RequestPtr output =
        utils::MakeShared<MRequest>(connection_key, correlation_id);
    ON_CALL(*output, default_timeout()).WillByDefault(Return(default_timeout));
    ON_CALL(*output, CheckPermissions()).WillByDefault(Return(true));
    return output;
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
  const TestSettings default_settings_;
};

TEST_F(RequestControllerTestClass,
       AddMobileRequest_DuplicateCorrelationId_INVALID_ID) {
  RequestPtr request_valid = GetMockRequest();
  TestAsyncWaiter waiter_valid;
  ON_CALL(*request_valid, default_timeout()).WillByDefault(Return(0));
  EXPECT_CALL(*request_valid, Init()).WillOnce(Return(true));
  EXPECT_CALL(*request_valid, Run())
      .Times(1)
      .WillRepeatedly(NotifyTestAsyncWaiter(&waiter_valid));

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(default_settings_,
                       request_valid,
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_NONE));
  EXPECT_TRUE(waiter_valid.WaitFor(1, 1000));

  // The command should not be run if another command with the same
  // correlation_id is waiting for a response
  RequestPtr request_dup_corr_id = GetMockRequest();
  TestAsyncWaiter waiter_dup;
  ON_CALL(*request_dup_corr_id, default_timeout()).WillByDefault(Return(0));
  EXPECT_CALL(*request_dup_corr_id, Init()).WillOnce(Return(true));
  ON_CALL(*request_dup_corr_id, Run())
      .WillByDefault(NotifyTestAsyncWaiter(&waiter_dup));

  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(default_settings_,
                       request_dup_corr_id,
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_NONE));
  EXPECT_FALSE(waiter_dup.WaitFor(1, 1000));
}

TEST_F(RequestControllerTestClass,
       CheckPosibilitytoAdd_ZeroValueLimiters_SUCCESS) {
  // Test case than pending_requests_amount,
  // app_time_scale_max_requests_ and
  // app_hmi_level_none_time_scale_max_requests_ equals 0
  // (in the default settings they setted to 0)
  for (size_t i = 0; i < kMaxRequestAmount; ++i) {
    EXPECT_EQ(RequestController::SUCCESS,
              AddRequest(default_settings_,
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
            AddRequest(default_settings_,
                       GetMockRequest(),
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_FULL));
}

TEST_F(RequestControllerTestClass,
       AddMobileRequest_SetInvalidData_INVALID_DATA) {
  EXPECT_EQ(RequestController::INVALID_DATA,
            AddRequest(default_settings_,
                       empty_mock_request_,
                       RequestInfo::RequestType::MobileRequest,
                       mobile_apis::HMILevel::HMI_NONE));
}

TEST_F(RequestControllerTestClass, AddHMIRequest_AddRequest_SUCCESS) {
  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(default_settings_,
                       GetMockRequest(),
                       RequestInfo::RequestType::HMIRequest));
}

TEST_F(RequestControllerTestClass, AddHMIRequest_AddInvalidData_INVALID_DATA) {
  EXPECT_EQ(RequestController::INVALID_DATA,
            AddRequest(default_settings_,
                       empty_mock_request_,
                       RequestInfo::RequestType::HMIRequest));
}

TEST_F(RequestControllerTestClass, OnTimer_SUCCESS) {
  const uint32_t request_timeout = 1u;
  RequestPtr mock_request = GetMockRequest(
      kDefaultCorrelationID, kDefaultConnectionKey, request_timeout);

  TestAsyncWaiter waiter;
  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(default_settings_,
                       mock_request,
                       RequestInfo::RequestType::MobileRequest));

  EXPECT_CALL(*mock_request, onTimeOut())
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  // Waiting for call of `onTimeOut` for `kTimeScale` seconds
  EXPECT_TRUE(waiter.WaitFor(1, kTimeScale));
}

}  // namespace request_controller_test
}  // namespace components
}  // namespace test
