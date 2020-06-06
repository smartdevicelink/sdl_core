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

#include "application_manager/rpc_protection_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

CREATE_LOGGERPTR_LOCAL(logger_, "RPCProtectionManagerImpl");

namespace application_manager {

namespace rpc_encryption_exceptions {
std::vector<std::string> kExceptionRPCs = {"RegisterAppInterface",
                                           "SystemRequest",
                                           "OnPermissionsChange",
                                           "OnSystemRequest",
                                           "PutFile",
                                           "OnHMIStatus"};
}

RPCProtectionManagerImpl::RPCProtectionManagerImpl(
    policy::PolicyHandlerInterface& policy_handler)
    : policy_handler_(policy_handler) {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool RPCProtectionManagerImpl::CheckPolicyEncryptionFlag(
    const uint32_t function_id,
    const ApplicationSharedPtr app,
    const bool is_rpc_service_secure) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto& policy_encryption_flag_getter =
      policy_handler_.PolicyEncryptionFlagGetter();
  if (!policy_encryption_flag_getter) {
    LOG4CXX_ERROR(logger_, "Policy Encryption Flag getter is not inited");
    return false;
  }
  const std::string function_name =
      policy_encryption_flag_getter->GetPolicyFunctionName(function_id);
  LOG4CXX_DEBUG(logger_, "Function for check is " << function_name);

  if (!is_rpc_service_secure && IsExceptionRPC(function_id)) {
    LOG4CXX_WARN(logger_,
                 "Exception RPC can be sent in an non secure service despite "
                 "encryption required flag");
    return false;
  }

  if (!app) {
    LOG4CXX_WARN(logger_, "Received app nullptr");
    return false;
  }

  const auto policy_app_id = app->policy_app_id();

  return IsEncryptionRequiredByPolicy(policy_app_id, function_name);
}

bool RPCProtectionManagerImpl::IsEncryptionRequiredByPolicy(
    const std::string& policy_app_id, const std::string& function_name) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto it = encrypted_rpcs_.find(policy_app_id);

  if (encrypted_rpcs_.end() == it) {
    LOG4CXX_WARN(
        logger_,
        "App specific policies for app: " << policy_app_id << " not found");
    it = encrypted_rpcs_.find(policy_table::kDefaultApp);
    return encrypted_rpcs_.end() != it
               ? (*it).second.find(function_name) != (*it).second.end()
               : false;
  }

  return (*it).second.find(function_name) != (*it).second.end();
}

bool RPCProtectionManagerImpl::IsInEncryptionNeededCache(
    const uint32_t app_id, const uint32_t correlation_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "correlation_id is " << correlation_id);

  return encryption_needed_cache_.find(std::make_pair(
             app_id, correlation_id)) != encryption_needed_cache_.end();
}

bool RPCProtectionManagerImpl::IsExceptionRPC(
    const uint32_t function_id) const {
  using namespace rpc_encryption_exceptions;
  const std::string policy_fucntion_id = policy_table::EnumToJsonString(
      static_cast<policy_table::FunctionID>(function_id));
  return helpers::in_range(kExceptionRPCs, policy_fucntion_id);
}

void RPCProtectionManagerImpl::AddToEncryptionNeededCache(
    const uint32_t app_id, const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(message_needed_encryption_lock_);

  LOG4CXX_DEBUG(logger_, "Adding rpc with correlation id: " << correlation_id);

  encryption_needed_cache_.insert(std::make_pair(app_id, correlation_id));
}

void RPCProtectionManagerImpl::RemoveFromEncryptionNeededCache(
    const uint32_t app_id, const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(message_needed_encryption_lock_);

  LOG4CXX_DEBUG(logger_,
                "Removing rpc with correlation id: " << correlation_id);

  encryption_needed_cache_.erase(std::make_pair(app_id, correlation_id));
}

void RPCProtectionManagerImpl::OnPTUFinished(const bool ptu_result) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(encrypted_rpcs_lock_);

  if (ptu_result) {
    LOG4CXX_TRACE(logger_,
                  "PTU finished successfully, commencing internal encrypted "
                  "RPC cache update");
    encrypted_rpcs_.clear();
    SaveEncryptedRPC();
  } else {
    LOG4CXX_WARN(logger_,
                 "PTU was unsuccessful. Keeping internal RPC cache from "
                 "current snapshot");
  }
}

void RPCProtectionManagerImpl::SaveEncryptedRPC() {
  LOG4CXX_AUTO_TRACE(logger_);

  const auto policy_encryption_flag_getter =
      policy_handler_.PolicyEncryptionFlagGetter();

  const auto policy_policy_app_ids =
      policy_encryption_flag_getter->GetApplicationPolicyIDs();

  for (const auto& app : policy_policy_app_ids) {
    LOG4CXX_DEBUG(logger_, "Processing app name: " << app);

    encrypted_rpcs_[app] = GetEncryptedRPCsForApp(app);
  }
}

void RPCProtectionManagerImpl::OnPTInited() {
  LOG4CXX_AUTO_TRACE(logger_);

  encrypted_rpcs_.clear();

  SaveEncryptedRPC();
}

RPCProtectionManagerImpl::FunctionNames
RPCProtectionManagerImpl::GetEncryptedRPCsForApp(
    const std::string& policy_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  FunctionNames encrypted_rpcs;

  const auto policy_encryption_flag_getter =
      policy_handler_.PolicyEncryptionFlagGetter();

  if (!policy_encryption_flag_getter->AppNeedEncryption(policy_app_id)) {
    return encrypted_rpcs;
  }

  const auto function_groups =
      policy_encryption_flag_getter->GetFunctionalGroupsForApp(policy_app_id);

  auto fill_encrypted_rpcs = [&encrypted_rpcs](
                                 const std::string& function_name) {
    LOG4CXX_DEBUG(logger_, "Adding required encryprion rpc: " << function_name);
    encrypted_rpcs.insert(function_name);
  };

  for (const auto& function_group : function_groups) {
    if (policy_encryption_flag_getter->FunctionGroupNeedEncryption(
            function_group)) {
      auto rpcs = policy_encryption_flag_getter->GetRPCsForFunctionGroup(
          function_group);

      std::for_each(rpcs.begin(), rpcs.end(), fill_encrypted_rpcs);
    }
  }

  return encrypted_rpcs;
}

}  // namespace application_manager
