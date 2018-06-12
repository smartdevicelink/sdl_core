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
#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_IMPL_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_IMPL_H_

#include <map>
#include "policy/policy_table/types.h"
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "policy/access_remote.h"
#include "policy/cache_manager.h"

using policy_table::FunctionalGroupings;

namespace policy {

class AccessRemoteImpl : public AccessRemote {
 public:
  AccessRemoteImpl();
  explicit AccessRemoteImpl(std::shared_ptr<CacheManager> cache);
  /**
   * @brief CheckModuleType check if module type is allowed for application
   * @param app_id application id
   * @param module module
   * @return true if allowed, if not - false
   */
  bool CheckModuleType(const PTString& app_id,
                       policy_table::ModuleType module) const OVERRIDE;

  /**
   * @brief SetDefaultHmiTypes setup default hmi typed for application
   * @param who application on specific device
   * @param hmi_types hmi types list
   */
  void SetDefaultHmiTypes(const ApplicationOnDevice& who,
                          const std::vector<int>& hmi_types) OVERRIDE;

  /**
   * @brief GetGroups return list of groups for applicaiton
   * @param who application on specific device
   * @return list of groups
   */
  const policy_table::Strings& GetGroups(
      const ApplicationOnDevice& who) OVERRIDE;

  /**
   * @brief GetPermissionsForApp read list of permissions for application
   * @param device_id device
   * @param app_id application
   * @param group_types output parameter for permissions
   * @return true
   */
  bool GetPermissionsForApp(const std::string& device_id,
                            const std::string& app_id,
                            FunctionalIdType& group_types) OVERRIDE;

  /**
   * @brief IsAppRemoteControl check is app is remote controll
   * @param who application on specific device
   * @return true is remote controll aotherwise return false
   */
  bool IsAppRemoteControl(const ApplicationOnDevice& who) OVERRIDE;

  /**
   * @brief GetModuleTypes get list of module types of application
   * @param policy_app_id application id
   * @param modules output parameter for module types
   * @return true on success otherwise false
   */
  bool GetModuleTypes(const std::string& policy_app_id,
                      std::vector<std::string>* modules) OVERRIDE;

 private:
  inline void set_enabled(bool value);
  inline bool country_consent() const;
  /**
   * @brief HmiTypes get list of hmi types for application
   * @param who  application on specific device
   * @return list of hmi types
   */
  const policy_table::AppHMITypes& HmiTypes(const ApplicationOnDevice& who);

  /**
   * @brief GetGroupsIds get list of groups for application
   * @param device_id device id
   * @param app_id application id
   * @param grops_ids output parameter for group ids storing
   */
  void GetGroupsIds(const std::string& device_id,
                    const std::string& app_id,
                    FunctionalGroupIDs& grops_ids);

  /**
   * @brief IsAllowed check if modulename and rpc is allowed for application
   * @param modules list of access modules
   * @param module_name module name to check
   * @param rpc_name rpc name to check
   * @param input list of rpc parameters
   * @return true if allowed otherwise return false
   */
  bool IsAllowed(const policy_table::AccessModules& modules,
                 const std::string& module_name,
                 const std::string& rpc_name,
                 RemoteControlParams* input) const;

  /**
   * @brief CompareParameters check if app parameters allowed
   * @param parameters list of allowed parameters
   * @param input list of parameters to check
   * @return true if allowed otherwise return false
   */
  bool CompareParameters(const policy_table::Strings& parameters,
                         RemoteControlParams* input) const;

  /**
   * @brief cache_ contains pointer to cache manager instance
   */
  std::shared_ptr<CacheManager> cache_;

  /**
   * @brief hmi_types_ contains list of default HMI types for applications
   */
  typedef std::map<ApplicationOnDevice, policy_table::AppHMITypes> HMIList;
  HMIList hmi_types_;

#ifdef BUILD_TESTS
  FRIEND_TEST(AccessRemoteImplTest, KeyMapTest);
  FRIEND_TEST(AccessRemoteImplTest, Allow);
  FRIEND_TEST(AccessRemoteImplTest, Deny);
  FRIEND_TEST(AccessRemoteImplTest, ChangeAccess);
  FRIEND_TEST(AccessRemoteImplTest, ResetBySubject);
  FRIEND_TEST(AccessRemoteImplTest, ResetByObject);
  FRIEND_TEST(AccessRemoteImplTest, CheckAllowed);
  FRIEND_TEST(AccessRemoteImplTest, CheckDisallowed);
  FRIEND_TEST(AccessRemoteImplTest, CheckManual);
  FRIEND_TEST(AccessRemoteImplTest, CheckModuleType);
  FRIEND_TEST(AccessRemoteImplTest, EnableDisable);
  FRIEND_TEST(AccessRemoteImplTest, SetDefaultHmiTypes);
  FRIEND_TEST(AccessRemoteImplTest, GetGroups);
#endif  // BUILD_TESTS
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_ACCESS_REMOTE_IMPL_H_
