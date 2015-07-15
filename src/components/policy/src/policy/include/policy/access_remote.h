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
#ifndef SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_H_
#define SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_H_

#include <vector>
#include <ostream>
#include "./types.h"
#include "policy/policy_types.h"

namespace policy_table = ::rpc::policy_table_interface_base;

namespace policy {

struct SeatLocation {
  int col, row, level;
};
inline bool operator<(const SeatLocation& x, const SeatLocation& y) {
  return (x.col < y.col) || (x.col == y.col && x.row < y.row) ||
      (x.col == y.col && x.row == y.row && x.level < y.level);
}
inline bool operator==(const SeatLocation& x, const SeatLocation& y) {
  return x.col == y.col && x.row == y.row && x.level == y.level;
}
inline bool operator==(const SeatLocation& x,
                       const policy_table::InteriorZone& y) {
  return x == SeatLocation{y.col, y.row, y.level};
}
inline bool operator==(const policy_table::InteriorZone& x,
                       const SeatLocation& y) {
  return y == x;
}
inline std::ostream& operator<<(std::ostream& output, const SeatLocation& x) {
  output << "Interior zone(col:" << x.col << ", row:" << x.row << ", level:"
         << x.level << ")";
  return output;
}

enum TypeAccess {
  kDisallowed,
  kAllowed,
  kManual
};
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
  SeatLocation zone;
};
inline bool operator<(const Object& x, const Object& y) {
  return x.module < y.module || (x.module == y.module && x.zone < y.zone);
}
inline bool operator==(const Object& x, const Object& y) {
  return x.module == y.module && x.zone == y.zone;
}
inline std::ostream& operator<<(std::ostream& output, const Object& what) {
  output << "Object(module:" << EnumToJsonString(what.module) << ", "
         << what.zone << ")";
  return output;
}

typedef std::vector<PTString> RemoteControlParams;
class PolicyListener;

class AccessRemote {
 public:
  virtual ~AccessRemote() {
  }

  /*
   * Sets pointer to listener to changes in PT
   */
  virtual void set_listener(PolicyListener* listener) = 0;

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
   * @param what object is group_id and zone
   */
  virtual void Allow(const Subject& who, const Object& what) = 0;

  /**
   * Denies access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id and zone
   */
  virtual void Deny(const Subject& who, const Object& what) = 0;

  /**
   * Resets access subject to all object
   * @param who subject is dev_id and app_id
   */
  virtual void Reset(const Subject& who) = 0;

  /**
   * Resets access to object for all subjects
   * @param what object is group and zone
   */
  virtual void Reset(const Object& what) = 0;

  /**
   * Checks access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id and zone
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
   * Checks permissions for parameters
   * @param what object is module type and interior zone
   * @param params
   * @return true allowed if access was given, disallowed if access was denied
   * manual if need to ask driver
   */
  virtual TypeAccess CheckParameters(
      const Object& what, const std::string& rpc,
      const RemoteControlParams& params) const = 0;

  /**
   * Sets HMI types if application has default policy permissions
   * @param app_id ID application
   * @param hmi_types list of HMI types
   */
  virtual void SetDefaultHmiTypes(const std::string& app_id,
                                  const std::vector<int>& hmi_types) = 0;

  /**
   * Gets groups
   * @param device_id ID device
   * @param app_id ID application
   * @return list of groups
   */
  virtual const policy_table::Strings& GetGroups(const PTString& device_id,
                                                 const PTString& app_id) = 0;

  /**
   * Gets permissions for application
   * @param device_id
   * @param app_id
   * @param group_types
   * @return true if success
   */
  virtual bool GetPermissionsForApp(const std::string &device_id,
                                    const std::string &app_id,
                                    FunctionalIdType& group_types) = 0;

  /**
   * Checks if application has reverse functionality
   * @param app_id ID application
   * @return true if application uses remote control
   */
  virtual bool IsAppReverse(const PTString& app_id) = 0;

  /**
   * Checks if PTU contains some updates that need to be reported
   * to any Reverse Mobile App
   * @param pt_update PTU
   * @param snapshot PTS
   * @return true if changing current state updates are present
   */
  virtual bool CheckPTURemoteCtrlChange(
    const utils::SharedPtr<policy_table::Table> pt_update,
    const utils::SharedPtr<policy_table::Table> snapshot) = 0;

  /**
   * Checks if PTU contains some updates in equipment section
   * that requires reseting User's consent
   * @param pt_update PTU
   * @param snapshot PTS
   */
  virtual void CheckPTUZonesChange(
    const utils::SharedPtr<policy_table::Table> pt_update,
    const utils::SharedPtr<policy_table::Table> snapshot) = 0;

  /**
   * Checks if PTU contains some updates in Reverse Mobile App groups
   * that need to be reported to this Reverse Mobile App
   * @param pt_update PTU
   * @param snapshot PTS
   */
  virtual void CheckPTUGroupsChange(
    const utils::SharedPtr<policy_table::Table> pt_update,
    const utils::SharedPtr<policy_table::Table> snapshot) = 0;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_H_
