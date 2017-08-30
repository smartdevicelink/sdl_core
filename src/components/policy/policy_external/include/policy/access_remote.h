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
#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_H_

#include <vector>
#include <ostream>
#include <string>
#include "policy/policy_table/types.h"
#include "policy/policy_types.h"

namespace policy_table = ::rpc::policy_table_interface_base;

namespace policy {

enum TypeAccess { kDisallowed, kAllowed };
inline std::ostream& operator<<(std::ostream& output, TypeAccess x) {
  output << "Access: ";
  switch (x) {
    case kDisallowed:
      output << "DISALLOWED";
      break;
    case kAllowed:
      output << "ALLOWED";
      break;
    default:
      output << "Error: Unknown type";
  }
  return output;
}

struct Subject {
  PTString dev_id;
  PTString app_id;
};
inline bool operator<(const Subject& x, const Subject& y) {
  return x.dev_id < y.dev_id || (x.dev_id == y.dev_id && x.app_id < y.app_id);
}
inline bool operator==(const Subject& x, const Subject& y) {
  return x.dev_id == y.dev_id && x.app_id == y.app_id;
}
inline std::ostream& operator<<(std::ostream& output, const Subject& who) {
  output << "Subject(dev:" << who.dev_id << ", app:" << who.app_id << ")";
  return output;
}

typedef std::vector<PTString> RemoteControlParams;

class AccessRemote {
 public:
  virtual ~AccessRemote() {}
  /**
   * @brief CheckModuleType check if module type is allowed for application
   * @param app_id application id
   * @param module module
   * @return true if allowed, if not - false
   */
  virtual bool CheckModuleType(const PTString& app_id,
                               policy_table::ModuleType module) const = 0;

  /**
   * @brief SetDefaultHmiTypes setup default hmi typed for application
   * @param who application on specific device
   * @param hmi_types hmi types list
   */
  virtual void SetDefaultHmiTypes(const Subject& who,
                                  const std::vector<int>& hmi_types) = 0;

  /**
   * @brief GetGroups return list of groups for applicaiton
   * @param who application on specific device
   * @return list of groups
   */
  virtual const policy_table::Strings& GetGroups(const Subject& who) = 0;

  /**
   * @brief GetPermissionsForApp read list of permissions for application
   * @param device_id device
   * @param app_id application
   * @param group_types output parameter for permissions
   * @return true
   */
  virtual bool GetPermissionsForApp(const std::string& device_id,
                                    const std::string& app_id,
                                    FunctionalIdType& group_types) = 0;

  /**
   * @brief IsAppRemoteControl check is app is remote controll
   * @param who application on specific device
   * @return true is remote controll aotherwise return false
   */
  virtual bool IsAppRemoteControl(const Subject& who) = 0;

  /**
   * @brief GetModuleTypes get list of module types of application
   * @param policy_app_id application id
   * @param modules output parameter for module types
   * @return true on success otherwise false
   */
  virtual bool GetModuleTypes(const std::string& policy_app_id,
                              std::vector<std::string>* modules) = 0;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_H_
