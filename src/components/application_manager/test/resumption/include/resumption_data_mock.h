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
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_RESUMPTION_INCLUDE_RESUMPTION_DATA_MOCK_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_RESUMPTION_INCLUDE_RESUMPTION_DATA_MOCK_H_
#include <string>
#include "gmock/gmock.h"
#include "application_manager/resumption/resumption_data.h"
#include "application_manager/application.h"

namespace test {
namespace components {
namespace resumption_test {
namespace app_mngr = application_manager;
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

class ResumptionDataMock : public ::resumption::ResumptionData {
 public:
  MOCK_METHOD1(SaveApplication,
               void(app_mngr::ApplicationSharedPtr application));
  MOCK_CONST_METHOD2(GetStoredHMILevel,
                     int32_t(const std::string& policy_app_id,
                             const std::string& device_id));
  MOCK_CONST_METHOD1(IsHMIApplicationIdExist, bool(uint32_t hmi_app_id));
  MOCK_METHOD2(CheckSavedApplication, bool(const std::string& policy_app_id,
                                           const std::string& device_id));
  MOCK_CONST_METHOD2(GetHMIApplicationID,
                     uint32_t(const std::string& policy_app_id,
                              const std::string& device_id));
  MOCK_METHOD0(OnSuspend, void());
  MOCK_CONST_METHOD3(GetHashId,
                     bool(const std::string& policy_app_id,
                          const std::string& device_id, std::string& hash_id));
  MOCK_METHOD0(OnAwake, void());
  MOCK_CONST_METHOD3(GetSavedApplication,
                     bool(const std::string& policy_app_id,
                          const std::string& device_id,
                          smart_objects::SmartObject& saved_app));
  MOCK_METHOD2(RemoveApplicationFromSaved,
               bool(const std::string& policy_app_id,
                    const std::string& device_id));
  MOCK_CONST_METHOD0(GetIgnOffTime, uint32_t());
  MOCK_CONST_METHOD2(IsApplicationSaved,
                     ssize_t(const std::string& policy_app_id,
                             const std::string& device_id));
  MOCK_CONST_METHOD1(GetDataForLoadResumeData,
                     void(smart_objects::SmartObject& saved_data));
  MOCK_METHOD3(UpdateHmiLevel, void(const std::string& policy_app_id,
                                    const std::string& device_id,
                                    mobile_apis::HMILevel::eType hmi_level));
  MOCK_METHOD0(Init, bool());
  MOCK_METHOD2(DropAppDataResumption,
               bool(const std::string& device_id, const std::string& app_id));
  MOCK_METHOD0(Persist, void());
};

}  // namespace resumption_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_RESUMPTION_INCLUDE_RESUMPTION_DATA_MOCK_H_
