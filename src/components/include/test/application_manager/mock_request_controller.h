/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_REQUEST_CONTROLLER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_REQUEST_CONTROLLER_H_

#include "application_manager/request_controller.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockRequestController
    : public application_manager::request_controller::RequestController {
 public:
  MOCK_METHOD0(InitializeThreadpool, void());
  MOCK_METHOD0(DestroyThreadpool, void());
  MOCK_METHOD2(
      addMobileRequest,
      TResult(const application_manager::request_controller::RequestPtr request,
              const mobile_apis::HMILevel::eType& hmi_level));
  MOCK_METHOD1(addHMIRequest,
               TResult(const application_manager::request_controller::RequestPtr
                           request));
  MOCK_METHOD1(
      addNotification,
      void(const application_manager::request_controller::RequestPtr request));
  MOCK_METHOD4(TerminateRequest,
               void(const uint32_t correlation_id,
                    const uint32_t connection_key,
                    const int32_t function_id,
                    bool force_terminate));
  MOCK_METHOD3(OnMobileResponse,
               void(const uint32_t mobile_correlation_id,
                    const uint32_t connection_key,
                    const int32_t function_id));
  MOCK_METHOD2(OnHMIResponse,
               void(const uint32_t correlation_id, const int32_t function_id));
  MOCK_METHOD1(
      removeNotification,
      void(const application_manager::commands::Command* notification));
  MOCK_METHOD1(terminateAppRequests, void(const uint32_t& app_id));
  MOCK_METHOD0(terminateAllHMIRequests, void());
  MOCK_METHOD0(terminateAllMobileRequests, void());
  MOCK_METHOD3(updateRequestTimeout,
               void(const uint32_t& app_id,
                    const uint32_t& mobile_correlation_id,
                    const uint32_t& new_timeout));
  MOCK_CONST_METHOD3(IsUpdateRequestTimeoutRequired,
                     bool(const uint32_t app_id,
                          const uint32_t correlation_id,
                          const uint32_t new_timeout));
  MOCK_METHOD0(OnLowVoltage, void());
  MOCK_METHOD0(OnWakeUp, void());
  MOCK_METHOD0(IsLowVoltage, bool());
};
}
}
}

#endif  // SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_REQUEST_CONTROLLER_H_
