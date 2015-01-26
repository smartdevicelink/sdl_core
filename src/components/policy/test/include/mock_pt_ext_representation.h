/* Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_PT_EXT_REPRESENTATION_H_
#define SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_PT_EXT_REPRESENTATION_H_


#include <string>
#include <vector>

#include "gmock/gmock.h"

#include "policy/pt_ext_representation.h"
#include "rpc_base/rpc_base.h"
#include "./types.h"
#include "mock_pt_representation.h"

namespace policy_table = ::rpc::policy_table_interface_base;

namespace policy {
class MockPTExtRepresentation : public MockPTRepresentation,
    public PTExtRepresentation {
 public:
  MOCK_METHOD1(CanAppKeepContext,
      bool(const std::string& app_id));
  MOCK_METHOD1(CanAppStealFocus,
      bool(const std::string& app_id));
  MOCK_METHOD2(GetDefaultHMI,
      bool(const std::string& app_id, std::string* default_hmi));
  MOCK_METHOD0(ResetUserConsent,
      bool());
  MOCK_METHOD0(ResetDeviceConsents, bool());
  MOCK_METHOD0(ResetAppConsents, bool());
  MOCK_METHOD3(GetUserPermissionsForDevice,
      bool(const std::string&, StringArray*, StringArray*));
  MOCK_METHOD3(GetPermissionsForApp,
      bool(const std::string&, const std::string&,
          FunctionalIdType* group_types));
  MOCK_METHOD2(GetDeviceGroupsFromPolicies,
      bool(policy_table::Strings*, policy_table::Strings*));
  MOCK_METHOD2(GetUserFriendlyMsg,
      std::vector<UserFriendlyMessage>(const std::vector<std::string>& msg_code,
          const std::string& language));
  MOCK_METHOD8(SetDeviceData, bool(const std::string& device_id,
          const std::string& hardware,
          const std::string& firmware,
          const std::string& os,
          const std::string& os_version,
          const std::string& carrier,
          const uint32_t number_of_ports,
          const std::string& connection_type));
  MOCK_METHOD6(SetDeviceData,
      bool(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&));
  MOCK_METHOD2(SetMaxNumberPorts,
      bool(const std::string& device_id, unsigned int number_of_ports));
  MOCK_METHOD3(SetUserPermissionsForDevice,
      bool(const std::string&, const StringArray&, const StringArray&));
  MOCK_METHOD1(SetUserPermissionsForApp,
      bool(const PermissionConsent&));
  MOCK_METHOD1(IncreaseStatisticsData,
      bool(StatisticsType type));
  MOCK_METHOD3(SetAppRegistrationLanguage,
      bool(const std::string& app_id, LanguageType type, const std::string& language));
  MOCK_METHOD3(SetMetaInfo,
      bool(const std::string& ccpu_version, const std::string& wers_country_code, const std::string& vin));
  MOCK_METHOD0(IsMetaInfoPresent,
      bool());
  MOCK_METHOD1(SetSystemLanguage,
      bool(const std::string& language));
  MOCK_METHOD0(GetKmFromSuccessfulExchange,
      int());
  MOCK_METHOD0(GetDayFromScsExchange,
      int());
  MOCK_METHOD0(GetIgnitionsFromScsExchange,
      int());
  MOCK_CONST_METHOD1(Increment,
      void(const std::string& type));
  MOCK_CONST_METHOD2(Increment,
      void(const std::string& app_id, const std::string& type));
  MOCK_CONST_METHOD3(Set,
      void(const std::string& app_id, const std::string& type, const std::string& value));
  MOCK_CONST_METHOD3(Add,
      void(const std::string& app_id, const std::string& type, int seconds));
  MOCK_CONST_METHOD3(CountUnconsentedGroups,
      bool(const std::string& app_id,
          const std::string& device_id,
          int* count));
  MOCK_METHOD1(GetFunctionalGroupNames,
      bool(FunctionalGroupNames& names));
  MOCK_CONST_METHOD1(CleanupUnpairedDevices,
      bool(const DeviceIds& device_ids));
  MOCK_METHOD2(ReactOnUserDevConsentForApp,
      bool(const std::string& app_id, bool is_device_allowed));
  MOCK_METHOD1(SetPredataPolicy, bool(const std::string& app_id));
  MOCK_METHOD2(SetIsPredata, bool(const std::string& app_id, bool is_predata));
  MOCK_CONST_METHOD1(SetUnpairedDevice, bool(const std::string& device_id));
  MOCK_CONST_METHOD1(UnpairedDevicesList, bool(DeviceIds* device_ids));
  MOCK_CONST_METHOD2(RemoveAppConsentForGroup, bool(const std::string& policy_app_id,
          const std::string& functional_group));
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_PT_EXT_REPRESENTATION_H_
