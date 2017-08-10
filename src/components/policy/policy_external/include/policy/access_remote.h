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

enum TypeAccess { kDisallowed, kAllowed, kManual };
inline std::ostream& operator<<(std::ostream& output, TypeAccess x) {
  output << "Access: ";
  switch (x) {
    case kDisallowed:
      output << "DISALLOWED";
      break;
    case kAllowed:
      output << "ALLOWED";
      break;
    case kManual:
      output << "MANUAL";
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

struct Object {
  policy_table::ModuleType module;
};
inline bool operator<(const Object& x, const Object& y) {
  return x.module < y.module;
}
inline bool operator==(const Object& x, const Object& y) {
  return x.module == y.module;
}
inline std::ostream& operator<<(std::ostream& output, const Object& what) {
  output << "Object(module:" << EnumToJsonString(what.module) << ")";
  return output;
}

typedef std::vector<PTString> RemoteControlParams;

class AccessRemote {
 public:
  virtual ~AccessRemote() {}

  /**
   * Initializes oneself
   */
  virtual void Init() = 0;

  /**
   * Enables remote control
   */
  virtual void Enable() = 0;

  /**
   * Disables remote control
   */
  virtual void Disable() = 0;

  /**
   * Checks if remote control is enabled
   * @return true if enabled
   */
  virtual bool IsEnabled() const = 0;

  /**
   * Checks whether device is driver's device
   * @param dev_id unique device id
   * @return true if device is have driver
   */
  virtual bool IsPrimaryDevice(const PTString& dev_id) const = 0;

  /**
   * Sets device as driver's device
   * @param dev_id ID device
   */
  virtual void SetPrimaryDevice(const PTString& dev_id) = 0;

  /**
   * Gets current primary device
   * @return ID device
   */
  virtual PTString PrimaryDevice() const = 0;

  /**
   * Allows access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id
   */
  virtual void Allow(const Subject& who, const Object& what) = 0;

  /**
   * Denies access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id
   */
  virtual void Deny(const Subject& who, const Object& what) = 0;

  /**
   * Resets access subject to all object
   * @param who subject is dev_id and app_id
   */
  virtual void Reset(const Subject& who) = 0;

  /**
   * Resets access to object for all subjects
   * @param what object is group
   */
  virtual void Reset(const Object& what) = 0;

  /*
   * Resets all stored consents
   */
  virtual void Reset() = 0;

  /**
   * Checks access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id
   * @return allowed if access was given, disallowed if access was denied
   * manual if need to ask driver
   */
  virtual TypeAccess Check(const Subject& who, const Object& what) const = 0;

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

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_H_
