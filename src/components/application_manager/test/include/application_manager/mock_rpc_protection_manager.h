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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RPC_PROTECTION_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RPC_PROTECTION_MANAGER_H_

#include "application_manager/rpc_protection_manager.h"
#include "gmock/gmock.h"

namespace application_manager {
class MockRPCProtectionManager : public RPCProtectionManager {
 public:
  MOCK_CONST_METHOD3(CheckPolicyEncryptionFlag,
                     bool(const uint32_t function_id,
                          const ApplicationSharedPtr app,
                          const bool is_rpc_service_secure));

  MOCK_METHOD3(CreateEncryptionNeededResponse,
               std::shared_ptr<smart_objects::SmartObject>(
                   const uint32_t connection_key,
                   const uint32_t function_id,
                   const uint32_t conrrelation_id));

  MOCK_CONST_METHOD2(IsInEncryptionNeededCache,
                     bool(const uint32_t app_id,
                          const uint32_t conrrelation_id));

  MOCK_METHOD2(AddToEncryptionNeededCache,
               void(const uint32_t app_id, const uint32_t correlation_id));

  MOCK_METHOD2(RemoveFromEncryptionNeededCache,
               void(const uint32_t app_id, const uint32_t correlation_id));
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RPC_PROTECTION_MANAGER_H_
