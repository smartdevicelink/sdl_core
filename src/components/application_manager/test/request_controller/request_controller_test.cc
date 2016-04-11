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
#include "application_manager/request_controller.h"
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

using application_manager::request_controller::RequestController;
using application_manager::request_controller::RequestInfo;

using ::testing::Return;
using ::testing::ReturnRef;

typedef utils::SharedPtr<application_manager_test::MockRequest> RequestPtr;
typedef utils::SharedPtr<RequestController> RequestControllerSPtr;

class RequestControllerTestClass : public ::testing::Test {
 public:
  RequestControllerTestClass()
      : request_ctrl_(utils::MakeShared<RequestController>(mock_request_controller_settings_)) {
    register_request_ = GetMockRequest();
  }

  RequestController::TResult AddRequest(
      const RequestInfo::RequestType request_type,
      const bool RegisterRequest = false,
      const mobile_apis::HMILevel::eType& hmi_level =
          mobile_apis::HMILevel::INVALID_ENUM) {
    RequestPtr request;
    if (RegisterRequest) {
      request = register_request_;
      EXPECT_CALL(*register_request_, default_timeout()).WillOnce(Return(0));
    } else {
      request = empty_register_request_;
    }
    if (RequestInfo::RequestType::HMIRequest == request_type) {
      return request_ctrl_->addHMIRequest(request);
    }
    return request_ctrl_->addMobileRequest(request, hmi_level);
  }

  RequestPtr GetMockRequest(const uint32_t id = 1,
                            const uint32_t connection_key = 0) {
    return utils::MakeShared<application_manager_test::MockRequest>(
        connection_key, id);
  }
  void CallSettings() {
    EXPECT_CALL(mock_request_controller_settings_,
                app_hmi_level_none_time_scale())
        .WillOnce(ReturnRef(kDefaultAppHmiLevelNoneRequestsTimeScale));
    EXPECT_CALL(mock_request_controller_settings_,
                app_hmi_level_none_time_scale_max_requests())
        .WillOnce(ReturnRef(kDefaultAppHmiLevelNoneTimeScaleMaxRequests));

    EXPECT_CALL(mock_request_controller_settings_, app_time_scale())
        .WillOnce(ReturnRef(kDefaultAppRequestsTimeScale));
    EXPECT_CALL(mock_request_controller_settings_,
                app_time_scale_max_requests())
        .WillOnce(ReturnRef(kDefaultAppTimeScaleMaxRequests));

    EXPECT_CALL(mock_request_controller_settings_, pending_requests_amount())
        .WillOnce(ReturnRef(kDefaultPendingRequestsAmount));
  }

  application_manager_test::MockRequestControlerSettings
      mock_request_controller_settings_;
  RequestPtr register_request_;
  RequestPtr empty_register_request_;
  RequestControllerSPtr request_ctrl_;

  const uint32_t kDefaultAppHmiLevelNoneRequestsTimeScale = 10;
  const uint32_t kDefaultAppHmiLevelNoneTimeScaleMaxRequests = 100u;
  const uint32_t kDefaultAppTimeScaleMaxRequests = 0;
  const uint32_t kDefaultAppRequestsTimeScale = 0;
  const uint32_t kDefaultPendingRequestsAmount = 0;
};

TEST_F(RequestControllerTestClass, CheckPosibilitytoAdd_HMI_FULL_SUCCESS) {
  CallSettings();
  EXPECT_EQ(RequestController::TResult::SUCCESS,
            AddRequest(RequestInfo::RequestType::MobileRequest, true,
                       mobile_apis::HMILevel::HMI_FULL));
}

TEST_F(RequestControllerTestClass, CheckPosibilitytoAdd_HMI_NONE_SUCCESS) {
  CallSettings();
  EXPECT_EQ(RequestController::TResult::SUCCESS,
            AddRequest(RequestInfo::RequestType::MobileRequest, true,
                       mobile_apis::HMILevel::HMI_NONE));
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

TEST_F(RequestControllerTestClass,
       AddMobileRequest_SetInvalidData_INVALID_DATA) {
  EXPECT_EQ(RequestController::INVALID_DATA,
            AddRequest(RequestInfo::RequestType::MobileRequest,
                       false,
                       mobile_apis::HMILevel::HMI_NONE));
}

TEST_F(RequestControllerTestClass, addHMIRequest_AddRequest_SUCCESS) {
  EXPECT_EQ(RequestController::SUCCESS,
            AddRequest(RequestInfo::RequestType::HMIRequest, true));
}

TEST_F(RequestControllerTestClass, addHMIRequest_AddInvalidData_INVALID_DATA) {
  EXPECT_EQ(RequestController::INVALID_DATA,
            AddRequest(RequestInfo::RequestType::HMIRequest));
}

TEST_F(RequestControllerTestClass, ZeroValuePendingRequestsAmount) {
  // Bigger than pending_requests_amount count
  const uint32_t big_count_of_requests_for_test_ = 10;
  for (uint32_t i = 0; i < big_count_of_requests_for_test_; ++i) {
    CallSettings();
    EXPECT_EQ(RequestController::SUCCESS, AddRequest(RequestInfo::RequestType::MobileRequest,
                                                     true,
                                                     mobile_apis::HMILevel::HMI_FULL));
  }
}


}  // namespace request_controller
}  // namespace components
}  // namespace test
