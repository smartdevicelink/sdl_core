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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_PROTOCOL_HANDLER_RPC_PROTECTION_MANAGER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_PROTOCOL_HANDLER_RPC_PROTECTION_MANAGER_IMPL_H_

#include <map>
#include <set>
#include "application_manager/policies/policy_handler.h"
#include "application_manager/rpc_protection_manager.h"

namespace application_manager {
/*
 * @brief RPCProtectionManager implementation
 */
class RPCProtectionManagerImpl : public RPCProtectionManager {
 public:
  typedef std::pair<uint32_t, uint32_t> AppIdCorrIdPair;
  typedef std::set<std::string> FunctionNames;
  typedef std::map<std::string, FunctionNames> AppEncryptedRpcMap;

  RPCProtectionManagerImpl(policy::PolicyHandlerInterface& policy_handler);

  ~RPCProtectionManagerImpl() OVERRIDE {}

  bool CheckPolicyEncryptionFlag(
      const uint32_t function_id,
      const ApplicationSharedPtr app,
      const bool is_rpc_service_secure) const OVERRIDE;

  bool IsInEncryptionNeededCache(const uint32_t app_id,
                                 const uint32_t conrrelation_id) const OVERRIDE;

  void AddToEncryptionNeededCache(const uint32_t app_id,
                                  const uint32_t correlation_id) OVERRIDE;

  void RemoveFromEncryptionNeededCache(const uint32_t app_id,
                                       const uint32_t correlation_id) OVERRIDE;

  // PolicyHandlerObserver interface
  void OnPTUFinished(const bool ptu_result) OVERRIDE;

  void OnPTInited() OVERRIDE;

 private:
  /*
   * @brief check whether given rpc is an exeption
   * @param function_id function id
   * @return true if function_id is an exception (rpc that can be sent before
   * app is registered, hence before secure rpc service is established)
   */
  bool IsExceptionRPC(const uint32_t function_id) const;

  /*
   * @brief retreives list of rpcs that require encryption by policy
   * @param policy_app_id policy application name
   * @return container with function names that require encryption by policy
   */
  FunctionNames GetEncryptedRPCsForApp(const std::string& policy_app_id);

  /*
   * @brief checks whether given function name is in saved encrypted rpc list
   * @param policy_app_id policy application name
   * @param function_name policy function name
   * @return true if function_name for this policy_app_id is saved, otherwise -
   * false
   */
  bool IsEncryptionRequiredByPolicy(const std::string& policy_app_id,
                                    const std::string& function_name) const;

  /*
   * @brief saves rpcs that have encryption_required flag in policy table to
   * internal container
   */
  void SaveEncryptedRPC();

  policy::PolicyHandlerInterface& policy_handler_;

  AppEncryptedRpcMap encrypted_rpcs_;
  sync_primitives::Lock encrypted_rpcs_lock_;

  std::set<AppIdCorrIdPair> encryption_needed_cache_;
  sync_primitives::Lock message_needed_encryption_lock_;
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_PROTOCOL_HANDLER_RPC_PROTECTION_MANAGER_IMPL_H_
