/*
 * Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RC_RPC_PLUGINS_RC_RPC_PLUGIN_TEST_INCLUDE_RC_RPC_PLUGIN_MOCK_MOCK_RC_HELPERS_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RC_RPC_PLUGINS_RC_RPC_PLUGIN_TEST_INCLUDE_RC_RPC_PLUGIN_MOCK_MOCK_RC_HELPERS_H

#include <functional>
#include <map>
#include <string>
#include "gmock/gmock.h"

#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "rc_rpc_plugin/rc_helpers.h"

namespace rc_rpc_plugin {

class MockRCHelpers {
 public:
  MOCK_METHOD0(GetModuleTypeToDataMapping,
               std::function<std::string(const std::string&)>());
  MOCK_METHOD0(GetModuleTypeToCapabilitiesMapping,
               std::function<std::string(const std::string&)>());
  MOCK_METHOD0(GetModuleTypesList, const std::vector<std::string>());
  MOCK_METHOD1(
      GetRCExtension,
      rc_rpc_plugin::RCAppExtensionPtr(application_manager::Application&));
  MOCK_METHOD2(CreateUnsubscribeRequestToHMI,
               smart_objects::SmartObjectSPtr(const rc_rpc_plugin::ModuleUid&,
                                              const uint32_t));
  MOCK_METHOD2(AppsSubscribedToModule,
               std::vector<application_manager::ApplicationSharedPtr>(
                   application_manager::ApplicationManager&,
                   const rc_rpc_plugin::ModuleUid&));
  MOCK_METHOD2(AppsSubscribedToModuleType,
               std::vector<application_manager::ApplicationSharedPtr>(
                   application_manager::ApplicationManager&,
                   const std::string&));
  MOCK_METHOD1(GetApplicationsAllowedModuleTypes,
               rc_rpc_plugin::RCHelpers::AppsModuleTypes(
                   application_manager::ApplicationManager&));
  MOCK_METHOD0(buttons_climate, const std::vector<std::string>());
  MOCK_METHOD0(buttons_radio, const std::vector<std::string>());
  MOCK_METHOD0(buttons_map, const rc_rpc_plugin::RCHelpers::ButtonsMap());
  MOCK_METHOD1(GetModuleReadOnlyParams,
               std::vector<std::string>(const std::string&));
  MOCK_METHOD3(
      FillModuleConsents,
      rc_rpc_types::ModuleIdConsentVector(const std::string&,
                                          const std::vector<std::string>&,
                                          const std::vector<bool>));
  MOCK_METHOD1(RetrieveModuleIds,
               std::vector<std::string>(const smart_objects::SmartObject&));
  MOCK_METHOD1(RetrieveModuleConsents,
               std::vector<bool>(const smart_objects::SmartObject& consents));
  MOCK_METHOD1(RemoveRedundantGPSDataFromIVDataMsg,
               void(smart_objects::SmartObject& msg_params));

  static MockRCHelpers* rc_helpers_mock();
};

}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RC_RPC_PLUGINS_RC_RPC_PLUGIN_TEST_INCLUDE_RC_RPC_PLUGIN_MOCK_MOCK_RC_HELPERS_H
