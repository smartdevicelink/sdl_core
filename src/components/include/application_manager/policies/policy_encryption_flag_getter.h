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

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_ENCRYPTION_FLAG_GETTER
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_ENCRYPTION_FLAG_GETTER

#include <string>
#include "policy/policy_table/types.h"

using rpc::policy_table_interface_base::Strings;

namespace policy {
/*
 * @brief PolicyEncryptionFlagGetterInterface interface
 */
class PolicyEncryptionFlagGetterInterface {
 public:
  /*!
   * @brief virtual destructor PolicyEncryptionFlagGetterInterface
   */
  virtual ~PolicyEncryptionFlagGetterInterface() {}

  /*
   * @brief retreives flag whether given application needs encryption
   * @param policy_app_id policy app id
   * @return true if the app need encryption
   */
  virtual bool AppNeedEncryption(const std::string& policy_app_id) const = 0;

  /*
   * @brief Retrieves encryption required flag on the application level
   * @param policy_app_id policy app id
   * @return true if the app need encryption
   */
  virtual const rpc::Optional<rpc::Boolean> GetAppEncryptionRequired(
      const std::string& policy_app_id) const = 0;

  /*
   * @brief retreives function group for app
   * @param policy_app_id policy app id
   * @return groups that exist for app
   */
  virtual const std::vector<std::string> GetFunctionalGroupsForApp(
      const std::string& policy_app_id) const = 0;

  /*
   * @brief retreives flag indicating whether function group needs encryption
   * @param policy_group group
   * @return true if the group need encryption
   */
  virtual bool FunctionGroupNeedEncryption(
      const std::string& policy_group) const = 0;

  /*
   * @brief retreives rpcs for given function group
   * @param policy_group group
   * @return RPCs that exists in group
   */
  virtual const std::vector<std::string> GetRPCsForFunctionGroup(
      const std::string& group) const = 0;

  /*
   * @brief retreives policy function name
   * @param function_id function id
   * @return policy function name
   */
  virtual const std::string GetPolicyFunctionName(
      const uint32_t function_id) const = 0;
  /*
   * @brief retrieves application names from policy table
   * @return container with policy application names
   */
  virtual const std::vector<std::string> GetApplicationPolicyIDs() const = 0;
};

}  // namespace policy
#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_ENCRYPTION_FLAG_GETTER
