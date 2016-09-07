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

#ifndef SRC_COMPONENTS_VR_MODULE_TEST_INCLUDE_MOCK_SERVICE_MODULE_H_
#define SRC_COMPONENTS_VR_MODULE_TEST_INCLUDE_MOCK_SERVICE_MODULE_H_

#include "vr_module/service_module.h"

namespace vr_module {

class MockServiceModule : public ServiceModule {
 public:
  MOCK_METHOD0(GetNextCorrelationID,
      int32_t());
  MOCK_METHOD2(RegisterRequest,
      void(int32_t correlation_id, commands::TimedCommand* command));
  MOCK_METHOD1(UnregisterRequest,
      void(int32_t correlation_id));
  MOCK_METHOD1(SendToHmi,
      bool(const vr_hmi_api::ServiceMessage& message));
  MOCK_METHOD1(SendToMobile,
      bool(const vr_mobile_api::ServiceMessage& message));
  MOCK_METHOD1(ActivateService,
      void(int32_t app_id));
  MOCK_METHOD0(DeactivateService,
      void());
  MOCK_METHOD1(SetDefaultService,
      void(int32_t app_id));
  MOCK_METHOD0(ResetDefaultService,
      void());
  MOCK_CONST_METHOD0(IsSupported,
      bool());
  MOCK_METHOD0(EnableSupport,
      void());
  MOCK_METHOD0(DisableSupport,
      void());
};

}  // namespace vr_module

#endif  // SRC_COMPONENTS_VR_MODULE_TEST_INCLUDE_MOCK_SERVICE_MODULE_H_
