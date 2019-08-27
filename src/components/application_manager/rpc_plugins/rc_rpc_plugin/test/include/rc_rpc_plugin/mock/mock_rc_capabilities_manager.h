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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_TEST_INCLUDE_RC_RPC_PLUGIN_MOCK_MOCK_RC_CAPABILITIES_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_TEST_INCLUDE_RC_RPC_PLUGIN_MOCK_MOCK_RC_CAPABILITIES_MANAGER_H_
#include "gmock/gmock.h"
#include "rc_rpc_plugin/rc_capabilities_manager.h"

namespace rc_rpc_plugin_test {
class MockRCCapabilitiesManager : public rc_rpc_plugin::RCCapabilitiesManager {
 public:
  MOCK_CONST_METHOD2(CheckButtonName,
                     bool(const std::string& module_type,
                          const std::string& button_name));

  MOCK_CONST_METHOD1(CheckIfModuleExistsInCapabilities,
                     bool(const rc_rpc_plugin::ModuleUid& module_type));

  MOCK_CONST_METHOD2(GetModuleDataCapabilities,
                     rc_rpc_plugin::ModuleTypeCapability(
                         const smart_objects::SmartObject& module_data,
                         const std::string& module_id));

  MOCK_CONST_METHOD2(ControlDataForType,
                     const smart_objects::SmartObject&(
                         const smart_objects::SmartObject& module_data,
                         const std::string& module_type));

  MOCK_CONST_METHOD3(
      AreReadOnlyParamsPresent,
      bool(const smart_objects::SmartObject& module_data,
           const std::string& module_type,
           rc_rpc_plugin::ModuleTypeCapability& module_data_capabilities));

  MOCK_CONST_METHOD2(AreAllParamsReadOnly,
                     bool(const smart_objects::SmartObject& module_data,
                          const std::string& module_type));

  MOCK_CONST_METHOD1(GetDefaultModuleIdFromCapabilities,
                     const std::string(const std::string& module_type));

  MOCK_CONST_METHOD1(CheckIfButtonExistInRCCaps,
                     bool(const mobile_apis::ButtonName::eType button));

  MOCK_CONST_METHOD0(GetResources,
                     const std::vector<rc_rpc_plugin::ModuleUid>());

  MOCK_CONST_METHOD1(GetModuleServiceArea,
                     rc_rpc_plugin::Grid(const rc_rpc_plugin::ModuleUid&));

  MOCK_CONST_METHOD1(IsMultipleAccessAllowed,
                     bool(const rc_rpc_plugin::ModuleUid&));

  MOCK_CONST_METHOD0(GetDriverLocationFromSeatLocationCapability,
                     const rc_rpc_plugin::Grid());

  MOCK_CONST_METHOD0(IsSeatLocationCapabilityProvided, bool());

  MOCK_CONST_METHOD1(
      GetModuleIdForSeatLocation,
      const std::string(const mobile_apis::SupportedSeat::eType id));
};
}  // namespace rc_rpc_plugin_test
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_TEST_INCLUDE_RC_RPC_PLUGIN_MOCK_MOCK_RC_СAPABILITIES_MANAGER_H_
