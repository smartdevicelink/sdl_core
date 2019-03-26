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

#include <set>
#include "application_manager/rpc_protection_manager.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {
/*
* @brief RPCProtectionManager implementation
*/
class RPCProtectionManagerImpl : public RPCProtectionManager {
 public:
  RPCProtectionManagerImpl(policy::PolicyHandlerInterface& policy_handler);

  ~RPCProtectionManagerImpl() OVERRIDE {}

  bool CheckPolicyEncryptionFlag(
      const uint32_t function_id,
      const ApplicationSharedPtr app,
      const uint32_t conrrelation_id,
      const bool is_rpc_service_secure) const OVERRIDE;

  bool IsInEncryptionNeededCache(const uint32_t app_id,
                                 const uint32_t conrrelation_id) const OVERRIDE;

  smart_objects::SmartObjectSPtr CreateEncryptionNeededResponse(
      const uint32_t connection_key,
      const uint32_t function_id,
      const uint32_t conrrelation_id) OVERRIDE;

  void AddToEncryptionNeededCache(const uint32_t app_id,
                                  const uint32_t correlation_id) OVERRIDE;

  void RemoveFromEncryptionNeededCache(const uint32_t app_id,
                                       const uint32_t correlation_id) OVERRIDE;

 private:
  /*
   * @brief check whether given rpc is an exeption
   * @param function_id function id
   * @return true if function_id is an exception (rpc that can be sent before
   * app is registered, hence before secure rpc service is established)
   */
  bool IsExceptionRPC(const uint32_t function_id) const;

  /*
   * @brief checks whether given function is in functional group
   * @param function_name function name
   * @param group group name
   * @return true if the function exists in group else return false
   */
  bool IsFunctionInGroup(const std::string& function_name,
                         const std::string& group) const;

  policy::PolicyHandlerInterface& policy_handler_;

  typedef std::pair<uint32_t, uint32_t> AppIdCorrIdPair;

  std::set<AppIdCorrIdPair> encryption_needed_cache_;
  sync_primitives::Lock message_needed_encryption_lock_;
};
}  // namespace policy

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_PROTOCOL_HANDLER_RPC_PROTECTION_MANAGER_IMPL_H_
