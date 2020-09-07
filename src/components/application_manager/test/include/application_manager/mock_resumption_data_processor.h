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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RESUMPTION_DATA_PROCESSOR_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RESUMPTION_DATA_PROCESSOR_H

#include "application_manager/application.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/resumption/resumption_data_processor.h"

#include "gmock/gmock.h"

namespace test {
namespace components {
namespace resumption_test {

class MockResumptionDataProcessor : public resumption::ResumptionDataProcessor {
 public:
  MOCK_METHOD3(Restore,
               void(application_manager::ApplicationSharedPtr application,
                    smart_objects::SmartObject& saved_app,
                    resumption::ResumeCtrl::ResumptionCallBack callback));
  MOCK_METHOD2(HandleOnTimeOut,
               void(const uint32_t correlation_id,
                    const hmi_apis::FunctionID::eType function_id));
  MOCK_METHOD2(SubscribeToResponse,
               void(const int32_t app_id,
                    const resumption::ResumptionRequest& request));
};

}  // namespace resumption_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RESUMPTION_DATA_PROCESSOR_H
