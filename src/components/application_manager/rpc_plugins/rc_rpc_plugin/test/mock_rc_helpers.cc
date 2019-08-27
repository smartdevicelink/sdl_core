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

#include "rc_rpc_plugin/mock/mock_rc_helpers.h"

namespace rc_rpc_plugin {

const std::function<std::string(const std::string&)>
rc_rpc_plugin::RCHelpers::GetModuleTypeToDataMapping() {
  return MockRCHelpers::rc_helpers_mock()->GetModuleTypeToDataMapping();
}

const std::function<std::string(const std::string&)>
rc_rpc_plugin::RCHelpers::GetModuleTypeToCapabilitiesMapping() {
  return MockRCHelpers::rc_helpers_mock()->GetModuleTypeToCapabilitiesMapping();
}

const std::vector<std::string> rc_rpc_plugin::RCHelpers::GetModuleTypesList() {
  return MockRCHelpers::rc_helpers_mock()->GetModuleTypesList();
}

rc_rpc_plugin::RCAppExtensionPtr rc_rpc_plugin::RCHelpers::GetRCExtension(
    application_manager::Application& app) {
  return MockRCHelpers::rc_helpers_mock()->GetRCExtension(app);
}

smart_objects::SmartObjectSPtr
rc_rpc_plugin::RCHelpers::CreateUnsubscribeRequestToHMI(
    const ModuleUid& module, const uint32_t correlation_id) {
  return MockRCHelpers::rc_helpers_mock()->CreateUnsubscribeRequestToHMI(
      module, correlation_id);
}

std::vector<application_manager::ApplicationSharedPtr>
rc_rpc_plugin::RCHelpers::AppsSubscribedToModule(
    application_manager::ApplicationManager& app_mngr,
    const ModuleUid& module) {
  return MockRCHelpers::rc_helpers_mock()->AppsSubscribedToModule(app_mngr,
                                                                  module);
}

std::vector<application_manager::ApplicationSharedPtr>
rc_rpc_plugin::RCHelpers::AppsSubscribedToModuleType(
    application_manager::ApplicationManager& app_mngr,
    const std::string& module_type) {
  return MockRCHelpers::rc_helpers_mock()->AppsSubscribedToModuleType(
      app_mngr, module_type);
}

rc_rpc_plugin::RCHelpers::AppsModuleTypes
rc_rpc_plugin::RCHelpers::GetApplicationsAllowedModuleTypes(
    application_manager::ApplicationManager& app_mngr) {
  return MockRCHelpers::rc_helpers_mock()->GetApplicationsAllowedModuleTypes(
      app_mngr);
}

const std::vector<std::string> RCHelpers::buttons_climate() {
  return MockRCHelpers::rc_helpers_mock()->buttons_climate();
}

const std::vector<std::string> RCHelpers::buttons_radio() {
  return MockRCHelpers::rc_helpers_mock()->buttons_radio();
}

const rc_rpc_plugin::RCHelpers::ButtonsMap RCHelpers::buttons_map() {
  return MockRCHelpers::rc_helpers_mock()->buttons_map();
}

std::vector<std::string> RCHelpers::GetModuleReadOnlyParams(
    const std::string& module_type) {
  return MockRCHelpers::rc_helpers_mock()->GetModuleReadOnlyParams(module_type);
}

rc_rpc_types::ModuleIdConsentVector RCHelpers::FillModuleConsents(
    const std::string& module_type,
    const std::vector<std::string>& module_ids,
    const std::vector<bool> allowed) {
  return MockRCHelpers::rc_helpers_mock()->FillModuleConsents(
      module_type, module_ids, allowed);
}

std::vector<std::string> RCHelpers::RetrieveModuleIds(
    const smart_objects::SmartObject& moduleIds) {
  return MockRCHelpers::rc_helpers_mock()->RetrieveModuleIds(moduleIds);
}

std::vector<bool> RCHelpers::RetrieveModuleConsents(
    const smart_objects::SmartObject& consents) {
  return MockRCHelpers::rc_helpers_mock()->RetrieveModuleConsents(consents);
}

void RCHelpers::RemoveRedundantGPSDataFromIVDataMsg(
    smart_objects::SmartObject& msg_params) {
  return MockRCHelpers::rc_helpers_mock()->RemoveRedundantGPSDataFromIVDataMsg(
      msg_params);
}

MockRCHelpers* MockRCHelpers::rc_helpers_mock() {
  static ::testing::NiceMock<MockRCHelpers> mock_rc_helpers;
  return &mock_rc_helpers;
}

}  // namespace rc_rpc_plugin
