/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_APP_SERVICE_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_APP_SERVICE_MANAGER_H_

#include <gmock/gmock.h>
#include "application_manager/app_service_manager.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/command_impl.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockAppServiceManager : public application_manager::AppServiceManager {
 public:
  MockAppServiceManager(application_manager::ApplicationManager& app_manager,
                        resumption::LastState& last_state)
      : application_manager::AppServiceManager(app_manager, last_state) {}
  MOCK_METHOD3(
      PublishAppService,
      smart_objects::SmartObject(const smart_objects::SmartObject& manifest,
                                 const bool mobile_service,
                                 const uint32_t connection_key));
  MOCK_METHOD1(UnpublishAppService, bool(const std::string service_id));
  MOCK_METHOD1(UnpublishServices, void(const uint32_t connection_key));
  MOCK_METHOD1(OnAppActivated,
               void(application_manager::ApplicationConstSharedPtr app));
  MOCK_METHOD1(SetDefaultService, bool(const std::string service_id));
  MOCK_METHOD1(RemoveDefaultService, bool(const std::string service_id));
  MOCK_METHOD1(ActivateAppService, bool(const std::string service_id));
  MOCK_METHOD1(DeactivateAppService, bool(const std::string service_id));
  MOCK_METHOD0(GetAllServiceRecords, std::vector<smart_objects::SmartObject>());
  MOCK_METHOD0(
      GetActiveServices,
      std::vector<std::pair<std::string, application_manager::AppService> >());
  MOCK_METHOD4(GetProviderByType,
               void(const std::string& service_type,
                    const bool mobile_consumer,
                    application_manager::ApplicationSharedPtr& app,
                    bool& hmi_service));
  MOCK_METHOD4(GetProviderByID,
               void(const std::string& service_id,
                    const bool mobile_consumer,
                    application_manager::ApplicationSharedPtr& app,
                    bool& hmi_service));
  MOCK_METHOD1(
      ActiveServiceForType,
      application_manager::AppService*(const std::string service_type));
  MOCK_METHOD1(
      EmbeddedServiceForType,
      application_manager::AppService*(const std::string service_type));
  MOCK_METHOD1(FindServiceByID,
               application_manager::AppService*(const std::string service_id));
  MOCK_METHOD1(DefaultServiceByType,
               std::string(const std::string service_type));
  MOCK_METHOD2(SetServicePublished,
               void(const std::string service_id,
                    const bool service_published));
  MOCK_METHOD1(UpdateNavigationCapabilities,
               bool(smart_objects::SmartObject& out_params));
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_APP_SERVICE_MANAGER_H_
