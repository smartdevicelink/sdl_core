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
#ifndef TEST_COMPONENTS_CAN_COOPERATION_INCLUDE_MOCK_SERVICE_H_
#define TEST_COMPONENTS_CAN_COOPERATION_INCLUDE_MOCK_SERVICE_H_

#include "gmock/gmock.h"
#include "application_manager/service.h"

namespace application_manager {

class MockService : public Service {
 public:
  MOCK_METHOD1(CheckPolicyPermissions,
               mobile_apis::Result::eType(MessagePtr msg));
  MOCK_METHOD1(GetApplication, ApplicationSharedPtr(ApplicationId app_id));
  MOCK_METHOD1(SendMessageToHMI, void(const MessagePtr& message));
  MOCK_METHOD1(SendMessageToMobile, void(const MessagePtr& message));
  MOCK_METHOD0(GetNextCorrelationID, uint32_t());
  MOCK_METHOD1(GetApplications,
               std::vector<ApplicationSharedPtr>(AppExtensionUID));
  MOCK_METHOD1(SubscribeToHMINotification,
               void(const std::string& hmi_notification));
  MOCK_METHOD2(ChangeNotifyHMILevel,
               void(ApplicationSharedPtr app,
                    mobile_apis::HMILevel::eType level));
  MOCK_METHOD2(NotifyHMIAboutHMILevel,
               void(ApplicationSharedPtr app,
                    mobile_apis::HMILevel::eType level));
  MOCK_METHOD5(CheckAccess,
               TypeAccess(const ApplicationId& app_id,
                          const SeatLocation& zone,
                          const std::string& module,
                          const std::string& rpc,
                          const std::vector<std::string>& params));
  MOCK_METHOD2(CheckModule,
               bool(const ApplicationId& app_id, const std::string& module));
  MOCK_METHOD4(SetAccess,
               void(const ApplicationId& app_id,
                    const SeatLocation& zone,
                    const std::string& module,
                    bool allowed));
  MOCK_METHOD1(ResetAccess, void(const ApplicationId& app_id));
  MOCK_METHOD2(ResetAccess,
               void(const SeatLocation& zone, const std::string& module));
  MOCK_METHOD1(SetPrimaryDevice, void(const uint32_t dev_id));
  MOCK_METHOD0(ResetPrimaryDevice, void());
  MOCK_CONST_METHOD0(PrimaryDevice, uint32_t());
  MOCK_METHOD1(SetRemoteControl, void(bool enabled));
  MOCK_CONST_METHOD0(IsRemoteControlAllowed, bool());
  MOCK_METHOD2(SetDeviceZone,
               void(const uint32_t dev_id, const SeatLocation& zone));
  MOCK_CONST_METHOD1(GetDeviceZone,
                     const SeatLocationPtr(const uint32_t dev_id));
  MOCK_CONST_METHOD1(IsRemoteControlApplication,
                     bool(ApplicationSharedPtr app));
  MOCK_CONST_METHOD2(GetModuleTypes,
                     bool(const std::string& application_id,
                          std::vector<std::string>* modules));
};
}
// namespace application_manager

#endif  // TEST_COMPONENTS_CAN_COOPERATION_INCLUDE_MOCK_SERVICE_H_
