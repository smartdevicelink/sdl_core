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
#ifndef SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_ACCESS_REMOTE_H_
#define SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_ACCESS_REMOTE_H_

#include <vector>
#include <ostream>
#include <string>
#include "policy/policy_table/types.h"
#include "policy/policy_types.h"

namespace policy_table = ::rpc::policy_table_interface_base;

namespace policy {
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
   * Checks permissions for module
   * @param app_id application ID
   * @param module type
   * @return true if allowed
   */
  virtual bool CheckModuleType(const PTString& app_id,
                               policy_table::ModuleType module) const = 0;

  /**
   * Sets HMI types if application has default policy permissions
   * @param who subject
   * @param hmi_types list of HMI types
   */
  virtual void SetDefaultHmiTypes(const Subject& who,
                                  const std::vector<int>& hmi_types) = 0;

  /**
   * Gets groups
   * @param who subject
   * @return list of groups
   */
  virtual const policy_table::Strings& GetGroups(const Subject& who) = 0;

  /**
   * Gets permissions for application
   * @param device_id
   * @param app_id
   * @param group_types
   * @return true if success
   */
  virtual bool GetPermissionsForApp(const std::string& device_id,
                                    const std::string& app_id,
                                    FunctionalIdType& group_types) = 0;

  /**
   * Checks if application has remote functionality
   * @param who subject
   * @return true if application uses remote control
   */
  virtual bool IsAppRemoteControl(const Subject& who) = 0;

  /**
   * Gets all allowed module types
   * @param app_id unique identifier of application
   * @param list of allowed module types
   * @return true if application has allowed modules
   */
  virtual bool GetModuleTypes(const std::string& policy_app_id,
                              std::vector<std::string>* modules) = 0;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_ACCESS_REMOTE_H_
