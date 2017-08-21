/* Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_POLICY_REGULAR_TEST_INCLUDE_MOCK_POLICY_LISTENER_H_
#define SRC_COMPONENTS_POLICY_POLICY_REGULAR_TEST_INCLUDE_MOCK_POLICY_LISTENER_H_

#include <string>

#include "gmock/gmock.h"

#include "policy/policy_listener.h"
#include "rpc_base/rpc_base.h"
#include "policy/policy_table/types.h"
#include "utils/custom_string.h"

namespace policy_table = ::rpc::policy_table_interface_base;

namespace policy {

namespace custom_str = utils::custom_string;

class MockPolicyListener : public PolicyListener {
 public:
  MOCK_METHOD3(OnPermissionsUpdated,
               void(const std::string& policy_app_id,
                    const Permissions& permissions,
                    const policy::HMILevel& default_hmi));
  MOCK_METHOD2(OnPermissionsUpdated,
               void(const std::string& policy_app_id,
                    const Permissions& permissions));
  MOCK_METHOD1(OnPendingPermissionChange,
               void(const std::string& policy_app_id));
  MOCK_METHOD1(OnUpdateStatusChanged, void(const std::string& status));
  MOCK_METHOD1(OnCurrentDeviceIdUpdateRequired,
               std::string(const std::string& policy_app_id));
  MOCK_METHOD0(OnSystemInfoUpdateRequired, void());
  MOCK_METHOD1(GetAppName,
               custom_str::CustomString(const std::string& policy_app_id));
  MOCK_METHOD0(OnUserRequestedUpdateCheckRequired, void());
  MOCK_METHOD2(OnDeviceConsentChanged,
               void(const std::string& device_id, bool is_allowed));
  MOCK_METHOD1(OnUpdateHMIAppType, void(std::map<std::string, StringArray>));
  MOCK_METHOD1(GetAvailableApps, void(std::queue<std::string>&));
  MOCK_METHOD1(OnSnapshotCreated, void(const BinaryMessage& pt_string));
  MOCK_METHOD0(CanUpdate, bool());
  MOCK_METHOD1(OnCertificateUpdated, void(const std::string&));
  MOCK_CONST_METHOD2(SendOnAppPermissionsChanged,
                     void(const AppPermissions&, const std::string&));
  MOCK_CONST_METHOD1(GetRegisteredLinks,
                     void(std::map<std::string, std::string>&));
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_REGULAR_TEST_INCLUDE_MOCK_POLICY_LISTENER_H_
