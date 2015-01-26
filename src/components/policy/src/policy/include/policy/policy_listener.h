/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_LISTENER_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_LISTENER_H_

#include "policy/policy_types.h"

namespace policy {
class PolicyListener {
 public:
  virtual ~PolicyListener() {
  }
  virtual void OnPTExchangeNeeded() = 0;
  virtual void OnPermissionsUpdated(const std::string& policy_app_id,
                                    const Permissions& permissions,
                                    const policy::HMILevel& default_hmi) = 0;
  virtual void OnPendingPermissionChange(const std::string& policy_app_id) = 0;
  virtual void OnAppRevoked(const std::string& policy_app_id) = 0;
  virtual void OnUpdateStatusChanged(policy::PolicyTableStatus status) = 0;
  virtual std::string OnCurrentDeviceIdUpdateRequired(
      const std::string& policy_app_id) = 0;
  virtual void OnSystemInfoUpdateRequired() = 0;
  virtual std::string GetAppName(const std::string& policy_app_id) = 0;
  virtual void OnUserRequestedUpdateCheckRequired() = 0;

  /**
   * @brief Make appropriate changes for related applications permissions and
   * notify them, if it possible
   * @param device_id Unique device id, which consent had been changed
   * @param device_consent Device consent, which is done by user
   */
  virtual void OnDeviceConsentChanged(const std::string& device_id,
                                      bool is_allowed) = 0;
};
}  //  namespace policy
#endif  //  SRC_COMPONENTS_POLICY_INCLUDE_POLICY_LISTENER_H_
